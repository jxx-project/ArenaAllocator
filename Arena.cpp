#include "Arena.h"
#include "Chunk.h"
#include "Logger.h"

Arena::Arena(std::size_t nChunks, std::size_t chunkSize) noexcept
{
	// Logger::log("Arena::Arena(%ul, %ul)\n", nChunks, chunkSize);
	const std::size_t wordsPerChunk{(chunkSize + sizeof(Arena::WordType) - 1U) / sizeof(Arena::WordType)};
	storage.resize(nChunks * wordsPerChunk);
	for (std::size_t offset = 0; offset < storage.size(); offset += wordsPerChunk) {
		free.emplace_back(Chunk{storage.data() + offset, this, false});
	}
}

void* Arena::allocate() noexcept
{
	void* result;
	if (free.empty()) {
		result = nullptr;
	} else {
		result = free.front().data;
		free.front().allocated = true;
		allocated.splice(allocated.begin(), free, free.begin());
	}
	return result;
}

void Arena::deallocate(std::list<Chunk>::const_iterator it) noexcept
{
	if (it->allocated) {
		allocated.splice(free.begin(), allocated, it);
		free.front().allocated = false;
	}
}

std::size_t Arena::nChunks() const noexcept
{
	return free.size() + allocated.size();
}
