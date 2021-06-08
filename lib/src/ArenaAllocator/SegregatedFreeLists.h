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

namespace ArenaAllocator {

class SegregatedFreeLists : public Allocator
{
public:
	SegregatedFreeLists(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept;
	SegregatedFreeLists(SegregatedFreeLists const&) = delete;
	void operator=(SegregatedFreeLists const&) = delete;
	virtual ~SegregatedFreeLists() noexcept;

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

	static constexpr char const* className{"SegregatedFreeLists"};

private:
	Allocator* delegate;
	Logger const& log;
	PoolMap<FreeList> pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_SegregatedFreeLists_h_INCLUDED
