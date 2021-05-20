#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include <cstdint>

class Allocator
{
public:
	virtual ~Allocator() noexcept = default;
	virtual void* malloc(std::size_t size) noexcept = 0;
	virtual void free(void* ptr) noexcept = 0;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept = 0;
	virtual void* realloc(void* ptr, std::size_t size) noexcept = 0;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept = 0;
};

#endif // ALLOCATOR_H_INCLUDED
