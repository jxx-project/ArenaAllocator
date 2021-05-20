#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#include <cstdint>
#include <map>

class Configuration
{
public:
	virtual ~Configuration() noexcept = default;
	virtual std::map<std::size_t, std::size_t> const& getArenas() const noexcept = 0;
};

#endif // CONFIGURATION_H_INCLUDED
