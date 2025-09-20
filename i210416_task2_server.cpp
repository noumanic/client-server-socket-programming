#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class TicTacToe {
private:
    vector<vector<char>> board;
    
public:
    TicTacToe() {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
    }
    
    void resetBoard() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';
            }
        }
    }
    
    string getBoardString() {
        string boardStr = "\n";
        boardStr += " " + string(1, board[0][0]) + " | " + string(1, board[0][1]) + " | " + string(1, board[0][2]) + " \n";
        boardStr += "-----------\n";
        boardStr += " " + string(1, board[1][0]) + " | " + string(1, board[1][1]) + " | " + string(1, board[1][2]) + " \n";
        boardStr += "-----------\n";
        boardStr += " " + string(1, board[2][0]) + " | " + string(1, board[2][1]) + " | " + string(1, board[2][2]) + " \n";
        boardStr += "\nPositions:\n";
        boardStr += " 1 | 2 | 3 \n";
        boardStr += "-----------\n";
        boardStr += " 4 | 5 | 6 \n";
        boardStr += "-----------\n";
        boardStr += " 7 | 8 | 9 \n\n";
        return boardStr;
    }
    
    bool makeMove(int position, char player) {
        if (position < 1 || position > 9) return false;
        
        int row = (position - 1) / 3;
        int col = (position - 1) % 3;
        
        if (board[row][col] != ' ') return false;
        
        board[row][col] = player;
        return true;
    }
    
    char checkWinner() {
        // Check rows
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
                return board[i][0];
            }
        }
        
        // Check columns
        for (int j = 0; j < 3; j++) {
            if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != ' ') {
                return board[0][j];
            }
        }
        
        // Check diagonals
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
            return board[0][0];
        }
        
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
            return board[0][2];
        }
        
        return ' '; // No winner
    }
    
    bool isBoardFull() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') return false;
            }
        }
        return true;
    }
    
    vector<int> getAvailableMoves() {
        vector<int> moves;
        for (int i = 0; i < 9; i++) {
            int row = i / 3;
            int col = i % 3;
            if (board[row][col] == ' ') {
                moves.push_back(i + 1);
            }
        }
        return moves;
    }
    
    int getRandomMove() {
        vector<int> availableMoves = getAvailableMoves();
        if (availableMoves.empty()) return -1;
        
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, availableMoves.size() - 1);
        
        return availableMoves[dis(gen)];
    }
};

int main() {
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cout << "Error creating socket!" << endl;
        return -1;
    }
    
    // Allow socket reuse
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8081);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cout << "Error binding socket!" << endl;
        close(server_socket);
        return -1;
    }
    
    // Listen for connections
    if (listen(server_socket, 1) == -1) {
        cout << "Error listening!" << endl;
        close(server_socket);
        return -1;
    }
    
    cout << "Tic Tac Toe Server listening on port 8081..." << endl;
    
    while (true) {
        cout << "Waiting for client connection..." << endl;
        
        // Accept client connection
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            cout << "Error accepting connection!" << endl;
            continue;
        }
        
        cout << "Client connected!" << endl;
        
        TicTacToe game;
        char buffer[1024];
        bool playAgain = true;
        int bytes_received;
        
        while (playAgain) {
            game.resetBoard();
            string gameMessage = "=== TIC TAC TOE GAME ===\n";
            gameMessage += "You are X, Server is O\n";
            gameMessage += game.getBoardString();
            gameMessage += "Your turn! Enter position (1-9): ";
            
            send(client_socket, gameMessage.c_str(), gameMessage.length(), 0);
            
            bool gameActive = true;
            char winner = ' ';
            
            while (gameActive) {
                // Client's turn
                memset(buffer, 0, sizeof(buffer));
                bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                
                if (bytes_received <= 0) {
                    cout << "Client disconnected during game." << endl;
                    gameActive = false;
                    playAgain = false;
                    break;
                }
                
                string clientInput(buffer);
                
                if (clientInput == "quit") {
                    cout << "Client requested to quit." << endl;
                    gameActive = false;
                    playAgain = false;
                    break;
                }
                
                try {
                    int position = stoi(clientInput);
                    
                    if (!game.makeMove(position, 'X')) {
                        string errorMsg = "Invalid move! Try again.\n";
                        errorMsg += game.getBoardString();
                        errorMsg += "Your turn! Enter position (1-9): ";
                        send(client_socket, errorMsg.c_str(), errorMsg.length(), 0);
                        continue;
                    }
                    
                    cout << "Client played position: " << position << endl;
                    
                    // Check for winner after client's move
                    winner = game.checkWinner();
                    if (winner != ' ') {
                        string winMsg = game.getBoardString();
                        if (winner == 'X') {
                            winMsg += "Congratulations! You won!\n";
                        } else {
                            winMsg += "Server wins!\n";
                        }
                        send(client_socket, winMsg.c_str(), winMsg.length(), 0);
                        gameActive = false;
                        break;
                    }
                    
                    if (game.isBoardFull()) {
                        string drawMsg = game.getBoardString();
                        drawMsg += "It's a draw!\n";
                        send(client_socket, drawMsg.c_str(), drawMsg.length(), 0);
                        gameActive = false;
                        break;
                    }
                    
                    // Server's turn
                    int serverMove = game.getRandomMove();
                    if (serverMove != -1) {
                        game.makeMove(serverMove, 'O');
                        cout << "Server played position: " << serverMove << endl;
                        
                        // Check for winner after server's move
                        winner = game.checkWinner();
                        if (winner != ' ') {
                            string winMsg = game.getBoardString();
                            if (winner == 'X') {
                                winMsg += "Congratulations! You won!\n";
                            } else {
                                winMsg += "Server wins!\n";
                            }
                            send(client_socket, winMsg.c_str(), winMsg.length(), 0);
                            gameActive = false;
                            break;
                        }
                        
                        if (game.isBoardFull()) {
                            string drawMsg = game.getBoardString();
                            drawMsg += "It's a draw!\n";
                            send(client_socket, drawMsg.c_str(), drawMsg.length(), 0);
                            gameActive = false;
                            break;
                        }
                        
                        string gameUpdate = game.getBoardString();
                        gameUpdate += "Server played position " + to_string(serverMove) + "\n";
                        gameUpdate += "Your turn! Enter position (1-9): ";
                        send(client_socket, gameUpdate.c_str(), gameUpdate.length(), 0);
                    }
                    
                } catch (const exception& e) {
                    string errorMsg = "Invalid input! Enter a number (1-9).\n";
                    errorMsg += game.getBoardString();
                    errorMsg += "Your turn! Enter position (1-9): ";
                    send(client_socket, errorMsg.c_str(), errorMsg.length(), 0);
                }
            }
            
            if (!playAgain) break;
            
            // Ask if client wants to play again
            string playAgainMsg = "\nDo you want to play again? (yes/no/quit): ";
            send(client_socket, playAgainMsg.c_str(), playAgainMsg.length(), 0);
            
            memset(buffer, 0, sizeof(buffer));
            bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            
            if (bytes_received <= 0) {
                cout << "Client disconnected." << endl;
                break;
            }
            
            string response(buffer);
            transform(response.begin(), response.end(), response.begin(), ::tolower);
            
            if (response == "no" || response == "quit" || response == "n") {
                playAgain = false;
                cout << "Client doesn't want to play again." << endl;
            } else if (response == "yes" || response == "y") {
                cout << "Starting new game..." << endl;
                playAgain = true;
            } else {
                playAgain = false;
            }
        }
        
        // Close client connection
        close(client_socket);
        cout << "Client disconnected." << endl;
    }
    
    close(server_socket);
    return 0;
}