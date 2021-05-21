//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "Arena.h"
#include "Chunk.h"
#include "Logger.h"

namespace CustomAllocators {

Arena::Arena(std::size_t nChunks, std::size_t chunkSize) noexcept
{
	const std::size_t wordsPerChunk{(chunkSize + sizeof(Arena::WordType) - 1U) / sizeof(Arena::WordType)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, 0});
	}
}

void* Arena::allocate(std::size_t size) noexcept
{
	void* result;
	if (free.empty()) {
		result = nullptr;
	} else {
		result = free.front().data;
		free.front().allocated = size;
		allocated.splice(allocated.begin(), free, free.begin());
	}
	return result;
}

void Arena::deallocate(ListType::const_iterator it) noexcept
{
	if (it->allocated) {
		allocated.splice(free.begin(), allocated, it);
		free.front().allocated = 0;
	}
}

std::size_t Arena::nChunks() const noexcept
{
	return free.size() + allocated.size();
}

} // namespace CustomAllocators
