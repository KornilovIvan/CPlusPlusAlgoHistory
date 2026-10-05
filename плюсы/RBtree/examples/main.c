#include "../include/RBtree.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
		return 1;
	}
	const char *filename_input = argv[1];
	const char *filename_output = argv[2];
	rbtree *graph = read_graph_from_file(filename_input);
	if (graph == NULL)
	{
		return 1;
	}
	write_graph_to_file(graph, filename_output);
	free_rbtree(graph);
}
