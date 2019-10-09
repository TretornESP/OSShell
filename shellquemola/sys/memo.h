#ifndef MEMO_H_INCLUDED
#define MEMO_H_INCLUDED
#endif

struct hst {
	char* cmd;
	struct hst* next;
	struct hst* prev;
}; 

struct hst* actual_node;

void push(struct hst**, char*);
void insert_after(struct hst*, char*);
char* next();
char* prev();
void generate_list();
