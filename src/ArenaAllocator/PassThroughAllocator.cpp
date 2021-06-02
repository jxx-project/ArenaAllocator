//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PassThroughAllocator.h"
#include "ArenaAllocator/Timer.h"

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);
extern "C" void* __libc_reallocarray(void* ptr, std::size_t nmemb, std::size_t size);
extern "C" int __libc_posix_memalign(void** memptr, std::size_t alignment, std::size_t size);
extern "C" void* __libc_aligned_alloc(std::size_t alignment, std::size_t size);
extern "C" void* __libc_valloc(std::size_t size);
extern "C" void* __libc_memalign(std::size_t alignment, std::size_t size);
extern "C" void* __libc_pvalloc(std::size_t size);

namespace ArenaAllocator {

PassThroughAllocator::PassThroughAllocator(Logger const& logger) noexcept : logger{logger}
{
	logger.debug("PassThroughAllocator::PassThroughAllocator(Logger const&)\n");
}

PassThroughAllocator::~PassThroughAllocator() noexcept
{
	logger.debug("PassThroughAllocator::~PassThroughAllocator()\n");
}

void* PassThroughAllocator::malloc(std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_malloc(size);
		logger.log("PassThroughAllocator::malloc(%lu) -> %p [%lu ns]\n", size, result, timer.getNanoseconds());
	} else {
		result = __libc_malloc(size);
	}
	return result;
}

void PassThroughAllocator::free(void* ptr) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		__libc_free(ptr);
		logger.log("PassThroughAllocator::free(%p) [%lu ns]\n", ptr, timer.getNanoseconds());
	} else {
		__libc_free(ptr);
	}
}

void* PassThroughAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_calloc(nmemb, size);
		logger.log("PassThroughAllocator::calloc(%lu, %lu) -> %p [%lu ns]\n", nmemb, size, result, timer.getNanoseconds());
	} else {
		result = __libc_calloc(nmemb, size);
	}
	return result;
}

void* PassThroughAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_realloc(ptr, size);
		logger.log("PassThroughAllocator::realloc(%p, %lu) -> %p [%lu ns]\n", ptr, size, result, timer.getNanoseconds());
	} else {
		result = __libc_realloc(ptr, size);
	}
	return result;
}

void* PassThroughAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_reallocarray(ptr, nmemb, size);
		logger.log(
			"PassThroughAllocator::reallocarray(%p, %lu, %lu) -> %p [%lu ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	} else {
		result = __libc_reallocarray(ptr, nmemb, size);
	}
	return result;
}

int PassThroughAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	int result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_posix_memalign(memptr, alignment, size);
		logger.log(
			"PassThroughAllocator::posix_memalign(&%p, %lu %lu) -> %d [%lu ns]\n",
			*memptr,
			alignment,
			size,
			result,
			timer.getNanoseconds());
	} else {
		result = __libc_posix_memalign(memptr, alignment, size);
	}
	return result;
}

void* PassThroughAllocator::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_aligned_alloc(alignment, size);
		logger.log(
			"PassThroughAllocator::aligned_alloc(%lu %lu) -> %p [%lu ns]\n", alignment, size, result, timer.getNanoseconds());
	} else {
		result = __libc_aligned_alloc(alignment, size);
	}
	return result;
}

void* PassThroughAllocator::valloc(std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_valloc(size);
		logger.log("PassThroughAllocator::valloc(%lu) -> %p [%lu ns]\n", size, result, timer.getNanoseconds());
	} else {
		result = __libc_valloc(size);
	}
	return result;
}

void* PassThroughAllocator::memalign(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_memalign(alignment, size);
		logger.log("PassThroughAllocator::memalign(%lu %lu) -> %p [%lu ns]\n", alignment, size, result, timer.getNanoseconds());
	} else {
		result = __libc_memalign(alignment, size);
	}
	return result;
}

void* PassThroughAllocator::pvalloc(std::size_t size) noexcept
{
	void* result;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_pvalloc(size);
		logger.log("PassThroughAllocator::pvalloc(%lu) -> %p [%lu ns]\n", size, result, timer.getNanoseconds());
	} else {
		result = __libc_pvalloc(size);
	}
	return result;
}

void PassThroughAllocator::dump() const noexcept
{
	// Nothing to do.
}

} // namespace ArenaAllocator
