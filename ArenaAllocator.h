#ifndef ARENA_ALLOCATOR_H_INCLUDED
#define ARENA_ALLOCATOR_H_INCLUDED

#include "Allocator.h"
#include "ArenaMap.h"
#include "Configuration.h"
#include "Logger.h"
#include <cstdint>

class ArenaAllocator : public Allocator
{
public:
	ArenaAllocator(Configuration const& configuration, Logger const& logger, Allocator*& activeAllocator) noexcept;
	ArenaAllocator(ArenaAllocator const&) = delete;
	void operator=(ArenaAllocator const&) = delete;
	virtual ~ArenaAllocator() noexcept;
	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
	Allocator*& activeAllocator;
	const ArenaMap arenas;
};

#endif // ARENA_ALLOCATOR_H_INCLUDED
