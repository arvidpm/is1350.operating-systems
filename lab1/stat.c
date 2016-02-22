#include <stdlib.h>
#include <sys/stat.h>

// Import external resource statPrint from libstat-64.o
extern int statPrint(char*,struct stat*);

// Initializing main
int main(int argc, char* argv[]) {

	// Creating stat structure named sb
	struct stat sb;

	// Sys-call stat with parameters (shell input, struct address)
	stat(argv[1],&sb);

	// statPrint call with parameters (shell input, struct address)
	statPrint(argv[1],&sb);

	// Exit
	exit(EXIT_SUCCESS);
}
