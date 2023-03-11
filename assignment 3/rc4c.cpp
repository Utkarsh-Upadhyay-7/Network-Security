//for compile : g++ -o rc4c rc4c.cpp 
//for run : ./rc4c
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    string key_str = "secretkey";
    int key_length = key_str.length();
    unsigned char key[key_length];
    
    // Convert the key to an unsigned char array
    memcpy(key, key_str.c_str(), key_length);
    
    // Create a socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    // Set server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    // Send plaintext to the server
    unsigned char plaintext[] = "Hello, server! I am UKARSH UPADHYAY";
    printf("%s\n",plaintext);
    
     // Encrypt the plaintext
    int plaintext_length = strlen((const char*)plaintext);
    unsigned char ciphertext[plaintext_length];
    RC4(key, key_length, plaintext, plaintext_length, ciphertext);
    printf("Ciphertext generated: %s\n", ciphertext);
    
    //ciphertext send to the server
    send(sock, ciphertext, strlen((const char*)ciphertext), 0);
    printf("ciphertext sent to server:\n");
    return 0;
}
