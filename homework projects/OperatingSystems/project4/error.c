/*
 * proj : IPC sample code
 * file : error.c
 * desc : Error message handling routines
 * rcs  : $Id$
 *
 *
 * $Log$
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "error.h"


/*
 * sunOs 4.blah does not have strerror, so we'll make one.
 */

#ifdef sun4

extern char *sys_errlist[];
#define strerror(no)(sys_errlist[no])

#endif


FILE* where;


int openErrorFile (const char *aFileName) {
  /* input   : filename where to write the errorfile to.
   *           the filename may not contain characters like ~
   * returns : -
   * output  : -
   * desc    : opens up an error file
   */

  //  static FILE* where = stderr;
  if (!strncmp (aFileName, "stdout", 7)) {
    
    where = stdout;
    return OK;
	
  }

  if (!strncmp (aFileName, "stderr", 7)) {

    where = stderr;
    return OK;

  }

  if ((where = fopen (aFileName, "w")) != (FILE*)NULL)
    return OK;

  return NOK;

}



void closeErrorFile (void){
  /* input   : -
   * returns : -
   * output  : -
   * desc    : closes errorfile.
   */

  fclose (where);

}



void debug (const char *aFilename, const int aLine, const char *aFunction, const char *aMsg) {
  /* input   : filename, linenumber within filename, functionname and the systemcall
   * returns : -
   * output  : -
   * desc    : Kindda my standard way to report crud, info is the other one, if
   *           this does not fit the purpose right.
   */

  fprintf (where, "debug : %s\n", strerror(errno));
  fprintf (where, "File : %s   Line : %d   Function : %s   Msg : %s\n",aFilename,aLine,aFunction,
	   aMsg);
  fflush (where);

}



void info (char *aFormatString, ...) {
  /* input   : see printf, same idea.
   * returns : -
   * output  : -
   * desc    : dumps a message to the same place errors go.
   *           this is more for debugging purposes.
   */

  va_list args;

  va_start (args, aFormatString);
  vfprintf (where, aFormatString, args);
  va_end (args);
  fflush (where);

}



/*
 * Error handling stuff.
 */

void systemError (const char *aFilename, const int aLine, const char *aFunction, const char *aSyscall) {
  /* input   : filename, linenumber within filename, functionname and the systemcall
   * returns : -
   * output  : -
   * desc    : In case of an error due to system failures.
   */

  fprintf (where, "System : %s\n", strerror(errno));
  fprintf (where, "File : %s   Line : %d   Function : %s   SysCall : %s\n",aFilename,aLine,aFunction, aSyscall);
  fflush (where);

  errno = 0;

}
