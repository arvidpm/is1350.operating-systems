/*

Created by arvid on 2016-02-10.

Get process ID: http://ubuntuforums.org/showthread.php?t=1430052

Changing Priority on Linux Processes:
https://www.nixtutor.com/linux/changing-priority-on-linux-processes/
http://pubs.opengroup.org/onlinepubs/009695399/functions/getpriority.html

SYNOPSIS
       #include <sys/time.h>
       #include <sys/resource.h>

       int getpriority(int which, id_t who);
       int setpriority(int which, id_t who, int prio);

*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

// int val is user input for child priority
int main(int argc, char* argv[]){

  if (argc <= 1) {

    perror("No arguments were given, please use './priority x (x = -20 to 19)'");
    exit(EXIT_FAILURE);
  }

  int val = strtol(argv[1], NULL, 0);

  // Parent PID and Priority fetched
	int p_pid = getpid();
	int p_prio = getpriority(PRIO_PROCESS, 0);

  // Child process created
	pid_t pid = fork();

  if (pid == -1) {
   	perror("fork failed");
    exit(EXIT_FAILURE);
  }

  else if (pid == 0) {

    // Setting child priority to input argument
    printf("\n...setting child priority to: %d\n\n", val );
    setpriority(PRIO_PROCESS, 0, val);
    sleep(1);

    // Child PID and Priority fetched
    int c_prio = getpriority(PRIO_PROCESS, 0);
    int c_pid = getpid();

  	// Parent print
    printf( "Parent: My PID is '%d' and my priority is '%d'.\n",
		p_pid, p_prio );

		// Child print
    printf( "Child: My parent's PID is '%d', my own PID is '%d', and my priority is '%d'.\n\n",
    p_pid, c_pid, c_prio);

  }

  else {
   	int status;
   	(void)waitpid(pid, &status, 0);

  }

  // Endless loop to confirm set priorities
  while(1);
	return EXIT_SUCCESS;

}