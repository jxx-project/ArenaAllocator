//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/SegregatedFreeLists.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>

namespace ArenaAllocator {

namespace {

const auto alignPageSize{[]() { return ::sysconf(_SC_PAGESIZE) <= sizeof(std::max_align_t); }};

} // namespace

SegregatedFreeLists::SegregatedFreeLists(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept :
	delegate{delegate}, log{log}, pools{configuration, log}, chunks{pools, delegate, log}
{
	log(LogLevel::DEBUG,
		[&] { return Message("{}::{}(Configuration const&, Allocator*, Logger const&) -> this:{}", className, className, this); });
}

SegregatedFreeLists::~SegregatedFreeLists() noexcept
{
	log(LogLevel::DEBUG, [&] { return Message("{}::~{}(this:{})", className, className, this); });
}

void* SegregatedFreeLists::malloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateMallocFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->malloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMallocFunc, ChunkMap::alignAlways);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::MALLOC, [&] {
				return Message("{}::malloc({}) -> {}", className, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegateMallocFunc, ChunkMap::alignAlways);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void SegregatedFreeLists::free(void* ptr) noexcept
{
	ChunkMap::DeallocateResult result{};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.deallocate(ptr);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::FREE, [&] { return Message("{}::free({})", className, ptr); });
		}
	} else {
		result = chunks.deallocate(ptr);
	}
	errno = result.propagateErrno;
}

void* SegregatedFreeLists::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateCallocFunc{[&](std::size_t nmemb, std::size_t size) {
		ChunkMap::AllocateResult result{delegate->calloc(nmemb, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(nmemb, size, delegateCallocFunc);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::CALLOC, [&] {
				return Message("{}::calloc({}, {}) -> {}", className, nmemb, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(nmemb, size, delegateCallocFunc);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::realloc(void* ptr, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::REALLOC, [&] {
				return Message("{}::realloc({}, {}) -> {}", className, ptr, size, result.ptr);
			});
		}
	} else {
		result = chunks.reallocate(ptr, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void SegregatedFreeLists::dump() const noexcept
{
	if (log.isLevel(LogLevel::INFO)) {
		pools.dump();
	}
}

} // namespace ArenaAllocator
