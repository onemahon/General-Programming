# makefile for OS Exercises: exercise 9
#
#  author:  Dr Ian G Graham - Ian.Graham@griffith.edu.au
#  date:    December 2003
#

CC=gcc
CFLAGS= -Wall 
PROGNAME=hostd
SOURCE=$(PROGNAME).c pcb.c
INCLUDE=$(PROGNAME).h pcb.h

$(PROGNAME): $(SOURCE) $(INCLUDE) makefile process
	$(CC) $(CFLAGS) $(SOURCE) -o $@

debug: $(SOURCE) $(INCLUDE) makefile process
	$(CC) $(CFLAGS) -g -D DEBUG $(SOURCE) -o $(PROGNAME)

process: sigtrap.c
	$(CC) $(CFLAGS) $+ -o $@
