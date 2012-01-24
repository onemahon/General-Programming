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

#define BUFSIZE 20

int clonedir(char *sourcename, char *targetname, char *avoidstring);
int emptydir(char *dirname);

int main(int argc, char* argv[]){
	/*
	 CLONE DIRECTORY 
	 
	 clone from source to target
	 (decide which comes first in args)
	 
	 recursively cycle through everything in source:
	 for each file, read it into a new file of the same name and permissions
	 for each dir, simply call mkdir on that pathName (altered to that of the target)
	 
	 all done in paths:
	 hold onto the "target" string as the beginning of the clone directory
	 i.e. for everything that needs to be done to a file in testdir/test3, while being moved to test2dir, use the path test2dir/testdir/test3 as location from executable file
	 
	 1
	 
	 
	 */
	
	if (argc < 3) {
		puts("Please specify a source and target directory name.");
		exit(1);
	}
	
	
	char *sourcename = argv[1];
	char *targetname = argv[2];
	
	struct stat sourceinfo;
	stat(sourcename, &sourceinfo);
	
	//if argument is not file	or	argument does not exist
	if (!S_ISDIR(sourceinfo.st_mode)){
		puts("Error: please select a directory.");
		exit(1);
	}
	
	
	if(emptydir(targetname) == 1) {
		if (rmdir(targetname) == -1) {
			perror(targetname);
			exit(EXIT_FAILURE);
		}
	}
	if(clonedir(sourcename, targetname, targetname) != 1){
		puts("error in clonedir.");
	}
	return 1;
}


