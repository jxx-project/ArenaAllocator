//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/malloc.h"
#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Console.h"
#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/InternalAllocatorFactory.h"
#include "ArenaAllocator/InternalLoggerFactory.h"
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
	ArenaAllocator::Logger& getLogger() noexcept;

private:
	ArenaAllocatorSingleton() noexcept;

	::pid_t pid;
	ArenaAllocator::Allocator* allocator;
	ArenaAllocator::Logger* logger;
	ArenaAllocator::InternalAllocatorFactory allocatorFactory;
	ArenaAllocator::InternalLoggerFactory loggerFactory;
	ArenaAllocator::EnvironmentConfiguration configuration;
};

ArenaAllocatorSingleton::~ArenaAllocatorSingleton() noexcept
{
	logger->operator()(ArenaAllocator::LogLevel::DEBUG, [&] {
		return ArenaAllocator::Message("ArenaAllocatorSingleton::~ArenaAllocatorSingleton(this:{})", this);
	});
	allocator->dump();
}

ArenaAllocatorSingleton& ArenaAllocatorSingleton::getInstance() noexcept
{
	static ArenaAllocatorSingleton instance;
	return instance;
}

ArenaAllocator::Allocator& ArenaAllocatorSingleton::getAllocator() noexcept
{
	return *allocator;
}

ArenaAllocator::Logger& ArenaAllocatorSingleton::getLogger() noexcept
{
	return *logger;
}

ArenaAllocatorSingleton::ArenaAllocatorSingleton() noexcept :
	pid{::getpid()},
	allocator{nullptr},
	allocatorFactory{configuration, logger},
	logger{nullptr},
	configuration{std::getenv("ARENA_ALLOCATOR_CONFIGURATION"), allocatorFactory, allocator, loggerFactory, logger}
{
	logger->operator()(ArenaAllocator::LogLevel::DEBUG, [&] {
		return ArenaAllocator::Message("ArenaAllocatorSingleton::ArenaAllocatorSingleton() -> this:{}", this);
	});
}

} // namespace Bootstrap

namespace ArenaAllocator {

void* malloc(std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().malloc(size);
}

void free(void* ptr)
{
	Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().free(ptr);
}

void* calloc(std::size_t nmemb, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().calloc(nmemb, size);
}

void* realloc(void* ptr, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().realloc(ptr, size);
}

} // namespace ArenaAllocator
