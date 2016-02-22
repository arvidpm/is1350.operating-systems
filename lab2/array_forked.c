/*
Created by arvid on 2016-02-10.

https://en.wikipedia.org/wiki/Fork_(system_call)
http://www.tutorialspoint.com/cprogramming/c_pointer_to_an_array.htm
http://linux.die.net/man/2/waitpid

*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void simplePrint(int size, int *array){

  printf("\nMain process ID: %d\n", getpid());
  printf("\nPrinting elements using single process\n");
  printf("----------------------------------------------\n\n");

  for (int i = 0; size > i ; i++ ) {

    printf("Parent print element %d: %d\n",i , array[i] );

  }
}

void forkedPrint(int size, int *array){

  /* output each array element's value */
  printf("\nPrinting elements using child processes\n");
  printf("----------------------------------------------\n\n");
   
  for(int i = 0; size > i; i++ ) {

    pid_t pid = fork();

    if (pid == -1) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
   
    else if (pid == 0) {
      printf("Child #%d PID: %d\n", i, getpid());
      printf("Element: %d : %d\n\n",  i, array[i]);
      _exit(EXIT_SUCCESS);
    }

    else {
      int status;
      (void)waitpid(pid, &status, 0);
    }
  }
}

int main(void) {

  /* an array with 5 elements */
  int array[5] = {10, 20, 30, 40, 50};
  int size = sizeof(array) / sizeof(int);

  //simplePrint(size, array);
  forkedPrint(size, array);

  return EXIT_SUCCESS;
}