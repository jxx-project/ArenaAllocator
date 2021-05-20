#ifndef ENVIRONMENT_CONFIGURATION_H_INCLUDED
#define ENVIRONMENT_CONFIGURATION_H_INCLUDED

#include "Configuration.h"
#include "Logger.h"

class EnvironmentConfiguration : public Configuration
{
public:
	explicit EnvironmentConfiguration(Logger& logger) noexcept;
	EnvironmentConfiguration(Configuration const&) = delete;
	void operator=(Configuration const&) = delete;
	~EnvironmentConfiguration() noexcept;
	virtual Configuration::MapType const& getArenas() const noexcept override;

private:
	Logger& logger;
	Configuration::MapType arenas;
};


#endif // CONFIGURATION_H_INCLUDED
