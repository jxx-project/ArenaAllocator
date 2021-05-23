//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef POOL_H_INCLUDED
#define POOL_H_INCLUDED

#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/NativeCXXAllocator.h"
#include <cstdint>
#include <list>
#include <vector>

namespace ArenaAllocator {

class Pool
{
public:
	struct Range
	{
		std::size_t first;
		std::size_t last;
	};

	using WordType = long;
	using ListType = std::list<Chunk, NativeCXXAllocator<Chunk>>;

	Pool(Range const& range, std::size_t nChunks, Logger const& logger) noexcept;
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
	using StorageType = std::vector<WordType, NativeCXXAllocator<WordType>>;
	const Range range;
	StorageType storage;
	ListType free;
	ListType allocated;
	std::size_t hwm;
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // POOL_H_INCLUDED
