#include "ArenaAllocator.h"
#include <cstdint>

extern "C" void* malloc(std::size_t size)
{
	return ArenaAllocator::getInstance().malloc(size);
}
extern "C" void free(void* ptr)
{
	return ArenaAllocator::getInstance().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return ArenaAllocator::getInstance().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return ArenaAllocator::getInstance().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return ArenaAllocator::getInstance().reallocarray(ptr, nmemb, size);
}
