//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PassThroughAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);
extern "C" void* __libc_reallocarray(void* ptr, std::size_t nmemb, std::size_t size);

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
	int resultErrno;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_malloc(size);
		resultErrno = errno;
		logger.log("PassThroughAllocator::malloc(%ld) -> %p [%ld ns]\n", size, result, timer.getNanoseconds());
	} else {
		result = __libc_malloc(size);
		resultErrno = errno;
	}
	errno = resultErrno;
	return result;
}

void PassThroughAllocator::free(void* ptr) noexcept
{
	void* result;
	int resultErrno;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		__libc_free(ptr);
		resultErrno = errno;
		logger.log("PassThroughAllocator::free(%p) [%ld ns]\n", ptr, timer.getNanoseconds());
	} else {
		__libc_free(ptr);
		resultErrno = errno;
	}
	errno = resultErrno;
}

void* PassThroughAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	int resultErrno;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_calloc(nmemb, size);
		logger.log("PassThroughAllocator::calloc(%ld, %ld) -> %p [%ld ns]\n", nmemb, size, result, timer.getNanoseconds());
	} else {
		result = __libc_calloc(nmemb, size);
		resultErrno = errno;
	}
	errno = resultErrno;
	return result;
}

void* PassThroughAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result;
	int resultErrno;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_realloc(ptr, size);
		logger.log("PassThroughAllocator::realloc(%p, %ld) -> %p [%ld ns]\n", ptr, size, result, timer.getNanoseconds());
	} else {
		result = __libc_realloc(ptr, size);
		resultErrno = errno;
	}
	errno = resultErrno;
	return result;
}

void* PassThroughAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	int resultErrno;
	if (logger.isLevel(LogLevel::INFO)) {
		Timer timer;
		result = __libc_reallocarray(ptr, nmemb, size);
		logger.log(
			"PassThroughAllocator::reallocarray(%p, %ld, %ld) -> %p [%ld ns]\n", ptr, nmemb, size, result, timer.getNanoseconds());
	} else {
		result = __libc_reallocarray(ptr, nmemb, size);
		resultErrno = errno;
	}
	errno = resultErrno;
	return result;
}

} // namespace ArenaAllocator
