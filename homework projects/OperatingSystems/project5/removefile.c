/*
 *  Created by Joe on 12/9/11.
 *  Copyright 2011 Boston College. All rights reserved.
 */

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	
	
	/*
	 
	 REMOVE FILE
	 
	 1. ensure there is an argument given
	 2. ensure that argument exists and is a file (not dir)
	 3. check if it is locked, i.e. open already
	 4. check permissions
	 if everything goes through, unlink it
	 
	 check that there is an argument
	 chech that there is something called that argument
	 check that it is a file
	 check that you have permissions
	 check if you can get a lock on it
	 
	
	 */
	
	
	if (argc < 2) {
		puts("Please specify a file name");
		exit(1);
	}
	
	char *filename = argv[1];
	struct stat info;
	stat(filename, &info);
	
	
	//if argument is not file	or	argument does not exist
	if (!S_ISREG(info.st_mode)){
		puts("Please select a file, not a directory.");
		exit(1);
	}
	
	//check if file exists and can be opened by user
	int fd = open(argv[1], O_RDWR);
	if(fd < 0){
		//file will not open, no permissions for it	
		printf("%s\n", errno == EACCES ? "You don't have read/write permission for that file.":"An error occured: check that the entry was correct.");
		exit(1);
	}

	//check if locked by OS:
	struct flock fl;
	fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
	fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
	fl.l_start  = 0; /* Offset from l_whence */
	fl.l_len    = 0; /* length, 0 = to EOF */
	
	
	if (fcntl(fd, F_SETLK, &fl) == -1){
		//file locked (probably)
		printf("%s\n", errno == EACCES || errno == EAGAIN ? "You can't remove this file, it's in use by another process":"another error occurred");
		printf("%d\n",errno);
		fcntl(fd, F_GETLK, &fl) ;
		printf("%d\n",fl.l_pid);
		exit(1);
	}
	
	//it's all good, remove file from current directory
	if (unlink(argv[1]) == -1) {
		perror(argv[1]);
		exit(EXIT_FAILURE);
	}
	
	return 1;
}

