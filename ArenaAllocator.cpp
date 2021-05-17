#include "ArenaAllocator.h"
#include "Logger.h"
#include "NativeAllocator.h"

ArenaAllocator::ArenaAllocator() noexcept : passthrough{true}
{
}

void* ArenaAllocator::malloc(std::size_t size) noexcept
{
	void* result;
	if (passthrough) {
		result = NativeAllocator::malloc(size);
	} else {
		result = nullptr;
		Logger::log("ArenaAllocator::malloc(%ld) = %p\n", size, result);
	}
	return result;
}

void ArenaAllocator::free(void* ptr) noexcept
{
	if (passthrough) {
		NativeAllocator::free(ptr);
	} else {
		Logger::log("ArenaAllocator::free(%p)\n", ptr);
	}
}

void* ArenaAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (passthrough) {
		result = NativeAllocator::calloc(nmemb, size);
	} else {
		result = nullptr;
		Logger::log("ArenaAllocator::calloc(%ld, %ld) = %p\n", nmemb, size, result);
	}
	return result;
}

void* ArenaAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result;
	if (passthrough) {
		result = NativeAllocator::realloc(ptr, size);
	} else {
		result = nullptr;
		Logger::log("ArenaAllocator::realloc(%p, %ld) = %p\n", ptr, size, result);
	}
	return result;
}

void* ArenaAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (passthrough) {
		result = NativeAllocator::reallocarray(ptr, nmemb, size);
	} else {
		result = nullptr;
		Logger::log("ArenaAllocator::reallocarray(%p, %ld, %ld) = %p\n", ptr, nmemb, size, result);
	}
	return result;
}
