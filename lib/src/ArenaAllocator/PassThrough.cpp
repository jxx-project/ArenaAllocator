//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PassThrough.h"
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

PassThrough::PassThrough(Logger const& log) noexcept : log{log}
{
	log(LogLevel::DEBUG, [&] { return Format("{}::{}(Logger const&) -> this:{}", className, className, this); });
}

PassThrough::~PassThrough() noexcept
{
	log(LogLevel::DEBUG, [&] { return Format("{}::~{}(this:{})", className, className, this); });
}

void* PassThrough::malloc(std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_malloc(size);
		log(timer.getNanoseconds(), OperationType::MALLOC, [&] { return Format("{}::malloc({}) -> {}", className, size, result); });
	} else {
		result = __libc_malloc(size);
	}
	return result;
}

void PassThrough::free(void* ptr) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		__libc_free(ptr);
		log(timer.getNanoseconds(), OperationType::FREE, [&] { return Format("{}::free({})", className, ptr); });
	} else {
		__libc_free(ptr);
	}
}

void* PassThrough::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_calloc(nmemb, size);
		log(timer.getNanoseconds(), OperationType::CALLOC, [&] {
			return Format("{}::calloc({}, {}) -> {}", className, nmemb, size, result);
		});
	} else {
		result = __libc_calloc(nmemb, size);
	}
	return result;
}

void* PassThrough::realloc(void* ptr, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_realloc(ptr, size);
		log(timer.getNanoseconds(), OperationType::REALLOC, [&] {
			return Format("{}::realloc({}, {}) -> {}", className, ptr, size, result);
		});
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
	void* result{nullptr};
	if (size > 0 && nmemb > std::numeric_limits<std::size_t>::max() / size) {
		// nmemb * size would overflow
		errno = ENOMEM;
	} else {
		result = __libc_realloc(ptr, nmemb * size);
	}
	return result;
}

} // namespace

void* PassThrough::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = reallocarrayUsingLibcRealloc(ptr, nmemb, size);
		log(timer.getNanoseconds(), OperationType::REALLOCARRAY, [&] {
			return Format("{}::reallocarray({}, {}, {}) -> {}", className, ptr, nmemb, size, result);
		});
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
		if (memalignResult != nullptr) {
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

int PassThrough::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	int result{0};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = posixMemalignUsingLibcMemalign(memptr, alignment, size);
		log(timer.getNanoseconds(), OperationType::POSIX_MEMALIGN, [&] {
			return Format("{}::posix_memalign(&{}, {} {}) -> %d", className, *memptr, alignment, size, result);
		});
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

void* PassThrough::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = alignedAllocUsingLibcMemalign(alignment, size);
		log(timer.getNanoseconds(), OperationType::ALIGNED_ALLOC, [&] {
			return Format("{}::aligned_alloc({} {}) -> {}", className, alignment, size, result);
		});
	} else {
		result = alignedAllocUsingLibcMemalign(alignment, size);
	}
	return result;
}

void* PassThrough::valloc(std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_valloc(size);
		log(timer.getNanoseconds(), OperationType::VALLOC, [&] { return Format("PassThrough::valloc({}) -> {}", size, result); });
	} else {
		result = __libc_valloc(size);
	}
	return result;
}

void* PassThrough::memalign(std::size_t alignment, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_memalign(alignment, size);
		log(timer.getNanoseconds(), OperationType::MEMALIGN, [&] {
			return Format("{}::memalign({} {}) -> {}", className, alignment, size, result);
		});
	} else {
		result = __libc_memalign(alignment, size);
	}
	return result;
}

void* PassThrough::pvalloc(std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = __libc_pvalloc(size);
		log(timer.getNanoseconds(), OperationType::PVALLOC, [&] {
			return Format("{}::pvalloc({}) -> {}", className, size, result);
		});
	} else {
		result = __libc_pvalloc(size);
	}
	return result;
}

void PassThrough::dump() const noexcept
{
	// Nothing to do.
}

} // namespace ArenaAllocator
