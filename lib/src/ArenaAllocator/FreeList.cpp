//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/FreeList.h"
#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include <cstring>

namespace ArenaAllocator {

FreeList::FreeList(SizeRange const& range, std::size_t nChunks, Logger const& log) noexcept :
	range{range},
	chunkSize{((range.last + sizeof(std::max_align_t) - 1U) / sizeof(std::max_align_t)) * sizeof(std::max_align_t)},
	log{log},
	hwm{0}
{
	log(LogLevel::DEBUG, "FreeList::FreeList([%lu, %lu], %lu) -> this:%p", range.first, range.last, nChunks, this);
	const std::size_t wordsPerChunk{chunkSize / sizeof(std::max_align_t)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, 0});
	}
}

FreeList::~FreeList() noexcept
{
	log(LogLevel::DEBUG, "FreeList::~FreeList(this:%p)", this);
}


void* FreeList::allocate(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	void* result{nullptr};
	if (free.empty()) {
		result = nullptr;
	} else {
		allocated.splice(allocated.begin(), free, free.begin());
		allocated.front().allocatedSize = size;
		hwm = std::max(hwm, allocated.size());
		result = allocated.front().data;
	}
	return result;
}

void* FreeList::reallocate(ListType::iterator it, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (it->allocatedSize == 0) {
		ConsoleLogger::abort("FreeList::reallocate(%p, %lu) not allocated", it->data, size);
	}
	it->allocatedSize = size;
	return it->data;
}

void FreeList::deallocate(ListType::const_iterator it) noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	if (it->allocatedSize == 0) {
		ConsoleLogger::abort("FreeList::deallocate(%p): not allocated", it->data);
	}
	free.splice(free.begin(), allocated, it);
	std::memset(free.front().data, 0, chunkSize);
	free.front().allocatedSize = 0;
}

std::size_t FreeList::nChunks() const noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	return free.size() + allocated.size();
}

void FreeList::dump() const noexcept
{
	std::lock_guard<std::mutex> guard{mutex};
	log("[%lu, %lu]: {free: %lu, allocated: %lu, hwm: %lu}", range.first, range.last, free.size(), allocated.size(), hwm);
}

} // namespace ArenaAllocator
