//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_POOL_H_INCLUDED
#define ARENA_ALLOCATOR_POOL_H_INCLUDED

#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cstdint>
#include <list>
#include <mutex>
#include <vector>

namespace ArenaAllocator {

class Pool
{
public:
	using WordType = long;
	using ListType = std::list<Chunk, PassThroughCXXAllocator<Chunk>>;

	Pool(SizeRange const& range, std::size_t nChunks, Logger const& logger) noexcept;
	Pool(Pool const& other) = delete;
	Pool& operator=(Pool const& other) = delete;
	~Pool() noexcept;

	void* allocate(std::size_t size) noexcept;
	void deallocate(ListType::const_iterator it) noexcept;
	std::size_t nChunks() const noexcept;

	template<typename F>
	void forEachChunk(F func) const noexcept
	{
		for (ListType::const_iterator it = free.begin(); it != free.end(); ++it) {
			func(it);
		}
		for (ListType::const_iterator it = allocated.begin(); it != allocated.end(); ++it) {
			func(it);
		}
	}

private:
	using StorageType = std::vector<WordType, PassThroughCXXAllocator<WordType>>;
	const SizeRange range;
	std::mutex mutex;
	StorageType storage;
	ListType free;
	ListType allocated;
	Logger const& logger;
	std::size_t hwm;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_POOL_H_INCLUDED
