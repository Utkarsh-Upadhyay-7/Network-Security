//for compile : g++ -o rc4s rc4s.cpp 
//for run : ./rc4s
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

// Function to generate RC4 key stream
void RC4(unsigned char *key, int key_length, unsigned char *input, int input_length, unsigned char *output) {
    // Initialization
    unsigned char S[256], K[256];
    for (int i = 0; i < 256; i++) {
        S[i] = i;
        K[i] = key[i % key_length];
    }
    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + K[i]) % 256;
        swap(S[i], S[j]);
    }
    // Stream generation
    int i = 0, k = 0;
    for (int n = 0; n < input_length; n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        k = S[(S[i] + S[j]) % 256];
        output[n] = input[n] ^ k;
    }
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    string key_str = "secretkey";
    int key_length = key_str.length();
    unsigned char key[key_length];
    
    // Convert the key to an unsigned char array
    memcpy(key, key_str.c_str(), key_length);
    
    // Create a server socket 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Accept incoming connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    // Receive plaintext from the client
    unsigned char ciphertext[1024] = {0};
    valread = read(new_socket, ciphertext, 1024);
    printf("ciphertext received from client: %s\n", ciphertext);

     // Decrypt the ciphertext
    int ciphertext_length = strlen((const char*)ciphertext);
    unsigned char decrypted_text[ciphertext_length];
    RC4(key, key_length, ciphertext, ciphertext_length, decrypted_text);
    printf("Decrypted text: %s\n", decrypted_text);  
    return 0;
}
