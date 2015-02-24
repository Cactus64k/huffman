#include "chunks.h"

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);

	int flag = 1;

	if(flag == 1)
	{
		FILE* src = fopen("/home/cactus/test_file.txt", "r");
		FILE* dst = fopen("/home/cactus/test_file.huff", "w");

		huffman_encode(src, dst);
	}
	else
	{
		FILE* dst = fopen("/home/cactus/test_file.txt", "w");
		FILE* src = fopen("/home/cactus/test_file.huff", "r");

		huffman_decode(src, dst);
	}

	return 0;
}
