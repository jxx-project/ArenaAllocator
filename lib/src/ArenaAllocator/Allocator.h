//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Allocator_h_INCLUDED
#define ArenaAllocator_Allocator_h_INCLUDED

#include <cstddef>
#include <map>
#include <unistd.h>

namespace ArenaAllocator {

class Allocator
{
public:
	virtual ~Allocator() noexcept = default;

	virtual void* malloc(std::size_t size) noexcept = 0;
	virtual void free(void* ptr) noexcept = 0;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept = 0;
	virtual void* realloc(void* ptr, std::size_t size) noexcept = 0;
	virtual void* mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept = 0;
	// virtual int munmap(void* addr, std::size_t length) noexcept = 0;
	virtual void dump() const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Allocator_h_INCLUDED
