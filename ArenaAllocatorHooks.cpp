#include "ArenaAllocator.h"
#include "ConsoleLogger.h"
#include "EnvironmentConfiguration.h"
#include "NativeAllocator.h"

class AllocatorSingleton
{
public:
	AllocatorSingleton(AllocatorSingleton const&) = delete;
	AllocatorSingleton& operator=(AllocatorSingleton const&) = delete;
	static AllocatorSingleton& getInstance() noexcept
	{
		static AllocatorSingleton instance;
		return instance;
	}
	Allocator& getActiveAllocator() noexcept
	{
		return *activeAllocator;
	}

private:
	AllocatorSingleton() noexcept
	{
	}

	ConsoleLogger logger;
	NativeAllocator nativeAllocator{logger};
	Allocator* activeAllocator{&nativeAllocator};
	EnvironmentConfiguration configuration{logger};
	ArenaAllocator arenaAllocator{configuration, logger, activeAllocator};
};

extern "C" void* malloc(std::size_t size)
{
	return AllocatorSingleton::getInstance().getActiveAllocator().malloc(size);
}

extern "C" void free(void* ptr)
{
	AllocatorSingleton::getInstance().getActiveAllocator().free(ptr);
}

extern "C" void* calloc(std::size_t nmemb, std::size_t size)
{
	return AllocatorSingleton::getInstance().getActiveAllocator().calloc(nmemb, size);
}

extern "C" void* realloc(void* ptr, std::size_t size)
{
	return AllocatorSingleton::getInstance().getActiveAllocator().realloc(ptr, size);
}

extern "C" void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size)
{
	return AllocatorSingleton::getInstance().getActiveAllocator().reallocarray(ptr, nmemb, size);
}
