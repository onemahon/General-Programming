/*
 *  Joe Mahon
 */

#include "myString.h"

char * fetchString(){
	char * s = (char *)malloc(sizeof(char));
	char c;

	int i=0;
	c = getchar();
	while(c != '\n'){
		*(s+i) = c;
		c = getchar();
		i++;
		s = (char *)realloc(s,sizeof(char)*(i+1));
	}
	*(s+i) = '\0';
	return s;
}

int strlength(char *s){
	int i = 0;
	char *p = s;
	while(*(p+i) != '\0'){
		i++;
	}
	return i;
}

int countChars(char *s, char c){
	int i = 0;
	int num = 0;
	char *p = s;
	while(*(p+i) != '\0'){
		if(*(p+i) == c){
			num++;
		}
		i++;
	}
	return num;
}

char favChar(char *s){
	character_t *chars = (character_t *)malloc(sizeof(character_t));
	
	int totalChars = 0;
	int swi = 0; //"switch"
	
	//character_t keeper;
	
	int i = 0;
	char *p = s;
	//puts("begin outer while");
	while(*(p+i) != '\0'){
		int j = 0;
		character_t *pchar = chars;
		if(charDNE(*(p+i), chars, totalChars)){
			//the character isn't in the struct array yet
			//chars = (character_t *)realloc(chars, sizeof(character_t)*(totalChars+1));
			//put a new struct at the end of the list
			
			character_t *ptr = chars;
			ptr = ptr + sizeof(character_t)*totalChars;
						
			ptr->letter = *(p+i);
			ptr->count = 1;

			totalChars++;
		}
		i++;

		//puts("    begin inner while");
		while(j < totalChars){
			character_t temp1 = *(pchar+j*sizeof(character_t));
			character_t *temp2 = (pchar+j*sizeof(character_t));
			
			//printf("        temp1.letter = %c, *(p+i) == %c, count: %d \n", temp1.letter, *(p+i), temp1.count);
			
			if(temp1.letter == *(p+i)){
				(temp2->count)++;
			}
			j++;
		}
		//puts("    end inner while");
	}
	//puts("end outer while");

	
	int j2 = 0;
	char k;
	
	character_t result = *chars;
	character_t *pchar2 = chars;
	while(j2 < totalChars){
		/*
		 need to cycle through the list of structs
		 
		 for each struct, check if the current node's count is greater than the result node's
		 if it is,
			replace the result node with the current node pointer's contents
		 */
		
		character_t temp = *(pchar2+j2*sizeof(character_t));
		
		if(temp.count > result.count)
			result = temp;
		
		j2++;
	}
	k = result.letter;
	
	return k;
}

int compare(char *s1, char *s2){
	int i = 0;
	char *p1 = s1;
	char *p2 = s2;
	while(*(p1+i) != '\0' && *(p2+i) != '\0'){
		if(*(p1+i) < *(p2+i))
			return 1;
		if(*(p1+i) > *(p2+i))
			return -1;
		i++;
	}
	if(*(p1+i) == '\0' && *(p2+i) != '\0')
		return 1;
	if(*(p2+i) == '\0' && *(p1+i) != '\0')
		return -1;
	return 0;
}

char * frankenString(char *s1, char *s2){		
	char * res = (char *)malloc(sizeof(char)*(strlength(s1)+strlength(s2)));

	int i = 0;
	int j = 0;
	char *p = s1;
	while(*(p+i) != '\0'){
		*(res+j) = *(p+i); //enters the first string into the beginning of the section of memory
		
		/*
		printf("j: %d\n", j);
		if(strlength(res) < j)
			puts("oh no");
		 */
		
		i++;
		j++;
	}

	i = 0;
	p = s2;
	while(*(p+i) != '\0'){
		*(res+j) = *(p+i);
		//res = (char *)realloc(res,(sizeof(char)*(j+1)));
		
		/*
		printf("j: %d\n", j);
		if(strlength(res) < j)
			puts("oh no");
		*/
		
		i++;
		j++;
	}

	return res;
}

char * removeChar(char c, char *s){
	char * res = (char *)malloc(sizeof(char)*strlength(s));

	int j = 0; // moves along the result string
	int i = 0; // moves along the input string
	int num = 0;
	char *p = s;
	while(*(p+i) != '\0'){
		if(*(p+i) != c){
			*(res+j) = *(p+i);
			//res = (char *)realloc(res,sizeof(char)*(j+1));
			j++;
		}
		i++;
	}
	return res;
}


int charDNE(char c, character_t *list, int length){
	int i = 0;
	while(i<length){
		if((list+i*sizeof(character_t))->letter == c)
			return 0;
		i++;

	}
	return 1;
}



