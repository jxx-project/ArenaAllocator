//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


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

namespace {

ArenaAllocatorSingleton* instance;

} // namespace

ArenaAllocatorSingleton::~ArenaAllocatorSingleton() noexcept
{
	logger->operator()(ArenaAllocator::LogLevel::DEBUG, [&] {
		return ArenaAllocator::Message("ArenaAllocatorSingleton::~ArenaAllocatorSingleton(this:{})", this);
	});
}

ArenaAllocatorSingleton& ArenaAllocatorSingleton::getInstance() noexcept
{
	// The idiomatic singleton based on a static local variable looks substatially more straightforward, but
	// occasionally get destroyed while requests are still pouring in, resulting in "pure virtual method called",
	// segmentation fault, crashes. We therefore base it on a global pointer variable, have it never explicitly
	// destroyed and the "cleanup" dump triggered by the shared object .fini hook instead.
	if (instance == nullptr) {
		static std::mutex mutex;
		std::lock_guard<std::mutex> guard{mutex};
		if (instance == nullptr) {
			instance = static_cast<ArenaAllocatorSingleton*>(__libc_malloc(sizeof(ArenaAllocatorSingleton)));
			if (instance != nullptr) {
				instance = new (instance) ArenaAllocatorSingleton(); // NOLINT unlimited lifetime intended
			} else {
				ArenaAllocator::Console::exit(
					[] { return ArenaAllocator::Message("failed to allocate Bootstrap::ArenaAllocatorSingleton\n"); });
			}
		}
	}
	return *instance;
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

extern "C" void initializeArenaAllocator()
{
	Bootstrap::ArenaAllocatorSingleton::getInstance().getLogger()(
		ArenaAllocator::LogLevel::DEBUG, [&] { return ArenaAllocator::Message("initializeArenaAllocator()"); });
}

extern "C" void finishArenaAllocator()
{
	if (Bootstrap::instance != nullptr) {
		Bootstrap::instance->getLogger()(
			ArenaAllocator::LogLevel::DEBUG, [&] { return ArenaAllocator::Message("finishArenaAllocator()"); });
		Bootstrap::instance->getAllocator().dump();
	}
}

extern "C" void* malloc(std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().reallocarray(ptr, nmemb, size);
}

extern "C" int posix_memalign(void** memptr, std::size_t alignment, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().posix_memalign(memptr, alignment, size);
}

extern "C" void* aligned_alloc(std::size_t alignment, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().aligned_alloc(alignment, size);
}

extern "C" void* valloc(std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().valloc(size);
}

extern "C" void* memalign(std::size_t alignment, std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().memalign(alignment, size);
}

extern "C" void* pvalloc(std::size_t size)
{
	return Bootstrap::ArenaAllocatorSingleton::getInstance().getAllocator().pvalloc(size);
}
