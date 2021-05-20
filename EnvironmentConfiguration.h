#ifndef ENVIRONMENT_CONFIGURATION_H_INCLUDED
#define ENVIRONMENT_CONFIGURATION_H_INCLUDED

#include "Configuration.h"

class EnvironmentConfiguration : public Configuration
{
public:
	EnvironmentConfiguration() noexcept;
	EnvironmentConfiguration(Configuration const&) = delete;
	void operator=(Configuration const&) = delete;
	~EnvironmentConfiguration() noexcept;
	virtual Configuration::MapType const& getArenas() const noexcept override;

private:
	Configuration::MapType arenas;
};


#endif // CONFIGURATION_H_INCLUDED
