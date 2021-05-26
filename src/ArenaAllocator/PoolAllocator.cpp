//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>
#include <cstring>

namespace ArenaAllocator {

PoolAllocator::PoolAllocator(Configuration const& configuration, Logger const& logger) noexcept :
	logger{logger}, pools{configuration, logger}, chunks{pools, logger}
{
	logger.log("PoolAllocator::PoolAllocator() ctor");
}

PoolAllocator::~PoolAllocator() noexcept
{
}

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	Pool* pool{pools.at(size)};
	if (pool) {
		result = pool->allocate(size);
	}
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("PoolAllocator::malloc(%ld) -> %p [%ld ns]\n", size, result, timer.getNanoseconds());
	return result;
}

void PoolAllocator::free(void* ptr) noexcept
{
	Timer timer;
	Pool::ListType::const_iterator const* chunk{chunks.at(ptr)};
	if (chunk) {
		(*chunk)->pool->deallocate(*chunk);
	}
	logger.log("PoolAllocator::free(%p) [%ld ns]\n", ptr, timer.getNanoseconds());
}

void* PoolAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	logger.log("PoolAllocator::calloc(%ld, %ld) not implemented\n", nmemb, size);
	abort();
	Timer timer;
	void* result = nullptr;
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("PoolAllocator::calloc(%ld, %ld) -> %p [%ld ns]\n", nmemb, size, result, timer.getNanoseconds());
	return result;
}

void* PoolAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	Pool::ListType::const_iterator const* currentChunk{chunks.at(ptr)};
	if (currentChunk) {
		Pool* newPool{pools.at(size)};
		if (newPool) {
			Pool* currentPool{(*currentChunk)->pool};
			if (newPool == currentPool) {
				result = ptr;
			} else {
				result = newPool->allocate(size);
				std::memcpy(result, (*currentChunk)->data, std::min((*currentChunk)->allocatedSize, size));
				currentPool->deallocate(*currentChunk);
			}
		}
	}
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("PoolAllocator::realloc(%p, %ld) -> %p [%ld ns]\n", ptr, size, result, timer.getNanoseconds());
	return result;
}

void* PoolAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	logger.log("PoolAllocator::reallocarray(%p, %ld, %ld): not implemented\n", ptr, nmemb, size);
	abort();
	Timer timer;
	void* result = nullptr;
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("PoolAllocator::reallocarray(%p, %ld, %ld) -> %p [%ld ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	return result;
}

} // namespace ArenaAllocator