int clonedir(char *sourcename, char *targetname, char *avoidstring){	
	DIR *target;
	DIR *source;
	
	struct stat targetinfo;
	stat(targetname, &targetinfo);
	//printf(">>>>>>>>checking target: %s\nwhich has st_mode of %d\n", targetname, targetinfo.st_mode);
	
	target = opendir(targetname);
	if(target == NULL || access(targetname, W_OK) != 0){
		if (errno == EACCES) {
			printf("%s%s\n", errno == EACCES ? "You don't have write access for target directory " : "another error in target dir ", targetname);
			return 1;
		}else {
			puts("making new directory...");
			mkdir(targetname, S_IRWXU);
			target = opendir(targetname);
		}
	}

	
	
	source = opendir(sourcename);
	if (source == NULL){
		printf("%s%s\n", errno == EACCES ? "You don't have write access for source directory " : "another error in source dir ", sourcename);
		return 1;
	}
	
	if (access(sourcename, W_OK) != 0) {
		printf("can't access %s\n",sourcename);
		if (rmdir(targetname) == -1) {
			perror(targetname);
			exit(1);
		}
		return 1;
	}
	
	//if you have access, cycle through all of its contents and clone each of them into whereever they belong now
	struct dirent srcEntry;
	struct dirent *srcEntryPtr = NULL;
	int retval = 0;
	char sourcePath[PATH_MAX + 1], targetPath[PATH_MAX + 1];
	retval = readdir_r(source, &srcEntry, &srcEntryPtr);
	while (srcEntryPtr != NULL) {
		//foreach directory entry that doesn't begin in . or .., remove it if possible (i.e. if not locked and read/writeable)
		struct stat entryInfo;
		
		if( ( strncmp( srcEntry.d_name, ".", PATH_MAX ) == 0 ) ||
		   ( strncmp( srcEntry.d_name, "..", PATH_MAX ) == 0 )	 ) {
			/* this avoids the . and .. entries. */
			retval = readdir_r(source, &srcEntry, &srcEntryPtr );
			//puts("should break here \n");
			//printf("============= on %s\n", srcEntry.d_name);
			continue;
		}
		strncpy(sourcePath,sourcename,PATH_MAX );
		strncat(sourcePath,"/",PATH_MAX );
		strncat(sourcePath,srcEntry.d_name,PATH_MAX );			
		
		strncpy(targetPath,targetname,PATH_MAX );
		strncat(targetPath,"/",PATH_MAX );
		strncat(targetPath,srcEntry.d_name,PATH_MAX );
		
		if (strcmp(sourcePath, avoidstring) == 0) {
			//avoiding the destination folder
			retval = readdir_r(source, &srcEntry, &srcEntryPtr);
			continue;
		}
		
		printf("......attempting to clone %s to %s...\n", sourcePath, targetPath);
		
		if(stat(sourcePath, &entryInfo) == 0){
			if (S_ISREG(entryInfo.st_mode)){
				//check file's perimssions, if it's allowed, go ahead and copy it to targetPath
				
				
				
				
				
				int fd = open(sourcePath, O_RDWR);
				if(fd < 0){
					printf("%s was not opened. -- ", sourcePath);
					printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
					retval = readdir_r(source, &srcEntry, &srcEntryPtr);
					continue;
				}else {
					//file opened OK
					int fdt = open(targetPath, O_RDWR | O_CREAT);
					
					if(fdt < 0){
						printf("%s was not opened. -- ", targetPath);
						printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
						retval = readdir_r(source, &srcEntry, &srcEntryPtr);
						continue;
					}
					
					struct flock fl;
					fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
					fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
					fl.l_start  = 0; /* Offset from l_whence */
					fl.l_len    = 0; /* length, 0 = to EOF */
					if (fcntl(fd, F_SETLK, &fl) == -1){
						printf("%s is locked. -- ", sourcePath);
						printf("%s\n", errno == EACCES || errno == EAGAIN ? "You can't duplicate this file, it's in use by another process":"another error occurred");
						printf("%d\n",errno);
						fcntl(fd, F_GETLK, &fl) ;
						printf("%d\n",fl.l_pid);
						retval = readdir_r(source, &srcEntry, &srcEntryPtr);
						continue;
					}else {
						//printf("reading %s and writing to %s\n", sourcePath, targetPath);

						//read source file and write to target file
						char buf[BUFSIZE];
						
						while (read(fd, buf, BUFSIZE) != 0) {
							//every time we read in a byte, write it out to the destination file
							write(fdt, buf, BUFSIZE);
						}
						
						//get file permission information from sourcePath
						//resetting file permissions
						chmod(targetPath, entryInfo.st_mode);
						chown(targetPath, entryInfo.st_uid, entryInfo.st_gid);
						
						close(fd);
						close(fdt);
					}
				}
			}else if (S_ISLNK(entryInfo.st_mode)) {
				//ditto to above
				char tName[PATH_MAX + 1];
				
				if(readlink(sourcePath, tName, PATH_MAX) != -1){
					int fd = open(tName, O_RDWR);
					if(fd < 0){
						printf("%s was not opened. -- ", tName);
						printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
						retval = readdir_r(source, &srcEntry, &srcEntryPtr);
						continue;
					}else {
						//file opened OK
						int fdt = open(targetPath, O_RDWR | O_CREAT);
						
						if(fdt < 0){
							printf("%s was not opened. -- ", targetPath);
							printf("%s\n", errno == EACCES ? "User does not have permission to write this file." : "Something went wrong.");
							retval = readdir_r(source, &srcEntry, &srcEntryPtr);
							continue;
						}
						
						struct flock fl;
						fl.l_type   = F_WRLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK */
						fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
						fl.l_start  = 0; /* Offset from l_whence */
						fl.l_len    = 0; /* length, 0 = to EOF */
						if (fcntl(fd, F_SETLK, &fl) == -1){
							printf("%s is locked. -- ", tName);
							printf("%s\n", errno == EACCES || errno == EAGAIN ? "You can't remove this file, it's in use by another process":"another error occurred");
							printf("%d\n",errno);
							fcntl(fd, F_GETLK, &fl) ;
							printf("%d\n",fl.l_pid);
							retval = readdir_r(source, &srcEntry, &srcEntryPtr);
							continue;
						}else {
							//printf("reading %s and writing to %s\n", tName, targetPath);
							
							//read source file and write to target file
							char buf[BUFSIZE];
							
							while (read(fd, buf, BUFSIZE) != 0) {
								//every time we read in a byte, write it out to the destination file
								write(fdt, buf, BUFSIZE);
							}
							
							//get file permission information from tName
							//resetting file permissions
							chmod(targetPath, entryInfo.st_mode);
							chown(targetPath, entryInfo.st_uid, entryInfo.st_gid);
							
							close(fd);
							close(fdt);
						}
					}
				}else {
					printf("%s: invalid symbolic link prevented deletion.", targetPath);
					retval = readdir_r(source, &srcEntry, &srcEntryPtr);
					continue;
				}
			}else if (S_ISDIR(entryInfo.st_mode)) {
				//clone it - clone function will check perimssions and pre-existance				
				if(clonedir(sourcePath, targetPath, avoidstring) != 1){
					printf("issue in cloning directory %s to %s", sourcePath, targetPath);
				}
			}
		}else {
			printf("stat() broke on directory %s\n", sourcePath);
			exit(1);
		}
		retval = readdir_r(source, &srcEntry, &srcEntryPtr);
	}
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




