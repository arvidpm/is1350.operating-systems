/* 
Arvid Persson Moosavi
amoosavi@kth.se

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

void usr1b(int sig) {

	static int mycounter = 0;

    /* increment counter everytime SIGUSR1 is recieved */
    if (sig == SIGUSR1) {
        mycounter++;
        printf("Count: %d\r", mycounter);
        /* flushing buffer */
        fflush(stdout);
    }
}

/* Handles SIGUSR1 signals */
void usr1a(int sig) {

    if (sig == SIGUSR1) {
        /* Changing subscribtion from usr1a to usr1b */
        if (signal(SIGUSR1, usr1b) == SIG_ERR)
            /* if unsuccessful */
            printf("\nproblem changing to usr1b\n");
    }
}

/* Handles SIGUSR2 signals */
void usr2(int sig) {

    if (sig == SIGUSR2){
        printf("\nI'll handle this as well.");
    }
}

int main(void) {

    /* Ignores SIGINT signals, print if error returned */
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        printf("\ncan't ignore SIGINT\n");

    /* Handles SIGUSR1 signals, print if error returned */
    if (signal(SIGUSR1, usr1a) == SIG_ERR)
        printf("\ncan't handle SIGUSR1\n");

    /* Handles SIGUSR2 for fun */
    if (signal(SIGUSR2, usr2) == SIG_ERR)
        printf("\ncan't handle SIGUSR2\n");


    printf("Send some signal to process %d\n", getpid());

    /* Eternal sleep */
    while(1)
        sleep(1);
    return 0;
}