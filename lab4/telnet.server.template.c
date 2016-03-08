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

    /* TODO install signalhandler clean_up for clean termination */
    
    set_up_server();

    print_ip_and_port();   /* inform user of where we are running */

    server_loop();

}


static void set_up_server(void) {
    struct sockaddr_in serverINETAdress;
    
    /* TODO create a socket to listen to in serverFD, with default protocol for stream (TCP) */

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
    
    /* TODO bind socket to address */

    /* TODO listen to socket */
}

static void server_loop(void) {
    for (;/* EVER */;) {
        struct sockaddr_in clientINETAdress;
        socklen_t clientLen = sizeof(clientINETAdress);
        
        /* TODO accept connection from serverFD into clientFD */
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
        /* TODO receive message length...remember to check for errors! */
        fprintf(stdout, "reading %lu bytes\n", bufferlen);
        if (bufferlen > 0) {
            
            /* TODO receive message contents...remember to check for errors! */
            /* TODO deal with command "exit"
             or deal with command "cd"
             or deal with anything else using popen
             
             forward responses to the client and terminate with a bufferlength 0
             */
            /* TODO remember to allocate and free the receive buffer */
            
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
