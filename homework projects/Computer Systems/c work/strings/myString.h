/*
 * Joe Mahon
 */

#include <stdlib.h>
#include <stdio.h>

typedef struct character{
	int count;
	char letter;
}character_t;

char *fetchString(); //asks for and enters a string into memory from the user's command
int countChars(char *s, char c); //counts a char c for number of occurances within string s
char favChar(char *s); //finds out which char has the most friends; returns it
int compare(char *s1, char *s2); //returns 1 if string s1 is alphabetically less than (BEFORE) string s2, 0 if they are the SAME, and +1 if s1 would go AFTER s2
char *frankenString(char *s1, char *s2); //concatenates s2 onto the end of s1
char *removeChar(char c, char *s); //removes any instance of char c from string s
int strlength(char *s);
int charDNE(char c, character_t *list, int length);


/* 
To create a new string in any situation,
allocate new char space on the heap, and
realloc to add more characters individually

examples: fetchString should realloc according
to how much the user types in, so there's never
too much space in memory.
removeChar should allocate a char* space on the
heap, and then add each character individually
(while checking to see if it = char c, in which
case skip it). Return the new char * to the beginning
of the string.
It's up to the user to free the old string at the
end of their program. 
*/


