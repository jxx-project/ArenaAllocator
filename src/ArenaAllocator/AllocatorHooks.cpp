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

namespace ArenaAllocator {

class AllocatorHooks : public AllocatorFactory
{
public:
	AllocatorHooks(AllocatorHooks const&) = delete;
	AllocatorHooks& operator=(AllocatorHooks const&) = delete;

	static AllocatorHooks& getInstance() noexcept
	{
		static AllocatorHooks instance;
		return instance;
	}

	virtual Allocator* getAllocator(Configuration::StringType const& className) noexcept override
	{
		if (className == "PassThroughAllocator") {
			if (!passThroughAllocator.has_value()) {
				passThroughAllocator.emplace(logger);
			}
			return &passThroughAllocator.value();
		} else if (className == "PoolAllocator") {
			if (!passThroughAllocator.has_value()) {
				passThroughAllocator.emplace(logger);
			}
			if (!poolAllocator.has_value()) {
				poolAllocator.emplace(configuration, &passThroughAllocator.value(), logger);
			}
			return &poolAllocator.value();
		} else if (className == "PoolStatisticsAllocator") {
			if (!passThroughAllocator.has_value()) {
				passThroughAllocator.emplace(logger);
			}
			if (!poolStatisticsAllocator.has_value()) {
				poolStatisticsAllocator.emplace(configuration, passThroughAllocator.value(), logger);
			}
			return &poolStatisticsAllocator.value();
		} else {
			return nullptr;
		}
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
	Allocator* activeAllocator;
	EnvironmentConfiguration configuration{*this, activeAllocator, logger};
	std::optional<PassThroughAllocator> passThroughAllocator;
	std::optional<PoolAllocator> poolAllocator;
	std::optional<PoolStatisticsAllocator> poolStatisticsAllocator;
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
