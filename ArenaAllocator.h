#ifndef ARENA_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_H_INCLUDED

#include <cstdint>

class ArenaAllocator
{
public:
	ArenaAllocator(ArenaAllocator const&) = delete;
	void operator=(ArenaAllocator const&) = delete;
	static ArenaAllocator& getInstance() noexcept
	{
		static ArenaAllocator instance;
		return instance;
	}
	void* malloc(std::size_t size) noexcept;
	void free(void* ptr) noexcept;
	void* calloc(std::size_t nmemb, std::size_t size) noexcept;
	void* realloc(void* ptr, std::size_t size) noexcept;
	void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept;

private:
	ArenaAllocator() noexcept;
	bool passthrough;
};

#endif // ARENA_ALLOCATOR_H_INCLUDED
