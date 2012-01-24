CC = gcc
CCOPTS = -c -g
LINKOPTS = -g

all: shell

shell: shell.o 
	$(CC) $(LINKOPTS) -o $@ $^

shell.o: shell.c 
	$(CC) $(CCOPTS) -o $@ shell.c

clean:
	rm -rf *.o shell shell.exe *~
