/* 
Arvid Persson Moosavi
amoosavi@kth.se
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv) {

    /* Creating a pipe array */
    int fd[2];
    
    /* pipe()  creates a pipe, a unidirectional data channel that can be 
    used for interprocess communication. The array pipefd is used to 
    return two file descriptors referring to the ends of the pipe.
    
    fd[0] refers to the read end of the pipe.  
    fd[1] refers to the write end of the pipe.*/
    pipe(fd);

    /* Declarations */
    pid_t pid1, pid2;

    /* Forking first child */
    pid1 = fork();

    if (pid1 == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
   
    else if (pid1 == 0) {

        /* First child preparing first part of the pipe */
        printf("\nChild 1: preparing 'ls -la'\n");
        printf("------------------------------\n");

        /* Closing read end of the pipe. */
        close(fd[0]);

        /* Duplicate a file descriptor.
        The dup2() system call performs the same task as dup(), 
        but instead of using the lowest-numbered unused file descriptor, 
        it uses the descriptor number specified in newfd (=STDOUT). */
        dup2(fd[1], 1);
        
        /* int execlp(const char *file, const char *arg, ...);
        The  execlp(), execvp(), and execvpe() functions duplicate 
        the actions of the shell in searching for an executable file 
        if the specified filename does not contain a '(/)' character.*/
        execlp("ls", "ls", "-la", NULL);

        /* Closing write end of the pipe. */
        close(fd[1]);
    }

    else {

        /* Forking second child */
        sleep(1);
        pid2 = fork();

        if (pid2 == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    
        else if (pid2 == 0) {

            /* Second child preparing first part of the pipe */
            printf("\nChild 2: preparing 'less'\n");
            printf("------------------------------\n");

            /* Closing write end of the pipe. */
            close(fd[1]);

            /* Duplicate a file descriptor.
            The dup2() system call performs the same task as dup(), 
            but instead of using the lowest-numbered unused file descriptor, 
            it uses the descriptor number specified in newfd (=STDIN). */
            dup2(fd[0], 0);
            
            /* int execlp(const char *file, const char *arg, ...);
            The  execlp(), execvp(), and execvpe() functions duplicate 
            the actions of the shell in searching for an executable file 
            if the specified filename does not contain a '(/)' character.*/
            execlp("less", "less", NULL);

            /* Closing read end of the pipe. */
            close(fd[0]);

        }

    }

    /* Closing both ends for parent since children are doing the work*/
    close(fd[0]);
    close(fd[1]);

    /* Waiting for children to complete */
    wait(&pid1);
    wait(&pid2);

    return 0;
    
}