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

namespace ArenaAllocator {

class PassThrough : public Allocator
{
public:
	explicit PassThrough(Logger const& log) noexcept;
	PassThrough(PassThrough const&) = delete;
	PassThrough& operator=(PassThrough const&) = delete;
	virtual ~PassThrough() noexcept;

	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;
	virtual int posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept override;
	virtual void* aligned_alloc(std::size_t alignment, std::size_t size) noexcept override;
	virtual void* valloc(std::size_t size) noexcept override;
	virtual void* memalign(std::size_t alignment, std::size_t size) noexcept override;
	virtual void* pvalloc(std::size_t size) noexcept override;
	virtual void dump() const noexcept override;

	static constexpr char const* className{"PassThrough"};

private:
	Logger const& log;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PassThrough_h_INCLUDED