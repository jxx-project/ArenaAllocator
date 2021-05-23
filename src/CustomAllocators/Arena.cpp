//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "CustomAllocators/Arena.h"
#include "CustomAllocators/Chunk.h"

namespace CustomAllocators {

Arena::Arena(Range const& range, std::size_t nChunks, Logger const& logger) noexcept : range{range}, logger{logger}, hwm{0}
{
	logger.log("Arena::Arena(Range{%ld, %ld}, %ld)\n", range.first, range.last, nChunks);
	const std::size_t wordsPerChunk{(range.last + sizeof(WordType) - 1U) / sizeof(WordType)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, 0});
	}
}

Arena::~Arena() noexcept
{
	logger.log("Arena::~Arena() Range{%ld, %ld} [hwm %ld]\n", range.first, range.last, hwm);
}


void* Arena::allocate(std::size_t size) noexcept
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

void Arena::deallocate(ListType::const_iterator it) noexcept
{
	if (it->allocated) {
		free.splice(free.begin(), allocated, it);
		free.front().allocated = 0;
	}
}

std::size_t Arena::nChunks() const noexcept
{
	return free.size() + allocated.size();
}

} // namespace CustomAllocators
