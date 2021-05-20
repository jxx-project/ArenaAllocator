#include "ArenaAllocator.h"
#include "ConsoleLogger.h"
#include "EnvironmentConfiguration.h"
#include "NativeAllocator.h"

class StaticAllocatorSingleton
{
public:
	StaticAllocatorSingleton(StaticAllocatorSingleton const&) = delete;
	StaticAllocatorSingleton& operator=(StaticAllocatorSingleton const&) = delete;
	static StaticAllocatorSingleton& getInstance() noexcept
	{
		static StaticAllocatorSingleton instance;
		return instance;
	}
	Allocator& getActiveAllocator() noexcept
	{
		return *activeAllocator;
	}

private:
	StaticAllocatorSingleton() noexcept
	{
	}

	ConsoleLogger logger;
	NativeAllocator nativeAllocator{logger};
	Allocator* activeAllocator{&nativeAllocator};
	EnvironmentConfiguration configuration;
	ArenaAllocator arenaAllocator{configuration, logger, activeAllocator};
};

extern "C" void* malloc(std::size_t size)
{
	return StaticAllocatorSingleton::getInstance().getActiveAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	StaticAllocatorSingleton::getInstance().getActiveAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return StaticAllocatorSingleton::getInstance().getActiveAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return StaticAllocatorSingleton::getInstance().getActiveAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return StaticAllocatorSingleton::getInstance().getActiveAllocator().reallocarray(ptr, nmemb, size);
}
