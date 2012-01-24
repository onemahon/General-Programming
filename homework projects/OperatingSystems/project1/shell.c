/* Simple Shell */

/* LIBRARY SECTION */
#include <ctype.h>              /* Character types                       */
#include <stdio.h>              /* Standard buffered input/output        */
#include <stdlib.h>             /* Standard library functions            */
#include <string.h>             /* String operations                     */
#include <sys/types.h>          /* Data types                            */
#include <sys/wait.h>           /* Declarations for waiting              */
#include <unistd.h>             /* Standard symbolic constants and types */

/* DEFINE SECTION */
#define SHELL_BUFFER_SIZE 256   /* Size of the Shell input buffer        */
#define SHELL_MAX_ARGS 8        /* Maximum number of arguments parsed    */
#define MAX_SHELL_DEPTH 4        

/* VARIABLE SECTION */
enum { STATE_SPACE, STATE_NON_SPACE };	/* Parser states */

void Handler( int signo, siginfo_t *si, void *context );

typedef struct command {
	int id;
	
	char *buffer;
	
	struct command * next;
}command_t;

void killList(command_t *a);
char *getBuffer(char arg[SHELL_BUFFER_SIZE], command_t *top);
char *getListItem(int index, command_t *m);


int shell_depth = 0;

int imthechild(const char *path_to_exec, char *const args[])
{
	shell_depth++;
	if(path_to_exec != NULL){
		return execv(path_to_exec, args) ? -1 : 0;
	}else {
		puts("Creating new shell...");
		if(shell_depth < MAX_SHELL_DEPTH){
			return main(1, NULL);
		}else{
			puts("Too many layers of shells! Exit some.");
			return 0;
		}
	}
}

void imtheparent(pid_t child_pid, int run_in_background)
{
	int child_return_val, child_error_code;
	
	/* fork returned a positive pid so we are the parent */
	fprintf(stderr,
	        "  Parent says 'child process has been forked with pid=%d'\n",
	        child_pid);
	if (run_in_background) {
		fprintf(stderr,
		        "  Parent says 'run_in_background=1 ... so we're not waiting for the child'\n");
		return;
	}
	waitpid(child_pid,&child_return_val, 0);
	/* Use the WEXITSTATUS to extract the status code from the return value */
	child_error_code = WEXITSTATUS(child_return_val);
	fprintf(stderr,
	        "  Parent says 'waitpid() returned so the child with pid=%d is finished.'\n",
	        child_pid);
	if (child_error_code != 0) {
		/* Error: Child process failed. Most likely a failed exec */
		fprintf(stderr,
		        "  Parent says 'Child process %d failed with code %d'\n",
		        child_pid, child_error_code);
	}
}

