#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <malloc.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	{
		std::fprintf(stderr, "-------- malloc\n");
		errno = 0;
		void* ptr{::malloc(1)};
		std::fprintf(stderr, "-------- malloc(1) -> %p\n", ptr);
	}
	{
		std::fprintf(stderr, "-------- calloc\n");
		errno = 0;
		void* ptr{::calloc(1, 1)};
		std::fprintf(stderr, "-------- calloc(1, 1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- realloc\n");
		errno = 0;
		void* ptr1{::realloc(nullptr, 1)};
		std::fprintf(stderr, "-------- realloc(nullptr, 1) -> %p, errno: %d\n", ptr1, errno);
		errno = 0;
		void* ptr2{::realloc(ptr1, 1000)};
		// NOLINTNEXTLINE ptr1 will point to memory after free
		std::fprintf(stderr, "-------- realloc(%p, 1000) -> %p, errno: %d\n", ptr1, ptr2, errno);
	}
	return 0;
}
