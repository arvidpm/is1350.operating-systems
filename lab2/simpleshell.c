#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_LENGTH 1024
/* 
    ' '      space 
    '\t'     horizontal tab
    '\r'     carriage return   
    '\n'     newline
*/
#define REGEX " \t\r\n"

int main(int argc, char *argv[]) {
  char *cmd;
  char line[MAX_LENGTH];
  int a;
  
  /* Mimic */
  printf("starting super-shell...\n\n");
  sleep(1);

  while (1) {
    printf("$ ");
    if (!fgets(line, MAX_LENGTH, stdin))
      break;

    if(strncmp(line, "exit", 4) == 0) {
      printf("bye.\n");
      break;
    }
        
    else if(strncmp(line, "cd ", 3) == 0) {
      cmd = strtok(line, REGEX);

      if (!(cmd = strtok(NULL, REGEX)))
        printf("No path with cd\n");
            
      else if(!chdir(cmd)){

        write(1, "dir changed to ", 15);
        system("pwd");
        
      }
            
      else
        printf("no such dir\n");
    }
    
    else{
      a = system(line);
    }

    /* The return value includes both the exit status 
    of the process (top 8 bits) and the signal # that killed 
    the process (if any, lower 8 bits). 
    32512 == 127 << 8, so the shell exit code was 127. */

    if (a == 32512) {
      printf("system() failed.\n");
    }
  }

  return 0;
}