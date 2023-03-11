#include <iostream>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

void handle_client(int client_socket) {
    // Receive data from the client
    char buffer[1024];
    memset(buffer, 0, 1024);
    int bytes_received = recv(client_socket, buffer, 1024, 0);

    // Send a response back to the client
    send(client_socket, buffer, bytes_received, 0);

    // Close the client socket
    close(client_socket);
}

int main() {
    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(1234);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    // Listen for incoming connections
    listen(server_socket, 5);

    // Vector to store the client threads
    std::vector<std::thread> client_threads;

    while (true) {
        // Accept a connection from a client
        int client_socket = accept(server_socket, nullptr, nullptr);

        // Create a new thread for the client and add it to the vector
        client_threads.push_back(std::thread(handle_client, client_socket));
    }

    // Join all the client threads
    for (auto& t : client_threads) {
        t.join();
    }

    // Close the server socket
    close(server_socket);

    return 0;
}