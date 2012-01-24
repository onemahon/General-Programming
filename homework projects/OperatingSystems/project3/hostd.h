#ifndef GHOST_H
#define GHOST_H
/*******************************************************************
   include file for HOST dispatcher

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "pcb.h"
#include "mab.c"

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define DEFAULT_NAME "hostd"
#define QUANTUM  1
#endif
