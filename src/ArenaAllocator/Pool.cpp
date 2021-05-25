//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/Chunk.h"

namespace ArenaAllocator {

Pool::Pool(SizeRange const& range, std::size_t nChunks, Logger const& logger) noexcept : range{range}, logger{logger}, hwm{0}
{
	logger.log("Pool::Pool(SizeRange{%ld, %ld}, %ld)\n", range.first, range.last, nChunks);
	const std::size_t wordsPerChunk{(range.last + sizeof(WordType) - 1U) / sizeof(WordType)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, 0});
	}
}

Pool::~Pool() noexcept
{
	logger.log("Pool::~Pool() SizeRange{%ld, %ld} [hwm %ld]\n", range.first, range.last, hwm);
}


void* Pool::allocate(std::size_t size) noexcept
{
	void* result;
	if (free.empty()) {
		result = nullptr;
	} else {
		allocated.splice(allocated.begin(), free, free.begin());
		allocated.front().allocated = size;
		hwm = std::max(hwm, allocated.size());
		result = allocated.front().data;
	}
	return result;
}

void Pool::deallocate(ListType::const_iterator it) noexcept
{
	if (it->allocated) {
		free.splice(free.begin(), allocated, it);
		free.front().allocated = 0;
	}
}

std::size_t Pool::nChunks() const noexcept
{
	return free.size() + allocated.size();
}

} // namespace ArenaAllocator
