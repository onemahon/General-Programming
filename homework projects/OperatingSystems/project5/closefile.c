#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
 char *fn = argv[1]; //the test file
	int signo;
	int signum = SIGUSR1;
	sigset_t sigset;
struct flock fl;
int fd = open(fn, O_RDWR);
fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
fl.l_start  = 0; /* Offset from l_whence */
fl.l_len    = 0; /* length, 0 = to EOF */


if (fcntl(fd, F_SETLK, &fl) == -1){
	printf("%s\n", errno == EACCES || errno == EAGAIN ? "Closing file.":"another error occurred");
	printf("%d\n",errno);
	fcntl(fd, F_GETLK, &fl) ;
	printf("%d\n",fl.l_pid);
	kill((int)fl.l_pid,SIGUSR1); //send SIGUSR1 to test1
	
	}
else
    printf("\n");

 close(fd);
 return 0;
}
