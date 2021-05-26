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
			if (!passThroughAllocator.hasValue()) {
				passThroughAllocator.emplace(logger);
			}
			return &passThroughAllocator.value();
		} else if (className == "PoolAllocator") {
			if (!poolAllocator.hasValue()) {
				poolAllocator.emplace(configuration, logger);
			}
			return &poolAllocator.value();
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
	Optional<PassThroughAllocator> passThroughAllocator;
	Optional<PoolAllocator> poolAllocator;
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
