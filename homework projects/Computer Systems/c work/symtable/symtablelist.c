/*
 Joe Mahon
 */


#include "symtable.c"

int main(){
	int i = 0;
	SymTable_T *s = SymTable_new();
	int res;
	
	
	for(i=0; i<150; i++){
		int squares = SymTable_getNumOfUnits(s, "square\0");
		int circles = SymTable_getNumOfUnits(s, "circle\0");
		int triangles = SymTable_getNumOfUnits(s, "triangle\0");
		int rects = SymTable_getNumOfUnits(s, "rect\0");

		res = 0;
		if(i%4 == 0){
			//make another square
			square_t *binding = (square_t *)malloc(sizeof(square_t));
			binding->id = i;
			binding->sideLength = i%15;
			binding->area = areaSq;
			binding->perim = perimSq;
			if(res = SymTable_put(s, "square", i, binding) == 0)
				printf("Error: square not added on iteration %d\n", i);
		}else if(i%4 == 1){
			//add another circle
			circle_t *binding = (circle_t *)malloc(sizeof(circle_t));
			binding->id = i;
			binding->radius = i%21;
			binding->area = areaCir;
			binding->perim = perimCir;
			if(res = SymTable_put(s, "circle", i, binding) == 0)
				printf("Error: circle not added on iteration %d\n", i);
		}else if(i%4 == 2){
			//add another triangle
			triangle_t *binding = (triangle_t *)malloc(sizeof(triangle_t));
			binding->id = i;
			binding->width = i%19;
			binding->height = i%19;
			binding->area = areaTri;
			binding->perim = perimTri;
			if(res = SymTable_put(s, "triangle", i, binding) == 0)
				printf("Error: triangle not added on iteration %d\n", i);
		}else{
			//add another rect
			rect_t	*binding = (rect_t *)malloc(sizeof(rect_t));
			binding->id = i;
			binding->width = i%11;
			binding->height = i%28;
			binding->area = areaRect;
			binding->perim = perimRect;
			if(res = SymTable_put(s, "rect", i, binding) == 0)
				printf("Error: rect not added on iteration %d\n", i);			
		}	
	}
		
	SymTable_T *ptr = s;

	int counter = 0;
	while(counter < i/2){
		ptr = ptr->next;
		counter++;
	}

	//print out a small section of the entire table
	printf("Node:	Type:		KeyNumber:\n");
	while(counter < i/2+20){
		
		if(strcmp(ptr->type, "square") == 0){
			square_t *temp = (square_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "circle") == 0){
			circle_t *temp = (circle_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "triangle") == 0){
			triangle_t *temp = (triangle_t *)(ptr->pvValue);
			printf("%d	%s	%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "rect") == 0){
			rect_t *temp = (rect_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}
		ptr = ptr->next;
		counter++;
	}
	
	
	
	puts("\n\nreplacing circle #24 with a circle of radius 3 and ID of 368");
	
	circle_t *nCircle = (circle_t *)malloc(sizeof(circle_t));
	nCircle->id = 368;
	nCircle->radius = 3;
	nCircle->area = areaCir;
	nCircle->perim = perimCir;
	nCircle->print = printCir;
	
	SymTable_replace(s, "circle", 24, nCircle);
	
	//print out the end of the list again
	ptr = s; 
	counter =0;
	while(counter<145){
		ptr = ptr->next;
		counter++;
	}
	printf("Node:	Type:		KeyNumber:\n");
	while(ptr!=NULL){
		
		if(strcmp(ptr->type, "square") == 0){
			square_t *temp = (square_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "circle") == 0){
			circle_t *temp = (circle_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "triangle") == 0){
			triangle_t *temp = (triangle_t *)(ptr->pvValue);
			printf("%d	%s	%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}else if(strcmp(ptr->type, "rect") == 0){
			rect_t *temp = (rect_t *)(ptr->pvValue);
			printf("%d	%s		%d\n", 
				   temp->id, ptr->type, ptr->keyNumber);
		}
		ptr = ptr->next;
	}
	
	puts("\n\nDoes the table contain a Triangle node with KeyNumber of 35?");
	int f = SymTable_contains(s, "triangle", 35);
	if(f)
		puts("Yes.");
	else
		puts("No.");

	puts("Does the table contain a Triangle node with KeyNumber of 11?");
	f = SymTable_contains(s, "triangle", 11);
	if(f)
		puts("Yes.\n");
	else
		puts("No.");
	
	puts("What is in rectangle #24?");
	rect_t *aRectangle = SymTable_get(s, "rect", 23);
	printf("The ID is %d, the height %f, the width %f, area %f, and perim %f.\n\n", 
		   aRectangle->id, aRectangle->height, aRectangle->width, aRectangle->area(aRectangle), aRectangle->perim(aRectangle));
	
	puts("What is in circle #36?");
	circle_t *aCircle = SymTable_get(s, "circle", 36);
	printf("The ID is %d, the radius %f, area %f, and circumference %f.\n\n", 
		   aCircle->id, aCircle->radius, aCircle->area(aCircle), aCircle->perim(aCircle));
	
	
}