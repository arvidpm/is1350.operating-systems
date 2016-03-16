/* 
Arvid Persson Moosavi
amoosavi@kth.se
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>

#define MAXQUEUE      5   /* maximum number of pending connections */
#define PORTNUMBER 2225   /* arbitrary portnumber in range we may use */
#define MAXOUTPUT   500   /* outout buffer size */

static void print_ip_and_port(void);
static void clean_up(int);
static void set_up_server(void);
static void server_loop(void);
static void server_handler(void);

/* global variables so they are accessible in signalhandler */

static int serverFD = -1, clientFD = -1;  /* fd for server and client socket */


int main(int argc, char *argv[]) {

    /* signalhandler clean_up for clean termination */
    signal(SIGQUIT, clean_up);
    signal(SIGTERM, clean_up);
    signal(SIGKILL, clean_up);
    
    set_up_server();

    print_ip_and_port();   /* inform user of where we are running */

    server_loop();

}


static void set_up_server(void) {
    struct sockaddr_in serverINETAdress;
    
    /* 
    SOCK_STREAM = Provides sequenced, reliable, two-way, 
    connection-based byte streams. An out-of-band data transmission 
    mechanism may be supported.
    
    AF_INET = IPv4 Internet protocols

    Creating a socket to listen to in serverFD, 
    with default protocol for stream (TCP (0))

    */
    serverFD = socket(AF_INET, SOCK_STREAM, 0);

    /* initialize adresses of server */
    bzero((void *) &serverINETAdress, sizeof(serverINETAdress));

    /* Internet domain */
    serverINETAdress.sin_family = AF_INET;
    /* Accept connectionsfrom any IP adress */
    serverINETAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverINETAdress.sin_port = htons((u_short)PORTNUMBER);
    /* set reuse address so you don't have to wait for the port to be free again */
    int on = 1;
    setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    
    /* Bind socket to server INET address */
    bind(serverFD, (struct sockaddr*)&serverINETAdress, sizeof(serverINETAdress)); 

    /* Passivly listen to selected socket, MAXQUEUE(5) */
    listen(serverFD, MAXQUEUE);

}

static void server_loop(void) {
    for (;/* EVER and ever */;) {
        struct sockaddr_in clientINETAdress;
        socklen_t clientLen = sizeof(clientINETAdress);
        
        /* Accept connection from serverFD into clientFD */
        clientFD = accept(serverFD, (struct sockaddr *)&clientINETAdress, &clientLen);

        fprintf(stdout, "connected clientFD %d from IPadress %s\n",
                clientFD, inet_ntoa(clientINETAdress.sin_addr));
        
        /* Create new child to deal with this connection */
        pid_t cpid = fork();
        if (cpid == 0) { // child
            serverFD = -1; // avoid cleaning this up
            server_handler();
            exit(0);
        } else { // parent
            clientFD = -1; // reset so it doesn't get closed twice
        }
    }
}

static void server_handler(void) {
    unsigned long bufferlen = 0;
    for (;/* EVER */;) {

        /* Receiving message length, checking for errors,
        0 = flags 
        
        The  recv(),  recvfrom(),  and  recvmsg()  calls  are  used to receive messages from a
        socket.  They may be used to receive data on both connectionless  and  connection-ori‐
        ented  sockets.   This page first describes common features of all three system calls,
        and then describes the differences between the calls.


        */
        if(recv(clientFD, &bufferlen, sizeof(bufferlen), 0) == -1) {
            fprintf(stderr, "Error when receiving message length. errno: %d", errno);
        }

        fprintf(stdout, "reading %lu bytes\n", bufferlen-1);

        if (bufferlen > 0) {

            char reply[MAXOUTPUT];

            /* The C library function void 
            *memset(void *str, int c, size_t n) 
            copies the character c (an unsigned char) 
            to the first n characters of the string pointed to, 
            by the argument str.*/
            memset(&reply, 0, MAXOUTPUT);

            /* Allocate memory for recieved message */
            char *message = (char *) calloc(0, bufferlen);

            /* Recieving the message */
            if(recv(clientFD, message, bufferlen, 0) == -1){
                fprintf(stderr, "an error occured, errno: %d\n", errno);
                server_handler();
            }

            if (strncmp(message, "exit", 4) == 0){
                clean_up(0);
            }
             
            /* forward responses to the client and terminate with a bufferlength 0.
            popen opens a process by creating a pipe, 
            forking and invoking the shell. 
            Command parameter is the message receeived from client. 
            "r" means read.popen returns a stream that can be read.*/

            else {

                /* The popen() function opens a process by creating 
                a pipe, forking, and invoking the shell. 

                FILE *popen(const char *command, const char *type);
                */
                FILE* file = popen(message, "r");


                /*
                read()  attempts  to  read  up  to count bytes from file descriptor fd into the buffer
                starting at buf.


                The function fileno() examines the argument 
                stream and returns its integer descriptor.
                */
                while(read(fileno(file), reply, MAXOUTPUT)) {

                    unsigned long len = strlen(reply);
                    send(clientFD, &len, sizeof(len), 0);
                    send(clientFD, reply, len, 0);
                    memset(&reply, 0, MAXOUTPUT);

                }

                send(clientFD, "", 1, 0);

                /* popen, pclose - pipe stream to or from a process

                The pclose() function waits for the associated process 
                to terminate and returns the exit status of the 
                command as returned by wait4(2). */
                pclose(file);
            }

            /* Freeing the receive buffer */
            free(message);
            bufferlen = 0; // reset bufferlen
        }
    }
}

static void print_ip_and_port(void) {
    char hostname[121];
    struct hostent *hostPtr;

    gethostname(hostname, 120);
    hostPtr = gethostbyname(hostname);

    printf("Server is running on\n%s (",
           hostname);
    if (hostPtr != NULL) {
        int i = 0;
        while (hostPtr->h_addr_list[i] != NULL) {
            struct sockaddr_in server;
            memcpy(&server.sin_addr, hostPtr->h_addr_list[0], hostPtr->h_length);
            printf(" %s ", inet_ntoa(server.sin_addr));
            i++;
        }
    }
    printf(") : %d\n", PORTNUMBER);
}

static void clean_up(int sign) {
    fprintf(stderr, "Server is shutting down....\n");
    if (serverFD != -1) {
        fprintf(stderr, "Closing serverFD %d\n", serverFD);
        close(serverFD);
    }
    if (clientFD != -1) {
        fprintf(stderr, "Closing clientFD %d\n", clientFD);
        close(clientFD);
    }
    _exit(0);
}