/* MAIN PROCEDURE SECTION */
int main(int argc, char **argv)
{
	pid_t shell_pid, pid_from_fork;
	int n_read, i, exec_argc, parser_state, run_in_background, command_count;
	/* buffer: The Shell's input buffer. */
	char buffer[SHELL_BUFFER_SIZE];
	/* exec_argv: Arguments passed to exec call including NULL terminator. */
	char *exec_argv[SHELL_MAX_ARGS + 1];
	
	command_t *top;
	top = (command_t *)malloc(sizeof(command_t));
	top->id = 0;
	top->buffer = (char *)malloc(sizeof(char)*2);
	(top->buffer)[0] ='\0';
	top->next = NULL;
	
	struct sigaction SA;
    SA.sa_sigaction = Handler;
	sigaction( SIGINT, &SA, NULL );

	
//	command_t *top_commands = (command_t *)malloc(sizeof(command_t));
//	top_commands.id = 0;
//	
//	command_t *ptr_commands;
	
	/*******
	 
	 Best way to implement?
	 Two things: 
		keeping all the previously used commands in some kind of memory
		being able to call the code that parses and executes the command line rather than only doing it when the input comes through the buffer
	 Array or list of command_t structs, which have the information about the command
	 but how to store different kinds of argument lists?
	 
	 *******/
	
	/* Allow the Shell prompt to display the pid of this process */
	shell_pid = getpid();
	command_count = 1;
	
	char *path = (char *)malloc(sizeof(char)*5);
	strcpy(path, "/bin/");
	
	command_t *ptr = top;

	while (1) {
	/* The Shell runs in an infinite loop, processing input. */

		fprintf(stdout, "Shell(pid=%d)%i> ", shell_pid, command_count);
		fflush(stdout);

		/* Read a line of input. */
		if (fgets(buffer, SHELL_BUFFER_SIZE, stdin) == NULL)
			return EXIT_SUCCESS;
		n_read = strlen(buffer);
		run_in_background = n_read > 2 && buffer[n_read - 2] == '&';
		buffer[n_read - run_in_background - 1] = '\n';
		

		
		//add the command to the list of commands
		//later: free the list entirely

		/* Parse the arguments: the first argument is the file or command *
		 * we want to run.                                                */
	buff:
		
		if((buffer[0] == '\n')){
			continue;
		}
		
		if((exec_argv[0])[0] == '!'){
			//replace "buffer[]" with the buffer from the linked list
			strcpy(buffer, getBuffer(exec_argv[0], top));
			//return and do this again, but with the new buffer
//			printf("New buffer: %s", buffer);
			goto buff;
		}else{
			//puts(" NO Excl");
		}

		command_t *newcmd = (command_t *)malloc(sizeof(command_t));
		newcmd->id = command_count;
		newcmd->buffer = (char *)malloc(SHELL_BUFFER_SIZE);
		strcpy(newcmd->buffer, buffer);
//		printf("adding buffer %s to newcmd->buffer at id=%d\n", buffer, newcmd->id);
		newcmd->next = NULL;
		ptr->next = newcmd;
		ptr = newcmd;
		
		
		parser_state = STATE_SPACE;
		for (exec_argc = 0, i = 0;
		     (buffer[i] != '\n') && (exec_argc < SHELL_MAX_ARGS); i++) {

			if (!isspace(buffer[i])) {
				if (parser_state == STATE_SPACE)
					exec_argv[exec_argc++] = &buffer[i];
				parser_state = STATE_NON_SPACE;
			} else {
				buffer[i] = '\0';
				parser_state = STATE_SPACE;
			}
		}
		
		
		/* run_in_background is 1 if the input line's last character *
		 * is an ampersand (indicating background execution).        */


		buffer[i] = '\0';	/* Terminate input, overwriting the '&' if it exists */

		/* If no command was given (empty line) the Shell just prints the prompt again */
//		if (!exec_argc)
//			continue;

		/* Terminate the list of exec parameters with NULL */
		exec_argv[exec_argc] = NULL;

		/* If Shell runs 'exit' it exits the program. */
		if (!strcmp(exec_argv[0], "exit")) {
			printf("Exiting process %d\n", shell_pid);
			return EXIT_SUCCESS;	/* End Shell program */

		} else if (!strcmp(exec_argv[0], "cd") && exec_argc > 1) {
		/* Running 'cd' changes the Shell's working directory. */
			/* Alternative: try chdir inside a forked child: if(fork() == 0) { */
			if (chdir(exec_argv[1]))
				/* Error: change directory failed */
				fprintf(stderr, "cd: failed to chdir %s\n", exec_argv[1]);	
			else{
				command_t *new = (command_t *)malloc(sizeof(command_t));
				command_count++;
			}
			
			/* End alternative: exit(EXIT_SUCCESS);} */

		} else if(!(strcmp(exec_argv[0], "sub"))){
			command_count++;

			pid_from_fork = fork();
			if (pid_from_fork < 0) {
				fprintf(stderr, "fork failed\n");
				continue;
			}
			if (pid_from_fork == 0) {
				return imthechild(NULL, NULL);
			} else {
				imtheparent(pid_from_fork, run_in_background);
			}
			
		}else {
		/* Execute Commands */
			/* Try replacing 'fork()' with '0'.  What happens? */
			pid_from_fork = fork();
			command_count++;
			char cmd[strlen(path)+strlen(exec_argv[0])];
			strcpy(cmd, path);
			strcat(cmd, exec_argv[0]);
			if (pid_from_fork < 0) {
				/* Error: fork() failed.  Unlikely, but possible (e.g. OS *
				 * kernel runs out of memory or process descriptors).     */
				fprintf(stderr, "fork failed\n");
				continue;
			}
			if (pid_from_fork == 0) {
				return imthechild(cmd, &exec_argv[0]);
				/* Exit from main. */
			} else {
				imtheparent(pid_from_fork, run_in_background);
				/* Parent will continue around the loop. */
			}
		} /* end if */
	} /* end while loop */
	killList(top);
	free(path);
//	killList(top_commands);
	return EXIT_SUCCESS;
} /* end main() */


void Handler(int signo, siginfo_t *si, void *context){
    switch( signo ){
        case SIGINT:
            printf("SIGINT Caught in process %d\n", getpid());
            break;
    }
}

char *getBuffer(char arg[SHELL_BUFFER_SIZE], command_t *top){
	//cycle through arg EXCEPT for char 0 ("!")
	//add the number to a new char*
	//int index = atoi(char *)
	//go from top to the place in the linked list that the number tells you to
	//return ptr->buffer.
	
	char num[SHELL_BUFFER_SIZE];
	int i = 1;
	while(i <= strlen(arg)){
		num[i-1] = arg[i];
		i++;
	}
	
	int index = atoi(num);
	
	
	static char res[SHELL_BUFFER_SIZE];
	strcpy(res, getListItem(index, top));
	return res;
}

char *getListItem(int index, command_t *m){
	command_t *p = m;
	if(p->id == index){
//		printf("That command was: %s\n", p->buffer);
		return p->buffer;
	}else{
		return getListItem(index, p->next);
	}
	char *res = (char *)malloc(sizeof(char));
	res[0] = '\n';
	return res;
}

void killList(command_t *a){	
	if(a->next == NULL){
		
	}else{
		free(a->buffer);
		a = a->next;
		killList(a);
	}
	
}




