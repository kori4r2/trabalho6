#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct item ITEM;

ITEM *create_item(int, char*);
int compare_items(ITEM*, ITEM*);
int swap_items(ITEM**, ITEM**);
int get_item_id(ITEM*);
char *get_item_value(ITEM*);
int delete_item(ITEM**);
void print_item(ITEM*);

#endif
