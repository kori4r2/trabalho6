#include "chess.h"
void move_rook(TABLE*, QUEUE*, PIECE*);
void move_knight(TABLE*, QUEUE*, PIECE*);
void move_bishop(TABLE*, QUEUE*, PIECE*);
void move_queen(TABLE*, QUEUE*, PIECE*);
void move_king(TABLE*, QUEUE*, PIECE*);
void move_pawn(TABLE*, QUEUE*, PIECE*);

typedef struct piece{
	char name;
	int priority;
	int rank;
	char file;
	int side;
	void (*move)(TABLE*, QUEUE*, PIECE*);
}PIECE;

typedef struct node{
	PIECE *piece;
	struct node *next;
}AUX_NODE;

struct list{
	struct node *first;
	struct node *last;
};

typedef struct table{
	PIECE ***grid;
	char turn;
	char *castling;
	char *en_passant;
	int half_turns;
	int cur_turn;
	PIECE *turn_king;
}TABLE;

//PIECE related functions------------------------------------
int is_check(TABLE *table){
	int i, j, enemy_side, check = 0;
	PIECE *king = table->turn_king;
	PIECE *aux;
	enemy_side = (king->side == WHITES_SIDE ? BLACKS_SIDE : WHITES_SIDE);

	// Checa se existe uma torre ou uma rainha atacando o rei para cima na coluna
	i = 1;
	while(king->rank+i < 8 && table->grid[8-(king->rank+i)][king->file-'a'] == NULL) i++;
	aux = ((king->rank+i <= 8) ? table->grid[8-(king->rank+i)][king->file-'a'] : NULL);
	// Checa se encontrou uma peça inimiga
	if(aux != NULL && aux->side == enemy_side){
		// Checa se está imediatamente do lado do rei inimigo
		if(i == 1 && aux->move == &move_king) check = 1;
		// Caso não esteja, checa se há uma torre ou rainha ameaçando
		if(!check && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
		// Caso seja encontrado cheque, retorna check para sair da função
		if(check) return check;
	}

	// Checa se existe uma torre ou uma rainha atacando o rei para baixo na coluna
	i = 1;
	while(king->rank-i > 1 && table->grid[8-(king->rank-i)][king->file-'a'] == NULL) i++;
	aux = ((king->rank-i >= 1) ? table->grid[8-(king->rank-i)][king->file-'a'] : NULL);
	// Checa se encontrou uma peça inimigai
	if(aux != NULL && aux->side == enemy_side){
		// Checa se está imediatamente do lado do rei inimigo
		if(i == 1 && aux->move == &move_king) check = 1;
		// Caso não esteja, checa se há uma torre ou rainha ameaçando
		if(!check && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
		// Caso seja encontrado cheque, retorna check para sair da função
		if(check) return check;
	}

	// Checa se existe uma torre ou uma rainha atacando o rei para a direta na linha
	i = 1;
	while(king->file+i < 'h' && table->grid[8-king->rank][(king->file+i)-'a'] == NULL) i++;
	aux = ((king->file+i <= 'h') ? table->grid[8-king->rank][(king->file+i)-'a'] : NULL);
	// Checa se encontrou uma peça inimiga
	if(aux != NULL && aux->side == enemy_side){
		// Checa se está imediatamente do lado do rei inimigo
		if(i == 1 && aux->move == &move_king) check = 1;
		// Caso não esteja, checa se há uma torre ou rainha ameaçando
		if(!check && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
		// Caso seja encontrado cheque, retorna check para sair da função
		if(check) return check;
	}

	// Checa se existe uma torre ou uma rainha atacando o rei para a esquerda na linha
	i = 1;
	while(king->file-i > 'a' && table->grid[8-king->rank][(king->file-i)-'a'] == NULL) i++;
	aux = ((king->file-i >= 'a') ? table->grid[8-king->rank][(king->file-i)-'a'] : NULL);
	// Checa se encontrou uma peça inimiga
	if(aux != NULL && aux->side == enemy_side){
		// Checa se está imediatamente do lado do rei inimigo
		if(i == 1 && aux->move == &move_king) check = 1;
		// Caso não esteja, checa se há uma torre ou rainha ameaçando
		if(!check && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
		// Caso seja encontrado cheque, retorna check para sair da função
		if(check) return check;
	}

	// Checa se existe um bispo ou uma rainha atacando o rei em uma diagonal
	i = 1;
	while(king->rank+i < 8 && king->file+i < 'h' && table->grid[8-(king->rank+i)][(king->file+i)-'a'] == NULL) i++;
	aux = ((king->rank+i <= 8 && king->file+i <= 'h') ? table->grid[8-(king->rank+i)][(king->file+i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se existe um bispo ou uma rainha atacando o rei em uma diagonal
	i = 1;
	while(king->rank+i < 8 && king->file-i > 'a' && table->grid[8-(king->rank+i)][(king->file-i)-'a'] == NULL) i++;
	aux = ((king->rank+i <= 8 && king->file-i >= 'a') ? table->grid[8-(king->rank+i)][(king->file-i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se existe um bispo ou uma rainha atacando o rei em uma diagonal
	i = 1;
	while(king->rank-i > 1 && king->file+i < 'h' && table->grid[8-(king->rank-i)][(king->file+i)-'a'] == NULL) i++;
	aux = ((king->rank-i >= 1 && king->file+i <= 'h') ? table->grid[8-(king->rank-i)][(king->file+i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se existe um bispo ou uma rainha atacando o rei em uma diagonal
	i = 1;
	while(king->rank-i > 1 && king->file-i > 'a' && table->grid[8-(king->rank-i)][(king->file-i)-'a'] == NULL) i++;
	aux = ((king->rank-i >= 1 && king->file-i >= 'a') ? table->grid[8-(king->rank-i)][(king->file-i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && (aux->move == &move_rook || aux->move == &move_queen)) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se a posição acima para a esquerda contém uma ameaça ao rei
	i = 1;
	j = -1;
	aux = ((king->rank+i <= 8 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		if(aux->move == &move_king) check = 1;
		if(!check && king->side == WHITES_SIDE && aux->move == &move_pawn) check = 1;
	}
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se a posição acima para a direita contém uma ameaça ao rei
	i = 1;
	j = 1;
	aux = ((king->rank+i <= 8 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		if(aux->move == &move_king) check = 1;
		if(!check && king->side == WHITES_SIDE && aux->move == &move_pawn) check = 1;
	}
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se a posição abaixo para a direita contém uma ameaça ao rei
	i = -1;
	j = 1;
	aux = ((king->rank+i > 0 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		if(aux->move == &move_king) check = 1;
		if(!check && king->side == BLACKS_SIDE && aux->move == &move_pawn) check = 1;
	}
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	// Checa se a posição abaixo para a esquerda contém uma ameaça ao rei
	i = -1;
	j = -1;
	aux = ((king->rank+i > 0 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		if(aux->move == &move_king) check = 1;
		if(!check && king->side == BLACKS_SIDE && aux->move == &move_pawn) check = 1;
	}
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	//Checa se há algum cavalo ameaçando o rei
	i = 2;
	j = -1;
	aux = ((king->rank+i <= 8 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	j = 1;
	aux = ((king->rank+i <= 8 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	i = 1;
	j = -2;
	aux = ((king->rank+i <= 8 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	j = 2;
	aux = ((king->rank+i <= 8 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	i = -1;
	j = -2;
	aux = ((king->rank+i > 0 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	j = 2;
	aux = ((king->rank+i > 0 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	i = -2;
	j = -1;
	aux = ((king->rank+i > 0 && king->file+j >= 'a') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;

	j = 1;
	aux = ((king->rank+i > 0 && king->file+j <= 'h') ? table->grid[8-(king->rank+i)][(king->file+j)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side && aux->move == &move_knight) check = 1;
	// Caso seja encontrado cheque, retorna check para sair da função
	if(check) return check;


	return check;
}

void move_rook(TABLE *table, QUEUE *queue, PIECE *rook){
	
}
void move_knight(TABLE *table, QUEUE *queue, PIECE *knight){

}
void move_bishop(TABLE *table, QUEUE *queue, PIECE *bishop){

}
void move_queen(TABLE *table, QUEUE *queue, PIECE *queen){

}
void move_king(TABLE *table, QUEUE *queue, PIECE *king){

}
void move_pawn(TABLE *table, QUEUE *queue, PIECE *pawn){

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
				piece->priority = 3;
				break;
			case W_KNIGHT:
			case B_KNIGHT:
				piece->move = &move_knight;
				piece->priority = 1;
				break;
			case W_BISHOP:
			case B_BISHOP:
				piece->move = &move_bishop;
				piece->priority = 2;
				break;
			case W_QUEEN:
			case B_QUEEN:
				piece->move = &move_queen;
				piece->priority = 4;
				break;
			case W_KING:
			case B_KING:
				piece->move = &move_king;
				piece->priority = 5;
				break;
			case W_PAWN:
			case B_PAWN:
				piece->move = &move_pawn;
				piece->priority = 0;
				break;
		}
	}

	return piece;
}

int compare_priority(PIECE *piece1, PIECE *piece2){
	// Verifica se foram passadas peças válidas
	if(piece1 != NULL && piece2 != NULL){
		// Compara as prioridades das peças
		if(piece1->priority < piece2->priority) return -1;
		else if(piece1->priority > piece2->priority) return 1;
		else{
			// A peça na coluna(file) de menor valor tem maior prioridade
			if(piece1->file < piece2->file) return -1;
			else if(piece1->file > piece2->file) return 1;
			else{
				// A peça na linha(rank) de menor valor tem maior prioridade
				if(piece1->rank < piece2->rank) return -1;
				else return 1;
			}
		}
	}
	return 0;
}

AUX_NODE *create_aux_node(PIECE *piece){
	AUX_NODE *node = NULL;
	if(piece != NULL){
		node = (AUX_NODE*)malloc(sizeof(AUX_NODE));
		if(node != NULL){
			node->piece = piece;
			node->next = node;
		}
	}
	return node;
}

int insert_list(PIECE_LIST *list, PIECE *piece){
	if(list != NULL && piece != NULL){
		AUX_NODE *new_node;
		if(list->first == NULL) list->last = list->first = create_aux_node(piece);
		else if(compare_priority(piece, list->first->piece) < 0){
			new_node = create_aux_node(piece);
			new_node->next = list->first;
			list->last->next = new_node;
			list->first = new_node;
		}else{
			AUX_NODE *prev_node = list->first;
			while(compare_priority(piece, prev_node->next->piece) > 0 && prev_node->next != list->first)
				prev_node = prev_node->next;
			new_node = create_aux_node(piece);
			new_node->next = prev_node->next;
			prev_node->next = new_node;
			if(new_node->next == list->first) list->last = new_node;
		}
		return 0;
	}
	return 1;
}

PIECE_LIST *create_piece_list(TABLE *table){
	PIECE_LIST *list = NULL;
	if(table != NULL){
		list = (PIECE_LIST*)malloc(sizeof(PIECE_LIST));
		if(list != NULL){
			int i, j, side_turn;
			side_turn = (table->turn == WHITES_TURN ? WHITES_SIDE : BLACKS_SIDE);

			list->first = NULL;
			list->last = NULL;
			for(i = 0; i < 8; i++){
				for(j = 0; j < 8; j++){
					// Se existir uma peça e for o turno dela atualmente
					if(table->grid[i][j] != NULL && table->grid[i][j]->side == side_turn){
						// Insere a peça na lista de maneira ordenada
						insert_list(list, table->grid[i][j]);
					}
				}
			}
		}
	}
	return list;
}

int delete_list(PIECE_LIST **list){
	if(list != NULL && *list != NULL){
		AUX_NODE *aux;
		while((*list)->first->next != (*list)->first){
			aux = (*list)->first->next;
			(*list)->first->next = aux->next;
			free(aux);
		}
		free((*list)->first);
		free(*list);
		(*list) = NULL;
	}
	return 1;
}

// auxiliary function
int print_list(PIECE_LIST *list){
	if(list != NULL){
		AUX_NODE *print = list->first;
		if(print != NULL) printf("||%c||(%d,%c)", print->piece->name, print->piece->rank, print->piece->file);
		print = print->next;
		while(print != list->first){
			printf("->||%c||(%d,%c)", print->piece->name, print->piece->rank, print->piece->file);
			print = print->next;
		}
		printf("\n");
		return 0;
	}
	return 1;
}

void delete_piece(PIECE **piece){
	if(piece != NULL && *piece != NULL){
		free(*piece);
		(*piece) = NULL;
	}
}

// TABLE related functions--------------------------------------------------------
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
		int i, j, k, turn_side;

		token = strtok(input, DELIMITERS);
		for(i = 0; i < 8; i++){
			for(j = 0, k = 0; token[j] != '\0'; j++){
				if(isdigit(token[j])) k += (token[j] - '0');
				else{
					table->grid[i][k] = create_piece(token[j], (8-i), ('a'+k));
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

		turn_side = (table->turn == WHITES_TURN) ? WHITES_SIDE : BLACKS_SIDE;

		for(i = 0; i < 8; i++){
			for(j = 0; j < 8; j++){
				if(table->grid[i][j] != NULL && table->grid[i][j]->move == &move_king && table->grid[i][j]->side == turn_side){
					table->turn_king = table->grid[i][j];
					i = 8;
					j = 8;
				}
			}
		}

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

// Auxiliary function
int print_table(TABLE *table){
	if(table != NULL){
		int i, j;
		if(table->grid != NULL){
			for(i = 0; i < 8; i++){
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
		printf("turn king: side = %d; rank = %d; file = %c\n", table->turn_king->side, table->turn_king->rank, table->turn_king->file);

		return 0;
	}
	return 1;
}
