
/*
 * proj : IPC sample code
 * file : error.h
 * desc : error handling routine definitions
 * rcs  : $Id$
 *
 */

#ifndef ERROR_H
#define ERROR_H

#define SYSTEMERROR (int) -1
#define SYSTEMPTRERROR (void*) -1

/*#define ERROR 0*/
#define NOK   0
#define OK    1

int openErrorFile (const char *aFileName) ;
  /* input   : filename where to write the errorfile to.
   *           the filename may not contain characters like ~
   * returns : -
   * output  : -
   * desc    : opens up an error file
   */

void closeErrorFile (void);
  /* input   : -
   * returns : -
   * output  : -
   * desc    : closes errorfile.
   */

void debug (const char *aFilename, const int aLine, const char *aFunction, const char *aMsg) ;
  /* input   : filename, linenumber within filename, functionname and the systemcall
   * returns : -
   * output  : -
   * desc    : Kindda my standard way to report crud, info is the other one, if
   *           this does not fit the purpose right.
   */

void info (char *aFormatString, ...) ;
  /* input   : see printf, same idea.
   * returns : -
   * output  : -
   * desc    : dumps a message to the same place errors go.
   *           this is more for debugging purposes.
   */

void systemError (const char *aFilename, const int aLine, const char *aFunction, 
		  const char *aSyscall) ;
  /* input   : filename, linenumber within filename, functionname and the systemcall
   * returns : -
   * output  : -
   * desc    : In case of an error due to system failures.
   */

#endif
