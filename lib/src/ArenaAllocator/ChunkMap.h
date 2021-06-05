//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED
#define ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/PoolMap.h"
#include <limits>
#include <unistd.h>
#include <unordered_map>

namespace ArenaAllocator {

class ChunkMap
{
public:
	struct AllocateResult
	{
		void* ptr;
		int propagateErrno;
		bool fromDelegate;
	};

	struct DeallocateResult
	{
		int propagateErrno;
		bool fromDelegate;
	};

	ChunkMap(PoolMap<Pool>& pools, Allocator* delegate, Logger const& log) noexcept;

	template<typename DelegateF, typename AlignmentPredicate>
	AllocateResult allocate(std::size_t size, DelegateF delegateF, AlignmentPredicate alignmentPredicate) const noexcept
	{
		AllocateResult result{nullptr, 0, false};
		if (size) {
			Pool* pool{alignmentPredicate() ? pools.at(size) : nullptr};
			if (pool) {
				if (!(result.ptr = pool->allocate(size))) {
					result.propagateErrno = ENOMEM;
				}
			} else {
				result = delegateF(size);
			}
		} else {
			result.ptr = ptrToEmpty;
		}
		log(LogLevel::DEBUG, "ChunkMap::allocate(%lu) -> %p", size, result.ptr);
		return result;
	}

	DeallocateResult deallocate(void* ptr) const noexcept;

	template<typename DelegateF>
	AllocateResult allocate(std::size_t nmemb, std::size_t size, DelegateF delegateF) const noexcept
	{
		AllocateResult result{nullptr, 0, false};
		if (nmemb <= std::numeric_limits<std::size_t>::max() / size) {
			std::size_t totalSize{nmemb * size};
			if (totalSize) {
				Pool* pool{pools.at(totalSize)};
				if (pool) {
					if (!(result.ptr = pool->allocate(totalSize))) {
						result.propagateErrno = ENOMEM;
					}
				} else {
					result = delegateF(nmemb, size);
				}
			} else {
				result.ptr = ptrToEmpty;
			}
		} else {
			// nmemb * size would overflow
			result.ptr = nullptr;
			result.propagateErrno = ENOMEM;
		}
		return result;
	}

	AllocateResult reallocate(void* ptr, std::size_t size) const noexcept;
	AllocateResult reallocate(void* ptr, std::size_t nmemb, std::size_t size) const noexcept;
	AllocateResult reallocate(Pool::ListType::iterator const& currentChunk, std::size_t size) const noexcept;

	static constexpr auto alignAlways{[]() { return true; }};

private:
	using AggregateType = std::unordered_map<
		void*,
		Pool::ListType::iterator,
		std::hash<void*>,
		std::equal_to<void*>,
		PassThroughCXXAllocator<std::pair<void* const, Pool::ListType::iterator>>>;

	void* const ptrToEmpty;
	Allocator* delegate;
	Logger const& log;
	PoolMap<Pool>& pools;
	AggregateType chunks;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_CHUNK_MAP_H_INCLUDED