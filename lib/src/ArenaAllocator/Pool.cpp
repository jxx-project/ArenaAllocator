//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/Chunk.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include <cstring>

namespace ArenaAllocator {

Pool::Pool(SizeRange const& range, std::size_t nChunks, Logger const& log) noexcept :
	range{range},
	chunkSize{((range.last + sizeof(std::max_align_t) - 1U) / sizeof(std::max_align_t)) * sizeof(std::max_align_t)},
	log{log},
	hwm{0}
{
	log(LogLevel::DEBUG, "Pool::Pool([%lu, %lu], %lu) -> this:%p", range.first, range.last, nChunks, this);
	const std::size_t wordsPerChunk{chunkSize / sizeof(std::max_align_t)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, 0});
	}
}

Pool::~Pool() noexcept
{
	log(LogLevel::DEBUG, "Pool::~Pool(this:%p)", this);
}


void* Pool::allocate(std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	void* result;
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

void* Pool::reallocate(ListType::iterator it, std::size_t size) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	if (it->allocatedSize == 0) {
		ConsoleLogger::abort("Pool::reallocate(%p, %lu) not allocated", it->data, size);
	}
	it->allocatedSize = size;
	return it->data;
}

void Pool::deallocate(ListType::const_iterator it) noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	if (it->allocatedSize == 0) {
		ConsoleLogger::abort("Pool::deallocate(%p): not allocated", it->data);
	}
	free.splice(free.begin(), allocated, it);
	std::memset(free.front().data, 0, chunkSize);
	free.front().allocatedSize = 0;
}

std::size_t Pool::nChunks() const noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	return free.size() + allocated.size();
}

void Pool::dump() const noexcept
{
	std::lock_guard<std::mutex> guard(mutex);
	log("[%lu, %lu]: {free: %lu, allocated: %lu, hwm: %lu}", range.first, range.last, free.size(), allocated.size(), hwm);
}

} // namespace ArenaAllocator
