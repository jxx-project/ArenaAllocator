#include "ArenaAllocator.h"
#include <chrono>

ArenaAllocator::ArenaAllocator(Configuration const& configuration, Logger const& logger, Allocator*& activeAllocator) noexcept :
	logger{logger}, activeAllocator{activeAllocator}, arenas{configuration}
{
}

ArenaAllocator::~ArenaAllocator() noexcept
{
}

void* ArenaAllocator::malloc(std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"ArenaAllocator::malloc(%ld) = %p, %ld ns\n",
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void ArenaAllocator::free(void* ptr) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"ArenaAllocator::free(%p)\n", ptr, std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
}

void* ArenaAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"ArenaAllocator::calloc(%ld, %ld) = %p, %ld ns\n",
		nmemb,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void* ArenaAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"ArenaAllocator::realloc(%p, %ld) = %p, %ld ns\n",
		ptr,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void* ArenaAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = nullptr;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"ArenaAllocator::reallocarray(%p, %ld, %ld) = %p, %ld ns\n",
		ptr,
		nmemb,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}
