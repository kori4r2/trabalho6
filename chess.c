#include "chess.h"

typedef struct piece{
	char name;
	int rank;
	char file;
	void (*move)(TABLE*, QUEUE*);
}PIECE;

typedef struct table{
	PIECE ***grid;
	char turn;
	char *castling;
	char *en_pasant;
	int half_turns;
	int turn;
}TABLE;

TABLE *create_table(void){
	int i, j;
	int error = 0;

	TABLE *table = (TABLE*)malloc(sizeof(TABLE));
	if(table == NULL) error = 1;
	else{
		// Aloca as colunas da grid
		table->grid = (PIECE***)malloc(sizeof(PIECE) * 8);
		// Caso falhe em alocar a memória, muda o indicador de erro para 1
		if(table == NULL) error = 1;
		// Caso contrário prossegue com a execução
		else{
			// Cria todas as linhas da grid
			for(i = 0; i < 8; i++){
				table->grid[i] = (PIECE*)malloc(sizeof(PIECE*) * 8);
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
				table->en_pasant = NULL;
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

	}

	fprintf(stderr, "read_table(): table is NULL\n");
	return 1;
}
