#include "chess.h"

typedef struct piece{
	char name;
	int rank;
	char file;
	int side;
	void (*move)(TABLE*, QUEUE*);
}PIECE;

typedef struct table{
	PIECE ***grid;
	char turn;
	char *castling;
	char *en_passant;
	int half_turns;
	int cur_turn;
}TABLE;

void move_rook(TABLE *table, QUEUE *queue){

}
void move_knight(TABLE *table, QUEUE *queue){

}
void move_bishop(TABLE *table, QUEUE *queue){

}
void move_queen(TABLE *table, QUEUE *queue){

}
void move_king(TABLE *table, QUEUE *queue){

}
void move_pawn(TABLE *table, QUEUE *queue){

}

PIECE *create_piece(char name, int rank, char file){
	PIECE *piece = (PIECE*)malloc(sizeof(PIECE));
	if(piece != NULL){
		piece->name = name;
		piece->rank = rank;
		piece->file = file;
		piece->side = (isupper(name) ? WHITES_SIDE : BLACKS_SIDE);
		switch(name){
			case W_ROOK:
			case B_ROOK:
				piece->move = &move_rook;
				break;
			case W_KNIGHT:
			case B_KNIGHT:
				piece->move = &move_knight;
				break;
			case W_BISHOP:
			case B_BISHOP:
				piece->move = &move_bishop;
				break;
			case W_QUEEN:
			case B_QUEEN:
				piece->move = &move_queen;
				break;
			case W_KING:
			case B_KING:
				piece->move = &move_king;
				break;
			case W_PAWN:
			case B_PAWN:
				piece->move = &move_pawn;
				break;
		}
	}

	return piece;
}

void delete_piece(PIECE **piece){
	if(piece != NULL && *piece != NULL){
		free(*piece);
		(*piece) = NULL;
	}
}

TABLE *create_table(void){
	int i, j;
	int error = 0;

	TABLE *table = (TABLE*)malloc(sizeof(TABLE));
	if(table == NULL) error = 1;
	else{
		// Aloca as colunas da grid
		table->grid = (PIECE***)malloc(sizeof(PIECE**) * 8);
		// Caso falhe em alocar a memória, muda o indicador de erro para 1
		if(table == NULL) error = 1;
		// Caso contrário prossegue com a execução
		else{
			// Cria todas as linhas da grid
			for(i = 0; i < 8; i++){
				table->grid[i] = (PIECE**)malloc(sizeof(PIECE*) * 8);
				// Caso a memoria seja alocada corretamente
				if(table->grid[i] != NULL)
					// Inicializa todas as posições como NULL
					for(j = 0; j < 8; j++) table->grid[i][j] = NULL;
				// Caso haja algum erro de alocação de memória
				else{
					// Indica o erro
					error = 1;
					// Libera toda a memória já alocada
					for(j = i-1; j >= 0; j--) free(table->grid[j]);
					free(table->grid);
					free(table);
					// Muda o valor de retorno para null e garante que vai sair do loop atual
					table = NULL;
					i = 9;
				}
			}
			// Caso a grid tenha sido devidamente alocada, inicializa os outros campos
			if(!error){
				table->turn = ' ';
				table->castling = NULL;
				table->en_passant = NULL;
				table->half_turns = -1;
				table->turn = 0;
			}
		}
	}
	// Se houver erro, imprime na saída correspondente
	if(error) fprintf(stderr, "create_table(): error allocating memory\n");

	return table;
}

int read_table(FILE *stream, TABLE *table){
	if(table != NULL){
		char *input = my_get_line(stream);
		char *token;
		int i, j, k;

		token = strtok(input, DELIMITERS);
		for(i = 0; i < 8; i++){
			for(j = 0, k = 0; token[j] != '\0'; j++){
				if(isdigit(token[j])) k += (token[j] - '0');
				else{
					table->grid[7-i][k] = create_piece(token[j], (8-i), ('a'+k));
					k++;
				}
			}

			token = strtok(NULL, DELIMITERS);
		}

		table->turn = token[0];
		token = strtok(NULL, DELIMITERS);

		table->castling = strdup(token);
		token = strtok(NULL, DELIMITERS);

		table->en_passant = strdup(token);
		token = strtok(NULL, DELIMITERS);

		table->half_turns = atoi(token);
		token = strtok(NULL, DELIMITERS);

		table->cur_turn = atoi(token);

		free(input);
		return 0;
	}

	fprintf(stderr, "read_table(): table is NULL\n");
	return 1;
}

int delete_table(TABLE **table){
	if(table != NULL && *table != NULL){
		int i, j;
		if((*table)->grid != NULL){
			for(i = 0; i < 8; i++){
				for(j = 0; j < 8; j++)
					if((*table)->grid[i][j] != NULL) delete_piece(&(*table)->grid[i][j]);
				free((*table)->grid[i]);
			}
			free((*table)->grid);
		}

		if((*table)->castling != NULL) free((*table)->castling);
		if((*table)->en_passant != NULL) free((*table)->en_passant);

		free(*table);
		(*table) = NULL;
		return 0;
	}
	return 1;
}

int print_table(TABLE *table){
	if(table != NULL){
		int i, j;
		if(table->grid != NULL){
			for(i = 7; i >= 0; i--){
				for(j = 0; j < 8; j++){
					if(table->grid[i][j] != NULL) printf("|%c| ", table->grid[i][j]->name);
					else printf("| | ");
				}
				printf("\n");
			}
		}
		printf("\n\n");

		printf("turn: |%c|\n", table->turn);
		if(table->castling != NULL) printf("castling: |%s|\n", table->castling);
		if(table->en_passant != NULL) printf("en_passant: |%s|\n", table->en_passant);
		printf("half-turns: %d\n", table->half_turns);
		printf("current turn: %d\n", table->cur_turn);

		return 0;
	}
	return 1;
}
