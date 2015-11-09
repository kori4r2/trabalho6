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

typedef struct aux_node{
	PIECE *piece;
	struct aux_node *next;
}AUX_NODE;

struct list{
	struct aux_node *first;
	struct aux_node *last;
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
	int i, enemy_side;
	PIECE *aux;
	enemy_side = (rook->side == WHITES_SIDE)? BLACKS_SIDE : WHITES_SIDE;

	// Move para a posição disponível mais distante para a esquerda na coluna
	i = 1;
	while(rook->file-i > 'a' && table->grid[8-rook->rank][(rook->file-i)-'a'] == NULL) i++;
	// Checa se é uma posição válida, atribuindo o valor da posição a aux se necessário
	aux = ((rook->file-i >= 'a') ? table->grid[8-rook->rank][(rook->file-i)-'a'] : NULL);
	// Checa se encontrou uma peça inimiga
	if(aux != NULL && aux->side == enemy_side){
		// Verifica se capturar a peça deixa o rei em cheque
		table->grid[8-rook->rank][(rook->file-i)-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank, rook->file-i, 1, 0));
		// Desfaz a jogada
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-rook->rank][(rook->file-i)-'a'] = aux;
	}
	// Se possível
	if(rook->file-i >= 'a'){
		while(i > 1){
			// Volta todas as casas até a casa imediatamente do lado
			i--;
			// Enfileira as jogadas possíveis
			table->grid[8-rook->rank][(rook->file-i)-'a'] = rook;
			table->grid[8-rook->rank][rook->file-'a'] = NULL;
			if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank, rook->file-i, 0, 0));
			table->grid[8-rook->rank][rook->file-'a'] = rook;
			table->grid[8-rook->rank][(rook->file-i)-'a'] = NULL;
		}
	}

	
	// Faz o mesmo que foi descrito acima para as casa abaixo da torre
	i = 1;
	while(rook->rank-i > 1 && table->grid[8-(rook->rank-i)][rook->file-'a'] == NULL) i++;
	aux = ((rook->rank-i >= 1) ? table->grid[8-(rook->rank-i)][rook->file-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		table->grid[8-(rook->rank-i)][rook->file-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank-i, rook->file, 1, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-(rook->rank-i)][rook->file-'a'] = aux;
	}
	if(rook->rank-i >= 1){
		while(i > 1){
			i--;
			table->grid[8-(rook->rank-i)][rook->file-'a'] = rook;
			table->grid[8-rook->rank][rook->file-'a'] = NULL;
			if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank-i, rook->file, 0, 0));
			table->grid[8-rook->rank][rook->file-'a'] = rook;
			table->grid[8-(rook->rank-i)][rook->file-'a'] = NULL;
		}
	}

	//Faz o mesmo para as casas acima da torre
	i = 1;
	// Nesse caso, porém, enfileira os movimentos à medida que sobe
	while(rook->rank+i <= 8 && table->grid[8-(rook->rank+i)][rook->file-'a'] == NULL){
		table->grid[8-(rook->rank+i)][rook->file-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank+i, rook->file, 0, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-(rook->rank+i)][rook->file-'a'] = NULL;
		i++;
	}
	aux = ((rook->rank+i <= 8) ? table->grid[8-(rook->rank+i)][rook->file-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		table->grid[8-(rook->rank+i)][rook->file-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank+i, rook->file, 1, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-(rook->rank+i)][rook->file-'a'] = aux;
	}

	//Faz o mesmo para as casas à direita da torre
	i = 1;
	// Também enfileirando os movimentos à medida que se distancia da torre
	while(rook->file+i <= 'h' && table->grid[8-rook->rank][(rook->file+i)-'a'] == NULL){
		table->grid[8-rook->rank][(rook->file+i)-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file+i, rook->rank, rook->file+i, 0, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-rook->rank][(rook->file+i)-'a'] = NULL;
		i++;
	}
	aux = ((rook->file+i <= 'h') ? table->grid[8-rook->rank][(rook->file+i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		table->grid[8-rook->rank][(rook->file+i)-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank+i, rook->file+i, 1, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-rook->rank][(rook->file+i)-'a'] = aux;
	}
}

void move_knight(TABLE *table, QUEUE *queue, PIECE *knight){
	int i, j, enemy_side, flag, capture;
	PIECE *aux;
	enemy_side = (knight->side == WHITES_SIDE)? BLACKS_SIDE : WHITES_SIDE;

	j = -2;
	i = -1;
	// Checa o conteúdo da posição a ser avaliada
	aux = ((knight->rank+i >= 1 && knight->file+j >= 'a') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	// Checa se é necessário capturar uma peça
	capture = (aux != NULL && aux->side == enemy_side);
	// flag será 0 quando a posição avaliada for uma peça aliada
	flag = (aux != NULL && !capture)? 0 : 1;
	// Caso necessário
	if(flag && (knight->rank+i >= 1 && knight->file+j >= 'a')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	i = 1;
	aux = ((knight->rank+i <= 8 && knight->file+j >= 'a') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i <= 8 && knight->file+j >= 'a')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	j = -1;
	i = -2;
	aux = ((knight->rank+i >= 1 && knight->file+j >= 'a') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i >= 1 && knight->file+j >= 'a')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	i = 2;
	aux = ((knight->rank+i <= 8 && knight->file+j >= 'a') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i <= 8 && knight->file+j >= 'a')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	j = 1;
	i = -2;
	aux = ((knight->rank+i >= 1 && knight->file+j <= 'h') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i >= 1 && knight->file+j <= 'h')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	i = 2;
	aux = ((knight->rank+i <= 8 && knight->file+j <= 'h') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i <= 8 && knight->file+j <= 'h')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	j = 2;
	i = -1;
	aux = ((knight->rank+i >= 1 && knight->file+j <= 'h') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i >= 1 && knight->file+j <= 'h')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

	i = 1;
	aux = ((knight->rank+i <= 8 && knight->file+j <= 'h') ? table->grid[8-(knight->rank+i)][(knight->file+j)-'a'] : NULL);
	capture = (aux != NULL && aux->side == enemy_side);
	flag = (aux != NULL && !capture)? 0 : 1;
	if(flag && (knight->rank+i <= 8 && knight->file+j <= 'h')){
		// Verifica se realizar o movimento deixa o rei em cheque
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = knight;
		table->grid[8-knight->rank][knight->file-'a'] = NULL;
		// Caso não coloque em cheque, enfilera o movimento
		if(!is_check(table))
			enqueue(queue, create_move(knight->name, knight->rank, knight->file, knight->rank+i, knight->file+j, capture, 0));
		// Desfaz a jogada
		table->grid[8-knight->rank][knight->file-'a'] = knight;
		table->grid[8-(knight->rank+i)][knight->file+j-'a'] = aux;
	}

}

void move_bishop(TABLE *table, QUEUE *queue, PIECE *bishop){
	int i, up, down, enemy_side, repeat, flag1, flag2, capture;
	PIECE *aux1, *aux2;
	enemy_side = (bishop->side == WHITES_SIDE)? BLACKS_SIDE : WHITES_SIDE;

	// Move para as posições disponíveis mais distante para a esquerda e para cima ou para baixo
	i = 1;
	up = 1;
	down = 1;
	repeat = 1;
	// Enquanto estiver dentro dos limites do tabuleiro e alguma das diagonais tiver avançado
	while(bishop->file-i > 'a' && repeat){
		repeat = 0;
		// Incrementa a variável necessária para o movimento para cima
		if(bishop->rank + up < 8 && table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] == NULL){
			up++;
			repeat = 1;
		}
		// Ou para baixo
		if(bishop->rank - down > 1 && table->grid[8-(bishop->rank - down)][bishop->file-down-'a'] == NULL){
			down++;
			repeat = 1;
		}
		i++;
	}
	// Checa se é uma posição válida, atribuindo o valor da posição a aux se necessário
	aux1 = ((bishop->file - up >= 'a' && bishop->rank + up <= 8) ? table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] : NULL);
	aux2 = ((bishop->file - down >= 'a' && bishop->rank - down >= 1) ? table->grid[8-(bishop->rank-down)][bishop->file-down-'a'] : NULL);

	flag1 = (aux1 != NULL && aux1->side == enemy_side)? 1 : 0;
	flag2 = (aux2 != NULL && aux2->side == enemy_side)? 1 : 0;
	if(aux1 != NULL && !flag1) up--;
	if(aux2 != NULL && !flag2) down--;

	// Se possível
	while(bishop->file - i >= 'a' && bishop->file - i <= 'h' && i > 0){

		// Se down for menor ou igual a i, deve ser impresso o movimento da diagonal inferior
		if(down >= i && bishop->file-down >= 'a' && bishop->rank-down >= 1){
			// flag1 indica se é necessário checar captura de peças inimigas
			if(flag2){
				// Verifica se capturar a peça deixa o rei em cheque
				table->grid[8-(bishop->rank-down)][bishop->file-down-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank-down, bishop->file-down, 1, 0));
				// Desfaz a jogada
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank-down)][bishop->file-down-'a'] = aux2;
				flag2 = 0;
			}else{
				table->grid[8-(bishop->rank-down)][bishop->file-down-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank-down, bishop->file-down, 0, 0));
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank-down)][bishop->file-down-'a'] = NULL;
			}
			down--;
		}
		if(up > down && bishop->file-up >= 'a' && bishop->rank+up <= 8){
			// flag1 indica se é necessário checar captura de peças inimigas
			if(flag1){
				// Verifica se capturar a peça deixa o rei em cheque
				table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank+up, bishop->file-up, 1, 0));
				// Desfaz a jogada
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] = aux1;
				flag1 = 0;
			}else{
				table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank+up, bishop->file-up, 0, 0));
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank+up)][bishop->file-up-'a'] = NULL;
			}
			up--;
		}
		i--;
	}


	// A seguir checa todas as posições para a direita
	i = 1;
	up = 1;
	down = 1;
	repeat = 1;
	aux1 = NULL;
	aux2 = NULL;
	flag1 = flag2 = 1;
	// Enquanto estiver dentro dos limites do tabuleiro e alguma das diagonais tiver avançado
	while(bishop->file+i <= 'h' && repeat){
		repeat = 0;

		// Se aux2 ainda não tiver encontrado um valor anteriormente, atualiza com a posição atual
		if(!aux2)
			aux2 = ((bishop->file+down <= 'h' && bishop->rank-down >= 1) ? table->grid[8-(bishop->rank-down)][bishop->file+down-'a'] : NULL);
		// Caso seja necessário e seja uma posição válida
		if(flag2 && bishop->rank - down >= 1){
			// Verifica se o movimento é uma captura de peça
			capture = (aux2 != NULL && aux2->side == enemy_side)? 1 : 0;
			// Caso haja uma peça aliada na posição avaliada, muda a flag e não faz mais nada
			if(aux2 != NULL && !capture) flag2 = 0;
			else{
				// Verifica se realizar o movimento deixa o rei em cheque
				table->grid[8-(bishop->rank-down)][bishop->file+down-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank-down, bishop->file+down, capture, 0));
				// Desfaz a jogada
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank-down)][bishop->file+down-'a'] = aux2;
				down++;
				if(aux2 != NULL) flag2 = 0;
				repeat = 1;
			}
		}
		// Se aux1 ainda não tiver encontrado um valor anteriormente, atualiza com a posição atual
		if(!aux1)
			aux1 = ((bishop->file + up <= 'h' && bishop->rank + up <= 8) ? table->grid[8-(bishop->rank+up)][bishop->file+up-'a'] : NULL);
		// Caso seja necessário e seja uma posição válida
		if(flag1 && bishop->rank + up <= 8){
			// Verifica se o movimento é uma captura de peça
			capture = (aux1 != NULL && aux1->side == enemy_side)? 1 : 0;
			// Caso haja uma peça aliada na posição avaliada, muda a flag e não faz mais nada
			if(aux1 != NULL && !capture) flag1 = 0;
			else{
				// Verifica se realizar o movimento deixa o rei em cheque
				table->grid[8-(bishop->rank+up)][bishop->file+up-'a'] = bishop;
				table->grid[8-bishop->rank][bishop->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(bishop->name, bishop->rank, bishop->file, bishop->rank+up, bishop->file+up, capture, 0));
				// Desfaz a jogada
				table->grid[8-bishop->rank][bishop->file-'a'] = bishop;
				table->grid[8-(bishop->rank+up)][bishop->file+up-'a'] = aux1;
				up++;
				if(aux1 != NULL) flag1 = 0;
				repeat = 1;
			}
		}

		i++;
	}
}
void move_queen(TABLE *table, QUEUE *queue, PIECE *queen){
	int i, up, down, side, enemy_side, repeat, flag1, flag2, flag3, capture;
	PIECE *aux1, *aux2, *aux3;
	enemy_side = (queen->side == WHITES_SIDE)? BLACKS_SIDE : WHITES_SIDE;

	// Move para as posições disponíveis mais distante para a esquerda e para cima ou para baixo
	i = 1;
	up = 1;
	down = 1;
	side = 1;
	repeat = 1;
	// Enquanto estiver dentro dos limites do tabuleiro e alguma das 3 direções estiver avançando 
	while(queen->file-i > 'a' && repeat){
		repeat = 0;
		// Incrementa a variável necessária para o movimento para cima
		if(queen->rank + up < 8 && table->grid[8-(queen->rank+up)][queen->file-up-'a'] == NULL){
			up++;
			repeat = 1;
		}
		// Para o lado
		if(queen->rank - side > 1 && table->grid[8-queen->rank][queen->file-side-'a'] == NULL){
			side++;
			repeat = 1;
		}
		// Ou para baixo
		if(queen->rank - down > 1 && table->grid[8-(queen->rank - down)][queen->file-down-'a'] == NULL){
			down++;
			repeat = 1;
		}
		i++;
	}
	// Checa se é uma posição válida, atribuindo o valor da posição a aux se necessário
	aux1 = ((queen->file - up >= 'a' && queen->rank + up <= 8) ? table->grid[8-(queen->rank+up)][queen->file-up-'a'] : NULL);
	aux2 = ((queen->file - down >= 'a' && queen->rank - down >= 1) ? table->grid[8-(queen->rank-down)][queen->file-down-'a'] : NULL);
	aux3 = ((queen->file - side >= 'a') ? table->grid[8-queen->rank][queen->file-side-'a'] : NULL);

	flag1 = (aux1 != NULL && aux1->side == enemy_side)? 1 : 0;
	flag2 = (aux2 != NULL && aux2->side == enemy_side)? 1 : 0;
	flag3 = (aux3 != NULL && aux3->side == enemy_side)? 1 : 0;
	if(aux1 != NULL && !flag1) up--;
	if(aux2 != NULL && !flag2) down--;
	if(aux3 != NULL && !flag3) side--;

	// Se possível
	while(queen->file - i >= 'a' && queen->file - i <= 'h' && i > 0){

		// Se down for menor ou igual a i, deve ser impresso o movimento da diagonal inferior
		if(down >= i && queen->file-down >= 'a' && queen->rank-down >= 1){
			// flag2 indica se é necessário checar captura de peças inimigas
			if(flag2){
				// Verifica se capturar a peça deixa o rei em cheque
				table->grid[8-(queen->rank-down)][queen->file-down-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank-down, queen->file-down, 1, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank-down)][queen->file-down-'a'] = aux2;
				flag2 = 0;
			}else{
				table->grid[8-(queen->rank-down)][queen->file-down-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank-down, queen->file-down, 0, 0));
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank-down)][queen->file-down-'a'] = NULL;
			}
			down--;
		}

		// Se side for menor que down, deve ser impresso o movimento da horizontal
		if(side > down && queen->file-side >= 'a'){
			// flag3 indica se é necessário checar captura de peças inimigas
			if(flag3){
				// Verifica se capturar a peça deixa o rei em cheque
				table->grid[8-queen->rank][queen->file-side-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank, queen->file-side, 1, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-queen->rank][queen->file-side-'a'] = aux2;
				flag3 = 0;
			}else{
				table->grid[8-queen->rank][queen->file-side-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank, queen->file-side, 0, 0));
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-queen->rank][queen->file-side-'a'] = NULL;
			}
			side--;
		}

		// Se up for menor que side, deve ser impresso o movimento da horizontal
		if(up > side && queen->file-up >= 'a' && queen->rank+up <= 8){
			// flag1 indica se é necessário checar captura de peças inimigas
			if(flag1){
				// Verifica se capturar a peça deixa o rei em cheque
				table->grid[8-(queen->rank+up)][queen->file-up-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank+up, queen->file-up, 1, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank+up)][queen->file-up-'a'] = aux1;
				flag1 = 0;
			}else{
				table->grid[8-(queen->rank+up)][queen->file-up-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank+up, queen->file-up, 0, 0));
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank+up)][queen->file-up-'a'] = NULL;
			}
			up--;
		}
		i--;
	}

	// A seguir checa as posições para baixo
	i = 1;
	while(queen->rank-i > 1 && table->grid[8-(queen->rank-i)][queen->file-'a'] == NULL) i++;
	aux1 = ((queen->rank-i >= 1) ? table->grid[8-(queen->rank-i)][queen->file-'a'] : NULL);
	if(aux1 != NULL && aux1->side == enemy_side){
		table->grid[8-(queen->rank-i)][queen->file-'a'] = queen;
		table->grid[8-queen->rank][queen->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank-i, queen->file, 1, 0));
		table->grid[8-queen->rank][queen->file-'a'] = queen;
		table->grid[8-(queen->rank-i)][queen->file-'a'] = aux1;
	}
	if(queen->rank-i >= 1){
		while(i > 1){
			i--;
			table->grid[8-(queen->rank-i)][queen->file-'a'] = queen;
			table->grid[8-queen->rank][queen->file-'a'] = NULL;
			if(!is_check(table)) enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank-i, queen->file, 0, 0));
			table->grid[8-queen->rank][queen->file-'a'] = queen;
			table->grid[8-(queen->rank-i)][queen->file-'a'] = NULL;
		}
	}

	//Faz o mesmo para as casas acima da rainha
	i = 1;
	// Nesse caso, porém, enfileira os movimentos à medida que sobe
	while(queen->rank+i <= 8 && table->grid[8-(queen->rank+i)][queen->file-'a'] == NULL){
		table->grid[8-(queen->rank+i)][queen->file-'a'] = queen;
		table->grid[8-queen->rank][queen->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank+i, queen->file, 0, 0));
		table->grid[8-queen->rank][queen->file-'a'] = queen;
		table->grid[8-(queen->rank+i)][queen->file-'a'] = NULL;
		i++;
	}
	aux1 = ((queen->rank+i <= 8) ? table->grid[8-(queen->rank+i)][queen->file-'a'] : NULL);
	if(aux1 != NULL && aux1->side == enemy_side){
		table->grid[8-(queen->rank+i)][queen->file-'a'] = queen;
		table->grid[8-queen->rank][queen->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank+i, queen->file, 1, 0));
		table->grid[8-queen->rank][queen->file-'a'] = queen;
		table->grid[8-(queen->rank+i)][queen->file-'a'] = aux1;
	}


	// A seguir checa todas as posições para a direita
	i = 1;
	up = 1;
	down = 1;
	side = 1;
	repeat = 1;
	aux1 = NULL;
	aux2 = NULL;
	aux3 = NULL;
	flag1 = flag2 = flag3 = 1;
	// Enquanto estiver dentro dos limites do tabuleiro e alguma das diagonais tiver avançado
	while(queen->file+i <= 'h' && repeat){
		repeat = 0;

		// Se aux2 ainda não tiver encontrado um valor anteriormente, atualiza com a posição atual
		if(!aux2)
			aux2 = ((queen->file+down <= 'h' && queen->rank-down >= 1) ? table->grid[8-(queen->rank-down)][queen->file+down-'a'] : NULL);
		// Caso seja necessário e seja uma posição válida
		if(flag2 && queen->rank - down >= 1){
			// Verifica se o movimento é uma captura de peça
			capture = (aux2 != NULL && aux2->side == enemy_side)? 1 : 0;
			// Caso haja uma peça aliada na posição avaliada, muda a flag e não faz mais nada
			if(aux2 != NULL && !capture) flag2 = 0;
			else{
				// Verifica se realizar o movimento deixa o rei em cheque
				table->grid[8-(queen->rank-down)][queen->file+down-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank-down, queen->file+down, capture, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank-down)][queen->file+down-'a'] = aux2;
				down++;
				if(aux2 != NULL) flag2 = 0;
				repeat = 1;
			}
		}
		// Se aux3 ainda não tiver encontrado um valor anteriormente, atualiza com a posição atual
		if(!aux3)
			aux3 = ((queen->file+side <= 'h') ? table->grid[8-queen->rank][queen->file+side-'a'] : NULL);
		// Caso seja necessário e seja uma posição válida
		if(flag3){
			// Verifica se o movimento é uma captura de peça
			capture = (aux3 != NULL && aux3->side == enemy_side)? 1 : 0;
			// Caso haja uma peça aliada na posição avaliada, muda a flag e não faz mais nada
			if(aux3 != NULL && !capture) flag3 = 0;
			else{
				// Verifica se realizar o movimento deixa o rei em cheque
				table->grid[8-queen->rank][queen->file+side-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank, queen->file+side, capture, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-queen->rank][queen->file+side-'a'] = aux3;
				side++;
				if(aux3 != NULL) flag3 = 0;
				repeat = 1;
			}
		}
		// Se aux1 ainda não tiver encontrado um valor anteriormente, atualiza com a posição atual
		if(!aux1)
			aux1 = ((queen->file + up <= 'h' && queen->rank + up <= 8) ? table->grid[8-(queen->rank+up)][queen->file+up-'a'] : NULL);
		// Caso seja necessário e seja uma posição válida
		if(flag1 && queen->rank + up <= 8){
			// Verifica se o movimento é uma captura de peça
			capture = (aux1 != NULL && aux1->side == enemy_side)? 1 : 0;
			// Caso haja uma peça aliada na posição avaliada, muda a flag e não faz mais nada
			if(aux1 != NULL && !capture) flag1 = 0;
			else{
				// Verifica se realizar o movimento deixa o rei em cheque
				table->grid[8-(queen->rank+up)][queen->file+up-'a'] = queen;
				table->grid[8-queen->rank][queen->file-'a'] = NULL;
				// Caso não coloque em cheque, enfilera o movimento
				if(!is_check(table))
					enqueue(queue, create_move(queen->name, queen->rank, queen->file, queen->rank+up, queen->file+up, capture, 0));
				// Desfaz a jogada
				table->grid[8-queen->rank][queen->file-'a'] = queen;
				table->grid[8-(queen->rank+up)][queen->file+up-'a'] = aux1;
				up++;
				if(aux1 != NULL) flag1 = 0;
				repeat = 1;
			}
		}

		i++;
	}

}

