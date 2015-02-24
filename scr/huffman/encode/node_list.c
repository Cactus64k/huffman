#include "../huffman.h"


void q_sort(node_list* a, size_t ln)
{
	assert(a != NULL);
	assert(ln != 0);

	int lp = 0;
	int rp = ln;
	float p = (*a[ln/2].nd).prob;
	do {
		while (lp <= ln && a[lp].nd->prob < p) lp++;
		while (rp >= 0  && a[rp].nd->prob > p) rp--;

		if (lp <= rp)
		{
			node *temp = a[lp].nd;
			a[lp].nd = a[rp].nd;
			a[rp].nd = temp;
			lp++;
			rp--;
		}
	} while (lp <= rp);
	if (rp > 0)  q_sort(a, rp);
	if (ln > lp) q_sort(a+lp, ln-lp);
}

node_list* create_node_list(FILE* f, node_list* buff)		// собираем статистику о 4 килобайт файла
{
	assert(f != NULL);
	assert(buff != NULL);

	fpos_t file_pos;
	int res;

	res = fgetpos(f, &file_pos);				// запоминаем текущую позицию в файле
	assert(res == 0);

	node_list* list = buff;						// буфер на 256 node_list
	assert(list != NULL);

	for(size_t i=0; i<256; i++)					// инициализируем массив нодов
	{
		list[i].next		= list+i+1;
		list[i].nd			= malloc(sizeof(node));
		assert(list[i].nd  != NULL);

		memset(list[i].nd, 0, sizeof(node));

		list[i].nd->left	= (node*)(uintptr_t)i;
	}
	list[255].next = NULL;

	int byte = 0;
	while((byte=fgetc(f)) != EOF)
		list[byte].nd->prob += FLT_MIN;

	q_sort(list, 256-1);						// все отсутствующие символы будут в начале

	node_list* cur_itm = list;
	while(cur_itm->nd->prob == 0)				// бинарный поиск тут не нужен, т.к. все равно нужно освобождать незадействованные ноды
	{
		free(cur_itm->nd);
		cur_itm->nd = NULL;
		cur_itm = cur_itm->next;

		assert(cur_itm != NULL);				// не должно быть такого, что обрабатываем пустой хвост файла
	}

	*list = *cur_itm;							// копируем содержимое первого не отсутствующего символа в первый элемент массива

	res = fsetpos(f, &file_pos);				// возвращаем кодируемый файл в исходное положение
	assert(res == 0);


	return list;
}
