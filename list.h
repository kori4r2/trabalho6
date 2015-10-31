#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "item.h"

typedef struct node NODE;
typedef struct list LIST;

NODE *create_node(int, char*);
int node_id(NODE*);
char *node_value(NODE*);
int delete_node(NODE**);
LIST *create_list(void);
int empty_list(LIST*);
int list_size(LIST*);
int insert_beginning(LIST*, NODE*);
int insert_end(LIST*, NODE*);
int remove_beginning(LIST*);
int remove_end(LIST*);
int remove_position(LIST*, int);
int delete_list(LIST**);
int print_list(LIST*);

#endif