void move_king(TABLE *table, QUEUE *queue, PIECE *king){

}

void move_pawn(TABLE *table, QUEUE *queue, PIECE *pawn){
	int enemy_side, start, finish, i, j, mirror, capture, en_passant_rank, flag;
	char en_passant_file, special;
	PIECE *aux;

	enemy_side = (pawn->side == WHITES_SIDE)? BLACKS_SIDE : WHITES_SIDE;
	start = (pawn->side == WHITES_SIDE)? 2 : 7;
	finish = (pawn->side == WHITES_SIDE)? 8 : 1;
	// A variável mirror serve para fazer a análise independentemente do lado ao qual o peão pertence
	mirror = (pawn->side == WHITES_SIDE)? 1 : -1;

	// OBS: CAPTURA EN PASSANT NÃO IMPLEMENTADA!!!!
	special = 0;
	if(table->en_passant[0] != '-'){
		en_passant_file = table->en_passant[0];
		en_passant_rank = table->en_passant[1]-'0';
	}else{
		en_passant_rank = 0;
		en_passant_file = 'i';
	}

	// rank = start+(mirror*pawn->rank);
	// first check = pawn->file-mirror;
	// Verifica a captura para a casa à esquerda
	if(pawn->file-mirror >= 1 && pawn->file-mirror <= 8 && pawn->rank+mirror <= 8){
		aux = table->grid[8-(pawn->rank+mirror)][pawn->file-mirror-'a'];
		capture = (aux != NULL && aux->side == enemy_side);
		flag = !(aux != NULL && !capture);
		if(flag){
			table->grid[8-(pawn->rank+mirror)][pawn->file-mirror-'a'] = pawn;
			table->grid[8-(pawn->rank)][pawn->file-'a'] = NULL;
			if(!is_check(table)){
				// Se atingir o final enfileira todas as jogas possíveis
				if(pawn->rank+mirror == finish){
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file-mirror,
																capture, 'N'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file-mirror,
																capture, 'B'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file-mirror,
																capture, 'R'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file-mirror,
																capture, 'Q'));
				}else{
				// Caso contrário, só há uma jogada possível
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file-mirror,
																capture, special));
				}
			}
			table->grid[8-(pawn->rank)][pawn->file-'a'] = pawn;
			table->grid[8-(pawn->rank+mirror)][pawn->file-mirror] = aux;
		}
	}

	// j indica se primeiro deve-se checar o movimento de duas casas ou não. Se j = 1, o movimento de uma casa é checado primeiro
	j = (mirror > 0)? 1 : 0;
	for(i = 0; i < 2; i++, j = !j){
		if(!j && pawn->rank == start){
			aux = table->grid[8-(pawn->rank+(mirror*2))][pawn->file-'a'];
			capture = (aux != NULL && aux->side == enemy_side);
			flag = !(aux != NULL && !capture);
			if(flag){
				table->grid[8-(pawn->rank+(mirror*2))][pawn->file-'a'] = pawn;
				table->grid[8-(pawn->rank)][pawn->file-'a'] = NULL;
				if(!is_check(table))
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+(mirror*2), pawn->file,
																capture, special));
				table->grid[8-(pawn->rank)][pawn->file-'a'] = pawn;
				table->grid[8-(pawn->rank+(mirror*2))][pawn->file-'a'] = aux;
			}
		}else if(j && pawn->rank+mirror <= 8){
			aux = table->grid[8-(pawn->rank+mirror)][pawn->file-'a'];
			capture = (aux != NULL && aux->side == enemy_side);
			flag = !(aux != NULL && !capture);
			if(flag){
				table->grid[8-(pawn->rank+mirror)][pawn->file-'a'] = pawn;
				table->grid[8-(pawn->rank)][pawn->file-'a'] = NULL;
				if(!is_check(table)){
					// Se atingir o final enfileira todas as jogas possíveis
					if(pawn->rank+mirror == finish){
						enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file,
																	capture, 'N'));
						enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file,
																	capture, 'B'));
						enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file,
																	capture, 'R'));
						enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file,
																	capture, 'Q'));
					}else{
					// Caso contrário, só há uma jogada possível
						enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file,
																capture, special));
					}
				}
				table->grid[8-(pawn->rank)][pawn->file-'a'] = pawn;
				table->grid[8-(pawn->rank+mirror)][pawn->file-'a'] = aux;
			}	
		}
	}

	if(pawn->file+mirror >= 1 && pawn->file+mirror <= 8 && pawn->rank+mirror <= 8){
		aux = table->grid[pawn->rank+mirror][pawn->file-'a'+mirror];
		capture = (aux != NULL && aux->side == enemy_side);
		flag = !(aux != NULL && !capture);
		if(flag){
			table->grid[8-(pawn->rank+mirror)][pawn->file-'a'+mirror] = pawn;
			table->grid[8-(pawn->rank)][pawn->file-'a'] = NULL;
			if(!is_check(table)){
				// Se atingir o final enfileira todas as jogas possíveis
				if(pawn->rank+mirror == finish){
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file+mirror,
																capture, 'N'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file+mirror,
																capture, 'B'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file+mirror,
																capture, 'R'));
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file+mirror,
																capture, 'Q'));
				}else{
				// Caso contrário, só há uma jogada possível
					enqueue(queue, create_move(pawn->name, pawn->rank, pawn->file, pawn->rank+mirror, pawn->file+mirror,
																capture, special));
				}
			}
			table->grid[8-(pawn->rank)][pawn->file-'a'] = pawn;
			table->grid[8-(pawn->rank+mirror)][pawn->file-'a'+mirror] = aux;
		}
	}
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

void list_moves(TABLE *table, QUEUE *queue, PIECE_LIST *list){
	AUX_NODE *node = list->first;
	if(node != NULL){
		do{
			node->piece->move(table, queue, node->piece);
			node = node->next;
		}while(node != list->first);
	}
	print_queue(queue);
}

int delete_table(TABLE **table){
	if(table != NULL && *table != NULL){
		int i, j;
		if((*table)->grid != NULL){
			for(i = 0; i < 8; i++){
				for(j = 0; j < 8; j++){
//fprintf(stderr, "i = %d; j = %d\n", i, j);
					if((*table)->grid[i][j] != NULL) delete_piece(&(*table)->grid[i][j]);
				}
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
