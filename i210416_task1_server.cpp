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
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cout << "Error binding socket!" << endl;
        close(server_socket);
        return -1;
    }
    
    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        cout << "Error listening!" << endl;
        close(server_socket);
        return -1;
    }
    
    cout << "Server listening on port 8080..." << endl;
    cout << "Waiting for client connection..." << endl;
    
    // Accept client connection
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        cout << "Error accepting connection!" << endl;
        close(server_socket);
        return -1;
    }
    
    cout << "Client connected successfully!" << endl;
    
    char buffer[1024];
    string message;
    
    while (true) {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
        
        // Receive message from client
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received <= 0) {
            cout << "Client disconnected." << endl;
            break;
        }
        
        message = string(buffer);
        
        // Check for end conversation signal
        if (message == "QUIT" || message == "quit" || message == "EXIT" || message == "exit") {
            cout << "Client requested to end conversation." << endl;
            cout << "Closing connection..." << endl;
            break;
        }
        
        // Display received message
        cout << "Client: " << message << endl;
        
        // Send acknowledgment back to client
        string ack = "Message received: " + message;
        send(client_socket, ack.c_str(), ack.length(), 0);
    }
    
    // Close connections
    close(client_socket);
    close(server_socket);
    
    cout << "Server terminated." << endl;
    return 0;
}