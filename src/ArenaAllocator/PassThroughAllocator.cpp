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
extern "C" int __libc_posix_memalign(void** memptr, std::size_t alignment, std::size_t size);
extern "C" void* __libc_aligned_alloc(std::size_t alignment, std::size_t size);
extern "C" void* __libc_valloc(std::size_t size);
extern "C" void* __libc_memalign(std::size_t alignment, std::size_t size);
extern "C" void* __libc_pvalloc(std::size_t size);

namespace ArenaAllocator {

PassThroughAllocator::PassThroughAllocator(Logger const& log) noexcept : log{log}
{
	log(LogLevel::DEBUG, "\tPassThroughAllocator::PassThroughAllocator(Logger const&)");
}

PassThroughAllocator::~PassThroughAllocator() noexcept
{
	log(LogLevel::DEBUG, "\tPassThroughAllocator::~PassThroughAllocator()");
}

void* PassThroughAllocator::malloc(std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_malloc(size);
		log(timer.getNanoseconds(), OperationType::MALLOC, "PassThroughAllocator::malloc(%lu) -> %p", size, result);
	} else {
		result = __libc_malloc(size);
	}
	return result;
}

void PassThroughAllocator::free(void* ptr) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		__libc_free(ptr);
		log(timer.getNanoseconds(), OperationType::FREE, "PassThroughAllocator::free(%p)", ptr);
	} else {
		__libc_free(ptr);
	}
}

void* PassThroughAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_calloc(nmemb, size);
		log(timer.getNanoseconds(), OperationType::CALLOC, "PassThroughAllocator::calloc(%lu, %lu) -> %p", nmemb, size, result);
	} else {
		result = __libc_calloc(nmemb, size);
	}
	return result;
}

void* PassThroughAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_realloc(ptr, size);
		log(timer.getNanoseconds(), OperationType::REALLOC, "PassThroughAllocator::realloc(%p, %lu) -> %p", ptr, size, result);
	} else {
		result = __libc_realloc(ptr, size);
	}
	return result;
}

namespace {

// glibc as of version 2.31 implements __libc_reallocarray in terms of realloc, which results in a recursion
// into the custom allocator: Deflect into __libc_realloc directly.
void* reallocarrayUsingLibcRealloc(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (nmemb && size) {
		if (nmemb <= std::numeric_limits<std::size_t>::max() / size) {
			result = __libc_realloc(ptr, nmemb * size);
		};
	} else {
		// nmemb * size would overflow
		result = nullptr;
		errno = ENOMEM;
	}
	return result;
}

} // namespace

void* PassThroughAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = reallocarrayUsingLibcRealloc(ptr, nmemb, size);
		log(timer.getNanoseconds(),
			OperationType::REALLOCARRAY,
			"PassThroughAllocator::reallocarray(%p, %lu, %lu) -> %p",
			ptr,
			nmemb,
			size,
			result);
	} else {
		result = reallocarrayUsingLibcRealloc(ptr, nmemb, size);
	}
	return result;
}

namespace {

// glibc as of version 2.31 does not provide __libc_posix_memalign.
int posixMemalignUsingLibcMemalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	int result{0};
	if ((alignment & (alignment - 1)) == 0 && alignment >= sizeof(void*)) {
		int propagateErrno{errno};
		void* memalignResult{__libc_memalign(alignment, size)};
		if (memalignResult) {
			*memptr = memalignResult;
		} else {
			result = ENOMEM;
		}
		errno = propagateErrno;
	} else {
		result = EINVAL;
	}
	return result;
}

} // namespace

int PassThroughAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	int result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = posixMemalignUsingLibcMemalign(memptr, alignment, size);
		log(timer.getNanoseconds(),
			OperationType::POSIX_MEMALIGN,
			"PassThroughAllocator::posix_memalign(&%p, %lu %lu) -> %d",
			*memptr,
			alignment,
			size,
			result);
	} else {
		result = posixMemalignUsingLibcMemalign(memptr, alignment, size);
	}
	return result;
}

namespace {

// glibc as of version 2.31 does not provide __libc_aligned_alloc.
void* alignedAllocUsingLibcMemalign(std::size_t alignment, std::size_t size) noexcept
{
	return __libc_memalign(alignment, size);
}

} // namespace

void* PassThroughAllocator::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = alignedAllocUsingLibcMemalign(alignment, size);
		log(timer.getNanoseconds(),
			OperationType::ALIGNED_ALLOC,
			"PassThroughAllocator::aligned_alloc(%lu %lu) -> %p",
			alignment,
			size,
			result);
	} else {
		result = alignedAllocUsingLibcMemalign(alignment, size);
	}
	return result;
}

void* PassThroughAllocator::valloc(std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_valloc(size);
		log(timer.getNanoseconds(), OperationType::VALLOC, "PassThroughAllocator::valloc(%lu) -> %p", size, result);
	} else {
		result = __libc_valloc(size);
	}
	return result;
}

void* PassThroughAllocator::memalign(std::size_t alignment, std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_memalign(alignment, size);
		log(timer.getNanoseconds(),
			OperationType::MEMALIGN,
			"PassThroughAllocator::memalign(%lu %lu) -> %p",
			alignment,
			size,
			result);
	} else {
		result = __libc_memalign(alignment, size);
	}
	return result;
}

void* PassThroughAllocator::pvalloc(std::size_t size) noexcept
{
	void* result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_pvalloc(size);
		log(timer.getNanoseconds(), OperationType::PVALLOC, "PassThroughAllocator::pvalloc(%lu) -> %p", size, result);
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
