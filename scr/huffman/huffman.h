#ifndef HUFFMAN_H
	#define HUFFMAN_H

	#define _LARGE_FILES

	#include <sys/types.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#include <string.h>
	#include <stdint.h>
	#include <limits.h>
	#include <float.h>

	//==============================================
	#include "memprof.h"
	//==============================================

	#define PATH_CAPACITY (CHAR_BIT * sizeof(size_t))				// разрядность пути

	typedef struct node
	{
		struct node* left;
		struct node* right;
		float prob;
	} node;

	typedef struct node_list
	{
		struct node_list* next;
		struct node* nd;
	} node_list;

	typedef struct encode
	{
		size_t* path;
		int len;
	} encode;

	//================ENCODE========================
	int huffman_encode(FILE* sourse_file, FILE* output_file);
	int fill_encode_tab(node* tree, encode** encode_tab);
	void write_encode_tab(FILE* f, encode** encode_tab);
	void destroy_encode_tab(encode** encode_tab);

	node_list* create_node_list(FILE* f, node_list* buff);

	node* create_tree(node_list* list_itm, node* nodes_buff);
	//==============================================

	//================DECODE========================
	int huffman_decode(FILE* sourse_file, FILE* output_file);
	int read_encode_tab(FILE* f, encode** encode_tab);
	node* create_tree_from_tab(encode** encode_tab);
	//==============================================

#endif
