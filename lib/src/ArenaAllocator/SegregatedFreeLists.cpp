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
	log(LogLevel::DEBUG, "%s::%s(Configuration const&, Allocator*, Logger const&) -> this:%p", className, className, this);
}

SegregatedFreeLists::~SegregatedFreeLists() noexcept
{
	log(LogLevel::DEBUG, "%s::~%s(this:%p)", className, className, this);
	if (log.isLevel(LogLevel::INFO)) {
		dump();
	}
}

void* SegregatedFreeLists::malloc(std::size_t size) noexcept
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
			log(timer.getNanoseconds(), OperationType::MALLOC, "%s::malloc(%lu) -> %p", className, size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateMallocFunc, ChunkMap::alignAlways);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void SegregatedFreeLists::free(void* ptr) noexcept
{
	ChunkMap::DeallocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.deallocate(ptr);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::FREE, "%s::free(%p)", className, ptr);
		}
	} else {
		result = chunks.deallocate(ptr);
	}
	errno = result.propagateErrno;
}

void* SegregatedFreeLists::calloc(std::size_t nmemb, std::size_t size) noexcept
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
			log(timer.getNanoseconds(), OperationType::CALLOC, "%s::calloc(%lu, %lu) -> %p", className, nmemb, size, result.ptr);
		}
	} else {
		result = chunks.allocate(nmemb, size, delegateCallocFunc);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::realloc(void* ptr, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(), OperationType::REALLOC, "%s::realloc(%p, %lu) -> %p", className, ptr, size, result.ptr);
		}
	} else {
		result = chunks.reallocate(ptr, size);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept
{
	ChunkMap::AllocateResult result;
	if (log.isLevel(LogLevel::TRACE)) {
		Timer timer;
		result = chunks.reallocate(ptr, nmemb, size);
		if (!result.fromDelegate) {
			log(timer.getNanoseconds(),
				OperationType::REALLOCARRAY,
				"%s::reallocarray(%p, %lu, %lu) -> %p",
				className,
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

int SegregatedFreeLists::posix_memalign(void** memptr, std::size_t alignment, std::size_t size) noexcept
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
				"%s::posix_memalign(&%p, %lu, %lu) -> %p",
				className,
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

void* SegregatedFreeLists::aligned_alloc(std::size_t alignment, std::size_t size) noexcept
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
				"%s::aligned_alloc(%lu, %lu) -> %p",
				className,
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

void* SegregatedFreeLists::valloc(std::size_t size) noexcept
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
			log(timer.getNanoseconds(), OperationType::VALLOC, "%s::valloc(%lu) -> %p", className, size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegateVallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void* SegregatedFreeLists::memalign(std::size_t alignment, std::size_t size) noexcept
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
				"%s::memalign(%lu, %lu) -> %p",
				className,
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

void* SegregatedFreeLists::pvalloc(std::size_t size) noexcept
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
			log(timer.getNanoseconds(), OperationType::PVALLOC, "%s::pvalloc(%lu) -> %p", className, size, result.ptr);
		}
	} else {
		result = chunks.allocate(size, delegatePvallocFunc, alignPageSize);
	}
	errno = result.propagateErrno;
	return result.ptr;
}

void SegregatedFreeLists::dump() const noexcept
{
	pools.dump();
}

} // namespace ArenaAllocator
