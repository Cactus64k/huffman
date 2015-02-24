#include "../huffman.h"

void __fill_encode_tab(encode* parent_itm, node* nd, encode** encode_tab)
{
	assert(parent_itm != NULL);									// right_itm передает предыдущий узел в рекурсии, левый создается каждый раз
	assert(nd != NULL);
	assert(encode_tab != NULL);

	if(nd->right == 0)											// если правый узел равен нулю, то в левом записан байт файла
		encode_tab[(size_t)nd->left] = parent_itm;
	else
	{
		parent_itm->len++;

		size_t path_size = sizeof(size_t) * (parent_itm->len/PATH_CAPACITY + 1);

		parent_itm->path = realloc(parent_itm->path, path_size);

		encode* right_itm = parent_itm;							// родитель идет на правую ветвь
		encode* left_itm = malloc(sizeof(encode));				// правую часть мы имеем из предыдущего узла

		assert(left_itm != NULL);

		left_itm->len = parent_itm->len;
		left_itm->path = malloc(path_size);
		memcpy(left_itm->path, parent_itm->path, path_size);

		__fill_encode_tab(left_itm, nd->left, encode_tab);		// для левой 0

		//===========================================================
		size_t pos		= (right_itm->len)/PATH_CAPACITY;		// позиция в массиве пути
		size_t offset	= (right_itm->len)%PATH_CAPACITY;		// смещение относительно целой ячейки пути

		right_itm->path[pos] |= 1<<offset;

		__fill_encode_tab(right_itm, nd->right, encode_tab);	// для правой 1
	}
}

int fill_encode_tab(node* tree, encode** encode_tab)
{
	assert(encode_tab != NULL);
	memset(encode_tab, 0, 256 * sizeof(encode*));

	encode* parent_itm = malloc(sizeof(encode));		// затравка для рекурсии
	assert(parent_itm != NULL);

	parent_itm->path = malloc(sizeof(size_t));
	assert(parent_itm != NULL);
	memset(parent_itm->path, 0, sizeof(size_t));

	parent_itm->len = 0;

	__fill_encode_tab(parent_itm, tree, encode_tab);

	return 0;
}

void write_encode_tab(FILE* f, encode** encode_tab)
{
	assert(f != NULL);
	assert(encode_tab != NULL);

	for(size_t i=0; i<256; i++)
	{
		if(encode_tab[i] != NULL)		// возможно такого символа могло и не быть в файле
		{
			int len = encode_tab[i]->len;

			fputc(len, f);

			fwrite(encode_tab[i]->path, sizeof(char), (len-1)/CHAR_BIT+1, f);
		}
		else
			fputc(0, f);
	}
}

void destroy_encode_tab(encode** encode_tab)
{
	for(size_t i=0; i<256; i++)
		if(encode_tab[i] != NULL)
		{
			free(encode_tab[i]->path);
			free(encode_tab[i]);
		}
}
