#include "chess_move.h"

struct move{
	int id;
	char* value;
};

CHESS_MOVE *create_move(int id, char *value){
	CHESS_MOVE *new_move = malloc(sizeof(CHESS_MOVE));
	if(new_move != NULL){
		if(value != NULL){
			new_move->value = value;
			new_move->id = id;
		}else{
			free(new_move);
			new_move = NULL;
		}
	}

	return new_move;
}

int compare_moves(CHESS_MOVE *move1, CHESS_MOVE *move2){
	if(move1 != NULL && move2 != NULL && move1->value != NULL && move2->value != NULL){
		return strcmp(move1->value, move2->value);
	}
	return INT_MIN;
}

int swap_moves(CHESS_MOVE **move1, CHESS_MOVE **move2){
	if(move1 != NULL && (*move1) != NULL && move2 != NULL && (*move2) != NULL){
		CHESS_MOVE *aux;
		aux = (*move1);
		(*move1) = (*move2);
		(*move2) = aux;
		return 1;
	}
	return 0;
}

int get_move_id(CHESS_MOVE *move){
	if(move != NULL){
		return move->id;
	}
	return INT_MIN;
}

char *get_move_value(CHESS_MOVE *move){
	if(move != NULL){
		return move->value;
	}
	return NULL;
}

int delete_move(CHESS_MOVE **move){
	if(move != NULL){
		if((*move) != NULL){
			if((*move)->value != NULL) free((*move)->value);
			free(*move);
			(*move) = NULL;
			return 0;
		}
		return 1;
	}
	return 2;
}

void print_move(CHESS_MOVE *move){
	if(move != NULL){
		printf("||id = %4d ; value = %7s||\n", move->id, move->value);
	}
}
