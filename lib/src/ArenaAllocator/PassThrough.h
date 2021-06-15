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
	void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;
	int posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept override;
	void* aligned_alloc(std::size_t alignment, std::size_t size) noexcept override;
	void* valloc(std::size_t size) noexcept override;
	void* memalign(std::size_t alignment, std::size_t size) noexcept override;
	void* pvalloc(std::size_t size) noexcept override;
	void dump() const noexcept override;

	static constexpr std::string_view className{"PassThrough"};

private:
	Logger const& log;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PassThrough_h_INCLUDED
