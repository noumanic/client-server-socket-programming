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
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    
    // Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cout << "Error connecting to server!" << endl;
        close(client_socket);
        return -1;
    }
    
    cout << "Connected to server!" << endl;
    cout << "Type your messages (type 'quit' or 'exit' to end conversation):" << endl;
    
    string message;
    char buffer[1024];
    
    while (true) {
        cout << "You: ";
        getline(cin, message);
        
        // Send message to server
        send(client_socket, message.c_str(), message.length(), 0);
        
        // Check if user wants to quit
        if (message == "quit" || message == "exit" || message == "QUIT" || message == "EXIT") {
            cout << "Ending conversation..." << endl;
            break;
        }
        
        // Receive acknowledgment from server
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            cout << "Server disconnected." << endl;
            break;
        }
        
        cout << "Server: " << buffer << endl;
    }
    
    // Close connection
    close(client_socket);
    cout << "Connection closed." << endl;
    
    return 0;
}