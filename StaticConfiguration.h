#ifndef STATIC_CONFIGURATION_H_INCLUDED
#define STATIC_CONFIGURATION_H_INCLUDED

#include "Allocator.h"
#include "Configuration.h"
#include "Logger.h"

class StaticConfiguration : public Configuration
{
public:
	StaticConfiguration(Allocator*& activeAllocator, Logger& logger) noexcept;
	StaticConfiguration(StaticConfiguration const&) = delete;
	StaticConfiguration& operator=(StaticConfiguration const&) = delete;
	~StaticConfiguration() noexcept;
	virtual Configuration::MapType const& getArenas() const noexcept override;

private:
	Allocator*& activeAllocator;
	Logger& logger;
	Configuration::MapType arenas;
};


#endif // STATIC_CONFIGURATION_H_INCLUDED
