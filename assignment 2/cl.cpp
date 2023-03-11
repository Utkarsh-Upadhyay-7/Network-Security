#include <iostream>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Could not create socket" << endl;
        return -1;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(1234);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cerr << "Connect failed" << endl;
        return -1;
    }

    EVP_CIPHER_CTX *ctx;

    unsigned char key[] = "0123456789abcdef";
    unsigned char iv[] = "abcdefghijklmnop";

    unsigned char plaintext[] = "My_self_UTKARSH";
    int plaintext_len = strlen((char *)plaintext);
    cout<<"Original message :"<<plaintext<<endl;

    unsigned char ciphertext[1024];
    int ciphertext_len;

    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len);
    EVP_EncryptFinal_ex(ctx, ciphertext + ciphertext_len, &ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);
    
    cout<<"Message after encryping : "<<ciphertext<<endl;

    send(sock, (char *)ciphertext, strlen((char *)ciphertext), 0);
    close(sock);

    return 0;
}