//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_SegregatedFreeLists_h_INCLUDED
#define ArenaAllocator_SegregatedFreeLists_h_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/ChunkMap.h"
#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/FreeList.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PoolMap.h"
#include <cstddef>
#include <string_view>

namespace ArenaAllocator {

class SegregatedFreeLists : public Allocator
{
public:
	SegregatedFreeLists(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept;
	SegregatedFreeLists(SegregatedFreeLists const&) = delete;
	void operator=(SegregatedFreeLists const&) = delete;
	~SegregatedFreeLists() noexcept override;

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

	static constexpr std::string_view className{"SegregatedFreeLists"};

private:
	Allocator* delegate;
	Logger const& log;
	PoolMap<FreeList> pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_SegregatedFreeLists_h_INCLUDED
