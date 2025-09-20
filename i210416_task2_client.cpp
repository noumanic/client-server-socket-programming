#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cout << "Error creating socket!" << endl;
        return -1;
    }
    
    // Define server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8081);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cout << "Error connecting to server!" << endl;
        cout << "Make sure the server is running on port 8081." << endl;
        close(client_socket);
        return -1;
    }
    
    cout << "Connected to Tic Tac Toe Server!" << endl;
    cout << "Instructions:" << endl;
    cout << "- Enter position numbers 1-9 to make your move" << endl;
    cout << "- Type 'quit' anytime to exit the game" << endl;
    cout << "- You are X, Server is O" << endl;
    cout << "\nWaiting for game to start..." << endl;
    
    char buffer[2048];
    string input;
    
    while (true) {
        // Receive message from server
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            cout << "\nServer disconnected." << endl;
            break;
        }
        
        string serverMessage(buffer);
        cout << serverMessage;
        
        // Check if server is asking for input
        if (serverMessage.find("Enter position") != string::npos || 
            serverMessage.find("Try again") != string::npos ||
            serverMessage.find("Your turn") != string::npos) {
            
            getline(cin, input);
            
            // Send input to server
            send(client_socket, input.c_str(), input.length(), 0);
            
            if (input == "quit") {
                cout << "Quitting game..." << endl;
                break;
            }
        }
        else if (serverMessage.find("play again") != string::npos) {
            getline(cin, input);
            send(client_socket, input.c_str(), input.length(), 0);
            
            if (input == "no" || input == "quit" || input == "n") {
                cout << "Thanks for playing!" << endl;
                break;
            }
        }
        else if (serverMessage.find("won") != string::npos || 
                 serverMessage.find("wins") != string::npos || 
                 serverMessage.find("draw") != string::npos) {
            // Game ended, continue to next iteration to get play again prompt
            continue;
        }
    }
    
    // Close connection
    close(client_socket);
    cout << "Connection closed." << endl;
    
    return 0;
}