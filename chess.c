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
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file+i, rook->rank, rook->file, 0, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-rook->rank][(rook->file+i)-'a'] = NULL;
		i++;
	}
	aux = ((rook->file+i <= 'h') ? table->grid[8-rook->rank][(rook->file+i)-'a'] : NULL);
	if(aux != NULL && aux->side == enemy_side){
		table->grid[8-rook->rank][(rook->file+i)-'a'] = rook;
		table->grid[8-rook->rank][rook->file-'a'] = NULL;
		if(!is_check(table)) enqueue(queue, create_move(rook->name, rook->rank, rook->file, rook->rank+i, rook->file, 1, 0));
		table->grid[8-rook->rank][rook->file-'a'] = rook;
		table->grid[8-rook->rank][(rook->file+i)-'a'] = aux;
	}
}

void move_knight(TABLE *table, QUEUE *queue, PIECE *knight){

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
		if(bishop->rank + up < 8 && table->grid[8-(bishop->rank+up)][bishop->file-i-'a'] == NULL){
			up++;
			repeat = 1;
		}
		// Ou para baixo
		if(bishop->rank - down > 1 && table->grid[8-(bishop->rank - down)][bishop->file-i-'a'] == NULL){
			down++;
			repeat = 1;
		}
		i++;
	}
	// Checa se é uma posição válida, atribuindo o valor da posição a aux se necessário
	aux1 = ((bishop->file - up >= 'a' && bishop->rank + up <= 8) ? table->grid[8-(bishop->rank+up)][bishop->file-i-'a'] : NULL);
	aux2 = ((bishop->file - down >= 'a' && bishop->rank - down >= 1) ? table->grid[8-(bishop->rank-down)][bishop->file-i-'a'] : NULL);

	flag1 = (aux1 != NULL && aux1->side == enemy_side)? 1 : 0;
	flag2 = (aux2 != NULL && aux2->side == enemy_side)? 1 : 0;
	if(aux1 != NULL && !flag1) up--;
	if(aux2 != NULL && !flag2) down--;

	// Se possível
	while(bishop->file - i >= 'a' && bishop->file - i <= 'h' && i > 0){

		// Se up for menor do que i, deve ser impresso o movimento da diagonal superior
		if(up >= i && bishop->file-up >= 'a' && bishop->rank+up <= 8){
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
		if(down > up && bishop->file-down >= 'a' && bishop->rank-down >= 1){
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

		i++;
	}
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
