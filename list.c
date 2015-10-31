#include "list.h"

struct node{
	ITEM *item;
	struct node *next;
	struct node *previous;
};

struct list{
	struct node *sentry;
	int size;
};

NODE *create_node(int id, char *value){
	NODE *new_node = malloc(sizeof(NODE));
	ITEM *new_item = create_item(id, value);
	if(new_node != NULL && new_item != NULL){
		new_node->item = new_item;
		new_node->next = new_node;
		new_node->previous = new_node;
	}else{
		if(new_node != NULL){
			free(new_node);
			new_node = NULL;
		}
		if(new_item != NULL) delete_item(&new_item);
	}

	return new_node;
}

int delete_node(NODE **node){
	if(node != NULL){
		if((*node) != NULL){
			delete_item( &((*node)->item) );
			free(*node);
			(*node) = NULL;
		}
	}
	return 2;
}

char *node_value(NODE *node){
	if(node != NULL){
		return get_item_value(node->item);
	}
	return NULL;
}

char *value_first(LIST *list){
	if(list != NULL){
		if(!empty_list(list)){
			return node_value(list->sentry->next);
		}
	}
	return NULL;
}

char *value_last(LIST *list){
	if(list != NULL){
		if(!empty_list(list)){
			return node_value(list->sentry->previous);
		}
	}
	return NULL;
}

char *value_pos(LIST *list, int pos){
	if(list != NULL){
		int i;
		NODE *node = list->sentry->next;
		for(i = 1; node != list->sentry; i++){
			if(i == pos) return node_value(node);
			node = node->next;
		}
	}
	return NULL;
}

LIST *create_list(void){
	LIST *list = malloc(sizeof(LIST));
	NODE *sentry = create_node(INT_MIN, NULL);
	if(sentry != NULL && list != NULL){
		list->sentry = sentry;
		list->size = 0;
	}else{
		if(list != NULL){
			free(list);
			list = NULL;
		}
		if(sentry != NULL) delete_node(&sentry);
	}

	return list;
}

int empty_list(LIST *list){
	if(list != NULL && list->size > 0) return 0;
	else return 1;
}

int list_size(LIST *list){
	if(list != NULL) return list->size;
	return -1;
}

int insert_beginning(LIST *list, NODE *node){
	if(list != NULL){
		if(node != NULL){
			node->previous = list->sentry;
			node->next = list->sentry->next;
			node->next->previous = node;
			node->previous->next = node;
			list->size++;

			return 0;
		}
		return 1;
	}
	return 2;
}

int insert_end(LIST *list, NODE *node){
	if(list != NULL){
		if(node != NULL){
			node->previous = list->sentry->previous;
			node->next = list->sentry;
			node->previous->next = node;
			node->next->previous = node;
			list->size++;

			return 0;
		}
		return 1;
	}
	return 2;
}

int remove_beginning(LIST *list){
	if(list != NULL){
		if(!empty_list(list)){
			NODE *aux = list->sentry->next;
			aux->next->previous = aux->previous;
			aux->previous->next = aux->next;
			delete_node(&aux);
			list->size--;

			return 0;
		}
		return 1;
	}
	return 2;
}

int remove_end(LIST *list){
	if(list != NULL){
		if(!empty_list(list)){
			NODE *aux = list->sentry->previous;
			aux->next->previous = aux->previous;
			aux->previous->next = aux->next;
			delete_node(&aux);
			list->size--;

			return 0;
		}
		return 1;
	}
	return 2;
}

int remove_position(LIST *list, int pos){
	if(list != NULL){
		int i;
		NODE *node = list->sentry->next;
		for(i = 1; node != list->sentry; i++){
			if(i == pos){
				node->previous->next = node->next;
				node->next->previous = node->previous;
				list->size--;
				delete_node(&node);
				return 0;
			}
			node = node->next;
		}
		return 1;
	}
	return 2;
}

int delete_list(LIST **list){
	if(list != NULL){
		if((*list) != NULL){
			while(!empty_list(*list)) remove_end(*list);
			delete_node( &((*list)->sentry) );
			free(*list);
			(*list) = NULL;

			return 0;
		}
		return 1;
	}
	return 2;
}

int print_list(LIST *list){
	if(list != NULL){
		NODE *aux = list->sentry->next;
		while(aux != list->sentry){
			print_item(aux->item);
			aux = aux->next;
		}
		return 0;
	}
	return 1;
}
