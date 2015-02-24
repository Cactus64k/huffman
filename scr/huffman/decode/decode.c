#include "../huffman.h"

int decodding_file(FILE* sourse_file, FILE* output_file, node* tree, int64_t sourse_file_size)
{
	assert(sourse_file != NULL);
	assert(output_file != NULL);
	assert(sourse_file_size > 0);
	assert(tree != NULL);

	node* nd = tree;

	while(!feof(sourse_file))
	{
		size_t buff;
		size_t marker = 1;

		size_t byte_count = fread(&buff, sizeof(size_t), 1, sourse_file);
		if(sourse_file_size != 0)
			if(byte_count != 1)
				return 1;

		while((marker != 0) && (sourse_file_size != 0))
		{
			if(buff & marker)
				nd = nd->right;
			else
				nd = nd->left;


			if(nd->right == 0)
			{
				fputc((int)(uintptr_t)nd->left, output_file);
				nd = tree;
				sourse_file_size--;
			}

			marker = marker<<1;
		}
	}
	return 0;
}

int huffman_decode(FILE* sourse_file, FILE* output_file)
{
	assert(sourse_file != 0);
	assert(output_file != 0);

	int64_t sourse_file_size = 0;
	fread(&sourse_file_size, sizeof(int64_t), 1, sourse_file);
	//==========================================================

	encode** encode_tab = calloc(256, sizeof(encode*));
	int res = read_encode_tab(sourse_file, encode_tab);		// заполняем кодировочную таблицу
	if(res != 0)
	{
		perror("Bad file format\n");
		exit(1);
	}
	//==========================================================

	node* tree = create_tree_from_tab(encode_tab);

	res = decodding_file(sourse_file, output_file, tree, sourse_file_size);
	if(res != 0)
	{
		perror("Bad file format\n");
		exit(1);
	}
	//==========================================================

	destroy_encode_tab(encode_tab);

	return 0;
}
