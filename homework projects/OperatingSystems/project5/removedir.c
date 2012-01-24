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
#include <string.h>
#include <fcntl.h>
#include <limits.h>

int emptydir(char *dirname);



/*
 
 TO DO:
 
 OPEN FILES CAN STILL BE DELETED, WHY????
 
 */




int main(int argc, char* argv[]){
	/*
	 REMOVE DIRECTORY 
	 */
	
	if (argc < 2) {
		puts("Please specify a directory name.");
		exit(1);
	}
	
	char *dirname = argv[1];
	struct stat info;
	stat(dirname, &info);
	
	//if argument is not file	or	argument does not exist
	if (!S_ISDIR(info.st_mode)){
		puts("Please select a directory.");
		exit(1);
	}
	
	if(emptydir(dirname) == 1) {
		if (rmdir(dirname) == -1) {
			perror(dirname);
			exit(EXIT_FAILURE);
		}
	}
	//else: directory contains at least one unremovable file or dir
	//do NOT unlink it
	
	return 1;
}


int emptydir(char *dirname){
	int deletable = 1;
	
	//try to open the directory, and empty everything in it
	DIR *dir;
	if ((dir = opendir(dirname)) == NULL) {
		printf("%s\n",errno == EACCES ? "You don't have read/write permission for this directory.":"An error occurred.");
		deletable = 0;
		
		//return instantly: do nothing inside directory
		return 0;
	}
	if (access(dirname, W_OK) != 0) {
		printf("%s\n", errno == EACCES ? "You don't have write access for that directory" : "another error");
		deletable = 0;
		
		//return instantly: do nothing inside directory
		return 0;
	}
	struct dirent entry;
	struct dirent *entryPtr = NULL;
	int retval = 0;
	unsigned count = 0;
	char pathName[PATH_MAX + 1];
	
	retval = readdir_r(dir, &entry, &entryPtr);
	while (entryPtr != NULL) {
		//foreach directory entry that doesn't begin in . or .., remove it if possible (i.e. if not locked and read/writeable)
		struct stat entryInfo;
		
		if( ( strncmp( entry.d_name, ".", PATH_MAX ) == 0 ) ||
		   ( strncmp( entry.d_name, "..", PATH_MAX ) == 0 ) ) {
			/* this avoids the . and .. entries. */
			retval = readdir_r( dir, &entry, &entryPtr );
			continue;
		}
		strncpy(pathName,dirname,PATH_MAX );
		strncat(pathName,"/",PATH_MAX );
		strncat(pathName,entry.d_name,PATH_MAX );
		//now pathName contains the "path/name" of this particular entry
		printf("......attempting to delete %s...\n", pathName);
		
		if(stat(pathName, &entryInfo) == 0){
			//print out some stats of the file/dir
			
			
			
			if (S_ISREG(entryInfo.st_mode)){
				//printf(" ------> FILE\n");
				
				//try to open file
				//check lock on file
				//unlink
				//if anything goes wrong, don't exit, but don't continue: and set deletable to 0
				
				int fd = open(pathName, O_RDWR);
				if(fd < 0){
					printf("%s was not opened or deleted. -- ", pathName);
					printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
					deletable = 0;
				}else {
					//file opened OK
					struct flock fl;
					fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
					fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
					fl.l_start  = 0; /* Offset from l_whence */
					fl.l_len    = 0; /* length, 0 = to EOF */
					if (fcntl(fd, F_SETLK, &fl) == -1){
						printf("%s is locked. -- ", pathName);
						printf("%s\n", errno == EACCES || errno == EAGAIN ? "You can't remove this file, it's in use by another process":"another error occurred");
						printf("%d\n",errno);
						fcntl(fd, F_GETLK, &fl) ;
						printf("%d\n",fl.l_pid);
						deletable = 0;
					}else {
						//printf("filepath: %s\n		filename: %s\n", pathName, entry.d_name);
						
						//OK to go
						if (unlink(pathName) == -1) {
							perror(pathName);
							deletable = 0;
						}else {
							//deletable = (deletable == 0)   1;
						}
					}
				}
			}else if (S_ISLNK(entryInfo.st_mode)) {
				//printf(" ------> LINK\n");
				
				char targetName[PATH_MAX + 1];
				if( readlink( pathName, targetName, PATH_MAX ) != -1 ) {
					
					int fd = open(targetName, O_RDWR);
					if(fd < 0){
						printf("%s was not opened or deleted. -- ", targetName);
						printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
						deletable = 0;
					}else {
						//file opened OK
						struct flock fl;
						fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
						fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
						fl.l_start  = 0; /* Offset from l_whence */
						fl.l_len    = 0; /* length, 0 = to EOF */
						if (fcntl(fd, F_SETLK, &fl) == -1){
							printf("%s is locked. -- ", targetName);
							printf("%s\n", errno == EACCES || errno == EAGAIN ? "You can't remove this file, it's in use by another process":"another error occurred");
							printf("%d\n",errno);
							fcntl(fd, F_GETLK, &fl) ;
							printf("%d\n",fl.l_pid);
							deletable = 0;
						}else {
							//OK to go
							if (unlink(targetName) == -1) {
								perror(targetName);
								deletable = 0;
							}else {
								//deletable = 1;
							}
						}
					}
				} else {
					printf("%s: invalid symbolic link prevented deletion.", pathName);
					deletable = 0;
				}
			} else if (S_ISDIR(entryInfo.st_mode)) {
				//printf(" ------> DIRECTORY\n");
				
				if (emptydir(pathName) == 1) {
					if (rmdir(pathName) == -1) {
						perror(pathName);
						deletable = 0;
					}else {
						//printf("directory path: %s\n		directory itself: %s\n", pathName, entry.d_name);
						
						//deletable = 1;
					}
				}else {
					deletable = 0;
				}
			}
		}else {
			printf("stat() went wrong on entry %s", pathName);
			deletable = 0;
		}
		retval = readdir_r(dir,&entry,&entryPtr);
		
	}
	return deletable;
}



