#include "../include/RBtree.h"

#include <graphviz/cgraph.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

rbtree_node *create_tree_node(const char *key, Agnode_t *graph_node)
{
	rbtree_node *node = malloc(sizeof(rbtree_node));
	if (!node || !key)
	{
		return NULL;
	}
	node->key = strdup(key);
	if (!node->key)
	{
		free(node);
		return NULL;
	}
	node->graph_node = graph_node;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	node->color = RED;
	return node;
}

void right_rotate(rbtree *tree, rbtree_node *x)
{
	rbtree_node *y = x->left;
	if (!y)
		return;

	x->left = y->right;
	if (y->right)
	{
		y->right->parent = x;
	}

	y->parent = x->parent;

	if (!x->parent)
	{
		tree->root = y;
	}
	else if (x == x->parent->right)
	{
		x->parent->right = y;
	}
	else
	{
		x->parent->left = y;
	}

	y->right = x;
	x->parent = y;
}

void left_rotate(rbtree *tree, rbtree_node *x)
{
	rbtree_node *y = x->right;
	if (!y)
		return;

	x->right = y->left;

	if (y->left)
	{
		y->left->parent = x;
	}
	y->parent = x->parent;

	if (!x->parent)
	{
		tree->root = y;
	}
	else if (x == x->parent->left)
	{
		x->parent->left = y;
	}
	else
	{
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

void rbtree_insert_fixup(rbtree *tree, rbtree_node *node)
{
	while (node != tree->root && node->parent->color == RED)
	{
		rbtree_node *grandparent = node->parent->parent;
		if (node->parent == grandparent->left)
		{
			rbtree_node *uncle = grandparent->right;
			if (uncle && uncle->color == RED)
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				node = grandparent;
			}
			else
			{
				if (node == node->parent->right)
				{
					node = node->parent;
					left_rotate(tree, node);
				}
				node->parent->color = BLACK;
				grandparent->color = RED;
				right_rotate(tree, grandparent);
			}
		}
		else
		{
			rbtree_node *uncle = grandparent->left;

			if (uncle && uncle->color == RED)
			{
				node->parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				node = grandparent;
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					right_rotate(tree, node);
				}
				node->parent->color = BLACK;
				grandparent->color = RED;
				left_rotate(tree, grandparent);
			}
		}
	}
	tree->root->color = BLACK;
}

void rbtree_insert(rbtree *tree, const char *key, Agnode_t *node)
{
	if (!tree || !key)
		return;

	rbtree_node *parent = NULL;
	rbtree_node *current = tree->root;
	char *endptr;
	long key_num = strtol(key, &endptr, 10);
	while (current)
	{
		char *current_endptr;
		long current_num = strtol(current->key, &current_endptr, 10);
		int cmp = (key_num > current_num) - (key_num < current_num);
		if (cmp == 0)
		{
			return;
		}
		parent = current;
		if (cmp < 0)
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}

	rbtree_node *new_node = create_tree_node(key, node);
	if (!new_node)
		return;

	new_node->parent = parent;

	if (!parent)
	{
		tree->root = new_node;
	}
	else
	{
		char *parent_endptr;
		long parent_num = strtol(parent->key, &parent_endptr, 10);

		if (key_num < parent_num)
		{
			parent->left = new_node;
		}
		else
		{
			parent->right = new_node;
		}
	}

	rbtree_insert_fixup(tree, new_node);
	tree->size++;
}

rbtree *build_rbtree(Agraph_t *graph)
{
	if (!graph)
	{
		return NULL;
	}
	rbtree *tree = calloc(1, sizeof(rbtree));
	if (!tree)
	{
		return NULL;
	}
	tree->root = NULL;
	tree->size = 0;
	tree->original_graph = graph;
	Agnode_t *node;
	for (node = agfstnode(graph); node; node = agnxtnode(graph, node))
	{
		const char *node_name = agnameof(node);
		rbtree_insert(tree, node_name, node);
	}
	return tree;
}

