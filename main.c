#include <stdio.h>
#include <stdlib.h>
#include "my_strings.h"
#include "chess.h"

int main(int argc, char *argv[]){

	TABLE *table = create_table();
	read_table(stdin, table);
//	print_table(table);
	PIECE_LIST *list = create_piece_list(table);
	QUEUE *queue = create_queue();
	list_moves(table, queue, list);
//printf("-----------------------------------------------------\n");
//	print_table(table);

	delete_queue(&queue);
	delete_list(&list);
//printf("-----------------------------------------------------\n");
//	print_table(table);
	delete_table(&table);

	return 0;
}
