//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_H_INCLUDED

#include "CustomAllocators/Allocator.h"
#include "CustomAllocators/ArenaMap.h"
#include "CustomAllocators/ChunkMap.h"
#include "CustomAllocators/Configuration.h"
#include "CustomAllocators/Logger.h"
#include <cstdint>

namespace CustomAllocators {

class ArenaAllocator : public Allocator
{
public:
	ArenaAllocator(Configuration const& configuration, Logger const& logger) noexcept;
	ArenaAllocator(ArenaAllocator const&) = delete;
	void operator=(ArenaAllocator const&) = delete;
	virtual ~ArenaAllocator() noexcept;
	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
	ArenaMap arenas;
	const ChunkMap chunks;
};

} // namespace CustomAllocators

#endif // ARENA_ALLOCATOR_H_INCLUDED
