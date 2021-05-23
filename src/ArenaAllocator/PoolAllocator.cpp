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
}

PoolAllocator::~PoolAllocator() noexcept
{
}

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	Optional<Pool*> pool{pools.find(size)};
	if (pool.hasValue()) {
		result = pool.value()->allocate(size);
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
	Optional<Pool::ListType::const_iterator> chunk{chunks.find(ptr)};
	if (chunk.hasValue()) {
		chunk.value()->pool->deallocate(chunk.value());
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
	Optional<Pool::ListType::const_iterator> currentChunk{chunks.find(ptr)};
	if (currentChunk.hasValue()) {
		Optional<Pool*> newArena{pools.find(size)};
		if (newArena.hasValue()) {
			Pool* currentArenaPtr{currentChunk.value()->pool};
			Pool* newArenaPtr{newArena.value()};
			if (newArenaPtr == currentArenaPtr) {
				result = ptr;
			} else {
				result = newArenaPtr->allocate(size);
				std::memcpy(result, currentChunk.value()->data, std::min(currentChunk.value()->allocated, size));
				currentArenaPtr->deallocate(currentChunk.value());
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
