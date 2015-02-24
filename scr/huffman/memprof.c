#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#define MEMPROF_H			// чтобы не заменяло malloc в этом файле

#include "memprof.h"

typedef struct allc_list
{
	void* ptr;
	char* file;
	int line;
	int stat;

	struct allc_list* next;
} allc_list;

static allc_list* allocation_list;	// список всех выделений

void* prof_malloc(size_t size, char* file, int line)
{
	void* ptr = malloc(size);

	allc_list* list = allocation_list;

	allocation_list = malloc(sizeof(allc_list));
	assert(allocation_list != NULL);

	allocation_list->next	= list;
	allocation_list->ptr	= ptr;
	allocation_list->line	= line;
	allocation_list->file	= strrchr(file, '/')+1;		// чтобы указывало прямо на имя файла
	allocation_list->stat	= 1;

	return ptr;
}

void* prof_calloc(size_t count, size_t eltsize, char* file, int line)
{
	void* ptr = calloc(count, eltsize);

	allc_list* list = allocation_list;

	allocation_list = malloc(sizeof(allc_list));
	assert(allocation_list != NULL);

	allocation_list->next	= list;
	allocation_list->ptr	= ptr;
	allocation_list->line	= line;
	allocation_list->file	= strrchr(file, '/')+1;		// чтобы указывало прямо на имя файла
	allocation_list->stat	= 1;

	return ptr;
}

void prof_free(void* ptr)
{
	allc_list* cur_itm = allocation_list;
	while(cur_itm != NULL)
	{
		if(cur_itm->ptr == ptr)
		{
			cur_itm->stat = 0;
			break;
		}

		cur_itm = cur_itm->next;
	}

	free(ptr);
}

#ifndef DEBUG
	void prof_print()
	{
		fputs("\n\n\n"
				"№\t"
				"adress\t\t"
				"file\t\t"
				"line\t"
				"stat\n", stderr);

		allc_list* cur_itm = allocation_list;
		int i=0;
		int leak_count = 0;

		while(cur_itm != NULL)
		{
			fprintf(stderr, "%d\t", i);
			fprintf(stderr, "%.8X\t", (unsigned int)(uintptr_t)cur_itm->ptr);
			fprintf(stderr, "%s\t", cur_itm->file);
			fprintf(stderr, "%d\t", cur_itm->line);
			fprintf(stderr, "%d\t", cur_itm->stat);
			fputc('\n', stderr);

			leak_count += (cur_itm->stat == 1)? 1: 0;

			cur_itm = cur_itm->next;
			i++;
		}
		fputs("*************************\n", stderr);
		fprintf(stderr, "leaks count = %d\n", leak_count);

		fflush(stderr);
	}

	void prof_clear()
	{
		fputs("clear allocation list\n", stderr);
		allc_list* alc_itm = allocation_list;
		while(alc_itm != NULL)
		{
			allc_list* prev_itm = alc_itm;
			alc_itm = alc_itm->next;
			free(prev_itm);
		}

		allocation_list = NULL;
	}
#else
	void prof_print()
	{

	}

	void prof_clear()
	{

	}
#endif

