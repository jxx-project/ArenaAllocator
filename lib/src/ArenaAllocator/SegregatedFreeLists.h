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
	void* mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept override;
	// int munmap(void* addr, std::size_t length) noexcept override;
	void dump() const noexcept override;

	static constexpr char const* className{"SegregatedFreeLists"};

private:
	Allocator* delegate;
	Logger const& log;
	PoolMap<FreeList> pools;
	const ChunkMap chunks;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_SegregatedFreeLists_h_INCLUDED
