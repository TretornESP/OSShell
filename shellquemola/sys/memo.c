#include <stdio.h>
#include <stdlib.h>
#include "memo.h"

void push(struct hst** head_ref, char* data) {
	struct hst* new_node = (struct hst*)malloc(sizeof(struct hst));

	new_node -> cmd = data;

	new_node -> next = (*head_ref);
	new_node -> prev = NULL;

    	if ((*head_ref) != NULL) 
    	    (*head_ref)->prev = new_node; 

	(*head_ref) = new_node;
}

void insert_after(struct hst* prev_node, char* new_data) 
{ 

    if (prev_node == NULL) { 
        printf("the given previous node cannot be NULL"); 
        return; 
    } 
  
    struct hst* new_node = (struct hst*)malloc(sizeof(struct hst)); 
  
    new_node->cmd = new_data; 
  
    new_node->next = prev_node->next; 
  
    prev_node->next = new_node; 
  
    new_node->prev = prev_node; 
  
    if (new_node->next != NULL) 
        new_node->next->prev = new_node; 
} 

char* next() {
	if (actual_node != NULL) {
		printf("%s", actual_node -> cmd);
		if (actual_node->next!=NULL) actual_node = actual_node->next;
	}
	return actual_node -> cmd;
}

char* prev() {
	if (actual_node != NULL) {
		printf("%s", actual_node -> cmd);
		if (actual_node->prev!=NULL) actual_node = actual_node->prev;
	}
	return actual_node -> cmd;
}

void generate_list() {
	actual_node = NULL;
	push(&actual_node, "");
}
