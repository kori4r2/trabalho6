#include "item.h"

struct item{
	int id;
	char* value;
};

ITEM *create_item(int id, char *value){
	ITEM *new_item = malloc(sizeof(ITEM));
	if(new_item != NULL){
		if(value != NULL){
			new_item->value = value;
			new_item->id = id;
		}else{
			free(new_item);
			new_item = NULL;
		}
	}

	return new_item;
}

int compare_items(ITEM *item1, ITEM *item2){
	if(item1 != NULL && item2 != NULL && item1->value != NULL && item2->value != NULL){
		return strcmp(item1->value, item2->value);
	}
	return INT_MIN;
}

int swap_items(ITEM **item1, ITEM **item2){
	if(item1 != NULL && (*item1) != NULL && item2 != NULL && (*item2) != NULL){
		ITEM *aux;
		aux = (*item1);
		(*item1) = (*item2);
		(*item2) = aux;
		return 1;
	}
	return 0;
}

int get_item_id(ITEM *item){
	if(item != NULL){
		return item->id;
	}
	return INT_MIN;
}

char *get_item_value(ITEM *item){
	if(item != NULL){
		return item->value;
	}
	return NULL;
}

int delete_item(ITEM **item){
	if(item != NULL){
		if((*item) != NULL){
			if((*item)->value != NULL) free((*item)->value);
			free(*item);
			(*item) = NULL;
			return 0;
		}
		return 1;
	}
	return 2;
}

void print_item(ITEM *item){
	if(item != NULL){
		printf("||id = %4d ; value = %7s||\n", item->id, item->value);
	}
}
