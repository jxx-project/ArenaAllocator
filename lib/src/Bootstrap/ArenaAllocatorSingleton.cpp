//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/InternalAllocatorFactory.h"
#include <cstdlib>
#include <optional>
#include <unistd.h>

namespace Bootstrap {

class ArenaAllocatorSingleton
{
public:
	ArenaAllocatorSingleton(ArenaAllocatorSingleton const&) = delete;
	ArenaAllocatorSingleton& operator=(ArenaAllocatorSingleton const&) = delete;
	~ArenaAllocatorSingleton() noexcept;

	static ArenaAllocatorSingleton& getInstance() noexcept;
	ArenaAllocator::Allocator& getAllocator() noexcept;

private:
	ArenaAllocatorSingleton() noexcept;

	::pid_t pid;
	ArenaAllocator::ConsoleLogger log;
	ArenaAllocator::Allocator* allocator;
	ArenaAllocator::InternalAllocatorFactory allocatorFactory;
	ArenaAllocator::EnvironmentConfiguration configuration;
};

ArenaAllocatorSingleton::~ArenaAllocatorSingleton() noexcept
{
}

ArenaAllocatorSingleton& ArenaAllocatorSingleton::getInstance() noexcept
{
	static ArenaAllocatorSingleton instance;
	::pid_t pid{::getpid()};
	if (pid != instance.pid) {
		instance.pid = pid;
		instance.allocator->dump();
	}
	return instance;
}

ArenaAllocator::Allocator& ArenaAllocatorSingleton::getAllocator() noexcept
{
	return *allocator;
}

ArenaAllocatorSingleton::ArenaAllocatorSingleton() noexcept :
	pid{::getpid()},
	log{},
	allocatorFactory{configuration, log},
	configuration{std::getenv("ARENA_ALLOCATOR_CONFIGURATION"), allocatorFactory, allocator, log}
{
	log(ArenaAllocator::LogLevel::DEBUG, "ArenaAllocatorSingleton::ArenaAllocatorSingleton()");
}

extern "C" void initArenaAllocator()
{
	ArenaAllocatorSingleton::getInstance().getAllocator();
}

extern "C" void* malloc(std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	ArenaAllocatorSingleton::getInstance().getAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().reallocarray(ptr, nmemb, size);
}

extern "C" int posix_memalign(void** memptr, std::size_t alignment, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().posix_memalign(memptr, alignment, size);
}

extern "C" void* aligned_alloc(std::size_t alignment, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().aligned_alloc(alignment, size);
}

extern "C" void* valloc(std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().valloc(size);
}

extern "C" void* memalign(std::size_t alignment, std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().memalign(alignment, size);
}

extern "C" void* pvalloc(std::size_t size)
{
	return ArenaAllocatorSingleton::getInstance().getAllocator().pvalloc(size);
}

} // namespace Bootstrap
