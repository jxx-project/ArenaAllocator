#ifndef NATIVE_ALLOCATOR_H_INCLUDED
#define NATIVE_ALLOCATOR_H_INCLUDED

#include <cstdint>

class NativeAllocator
{
public:
	NativeAllocator() = delete;
	NativeAllocator(NativeAllocator const&) = delete;
	void operator=(NativeAllocator const&) = delete;
	static void* malloc(std::size_t size) noexcept;
	static void free(void* ptr) noexcept;
	static void* calloc(std::size_t nmemb, std::size_t size) noexcept;
	static void* realloc(void* ptr, std::size_t size) noexcept;
	static void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept;
};

#endif // NATIVE_ALLOCATOR_H_INCLUDED
