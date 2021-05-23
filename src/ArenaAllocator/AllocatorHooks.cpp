//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/NativeAllocator.h"
#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/StaticConfiguration.h"

namespace ArenaAllocator {

class AllocatorHooks
{
public:
	AllocatorHooks(AllocatorHooks const&) = delete;
	AllocatorHooks& operator=(AllocatorHooks const&) = delete;
	static AllocatorHooks& getInstance() noexcept
	{
		static AllocatorHooks instance;
		return instance;
	}
	Allocator& getActiveAllocator() noexcept
	{
		return *activeAllocator;
	}

private:
	AllocatorHooks() noexcept
	{
	}

	ConsoleLogger logger;
	NativeAllocator nativeAllocator{logger};
	Allocator* activeAllocator{&nativeAllocator};
	StaticConfiguration configuration{activeAllocator, logger};
	PoolAllocator poolAllocator{configuration, logger};
};

extern "C" void* malloc(std::size_t size)
{
	return AllocatorHooks::getInstance().getActiveAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	AllocatorHooks::getInstance().getActiveAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return AllocatorHooks::getInstance().getActiveAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return AllocatorHooks::getInstance().getActiveAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return AllocatorHooks::getInstance().getActiveAllocator().reallocarray(ptr, nmemb, size);
}

} // namespace ArenaAllocator
