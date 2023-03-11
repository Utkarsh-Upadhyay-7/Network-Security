// Socket programming with DES.. Sever Code...
// g++ server.cpp -o server -lcrypto -lssl , command to compile
// ./desserver , command to run
//Used library functions for implenting DES

#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#define BUFSIZE 1024 
#define PORT 8080 
using namespace std;
class Server {
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    unsigned char buffer[BUFSIZE]={0};
    unsigned char *e = buffer;
    
    public:
        void log(string message) {
            cout << "<Server>: " << message << endl;
        }

    //creating socket
    void createSocket(int port) {
        log("Creating socket");
        // Creating socket file descriptor 
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        log("Socket created successfully");
        // Forcefully attaching socket to the port 8080 
        log("Adding options for socket");
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, & opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        log("Options added successfully");
    }

    //binding the socket 
    void bindAndListenSocket() {
        // Forcefully attaching socket to the port 8080 
        log("Binding socket to port");
        if (bind(server_fd, (const struct sockaddr * ) & address, (socklen_t)sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        log("Socket has been binded");
        log("Listening for client");
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    }

    //accepting and receiving from the client
    unsigned char * acceptAndReceive() {
        memset(buffer, 0, sizeof(buffer));
        log("Waiting for client request, Please wait...");
        if ((new_socket = accept(server_fd, (struct sockaddr * ) & address,
                (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        valread = read( new_socket , (DES_cblock *)buffer, sizeof(buffer));
        log("Request received...");
        return buffer;
    }
};

class DESDecrypt {
    DES_cblock key = "1234567";
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
    unsigned char back[BUFSIZE];
    public:
        void log(string message) {
            cout << "<Server>: " << message << endl;
        }
    //decrypting the ciphertext after receiving 
    unsigned char* decrypt(unsigned char* cipher) {
        memset(back, 0, sizeof(back));
        RAND_seed(seed, sizeof(DES_cblock));
        DES_set_key((DES_cblock *)key, &keysched);
        DES_ecb_encrypt((DES_cblock *)cipher,(DES_cblock *)back, &keysched, DES_DECRYPT);
        return back;
    }
};

int main(int argc, char const * argv[]) {
    Server server;
    server.createSocket(PORT);
    server.bindAndListenSocket();
    unsigned char * cipher = server.acceptAndReceive();
    
    cout<<"\n ACK = Enrcypted message recieved"<<endl;
    cout<<"\n Now decrypting the encrypted message"<<endl;
    DESDecrypt des;
    unsigned char* p = des.decrypt(cipher);    //decrypted cipher text is 
    printf("Plaintext=: [%s]\n",p);
    return 0;
}