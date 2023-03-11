#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Create the client socket
    
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // Connect to the server
    connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    // Send data to the server
    char buffer[1024];
    strcpy(buffer, "Hello, server! , I am Client 1");
    send(client_socket, buffer, strlen(buffer), 0);

    // Receive data from the server
    memset(buffer, 0, 1024);
    int bytes_received = recv(client_socket, buffer, 1024, 0);
    std::cout << "Received: " << buffer << std::endl;

    // Close the client socket
    close(client_socket);

    return 0;
}