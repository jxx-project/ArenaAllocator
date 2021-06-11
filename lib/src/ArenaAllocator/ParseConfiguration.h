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
		std::string_view str,
		std::optional<std::string_view>& className,
		std::optional<Configuration::PoolMapType>& pools,
		std::optional<LogLevel>& logLevel) noexcept;

private:
	LogLevel parseLogLevel() noexcept;
	SizeRange parseSizeRange() noexcept;
	void parsePool() noexcept;
	void parsePoolMap() noexcept;
	void parseConfigStr() noexcept;

	char parseDelimiter(std::string_view delimiters) noexcept;
	std::string_view parseIdentifier() noexcept;
	std::size_t parseUnsignedLong() noexcept;
	static bool isSpace(char c) noexcept;
	static bool isAlpha(char c) noexcept;

	std::string_view::const_iterator current;
	std::optional<std::string_view>& className;
	std::optional<Configuration::PoolMapType>& pools;
	std::optional<LogLevel>& logLevel;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ParseConfiguration_h_INCLUDED
