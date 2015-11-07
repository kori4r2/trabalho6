#include "chess_move.h"

struct move{
	int origin_rank;
	int origin_file;
	int destiny_rank;
	int destiny_file;
	unsigned char capture;
	unsigned char repeat;
};

CHESS_MOVE *create_move(int origin_rank, char origin_file, int destiny_rank, char destiny_file, unsigned char capture){
	CHESS_MOVE *new_move = malloc(sizeof(CHESS_MOVE));
	if(new_move != NULL){
		new_move->origin_rank = origin_rank;
		new_move->origin_file = origin_file;
		new_move->destiny_rank = destiny_rank;
		new_move->destiny_file = destiny_file;
		new_move->capture = capture;
		new_move->repeat = 0;
	}

	return new_move;
}

int compare_moves(CHESS_MOVE *move1, CHESS_MOVE *move2){
	if(move1 != NULL && move2 != NULL){
		// ???????????
		return 0;
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
		return move->origin_rank;
	}
	return INT_MIN;
}

int delete_move(CHESS_MOVE **move){
	if(move != NULL){
		if((*move) != NULL){
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
		printf("||id = %4d ; value = %c||\n", move->origin_rank, move->origin_file);
	}
}
