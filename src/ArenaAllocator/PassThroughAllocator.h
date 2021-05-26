//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_PASS_THROUGH_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_PASS_THROUGH_ALLOCATOR_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Logger.h"
#include <cstdint>

namespace ArenaAllocator {

class PassThroughAllocator : public Allocator
{
public:
	explicit PassThroughAllocator(Logger const& logger) noexcept;
	PassThroughAllocator(PassThroughAllocator const&) = delete;
	PassThroughAllocator& operator=(PassThroughAllocator const&) = delete;
	virtual ~PassThroughAllocator() noexcept;

	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_PASS_THROUGH_ALLOCATOR_H_INCLUDED
