
#include "instructions.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


void init (inst *q){
	q->first = NULL;
  	q->last = NULL;
}

void add (inst *q, char *name){
  inst_elt *elt;
  
  // Allocate a new list element with malloc
  elt = malloc (sizeof(*elt));


  // If malloc fails end the program
  if (elt == NULL) {
    printf ("Couldn't allocate a new queue element\n");
    exit (1);
  }

  // Allocate space to store the name
  elt->title = malloc(strlen(name) + 1);

  // If malloc fails end the program
  if (elt->title == NULL) {
    printf ("Couldn't allocate a new name\n");
    exit (1);
  }

  elt->next = NULL;

  // Copy the name
  
  strcpy (elt->title, name);  	


  // Check if queue is empty
  if (q->last) {
    // add elt to the end
    (q->last)->next = elt;
  } else {
    q->first = elt;
  }
  q->last = elt;
}

void pop (inst *q, char *name){
  inst_elt *elt;
  
  if (q->first) {
    elt = q->first;

    if (q->first == q->last) {
      // In this case you are deleting the only element in the queue
      q->first = NULL;
      q->last = NULL;
    } else {
      q->first = elt->next;
    }
	
	strcpy(name, elt->title);
    // free the elt
    free(elt->title);
    free(elt);
    //free(name);
  }
}

void clear (inst *q){
 char *dead = "dead";
 while (q->first) {
    pop (q, dead);
  }
  //free(dead);
}

void print (inst *q){
	inst_elt *elt = q->first;

  //printf ("\nQUEUE CONTENTS\n");

  while (elt) {
    printf ("%s\n", elt->title);
    elt = elt->next;
  }
}

int isEmpty(inst *q){
	if(q->first){
		return 1;
	} else{
		return 0;
	}
}
