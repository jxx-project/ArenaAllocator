#ifndef NATIVE_ALLOCATOR_H_INCLUDED
#define NATIVE_ALLOCATOR_H_INCLUDED

#include "Allocator.h"
#include "Logger.h"
#include <cstdint>

class NativeAllocator : public Allocator
{
public:
	NativeAllocator(Logger const& logger) noexcept;
	NativeAllocator(NativeAllocator const&) = delete;
	NativeAllocator& operator=(NativeAllocator const&) = delete;
	virtual ~NativeAllocator() noexcept;
	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
};

#endif // NATIVE_ALLOCATOR_H_INCLUDED
