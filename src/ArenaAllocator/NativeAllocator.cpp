//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/NativeAllocator.h"
#include "ArenaAllocator/Timer.h"

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);
extern "C" void* __libc_reallocarray(void* ptr, std::size_t nmemb, std::size_t size);

namespace ArenaAllocator {

NativeAllocator::NativeAllocator(Logger const& logger) noexcept : logger{logger}
{
}

NativeAllocator::~NativeAllocator() noexcept
{
}

void* NativeAllocator::malloc(std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_malloc(size);
	logger.log("NativeAllocator::malloc(%ld) -> %p [%ld ns]\n", size, result, timer.getNanoseconds());
	return result;
}

void NativeAllocator::free(void* ptr) noexcept
{
	Timer timer;
	__libc_free(ptr);
	logger.log("NativeAllocator::free(%p) [%ld ns]\n", ptr, timer.getNanoseconds());
}

void* NativeAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_calloc(nmemb, size);
	logger.log("NativeAllocator::calloc(%ld, %ld) -> %p [%ld ns]\n", nmemb, size, result, timer.getNanoseconds());
	return result;
}

void* NativeAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_realloc(ptr, size);
	logger.log("NativeAllocator::realloc(%p, %ld) -> %p [%ld ns]\n", ptr, size, result, timer.getNanoseconds());
	return result;
}

void* NativeAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer;
	void* result = __libc_reallocarray(ptr, nmemb, size);
	logger.log("NativeAllocator::reallocarray(%p, %ld, %ld) -> %p [%ld ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	return result;
}

} // namespace ArenaAllocator
