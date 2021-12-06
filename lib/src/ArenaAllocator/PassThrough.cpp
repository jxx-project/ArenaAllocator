//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PassThrough.h"
#include "ArenaAllocator/Timer.h"
#include "NativeAllocator/malloc.h"
#include <cerrno>

namespace ArenaAllocator {

PassThrough::PassThrough(Logger const& log) noexcept : log{log}
{
	log(LogLevel::DEBUG, [&] { return Message("{}::{}(Logger const&) -> this:{}", className, className, this); });
}

PassThrough::~PassThrough() noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("{}::~{}(this:{})", className, className, this); });
}

void* PassThrough::malloc(std::size_t size) noexcept
{
	Timer timer(OperationType::MALLOC, size);
	void* result{NativeAllocator::malloc(size)};
	std::chrono::nanoseconds nanosecondsUsed{timer.getNanoseconds()};
	if (log.isLevel(LogLevel::TRACE)) {
		log(nanosecondsUsed, OperationType::MALLOC, [&] { return Message("{}::malloc({}) -> {}", className, size, result); });
	}
	return result;
}

void PassThrough::free(void* ptr) noexcept
{
	Timer timer(OperationType::FREE);
	NativeAllocator::free(ptr);
	std::chrono::nanoseconds nanosecondsUsed{timer.getNanoseconds()};
	if (log.isLevel(LogLevel::TRACE)) {
		log(nanosecondsUsed, OperationType::FREE, [&] { return Message("{}::free({})", className, ptr); });
	}
}

void* PassThrough::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	Timer timer(OperationType::CALLOC, nmemb * size);
	void* result{NativeAllocator::calloc(nmemb, size)};
	std::chrono::nanoseconds nanosecondsUsed{timer.getNanoseconds()};
	if (log.isLevel(LogLevel::TRACE)) {
		log(nanosecondsUsed, OperationType::CALLOC, [&] {
			return Message("{}::calloc({}, {}) -> {}", className, nmemb, size, result);
		});
	}
	return result;
}

void* PassThrough::realloc(void* ptr, std::size_t size) noexcept
{
	Timer timer(OperationType::REALLOC, size);
	void* result{NativeAllocator::realloc(ptr, size)};
	std::chrono::nanoseconds nanosecondsUsed{timer.getNanoseconds()};
	if (log.isLevel(LogLevel::TRACE)) {
		log(nanosecondsUsed, OperationType::REALLOC, [&] {
			return Message("{}::realloc({}, {}) -> {}", className, ptr, size, result);
		});
	}
	return result;
}

void PassThrough::dump() const noexcept
{
	// Nothing to do.
}

} // namespace ArenaAllocator
