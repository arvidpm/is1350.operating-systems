/*
struct sigaction {
    void         (*sa_handler)(int);      // address of signal handler
    sigset_t     sa_mask;                 // additional signals to block
    int          sa_flags;                // signal options

    // alternate signal handler
    void         (*sa_sigaction)(int, siginfo_t *, void*);
};
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("received SIGINT\n");
    }
}

static void another_handler(int sig, siginfo_t *si, void *context) {
    printf("received signal %d\n", sig);
}

int main(void) {
    struct sigaction sa;



    if (signal(SIGINT, sig_handler) == SIG_ERR) { // subscribe sig_handler to SIGINT
        printf("\ncan't catch SIGINT\n");
    }

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = &another_handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1) { // same thing in more complicated for SIGHUP
        printf("\ncan't catch SIGHUP\n");
    }
    
    printf("Send some signal to process %d\n", getpid());

    // A long long wait so that we can easily issue a signal to this process
    while (1) {
        sleep(1);
    }
    return 0;
}