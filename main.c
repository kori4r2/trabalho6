#include <stdio.h>
#include <stdlib.h>
#include "my_strings.h"
#include "chess.h"

int main(int argc, char *argv[]){

	int aux;
	TABLE *table = create_table();
	aux = read_table(stdin, table);
	printf("||read_table() return value = %d||\n", aux);
	aux = print_table(table);
	printf("||print_table() return value = %d||\n", aux);
	printf("||is_check() return value = %d||\n", is_check(table));
	printf("\n list of pieces read:\n");
	PIECE_LIST *list = create_piece_list(table);
	print_list(list);
	delete_list(&list);
	delete_table(&table);

	return 0;
}
