/* 
Arvid Persson Moosavi
amoosavi@kth.se
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TRIES         5   /* maximum number of attempts to conect to server */
#define MAXCMD      500   /* max command length */


static int connect_client(in_addr_t, int);
static void client_handler(void);
static void handle_reply(void);

static char *SIP, *SPORT;
static int socketFD = -1;

int main(int argc, char *argv[]) {
    in_addr_t serverIP;
    int portNumber;

    if (argc != 3) {
        printf("Usage: client serverIPAdr portNumber\n");
        exit(0);
    }

    if (-1 == (serverIP = inet_addr(argv[1]))) {
        perror("First argument was not an IP adress");
        exit(0);
    }
    SIP = argv[1];

    if (1 != sscanf(argv[2], "%d", &portNumber)) {
        perror("Second argument was not a port number");
        exit(0);
    }
    SPORT = argv[2];

    /* connect client to server */
    if (-1 != (socketFD = connect_client(serverIP, portNumber))) {
        /* handle the connection */
        client_handler();
        /* clean up after us */
        close(socketFD);
    }
}

static void client_handler() {
    char cmd[MAXCMD];
    for (;;){
        cmd[0] = '\0'; // reset the buffer to be an empty string
        fprintf(stdout, "%s:%s> ", SIP, SPORT);
        
        /* Stdin console input */
        fgets(cmd, MAXCMD, stdin);
        
        /* Length of input string */
        int charlen = strlen(cmd);

        /* Sends size of string length */
        write(socketFD, &charlen, sizeof(charlen));
        
        /* Sends length of string */
        write(socketFD, cmd, strlen(cmd));

        /* If command was "exit" we should also terminate on this side */
        if (strncmp(cmd, "exit", 4) == 0) {
            return;
        }
        /* If not we wait for the server's reply */
        handle_reply();
    }
}
static void handle_reply() {
    unsigned long bufferlen = 0;
    for(;;) {
        /* Recieve message length and check for errors */
        if(recv(socketFD, &bufferlen, sizeof(bufferlen), 0) == -1) {
            fprintf(stderr, "Error when receiving message length. errno: %d", errno);
        }

        if (bufferlen == 0) {
            return; // finish on 0 length message
        }
        
        /* Allocate memory for recieve buffer */
        char *reply = (char*) calloc(1, bufferlen);

        /* Receive incoming message into buffer.*/
        if(recv(socketFD, reply, bufferlen, 0) == -1) {
            fprintf(stderr, "Error when receiving message. errno: %d", errno);
        }

        /* Print message to stdout */
        fprintf(stdout ,"%s", reply);
        
        /* Free the receive buffer */
        free(reply);
        
        bufferlen = 0; // reset bufferlen
    }
}

static int connect_client(in_addr_t serverIP, int portNumber) {
    int attempts = 0, result = -1;
    struct sockaddr_in serverINETAdress;

    /* Creating a socket to server in socketFD, with default protocol for stream (TCP) */
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void *) &serverINETAdress, sizeof(serverINETAdress));

    serverINETAdress.sin_family = AF_INET;       /* Internet domain */
    serverINETAdress.sin_addr.s_addr = serverIP; /* IP adress of server */
    serverINETAdress.sin_port = htons((short)portNumber);

    fprintf(stderr, "Connecting to: %s %d\n", inet_ntoa(serverINETAdress.sin_addr), portNumber);

    /* Try to connect until number of TRIES is exceeded */

    for (;attempts < TRIES; ++attempts) {
        if(connect(socketFD, (struct sockaddr*)&serverINETAdress, sizeof(serverINETAdress)) == 0){
            result = 1;
            break;
        }

        else {
            printf("retrying..\n");
            sleep(1);
        }
    }

    if(result == 1) {
    
        printf("Connected!\n");
        return socketFD;
    }

    else {
        printf("Failed to connect after %d tries.\n", attempts);
        return result;
    }
}

