/*
 Joe Mahon
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct SymTable {
	int keyNumber;
	void *pvValue;
	struct SymTable *next;
	char *type;
}SymTable_T;

typedef struct square {
	int id;
	float sideLength;
	float (* area)(struct square *);
	float (* perim)(struct square *);
	void (* print)(struct square *);
} square_t;

typedef struct rect {
	int id;
	float height;
	float width;
	float (* area)(struct rect *);
	float (* perim)(struct rect *);
	void (* print)(struct rect *);
} rect_t;

typedef struct triangle {
	int id;
	float height;
	float width;
	float (* area)(struct triangle *);
	float (* perim)(struct triangle *);
	void (* print)(struct triangle *);
}triangle_t;

typedef struct circle {
	int id;
	float radius;
	float (* area)(struct circle *);
	float (* perim)(struct circle *);
	void (* print)(struct circle *);
} circle_t;


float areaSq(square_t  *ob);
float areaCir(circle_t  *ob);
float areaTri(triangle_t  *ob);
float areaRect(rect_t  *ob);

float perimSq(square_t  *ob);
float perimCir(circle_t  *ob);
float perimTri(triangle_t  *ob);
float perimRect(rect_t  *ob);

void printSq(square_t  *ob);
void printCir(circle_t	 *ob);
void printTri(triangle_t  *ob);
void printRect(rect_t  *ob);

SymTable_T * SymTable_new(void); //SymTable_new should return a new SymTable object that contains no bindings
void SymTable_free(SymTable_T * oSymTable); //SymTable_free should free all memory occupied by oSymTable.".
int SymTable_getLength(SymTable_T * oSymTable); //SymTable_getLength should return the number of bindings in oSymTable.".
int SymTable_put(SymTable_T * oSymTable, char *type, int KeyNumber, void *pvValue);
//where char *type will be either cirlce, square, triangle or rect
//where int KeyNumber will be a computer (that means your program) generated int. for example, if you insert
//        4 circles into the SymTable, then the first circle has KeyNumber = 1, the second has KeyNumber = 2, etc.

//If oSymTable does not contain a binding with type and KeyNumber, then SymTable_put should add a new binding to oSymTable consisting of char *type, int KeyNumber, const void *pvValue and return 1 (TRUE). Otherwise the function should leave oSymTable unchanged and return 0 (FALSE). 
void *SymTable_replace(SymTable_T * oSymTable, char *type, int KeyNumber, const void *pvValue); //If oSymTable contains a binding with type and KeyNumber, then SymTable_replace should replace the binding's value with pvValue and return the old value. Otherwise it should leave oSymTable unchanged and return NULL. 
int SymTable_contains(SymTable_T * oSymTable, char *type, int KeyNumber); //SymTable_contains should return 1 (TRUE) if oSymTable contains a binding whose type and KeyNumber match, and 0 (FALSE) otherwise. 
void *SymTable_get(SymTable_T * oSymTable, char *type, int KeyNumber); //SymTable_get should return the value of the binding within oSymTable whose type and KeyNumber match, or NULL if no such binding exists. Then, you should call each of the fucntion IN THE VALUE STRUCT to perform the apprporiate tasks. 
void *SymTable_remove(SymTable_T * oSymTable, char *type, int KeyNumber); //If oSymTable contains a binding with type and KeyNumber, then SymTable_remove should remove that binding from oSymTable and return the binding's value. Otherwise the function should not change oSymTable and return NULL. 

//A SymTable object should "own" its keys. That is, a SymTable object is responsible for allocating and freeing the memory in which its keys reside. Specifically, SymTable_put should not simply store the value of pcKey within the binding that it creates. Rather, SymTable_put should make a copy of the string pointed to by pcKey, and store the address of that copy within the new binding. You will find the standard C functions strlen, malloc, and strcpy useful for making the copy. A SymTable object also should free the memory in which its keys reside when that memory is no longer required. 

