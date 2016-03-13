/* 
Arvid Persson Moosavi
amoosavi@kth.se
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


void child1(int *fd) {

    /* First child preparing first part of the pipe */
    printf("\nChild 1: preparing 'ls -la'\n");
    printf("------------------------------\n");


    pid_t pid1 = fork();

    if (pid1 == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
   
    else if (pid1 == 0) {

        /* Closing read end of the pipe. */
        close(fd[0]);

        /* Duplicate a file descriptor.
        The dup2() system call performs the same task as dup(), 
        but instead of using the lowest-numbered unused file descriptor, 
        it uses the descriptor number specified in newfd (=STDOUT). */
        dup2(fd[1],STDOUT_FILENO);

        /* Closing write end of the pipe. */
        close(fd[1]);

        /* int execlp(const char *file, const char *arg, ...);
        The  execlp(), execvp(), and execvpe() functions duplicate 
        the actions of the shell in searching for an executable file 
        if the specified filename does not contain a '(/)' character.*/
        execlp("ls", "ls", "-la", NULL);

        _exit(EXIT_SUCCESS);
    }

    
}

void child2(int *fd) {

    /* First child preparing first part of the pipe */
    printf("\nChild 2: preparing 'less'\n");
    printf("------------------------------\n");


    pid_t pid2 = fork();

    if (pid2 == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
   
    else if (pid2 == 0) {

        /* Closing write end of the pipe. */
        close(fd[1]);

        /* Duplicate a file descriptor.
        The dup2() system call performs the same task as dup(), 
        but instead of using the lowest-numbered unused file descriptor, 
        it uses the descriptor number specified in newfd (=STDIN). */
        dup2(fd[0], STDIN_FILENO);

        /* Closing read end of the pipe. */
        close(fd[0]);

        /* int execlp(const char *file, const char *arg, ...);
        The  execlp(), execvp(), and execvpe() functions duplicate 
        the actions of the shell in searching for an executable file 
        if the specified filename does not contain a '(/)' character.*/
        execlp("less", "less", NULL);
        //execl("/bin/less", "less", NULL);

        printf("\nFinished 'ls -la | less'\n");
        _exit(EXIT_SUCCESS);
    }
}

int main(int argc, char **argv) {
    
    int fd[2];
    
    /* pipe()  creates a pipe, a unidirectional data channel that can be 
    used for interprocess communication. The array pipefd is used to 
    return two file descriptors referring to the ends of the pipe.
    
    fd[0] refers to the read end of the pipe.  
    fd[1] refers to the write end of the pipe.*/
    pipe(fd);

    /* Closing both ends for parent since children are doing the work*/
    close(fd[0]);
    close(fd[1]);

    child1(fd);
    child2(fd);

    exit(0);
}