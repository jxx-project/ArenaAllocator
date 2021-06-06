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
#include <optional>

namespace ArenaAllocator {

class ParseConfiguration
{
public:
	ParseConfiguration(
		char const* str,
		std::optional<Configuration::StringType>& className,
		std::optional<Configuration::PoolMapType>& pools,
		std::optional<LogLevel>& logLevel) noexcept;

private:
	LogLevel parseLogLevel() noexcept;
	SizeRange parseSizeRange() noexcept;
	void parsePool() noexcept;
	void parsePoolMap() noexcept;
	void parseConfigStr() noexcept;

	char parseDelimiter(char const* delimiters) noexcept;
	char const* parseIdentifier() noexcept;
	std::size_t parseSize() noexcept;
	bool strEqual(char const* strConfig, char const* reference) const noexcept;
	static bool isSpace(const char c) noexcept;
	static bool isAlpha(const char c) noexcept;
	void raiseError(char const* message) noexcept;

	char const* current;
	std::optional<Configuration::StringType>& className;
	std::optional<Configuration::PoolMapType>& pools;
	std::optional<LogLevel>& logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ParseConfiguration_h_INCLUDED
