#include "queue.h"

struct node{
	CHESS_MOVE *move;
	struct node *next;
	struct node *previous;
};

struct queue{
	struct node *sentry;
	int size;
};

NODE *create_node(CHESS_MOVE *move){
	NODE *new_node = malloc(sizeof(NODE));
	if(new_node != NULL && move != NULL){
		new_node->move = move;
		new_node->next = new_node;
		new_node->previous = new_node;
	}else{
		if(new_node != NULL){
			free(new_node);
			new_node = NULL;
		}
	}

	return new_node;
}

int delete_node(NODE **node){
	if(node != NULL){
		if((*node) != NULL){
			delete_move( &((*node)->move) );
			free(*node);
			(*node) = NULL;
		}
	}
	return 2;
}

QUEUE *create_queue(void){
	QUEUE *queue = malloc(sizeof(QUEUE));
	NODE *sentry = create_node(-1, -1);
	if(sentry != NULL && queue != NULL){
		queue->sentry = sentry;
		queue->size = 0;
	}else{
		if(queue != NULL){
			free(queue);
			queue = NULL;
		}
		if(sentry != NULL) delete_node(&sentry);
	}

	return queue;
}

int empty_queue(QUEUE *queue){
	if(queue != NULL && queue->size > 0) return 0;
	else return 1;
}

int enqueue(QUEUE *queue, NODE *node){
	if(queue != NULL){
		if(node != NULL){
			node->previous = queue->sentry->previous;
			node->next = queue->sentry;
			node->previous->next = node;
			node->next->previous = node;
			queue->size++;

			return 0;
		}
		return 1;
	}
	return 2;
}

int dequeue(QUEUE *queue){
	if(queue != NULL){
		if(!empty_queue(queue)){
			NODE *aux = queue->sentry->next;
			aux->next->previous = aux->previous;
			aux->previous->next = aux->next;
			delete_node(&aux);
			queue->size--;

			return 0;
		}
		return 1;
	}
	return 2;
}

NODE *front_queue(QUEUE *queue){
	if(!empty_queue(queue)) return queue->sentry->next;
	return NULL;
}

int delete_queue(QUEUE **queue){
	if(queue != NULL){
		if((*queue) != NULL){
			while(!empty_queue(*queue)) dequeue(*queue);
			delete_node( &((*queue)->sentry) );
			free(*queue);
			(*queue) = NULL;

			return 0;
		}
		return 1;
	}
	return 2;
}

void print_queue(QUEUE *queue){
	if(queue != NULL){
		NODE *aux = queue->sentry->next;
		while(aux != queue->sentry){
			print_move(aux->move);
			aux = aux->next;
		}
		printf("||queue size = %d||\n", queue->size);
	}
}
