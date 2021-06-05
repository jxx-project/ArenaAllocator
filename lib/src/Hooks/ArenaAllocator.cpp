//
// Copyright (C) 2018 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/AllocatorFactory.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "ArenaAllocator/PassThroughAllocator.h"
#include "ArenaAllocator/PoolAllocator.h"
#include "ArenaAllocator/PoolStatisticsAllocator.h"
#include <optional>
#include <unistd.h>

namespace ArenaAllocator { namespace Hooks {

class ArenaAllocator
{
public:
	ArenaAllocator(ArenaAllocator const&) = delete;
	ArenaAllocator& operator=(ArenaAllocator const&) = delete;
	~ArenaAllocator() noexcept;

	static ArenaAllocator& getInstance() noexcept;
	Allocator& getAllocator() noexcept;

private:
	class Factory : public AllocatorFactory
	{
	public:
		Factory(Configuration const& configuration, Logger const& log) noexcept;
		Factory(Factory const&) = delete;
		Factory& operator=(Factory const&) = delete;
		~Factory() noexcept;

		virtual Allocator* getAllocator(Configuration::StringType const& className) noexcept override;

	private:
		Configuration const& configuration;
		Logger const& log;
		std::optional<PassThroughAllocator> passThroughAllocator;
		std::optional<PoolAllocator> poolAllocator;
		std::optional<PoolStatisticsAllocator> poolStatisticsAllocator;
	};

	ArenaAllocator() noexcept;

	::pid_t pid;
	ConsoleLogger log;
	Allocator* allocator;
	Factory factory;
	EnvironmentConfiguration configuration;
};

ArenaAllocator::~ArenaAllocator() noexcept
{
}

ArenaAllocator& ArenaAllocator::getInstance() noexcept
{
	static ArenaAllocator instance;
	::pid_t pid{::getpid()};
	if (pid != instance.pid) {
		instance.pid = pid;
		instance.allocator->dump();
	}
	return instance;
}

Allocator& ArenaAllocator::getAllocator() noexcept
{
	return *allocator;
}

ArenaAllocator::Factory::Factory(Configuration const& configuration, Logger const& log) noexcept :
	configuration{configuration}, log{log}
{
}

ArenaAllocator::Factory::~Factory() noexcept
{
}

Allocator* ArenaAllocator::Factory::getAllocator(Configuration::StringType const& className) noexcept
{
	Allocator* result{nullptr};
	if (className == "PassThroughAllocator") {
		if (!passThroughAllocator.has_value()) {
			passThroughAllocator.emplace(log);
		}
		result = &passThroughAllocator.value();
	} else if (className == "PoolAllocator") {
		if (!poolAllocator.has_value()) {
			poolAllocator.emplace(configuration, getAllocator("PassThroughAllocator"), log);
		}
		result = &poolAllocator.value();
	} else if (className == "PoolStatisticsAllocator") {
		if (!poolStatisticsAllocator.has_value()) {
			poolStatisticsAllocator.emplace(configuration, *getAllocator("PassThroughAllocator"), log);
		}
		result = &poolStatisticsAllocator.value();
	}
	return result;
}

ArenaAllocator::ArenaAllocator() noexcept :
	pid{::getpid()}, log{}, factory{configuration, log}, configuration{factory, allocator, log}
{
	log(LogLevel::DEBUG, "ArenaAllocator::ArenaAllocator()");
}

extern "C" void initArenaAllocator()
{
	ArenaAllocator::getInstance().getAllocator();
}

extern "C" void* malloc(std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	ArenaAllocator::getInstance().getAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().reallocarray(ptr, nmemb, size);
}

extern "C" int posix_memalign(void** memptr, std::size_t alignment, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().posix_memalign(memptr, alignment, size);
}

extern "C" void* aligned_alloc(std::size_t alignment, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().aligned_alloc(alignment, size);
}

extern "C" void* valloc(std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().valloc(size);
}

extern "C" void* memalign(std::size_t alignment, std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().memalign(alignment, size);
}

extern "C" void* pvalloc(std::size_t size)
{
	return ArenaAllocator::getInstance().getAllocator().pvalloc(size);
}

}} // namespace ArenaAllocator::Hooks
