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
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer(OperationType::MALLOC);
		result = NativeAllocator::malloc(size);
		log(timer.getNanoseconds(), OperationType::MALLOC, [&] {
			return Message("{}::malloc({}) -> {}", className, size, result);
		});
	} else {
		result = NativeAllocator::malloc(size);
	}
	return result;
}

void PassThrough::free(void* ptr) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer(OperationType::FREE);
		NativeAllocator::free(ptr);
		log(timer.getNanoseconds(), OperationType::FREE, [&] { return Message("{}::free({})", className, ptr); });
	} else {
		NativeAllocator::free(ptr);
	}
}

void* PassThrough::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer(OperationType::CALLOC);
		result = NativeAllocator::calloc(nmemb, size);
		log(timer.getNanoseconds(), OperationType::CALLOC, [&] {
			return Message("{}::calloc({}, {}) -> {}", className, nmemb, size, result);
		});
	} else {
		result = NativeAllocator::calloc(nmemb, size);
	}
	return result;
}

void* PassThrough::realloc(void* ptr, std::size_t size) noexcept
{
	void* result{nullptr};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer(OperationType::REALLOC);
		result = NativeAllocator::realloc(ptr, size);
		log(timer.getNanoseconds(), OperationType::REALLOC, [&] {
			return Message("{}::realloc({}, {}) -> {}", className, ptr, size, result);
		});
	} else {
		result = NativeAllocator::realloc(ptr, size);
	}
	return result;
}

void PassThrough::dump() const noexcept
{
	// Nothing to do.
}

} // namespace ArenaAllocator
