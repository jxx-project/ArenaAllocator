//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_PARSE_CONFIGURATION_H_INCLUDED
#define ARENA_ALLOCATOR_PARSE_CONFIGURATION_H_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/Optional.h"
#include "ArenaAllocator/SizeRange.h"

namespace ArenaAllocator {

class ParseConfiguration
{
public:
	ParseConfiguration(
		char const* str,
		Optional<Configuration::StringType>& className,
		Optional<Configuration::PoolMapType>& pools) noexcept;

private:
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
	Optional<Configuration::StringType>& className;
	Optional<Configuration::PoolMapType>& pools;
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_PARSE_CONFIGURATION_H_INCLUDED
