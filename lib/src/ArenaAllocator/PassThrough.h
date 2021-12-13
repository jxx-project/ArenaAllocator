//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PassThrough_h_INCLUDED
#define ArenaAllocator_PassThrough_h_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Logger.h"
#include <cstddef>
#include <string_view>

namespace ArenaAllocator {

class PassThrough : public Allocator
{
public:
	explicit PassThrough(Logger const& log) noexcept;
	PassThrough(PassThrough const&) = delete;
	PassThrough& operator=(PassThrough const&) = delete;
	~PassThrough() noexcept override;

	void* malloc(std::size_t size) noexcept override;
	void free(void* ptr) noexcept override;
	void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	void* realloc(void* ptr, std::size_t size) noexcept override;
	void* mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept override;
	// int munmap(void* addr, std::size_t length) noexcept override;
	void dump() const noexcept override;

	static constexpr char const* className{"PassThrough"};

private:
	Logger const& log;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PassThrough_h_INCLUDED
