#include "../huffman.h"

void encode_file(FILE* sourse_file, FILE* output_file, encode** encode_tab)		// обрабатывает ВЕСЬ файл в место 4кб !!!!!!!!!!
{
	size_t buff			= 0;		// буфер для соединения кодов символов
	size_t buff_size	= 0;		// размер буфера
	int byte;

	while((byte = fgetc(sourse_file)) != EOF)
	{
		assert(encode_tab[byte] != NULL);		// если у нас вдруг появился символ, которого не было при построении дерева

		ssize_t path_len		= encode_tab[byte]->len;
		size_t* path			= encode_tab[byte]->path;
		size_t i				= 0;

		while(path_len > 0)
		{
			buff = buff | (path[i]<<buff_size);						// сдвигаем двойное слово текущего символа так, чтобы оно налезло на хвост

			if(path_len + buff_size >= PATH_CAPACITY)				// если длины пути текущего символа хватает на полный буфер вместе с хвостом
			{
				fwrite(&buff, sizeof(size_t), 1, output_file);

				buff = path[i]>>(PATH_CAPACITY-buff_size);			//остаток заносим в буфер

				if(path_len < PATH_CAPACITY)						// если длина пути >= PATH_CAPACITY, то размер буфера остается прежний
					buff_size	= (path_len+buff_size)%PATH_CAPACITY;
			}
			else
				buff_size+=path_len;								// если нет заполнения буфера, то увеличиваем размер буфера на длину пути

			path_len -= PATH_CAPACITY;
			i++;
		}
	}
	if(buff_size>0)	// если в буфере еще что то осталось
		fwrite(&buff, sizeof(size_t), 1, output_file);

	fflush(output_file);
}

int huffman_encode(FILE* sourse_file, FILE* output_file)
{
	assert(sourse_file != NULL);
	assert(output_file != NULL);

	int64_t sourse_file_size = 0;
	fseek(sourse_file, 0, SEEK_END);
	sourse_file_size = ftell(sourse_file);		//x64?
	fseek(sourse_file,0, SEEK_SET);

	if(sourse_file_size == 0)
	{
		perror("Empty sourse file\n");
		exit(1);
	}

	//==========================================================

	node nodes_buff[256+1];
	node_list list_buff[256];
	encode* encode_tab[256];

	memset(nodes_buff,	0, sizeof(nodes_buff));
	memset(list_buff,	0, sizeof(list_buff));
	memset(encode_tab,	0, sizeof(encode_tab));

	node_list* leaf_list = create_node_list(sourse_file, list_buff);
	node* tree = create_tree(leaf_list, nodes_buff);

	fill_encode_tab(tree, encode_tab);		// ошибка

	fwrite(&sourse_file_size, sizeof(int64_t), 1, output_file);
	write_encode_tab(output_file, encode_tab);

	encode_file(sourse_file, output_file, encode_tab);

	//==========================================================
	destroy_encode_tab(encode_tab);



	printf("Encode success\n");

	return 0;
}
