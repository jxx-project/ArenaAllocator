//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_H_INCLUDED
#define ARENA_H_INCLUDED

#include "Chunk.h"
#include "NativeCXXAllocator.h"
#include <cstdint>
#include <list>
#include <vector>

namespace CustomAllocators {

class Arena
{
public:
	using WordType = long;
	using ListType = std::list<Chunk, NativeCXXAllocator<Chunk>>;

	Arena(std::size_t nChunks, std::size_t chunkSize) noexcept;
	void* allocate(std::size_t size) noexcept;
	void deallocate(std::list<Chunk>::const_iterator it) noexcept;
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
	StorageType storage;
	ListType free;
	ListType allocated;
};

} // namespace CustomAllocators

#endif // ARENA_H_INCLUDED
