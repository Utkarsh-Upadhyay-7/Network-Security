// Socket programming with DES.. Sever Code...
// g++ client.cpp -o client -lcrypto -lssl , command to compile
// ./desclient  , command to run
//Used openssl library functions for implenting DES

#include <iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#define BUFSIZE 1024 
#define PORT 8080 
using namespace std;

class Client {
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    unsigned char out[BUFSIZE], back[BUFSIZE];
    unsigned char *e = out;
    DES_cblock key = "1234567";
    DES_cblock seed = {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    DES_key_schedule keysched;
 
    public:
        void log(string message) {
            cout << "<Client>: " << message << endl;
        }

    //creating socket 
    int createSocket() {
        log("Creating socket...");
        if ((this -> sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "\nCreation of socket failed\n";
            return -1;
        }
        log("Socket created, ready to go..");
        return 0;
    }
    
    //adding srver address
    int addServerAddress(string address, int port) {
        log("Adding address and port to the socket");

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        // Convert IPv4 and IPv6 addresses from text to binary form 
        //checking if address is invalid
        if (inet_pton(AF_INET, address.c_str(), & (this -> serv_addr).sin_addr) <= 0) {
            log("Address is invalid, unsupported format");
            return -1;
        }
        //if addriss is valid then it will be connected successfully
        log("Address and port added successfully");
        return 0;
    }
    
    //connecting to the socket
    int doConnect() {
        log("Connecting to socket...");
        if (connect(sock, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
            log("Connection Failed");
            return -1;
        }
        log("Connected to socket successfully");
        return 0;
    }

    //sending the message to the server after connecting successfully
    void sendMessage(string message) {
        log("Sending message in encrypted form");
        memset(out, 0, sizeof(out));
        memset(back, 0, sizeof(back));
        RAND_seed(seed, sizeof(DES_cblock));
        DES_set_key((DES_cblock *)key, &keysched);
        log("Encrypting, plaintext : " + message);
        DES_ecb_encrypt((DES_cblock *)message.c_str(),(DES_cblock *)out, &keysched, DES_ENCRYPT);
        log("Encrypted form : ");
        //printing the encrypted text after encrypting with the help of DES
        while (*e) printf(" [%02x]", *e++);
        printf("\n");
        send(sock , out , sizeof(out) , 0 );

    }
};

int main(int argc, char const * argv[]) {
    Client client;
    client.createSocket();
    client.addServerAddress("127.0.0.1", PORT);
    client.doConnect();
    client.sendMessage("HI_SERVER");
    return 0;
}