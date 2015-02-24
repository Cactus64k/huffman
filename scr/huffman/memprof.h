#ifndef MEMPROF_H
#define MEMPROF_H

	void* prof_malloc(size_t size, char* file, int line);
	void* prof_calloc(size_t count, size_t eltsize, char* file, int line);
	void prof_free(void* ptr);
	void prof_print();
	void prof_clear();


	#ifndef DEBUG
		#define malloc(size)				prof_malloc(size, __FILE__, __LINE__)
		#define calloc(count, eltsize)		prof_calloc(count, eltsize,  __FILE__, __LINE__)
		#define free(ptr)					prof_free(ptr)
	#endif
#endif
