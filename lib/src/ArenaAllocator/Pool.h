//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Pool_h_INCLUDED
#define ArenaAllocator_Pool_h_INCLUDED

#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cstddef>
#include <list>
#include <mutex>
#include <vector>

namespace ArenaAllocator {

class Pool
{
public:
	using ListType = std::list<Chunk, PassThroughCXXAllocator<Chunk>>;

	Pool(SizeRange const& range, std::size_t nChunks, Logger const& log) noexcept;
	Pool(Pool const& other) = delete;
	Pool& operator=(Pool const& other) = delete;
	~Pool() noexcept;

	void* allocate(std::size_t size) noexcept;
	void* reallocate(ListType::iterator it, std::size_t size) noexcept;
	void deallocate(ListType::const_iterator it) noexcept;
	std::size_t nChunks() const noexcept;

	template<typename F>
	void forEachChunk(F f) noexcept
	{
		for (ListType::iterator it = free.begin(); it != free.end(); ++it) {
			f(it);
		}
		for (ListType::iterator it = allocated.begin(); it != allocated.end(); ++it) {
			f(it);
		}
	}

	void dump() const noexcept;

private:
	using StorageType = std::vector<std::max_align_t, PassThroughCXXAllocator<std::max_align_t>>;
	const SizeRange range;
	const std::size_t chunkSize;
	mutable std::mutex mutex;
	StorageType storage;
	ListType free;
	ListType allocated;
	Logger const& log;
	std::size_t hwm;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Pool_h_INCLUDED
