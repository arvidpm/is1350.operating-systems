/* 
Arvid Persson Moosavi
amoosavi@kth.se
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* pipe()  creates a pipe, a unidirectional data channel that can be 
used for interprocess communication. The array pipefd is used to 
return two file descriptors referring to the ends of the pipe.
    
pipefd[0] refers to the read end of the pipe.  
pipefd[1] refers to the write end of the pipe.*/





int main(int argc, char **argv)
{
    int n;
    int fd[2];
    char buf[1025];
    char *data = "some sample data...";
    
    /*  */



    pipe(fd);
    write(fd[1], data, strlen(data)*sizeof(char)+1);
    if ((n = read(fd[0], buf, 1024)) >= 0) {
        printf("read %d bytes from the pipe: \"%s\"\n", n, buf);
    } else {
        perror("read");
    }
    exit(0);
}