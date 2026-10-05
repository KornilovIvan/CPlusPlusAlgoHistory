#ifndef RBTREE_H
#define RBTREE_H

#include <graphviz/cgraph.h>

typedef enum
{
	BLACK,
	RED
} color_t;

typedef struct rbtree_node
{
	char *key;
	Agnode_t *graph_node;
	struct rbtree_node *left;
	struct rbtree_node *right;
	struct rbtree_node *parent;
	color_t color;
} rbtree_node;

typedef struct
{
	rbtree_node *root;
	size_t size;
	Agraph_t *original_graph;
} rbtree;

rbtree *read_graph_from_file(const char *filename);

void write_graph_to_file(rbtree *graph, const char *filename);

void free_rbtree(rbtree *tree);

#endif
