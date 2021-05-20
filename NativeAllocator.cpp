#include "NativeAllocator.h"
#include <chrono>

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);
extern "C" void* __libc_reallocarray(void* ptr, std::size_t nmemb, std::size_t size);

NativeAllocator::NativeAllocator(Logger const& logger) noexcept : logger{logger}
{
}

NativeAllocator::~NativeAllocator() noexcept
{
}

void* NativeAllocator::malloc(std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = __libc_malloc(size);
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"NativeAllocator::malloc(%ld) = %p, %ld ns\n",
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void NativeAllocator::free(void* ptr) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	__libc_free(ptr);
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"NativeAllocator::free(%p, %ld ns)\n",
		ptr,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
}

void* NativeAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = __libc_calloc(nmemb, size);
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"NativeAllocator::calloc(%ld, %ld) = %p, %ld ns\n",
		nmemb,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void* NativeAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = __libc_realloc(ptr, size);
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"NativeAllocator::realloc(%p, %ld) = %p, %ld ns\n",
		ptr,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}

void* NativeAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime{std::chrono::high_resolution_clock::now()};
	void* result = __libc_reallocarray(ptr, nmemb, size);
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime{std::chrono::high_resolution_clock::now()};
	logger.log(
		"NativeAllocator::reallocarray(%p, %ld, %ld) = %p, %ld ns\n",
		ptr,
		nmemb,
		size,
		result,
		std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());
	return result;
}
