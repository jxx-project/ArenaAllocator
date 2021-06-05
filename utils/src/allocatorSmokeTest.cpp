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
		std::fprintf(stderr, "-------- realloc(%p, 1000) -> %p, errno: %d\n", ptr1, ptr2, errno);
	}
	{
		std::fprintf(stderr, "-------- reallocarray\n");
		errno = 0;
		void* ptr1{::reallocarray(nullptr, 1, 1)};
		std::fprintf(stderr, "-------- reallocarray(nullptr, 1, 1) -> %p, errno: %d\n", ptr1, errno);
		errno = 0;
		void* ptr2{::reallocarray(ptr1, 1, 1000)};
		std::fprintf(stderr, "-------- reallocarray(%p, 1, 1000) -> %p, errno: %d\n", ptr1, ptr2, errno);
		errno = 0;
		void* ptr3{::reallocarray(ptr2, 9223372036854775807UL, 9223372036854775807UL)};
		std::fprintf(
			stderr, "-------- reallocarray(%p, 9223372036854775807, 9223372036854775807) -> %p, errno: %d\n", ptr2, ptr3, errno);
		errno = 0;
		int invalid;
		void* ptr4{::reallocarray(&invalid, 1, 1)};
		std::fprintf(stderr, "-------- reallocarray(&invalid, 1, 1) -> %p, errno: %d\n", ptr4, errno);
	}
	{
		std::fprintf(stderr, "-------- posix_memalign\n");
		void* ptr{nullptr};
		errno = 0;
		int result{::posix_memalign(&ptr, 1024, 1)};
		std::fprintf(stderr, "-------- posix_memalign(&%p, 1024, 1) -> %d, errno: %d\n", ptr, result, errno);
	}
	{
		std::fprintf(stderr, "-------- posix_memalign\n");
		void* ptr{nullptr};
		errno = 0;
		int result{::posix_memalign(&ptr, 3, 1)};
		std::fprintf(stderr, "-------- posix_memalign(&%p, 3, 1) -> %d, errno: %d\n", ptr, result, errno);
	}
	{
		std::fprintf(stderr, "-------- aligned_alloc\n");
		errno = 0;
		void* ptr{::aligned_alloc(1024, 1)};
		std::fprintf(stderr, "-------- aligned_alloc(1024, 1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- aligned_alloc\n");
		errno = 0;
		void* ptr{::aligned_alloc(3, 1)};
		std::fprintf(stderr, "-------- aligned_alloc(3, 1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- valloc\n");
		errno = 0;
		void* ptr{::valloc(1)};
		std::fprintf(stderr, "-------- valloc(1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- memalign\n");
		errno = 0;
		void* ptr{::memalign(1024, 1)};
		std::fprintf(stderr, "-------- memalign(1024, 1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- memalign\n");
		errno = 0;
		void* ptr{::memalign(30001, 1)};
		std::fprintf(stderr, "-------- memalign(30001, 1) -> %p, errno: %d\n", ptr, errno);
	}
	{
		std::fprintf(stderr, "-------- pvalloc\n");
		errno = 0;
		void* ptr{::pvalloc(1)};
		std::fprintf(stderr, "-------- pvalloc(1) -> %p, errno: %d\n", ptr, errno);
	}
	return 0;
}
