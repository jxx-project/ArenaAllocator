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

namespace ArenaAllocator {

PassThroughAllocator::PassThroughAllocator(Logger const& logger) noexcept : logger{logger}
{
}

PassThroughAllocator::~PassThroughAllocator() noexcept
{
}

void* PassThroughAllocator::malloc(std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_malloc(size);
	logger.log("PassThroughAllocator::malloc(%ld) -> %p [%ld ns]\n", size, result, timer.getNanoseconds());
	return result;
}

void PassThroughAllocator::free(void* ptr) noexcept
{
	Timer timer;
	__libc_free(ptr);
	logger.log("PassThroughAllocator::free(%p) [%ld ns]\n", ptr, timer.getNanoseconds());
}

void* PassThroughAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_calloc(nmemb, size);
	logger.log("PassThroughAllocator::calloc(%ld, %ld) -> %p [%ld ns]\n", nmemb, size, result, timer.getNanoseconds());
	return result;
}

void* PassThroughAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_realloc(ptr, size);
	logger.log("PassThroughAllocator::realloc(%p, %ld) -> %p [%ld ns]\n", ptr, size, result, timer.getNanoseconds());
	return result;
}

void* PassThroughAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_reallocarray(ptr, nmemb, size);
	logger.log(
		"PassThroughAllocator::reallocarray(%p, %ld, %ld) -> %p [%ld ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	return result;
}

} // namespace ArenaAllocator
