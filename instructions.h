/*
 * instructions.h
 */

typedef struct inst_elt_tag {
	char *title;
	struct inst_elt_tag *next;
} inst_elt;

typedef struct {
	inst_elt *first, *last;
} inst;

void init (inst *q);
void add (inst *q, char *name);
void pop (inst *q, char *name);
void clear (inst *q);
void print (inst *q);
int isEmpty(inst *q);