rbtree *read_graph_from_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Cannot open input file '%s'\n", filename);
		return NULL;
	}
	Agraph_t *graph = agread(file, NULL);
	if (!graph)
	{
		fprintf(stderr, "Error: Failed to parse graph from file stream\n");
		return NULL;
	}
	if (agnnodes(graph) == 0)
	{
		fprintf(stderr, "Error: Graph '%s' is empty (no nodes)\n", agnameof(graph));
		return NULL;
	}
	rbtree *tree = build_rbtree(graph);
	agclose(graph);
	fclose(file);
	return tree;
}

static void add_tree_to_graph(Agraph_t *new_graph, rbtree_node *node)
{
	if (!node)
		return;
	Agnode_t *ag_node = agnode(new_graph, (char *)node->key, true);
	if (node->color == RED)
	{
		agsafeset(ag_node, (char *)"color", (char *)"red", (char *)"");
	}
	else
	{
		agsafeset(ag_node, (char *)"color", (char *)"black", (char *)"");
	}
	add_tree_to_graph(new_graph, node->left);
	add_tree_to_graph(new_graph, node->right);
}

static void add_tree_edges(Agraph_t *new_graph, rbtree_node *node)
{
	if (!node)
		return;

	Agnode_t *current_ag_node = agnode(new_graph, (char *)node->key, false);

	if (current_ag_node)
	{
		if (node->left)
		{
			Agnode_t *left_ag_node = agnode(new_graph, (char *)node->left->key, false);
			if (left_ag_node)
			{
				agedge(new_graph, current_ag_node, left_ag_node, NULL, true);
			}
		}
		if (node->right)
		{
			Agnode_t *right_ag_node = agnode(new_graph, (char *)node->right->key, false);
			if (right_ag_node)
			{
				agedge(new_graph, current_ag_node, right_ag_node, NULL, true);
			}
		}
	}

	add_tree_edges(new_graph, node->left);
	add_tree_edges(new_graph, node->right);
}

Agraph_t *rbtree_to_graph(rbtree *tree)
{
	if (!tree || !tree->root)
		return NULL;
	Agdesc_t desc = agisdirected(tree->original_graph) ? Agdirected : Agundirected;
	Agraph_t *new_graph = agopen((char *)"RBTree", desc, NULL);

	if (!new_graph)
		return NULL;
	add_tree_to_graph(new_graph, tree->root);
	add_tree_edges(new_graph, tree->root);

	return new_graph;
}

void write_graph_to_file(rbtree *tree, const char *filename)
{
	if (!tree)
	{
		fprintf(stderr, "Error: Cannot write graph because the tree is NULL\n");
		return;
	}
	if (!filename)
	{
		fprintf(stderr, "Error: Output filename is NULL\n");
		return;
	}
	FILE *file = fopen(filename, "w");
	if (!file)
	{
		fprintf(stderr, "Error: Failed to open output file '%s'\n", filename);
		return;
	}
	Agraph_t *graph = rbtree_to_graph(tree);
	if (!graph)
	{
		fprintf(stderr, "Error: Failed to convert RBTree to Graphviz graph\n");
		fclose(file);
		return;
	}
	if (agwrite(graph, file) != 0)
	{
		fprintf(stderr, "Error: Failed to write graph to file '%s'\n", filename);
	}
	agclose(graph);
	fclose(file);
}

void free_rbtree_node(rbtree_node *node)
{
	if (!node)
		return;
	free_rbtree_node(node->left);
	free_rbtree_node(node->right);
	free(node->key);
	free(node);
}

void free_rbtree(rbtree *tree)
{
	if (!tree)
	{
		fprintf(stderr, "Error: Cannot free RBTree because it is NULL\n");
		return;
	}
	free_rbtree_node(tree->root);
	free(tree);
}
