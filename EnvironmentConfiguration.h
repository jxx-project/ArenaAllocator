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
	virtual std::map<std::size_t, std::size_t> const& getArenas() const noexcept override;

private:
	std::map<std::size_t, std::size_t> arenas;
};


#endif // CONFIGURATION_H_INCLUDED
