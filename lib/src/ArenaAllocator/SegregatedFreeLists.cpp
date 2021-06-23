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

void* SegregatedFreeLists::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, nmemb, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::REALLOCARRAY, [&] {
				return Message("{}::reallocarray({}, {}, {}) -> {}", className, ptr, nmemb, size, result.ptr);
			});
		}
	} else {
		result = chunks.reallocate(ptr, nmemb, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

int SegregatedFreeLists::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateMemAlignFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{nullptr, 0, true};
		result.propagateErrno = delegate->posix_memalign(&result.ptr, alignment, size);
		return result;
	}};
	auto alignWordTypeSize{[&]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMemAlignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::POSIX_MEMALIGN, [&] {
				return Message("{}::posix_memalign(&{}, {}, {}) -> {}", className, *memptr, alignment, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegateMemAlignFunc, alignWordTypeSize);
	}
	*memptr = result.ptr;
	return result.propagateErrno;
}

void* SegregatedFreeLists::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateAlignedAllocFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->aligned_alloc(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[&]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::ALIGNED_ALLOC, [&] {
				return Message("{}::aligned_alloc({}, {}) -> {}", className, alignment, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::valloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateVallocFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->valloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateVallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::VALLOC, [&] {
				return Message("{}::valloc({}) -> {}", className, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegateVallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::memalign(std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegateMemalignFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->memalign(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[&]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMemalignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::MEMALIGN, [&] {
				return Message("{}::memalign({}, {}) -> {}", className, alignment, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegateMemalignFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::pvalloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result{};
	auto delegatePvallocFunc{[&](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->pvalloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegatePvallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::PVALLOC, [&] {
				return Message("{}::pvalloc({}) -> {}", className, size, result.ptr);
			});
		}
	} else {
		result = chunks.allocate(size, delegatePvallocFunc, alignPageSize);
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
