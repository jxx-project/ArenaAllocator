//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/Timer.h"
#include <cerrno>

namespace ArenaAllocator {

namespace {

const auto alignPageSize{[]() { return ::sysconf(_SC_PAGESIZE) <= sizeof(std::max_align_t); }};

} // namespace

PoolAllocator::PoolAllocator(Configuration const& configuration, Allocator* delegate, Logger const& log) noexcept :
	delegate{delegate}, log{log}, pools{configuration, log}, chunks{pools, delegate, log}
{
	log(LogLevel::DEBUG, "PoolAllocator::PoolAllocator(Configuration const&, Allocator*, Logger const&)");
}

PoolAllocator::~PoolAllocator() noexcept
{
	log(LogLevel::DEBUG, "PoolAllocator::~PoolAllocator()");
	if (log.isLevel(LogLevel::INFO)) {
		dump();
	}
}

void* PoolAllocator::malloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateMallocFunc{[this](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->malloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMallocFunc, ChunkMap::alignAlways);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::MALLOC, "PoolAllocator::malloc(%lu) -> %p", size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateMallocFunc, ChunkMap::alignAlways);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void PoolAllocator::free(void* ptr) noexcept
{
	ChunkMap::DeallocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.deallocate(ptr);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::FREE, "PoolAllocator::free(%p)", ptr);
		}
	} else {
		result = chunks.deallocate(ptr);
	}
	errno = result.propagateErrno;
}

void* PoolAllocator::calloc(std::size_t nmemb, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateCallocFunc{[this](std::size_t nmemb, std::size_t size) {
		ChunkMap::AllocateResult result{delegate->calloc(nmemb, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(nmemb, size, delegateCallocFunc);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::CALLOC, "PoolAllocator::calloc(%lu, %lu) -> %p", nmemb, size, result.ptr);
		}
	} else {
		result = chunks.allocate(nmemb, size, delegateCallocFunc);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::realloc(void* ptr, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::REALLOC, "PoolAllocator::realloc(%p, %lu) -> %p", ptr, size, result.ptr);
		}
	} else {
		result = chunks.reallocate(ptr, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, nmemb, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::REALLOCARRAY,
				"PoolAllocator::reallocarray(%p, %lu, %lu) -> %p",
				ptr,
				nmemb,
				size,
				result.ptr);
		}
	} else {
		result = chunks.reallocate(ptr, nmemb, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

int PoolAllocator::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateMemAlignFunc{[this, alignment](std::size_t size) {
		ChunkMap::AllocateResult result{nullptr, 0, true};
		result.propagateErrno = delegate->posix_memalign(&result.ptr, alignment, size);
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMemAlignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::POSIX_MEMALIGN,
				"PoolAllocator::posix_memalign(&%p, %lu, %lu) -> %p",
				*memptr,
				alignment,
				size,
				result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateMemAlignFunc, alignWordTypeSize);
	}
	*memptr = result.ptr;
	return result.propagateErrno;
}

void* PoolAllocator::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateAlignedAllocFunc{[this, alignment](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->aligned_alloc(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::ALIGNED_ALLOC,
				"PoolAllocator::aligned_alloc(%lu, %lu) -> %p",
				alignment,
				size,
				result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateAlignedAllocFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::valloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateVallocFunc{[this](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->valloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateVallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::VALLOC, "PoolAllocator::valloc(%lu) -> %p", size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateVallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::memalign(std::size_t alignment, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegateMemalignFunc{[this, alignment](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->memalign(alignment, size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	auto alignWordTypeSize{[alignment]() { return alignment <= sizeof(std::max_align_t); }};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegateMemalignFunc, alignWordTypeSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::MEMALIGN,
				"PoolAllocator::memalign(%lu, %lu) -> %p",
				alignment,
				size,
				result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateMemalignFunc, alignWordTypeSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* PoolAllocator::pvalloc(std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	auto delegatePvallocFunc{[this](std::size_t size) {
		ChunkMap::AllocateResult result{delegate->pvalloc(size), 0, true};
		result.propagateErrno = errno;
		return result;
	}};
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.allocate(size, delegatePvallocFunc, alignPageSize);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::PVALLOC, "PoolAllocator::pvalloc(%lu) -> %p", size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegatePvallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void PoolAllocator::dump() const noexcept
{
	pools.dump();
}

} // namespace ArenaAllocator
