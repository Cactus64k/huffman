#include "../huffman.h"

int read_encode_tab(FILE* f, encode** encode_tab)
{
	for(size_t i=0; i<256; i++)
	{
		int len = fgetc(f);

		if(len == EOF)
			return 1;
		else if(len>0)
		{
			encode_tab[i] = malloc(sizeof(encode));
			assert(encode_tab[i] != NULL);
			memset(encode_tab[i], 0, sizeof(encode));

			encode_tab[i]->len		= len;

			fread(encode_tab[i]->path, sizeof(char), (len-1)/CHAR_BIT+1, f);
		}
		else
			encode_tab[i] = 0;
	}
	return 0;
}

node* create_tree_from_tab(encode** encode_tab)
{
	assert(encode_tab != 0);

	node* root = malloc(sizeof(node));
	assert(root != 0);

	memset(root, 0, sizeof(node));

	for(size_t i=0; i<256; i++)
	{
		if(encode_tab[i] != NULL)
		{
			int len			= encode_tab[i]->len;
			size_t* path	= encode_tab[i]->path;
			size_t marker	= 1;

			node** nd = &root;

			for(size_t j=0;j<len;j++)		// проходимся по каждому биту пути и создаем отсутствующие узлы
			{
				if((path[j/PATH_CAPACITY] & marker) != 0)
					nd = &((*nd)->right);
				else
					nd = &((*nd)->left);


				if(*nd == NULL)		// если такого узла нет, то создаем новый
				{
					*nd = malloc(sizeof(node));
					assert(*nd != NULL);

					memset(*nd, 0, sizeof(node));
				}

				marker = marker << 1 | marker >> (PATH_CAPACITY - 1);
			}

			(*nd)->left = (node*)(uintptr_t)i;
		}
	}
	return root;
}
