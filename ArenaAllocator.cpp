#include "ArenaAllocator.h"
#include "Timer.h"

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
		result = arena.value()->allocate();
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
	Timer timer;
	void* result = nullptr;
	logger.log("ArenaAllocator::calloc(%ld, %ld) = %p, %ld ns\n", nmemb, size, result, timer.getNanoseconds());
	return result;
}

void* ArenaAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	logger.log("ArenaAllocator::realloc(%p, %ld) = %p, %ld ns\n", ptr, size, result, timer.getNanoseconds());
	return result;
}

void* ArenaAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer;
	void* result = nullptr;
	logger.log("ArenaAllocator::reallocarray(%p, %ld, %ld) = %p, %ld ns\n", ptr, nmemb, size, result, timer.getNanoseconds());
	return result;
}
