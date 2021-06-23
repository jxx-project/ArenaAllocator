//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_ParseConfiguration_h_INCLUDED
#define ArenaAllocator_ParseConfiguration_h_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/LogLevel.h"
#include "ArenaAllocator/SizeRange.h"
#include <Static/ParsePrimitives.h>
#include <optional>

namespace ArenaAllocator {

class ParseConfiguration : public Static::ParsePrimitives
{
public:
	ParseConfiguration(std::string_view str, std::optional<Configuration::PoolMapType>& pools) noexcept;
	~ParseConfiguration() noexcept override = default;

	void operator()(
		std::optional<std::string_view>& className,
		std::optional<LogLevel>& logLevel,
		std::optional<std::string_view>& loggerName) noexcept;

private:
	LogLevel parseLogLevel() noexcept;
	SizeRange parseSizeRange() noexcept;
	void parsePool() noexcept;
	void parsePoolMap() noexcept;
	void parseConfigStr() noexcept;
	[[noreturn]] void raiseError(std::string_view message) override;

	std::optional<Configuration::PoolMapType>& pools;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ParseConfiguration_h_INCLUDED
