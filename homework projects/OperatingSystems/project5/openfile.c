#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

int main(int argc, char *argv[]) {
 char *fn = argv[1]; //the test file
 int fd = open(fn,O_RDWR);
	printf("fd: %d", fd);

	if(fd < 0){
		printf("ERROR -- fd: %d", fd);
		exit(1);
	}
  struct flock fl;
	int signo;
	int signum = SIGUSR1;
	sigset_t sigset;
  fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
  fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
  fl.l_start  = 0; /* Offset from l_whence */
  fl.l_len    = 0; /* length, 0 = to EOF */
  fl.l_pid    = getpid();
 int ret = fcntl(fd,F_SETLK,&fl);  //set lock so no one else can use it
 fprintf(stdout,"Opening the file, use closefile command to close it and release the lock.\n");
 fflush(stdout);
 //sleep(10); 

	if ((sigemptyset(&sigset) == -1) ||
		(sigaddset(&sigset, signum) == -1) ||
		(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1))
		perror("Failed to block signals before sigwait"); 
	if (sigwait(&sigset, &signo) == -1) {
		perror("Failed to wait using sigwait");
		return 1;
	}
	
	
 close(fd);
 return 0;
}
