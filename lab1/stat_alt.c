/* Information about strmode function
http://www.freebsd.org/cgi/man.cgi?query=strmode&sektion=3&apropos=0&manpath=freebsd */

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

// Import external resource statPrint from libstat-64.o
extern int statPrint(char*,struct stat*);
extern int strmode(mode_t mode,char*);


void altStat(char * file, struct stat* item){

	/* Printing requested data from struct stat members,
	i.e. it gets the member called bar from the struct that foo points to. */
	printf("\n-----------------------------------------\n\n");
	printf("Modified information:\n\n");
	printf("File: '%s'\n", file);


	// foo->bar is equivalent to (*foo).bar
	printf("Size: %lld\t",(long long) item->st_size);
	printf("Blocks: %lld\t",(long long) item->st_blocks);
	printf("IO Block: %ld\n",(long) item->st_blksize);

	// Device print goes here
	printf("Device: %lld\t", (long long) item->st_dev);
	printf("Inode: %ld\t", (long) item->st_ino);
	printf("Links: %ld\n", (long) item->st_nlink);


	char accessString[20];
    strmode(item->st_mode, accessString);
    struct passwd *pwd;
    pwd  = getpwuid(item->st_uid);

    struct group *group;
    group = getgrgid(item->st_gid);

    printf("Access: (%04o/%.10s)  Uid: (%5i/%9s) Gid: (%5i/%9s)\n",
          item->st_mode & 07777,
          accessString,
          item->st_uid,
          pwd->pw_name,
          item->st_gid,
          group->gr_name);

    printf("Access: %s", ctime(&item->st_atime));
    printf("Modify: %s", ctime(&item->st_mtime));
    printf("Change: %s\n", ctime(&item->st_ctime));

}

// Initializing main
int main(int argc, char* argv[]) {

	// Creating stat structure named sb
	struct stat sb;

	// Sys-call stat with parameters (shell input, struct address)
	stat(argv[1],&sb);

	// statPrint call with parameters (shell input, struct address)
	statPrint(argv[1],&sb);

	// Call modified stat print
	altStat(argv[1],&sb);

	// Exit
	exit(EXIT_SUCCESS);
}