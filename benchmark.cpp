#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	constexpr std::size_t size{10000000};
	void* ptr;
	ptr = malloc(size);
	fprintf(stderr, "malloc(%ld)=%p\n", size, ptr);
	free(ptr);
	fprintf(stderr, "free(%p)\n", ptr);
	return 0;
};
