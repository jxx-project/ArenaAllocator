//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ChunkMap.h"
#include <algorithm>
#include <cstring>

namespace ArenaAllocator {

namespace {

void* getPtrToEmpty()
{
	static struct
	{
	} empty;
	return &empty;
}

} // namespace

ChunkMap::ChunkMap(PoolMap<FreeList>& pools, Allocator* delegate, Logger const& log) noexcept :
	ptrToEmpty{getPtrToEmpty()}, delegate{delegate}, log{log}, pools{pools}
{
	chunks.reserve(pools.nChunks());
	pools.forEachChunk([&](FreeList::ListType::iterator it) { chunks.insert(AggregateType::value_type(it->data, it)); });
}

ChunkMap::DeallocateResult ChunkMap::deallocate(void* ptr) const noexcept
{
	DeallocateResult result{0, false};
	if (ptr != nullptr && ptr != ptrToEmpty) {
		AggregateType::const_iterator it{chunks.find(ptr)};
		if (it != chunks.end()) {
			it->second->pool->deallocate(it->second);
		} else if (delegate != nullptr) {
			delegate->free(ptr);
			result = {errno, true};
		}
	}
	return result;
}

ChunkMap::AllocateResult ChunkMap::reallocate(void* ptr, std::size_t size) const noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (ptr != nullptr && ptr != ptrToEmpty) {
		AggregateType::const_iterator it{chunks.find(ptr)};
		if (it != chunks.end()) {
			result = reallocate(it->second, size);
		} else if (delegate != nullptr) {
			result.ptr = delegate->realloc(ptr, size);
			result.propagateErrno = errno;
			result.fromDelegate = true;
		}
	} else {
		result = allocate(
			size,
			[&](std::size_t size) {
				AllocateResult result{delegate->realloc(nullptr, size), 0, true};
				result.propagateErrno = errno;
				return result;
			},
			alignAlways);
	}
	return result;
}

ChunkMap::AllocateResult ChunkMap::reallocate(void* ptr, std::size_t nmemb, std::size_t size) const noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (ptr != nullptr && ptr != ptrToEmpty) {
		AggregateType::const_iterator it{chunks.find(ptr)};
		if (it != chunks.end()) {
			if (size > 0 && nmemb > std::numeric_limits<std::size_t>::max() / size) {
				// nmemb * size would overflow
				result.ptr = nullptr;
				result.propagateErrno = ENOMEM;
			} else {
				result = reallocate(it->second, nmemb * size);
			}
		} else if (delegate != nullptr) {
			result.ptr = delegate->reallocarray(ptr, nmemb, size);
			result.propagateErrno = errno;
			result.fromDelegate = true;
		}
	} else {
		result = allocate(nmemb, size, [&](std::size_t nmemb, std::size_t size) {
			AllocateResult result{delegate->reallocarray(nullptr, nmemb, size), 0, true};
			result.propagateErrno = errno;
			return result;
		});
	}
	return result;
}

ChunkMap::AllocateResult ChunkMap::reallocate(FreeList::ListType::iterator const& currentChunk, std::size_t size) const noexcept
{
	AllocateResult result{nullptr, 0, false};
	if (size > 0) {
		FreeList* destinationPool{pools.at(size)};
		if (destinationPool != nullptr) {
			FreeList* currentPool{currentChunk->pool};
			if (destinationPool == currentPool) {
				result.ptr = currentPool->reallocate(currentChunk, size);
			} else {
				if ((result.ptr = destinationPool->allocate(size)) != nullptr) {
					std::memcpy(result.ptr, currentChunk->data, std::min(currentChunk->allocatedSize, size));
					currentPool->deallocate(currentChunk);
				} else {
					result.propagateErrno = ENOMEM;
				}
			}
		} else {
			result.propagateErrno = ENOMEM;
		}
	} else {
		currentChunk->pool->deallocate(currentChunk);
	}
	return result;
}

} // namespace ArenaAllocator
