//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_ALLOCATOR_H_INCLUDED

#include <cstddef>

namespace ArenaAllocator {

class Allocator
{
public:
	virtual ~Allocator() noexcept = default;

	virtual void* malloc(std::size_t size) noexcept = 0;
	virtual void free(void* ptr) noexcept = 0;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept = 0;
	virtual void* realloc(void* ptr, std::size_t size) noexcept = 0;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept = 0;
	virtual int posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept = 0;
	virtual void* aligned_alloc(std::size_t alignment, std::size_t size) noexcept = 0;
	virtual void* valloc(std::size_t size) noexcept = 0;
	virtual void* memalign(std::size_t alignment, std::size_t size) noexcept = 0;
	virtual void* pvalloc(std::size_t size) noexcept = 0;
	virtual void dump() const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_ALLOCATOR_H_INCLUDED
