/*
 Joe Mahon
 */

#include "symtable.h"

#define PI 3.14159265358979323846

SymTable_T *SymTable_new(){
	SymTable_T *res =  (SymTable_T *) malloc(sizeof(SymTable_T));
	res->keyNumber = -1;
	res->pvValue = NULL;
	res->next = NULL;
	res->type = NULL;
	return res;
}

void SymTable_free(SymTable_T *oSymTable){
	free(oSymTable->pvValue);
	free(oSymTable->next);
	free(oSymTable->type);
	free(oSymTable);
}

int SymTable_getLength(SymTable_T * oSymTable){
	int counter = 0;
	SymTable_T *ptr = oSymTable;
	while(ptr != NULL){
		ptr = ptr->next;
		counter++;
	}
	return counter;
}

int SymTable_getNumOfUnits(SymTable_T *s, char * type){
	SymTable_T *ptr = s;	
	int result = 0;
	if(ptr == NULL || ptr->keyNumber == -1){
		result = 0;
	}else if(ptr->next == NULL && strcmp(ptr->type, type) == 0){
		result = 1;
	}else if(ptr->next == NULL && strcmp(ptr->type, type) != 0){
		result = 0;
	}else{
		//printf("checking num of units on normal block\n");
		while(ptr->next != NULL){
			if(strcmp(ptr->type, type) == 0){
				result++;
			}
			ptr = ptr->next;
		}
	}
	return result;
}


//give this function any node in the symtable oSymTable, an object binding pvValue, and a type, and it will attach a new node at the back end of the list with the correct data and an automatically-generated
int SymTable_put(SymTable_T * oSymTable, char *type, int KeyNumber, void *pvValue){	
	SymTable_T *ptr = oSymTable;
	if(ptr == NULL){
		//just add the block
		printf("You need a real SymTable node to enter this\n");
		return 0;
	}else if(ptr->next == NULL && ptr->keyNumber != -1 ){
		SymTable_T *sBinding = (SymTable_T *)malloc(sizeof(SymTable_T));
		sBinding->type = (char *)malloc(sizeof(char)*strlen(type));
		strcpy(sBinding->type, type);
		sBinding->pvValue = pvValue;
		sBinding->next = NULL;
		
		int squares = SymTable_getNumOfUnits(oSymTable, "square");
		int circles = SymTable_getNumOfUnits(oSymTable, "circle");
		int triangles = SymTable_getNumOfUnits(oSymTable, "triangle");
		int rects = SymTable_getNumOfUnits(oSymTable, "rect");
		if(strcmp(type, "square") == 0){
			sBinding->keyNumber = squares;
		}
		if(strcmp(type, "circle") == 0){
			sBinding->keyNumber = circles;
		}
		if(strcmp(type, "triangle") == 0){
			sBinding->keyNumber = triangles;
		}
		if(strcmp(type, "rect") == 0){
			sBinding->keyNumber = rects;
		}
		
		ptr->next = sBinding;
	}else{
		if(ptr->keyNumber == -1){
			//adding first
			ptr->keyNumber = 0;
			ptr->type = (char *)malloc(sizeof(char)*strlen(type));
			strcpy(ptr->type, type);
			ptr->pvValue = pvValue;
			ptr->next = NULL;
		}else{
			//adding second+
			SymTable_T *sBinding = (SymTable_T *)malloc(sizeof(SymTable_T));
			sBinding->type = (char *)malloc(sizeof(char)*strlen(type));
			strcpy(sBinding->type, type);
			sBinding->pvValue = pvValue;
			sBinding->next = NULL;
			
			int squares = SymTable_getNumOfUnits(oSymTable, "square");
			int circles = SymTable_getNumOfUnits(oSymTable, "circle");
			int triangles = SymTable_getNumOfUnits(oSymTable, "triangle");
			int rects = SymTable_getNumOfUnits(oSymTable, "rect");
			if(strcmp(type, "square") == 0){
				sBinding->keyNumber = squares;
			}
			if(strcmp(type, "circle") == 0){
				sBinding->keyNumber = circles;
			}
			if(strcmp(type, "triangle") == 0){
				sBinding->keyNumber = triangles;
			}
			if(strcmp(type, "rect") == 0){
				sBinding->keyNumber = rects;
			}
			
			
			while(ptr->next != NULL){
				ptr = ptr->next;
			}
			
			
			ptr->next = sBinding;
		}
		return 1;
	}
}

void *SymTable_replace(SymTable_T * oSymTable, char *type, int KeyNumber, const void *pvValue){
	SymTable_T *ptr = oSymTable;
	
	while(ptr!=NULL){
		if(strcmp(ptr->type, type) == 0 && ptr->keyNumber == KeyNumber){
			void * res = ptr->pvValue;
			ptr->pvValue = pvValue;
			return res;
		}
		ptr = ptr->next;
	}
	return NULL;
}

int SymTable_contains(SymTable_T * oSymTable, char *type, int KeyNumber){
	SymTable_T *ptr = oSymTable;
	while(ptr != NULL){
		if(strcmp(ptr->type, type)==0 && ptr->keyNumber == KeyNumber)
			return 1;
		ptr = ptr->next;
	};
	return 0;
}

void *SymTable_get(SymTable_T * oSymTable, char *type, int KeyNumber){
	SymTable_T *ptr = oSymTable;
	void *res;
	
	if(SymTable_contains(oSymTable, type, KeyNumber)){
		while(ptr->next!=NULL){
			if(strcmp(ptr->type, type) == 0 && ptr->keyNumber == KeyNumber){
				return ptr->pvValue;
			}
			ptr = ptr->next;
		}
	}else{
		return NULL;
	}
	return NULL;
}

void *SymTable_remove(SymTable_T * oSymTable, char *type, int KeyNumber){
	SymTable_T *ptr = oSymTable;
	if(SymTable_contains(ptr, type, KeyNumber)){
		SymTable_T *tptr = oSymTable;
		while(tptr->next != ptr){
			tptr=tptr->next;
		}
		tptr->next = ptr->next;
		return ptr->pvValue;
	}else{
		return NULL;
	}
}

float areaSq(square_t *ob){		return (ob->sideLength)*(ob->sideLength);}
float areaCir(circle_t *ob){	
	float result =  (ob->radius)*(ob->radius)*PI;
	return result;
}
float areaTri(triangle_t *ob){	return (ob->width)*(ob->height)/2;}
float areaRect(rect_t *ob){		return (ob->width)*(ob->height);}

float perimSq(square_t *ob){	return (ob->sideLength)*4;}
float perimCir(circle_t *ob){	return (ob->radius)*2*PI;}
float perimTri(triangle_t *ob){	return (ob->width)*3; }//assume the triangle is equilateral?
float perimRect(rect_t *ob){	return (ob->width)*2+2*(ob->height);}

void printSq(square_t *ob){
	printf("Square %d has side length %f, area %f, and perimeter %f.", ob->id, ob->sideLength, ob->area(ob), ob->perim(ob));
}
void printCir(circle_t *ob){
	
}
void printTri(triangle_t *ob){
	
}
void printRect(rect_t *ob){
		
}