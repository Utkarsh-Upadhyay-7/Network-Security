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
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        cerr << "Could not create socket" << endl;
        return -1;
    }

    int client_sock;
    struct sockaddr_in server, client;
    int c = sizeof(struct sockaddr_in);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(1234);

    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        cout << "Bind failed" << endl;
        return -1;
    }
  
    listen(server_sock, 3);

    cout << "Waiting for incoming connections..." << endl;
    client_sock = accept(server_sock, (struct sockaddr )&client, (socklen_t)&c);
    if (client_sock < 0) {
        cout << "Accept failed" << endl;
        return -1;
    }

    EVP_CIPHER_CTX *ctx;

    unsigned char key[] = "0123456789abcdef";
    unsigned char iv[] = "abcdefghijklmnop";

    unsigned char ciphertext[1024];
    int ciphertext_len;

    int bytes_read = recv(client_sock, (char *)ciphertext, 1024, 0);
    cout<<ciphertext<<endl;
    ciphertext[bytes_read] = '\0';

    unsigned char decryptedtext[2048];
    int decryptedtext_len;

    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(ctx, decryptedtext, &decryptedtext_len, ciphertext, ciphertext_len);
    EVP_DecryptFinal_ex(ctx, decryptedtext + decryptedtext_len, &decryptedtext_len);
    EVP_CIPHER_CTX_free(ctx);

    cout << "Decrypted message: "<< decryptedtext;

    close(client_sock);
    close(server_sock);

    return 0;
}