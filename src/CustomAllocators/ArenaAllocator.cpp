//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator.h"
#include "Timer.h"
#include <cerrno>
#include <cstring>

namespace CustomAllocators {

ArenaAllocator::ArenaAllocator(Configuration const& configuration, Logger const& logger) noexcept :
	logger{logger}, arenas{configuration, logger}, chunks{arenas, logger}
{
}

ArenaAllocator::~ArenaAllocator() noexcept
{
}

void* ArenaAllocator::malloc(std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	Optional<Arena*> arena{arenas.find(size)};
	if (arena.hasValue()) {
		result = arena.value()->allocate(size);
	}
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("ArenaAllocator::malloc(%ld) = %p, %ld ns\n", size, result, timer.getNanoseconds());
	return result;
}

void ArenaAllocator::free(void* ptr) noexcept
{
	Timer timer;
	Optional<Arena::ListType::const_iterator> chunk{chunks.find(ptr)};
	if (chunk.hasValue()) {
		chunk.value()->arena->deallocate(chunk.value());
	}
	logger.log("ArenaAllocator::free(%p)\n", ptr, timer.getNanoseconds());
}

void* ArenaAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	logger.log("ArenaAllocator::calloc(%ld, %ld) not implemented\n", nmemb, size);
	abort();
	Timer timer;
	void* result = nullptr;
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("ArenaAllocator::calloc(%ld, %ld) = %p, %ld ns\n", nmemb, size, result, timer.getNanoseconds());
	return result;
}

void* ArenaAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	Optional<Arena::ListType::const_iterator> currentChunk{chunks.find(ptr)};
	if (currentChunk.hasValue()) {
		Optional<Arena*> newArena{arenas.find(size)};
		if (newArena.hasValue()) {
			Arena* currentArenaPtr{currentChunk.value()->arena};
			Arena* newArenaPtr{newArena.value()};
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
	logger.log("ArenaAllocator::realloc(%p, %ld) = %p, %ld ns\n", ptr, size, result, timer.getNanoseconds());
	return result;
}

void* ArenaAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	logger.log("ArenaAllocator::reallocarray(%p, %ld, %ld): not implemented\n", ptr, nmemb, size);
	abort();
	Timer timer;
	void* result = nullptr;
	if (!result) {
		errno = ENOMEM;
	}
	logger.log("ArenaAllocator::reallocarray(%p, %ld, %ld) = %p, %ld ns\n", ptr, nmemb, size, result, timer.getNanoseconds());
	return result;
}

} // namespace CustomAllocators
