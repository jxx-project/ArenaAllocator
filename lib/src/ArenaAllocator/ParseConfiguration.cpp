#include "ArenaAllocator/ParseConfiguration.h"
#include <cstdio>
#include <cstring>

namespace ArenaAllocator {

ParseConfiguration::ParseConfiguration(
	char const* str,
	std::optional<Configuration::StringType>& className,
	std::optional<Configuration::PoolMapType>& pools,
	std::optional<LogLevel>& logLevel) noexcept :
	current{str}, className{className}, pools{pools}, logLevel{logLevel}
{
	if (!parseDelimiter("{") == '{') {
		raiseError("Expected '{' at configuration string begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		char const* configItem{parseIdentifier()};
		if (strEqual(configItem, "pools")) {
			if (!parseDelimiter(":")) {
				raiseError("Expected ':' after item identifier");
			}
			parsePoolMap();
		} else if (strEqual(configItem, "class")) {
			if (!parseDelimiter(":")) {
				raiseError("Expected ':' after item identifier");
			}
			if (className.has_value()) {
				raiseError("Duplicate allocator class item");
			}
			char const* classNameStr{parseIdentifier()};
			className.emplace(classNameStr, current);
		} else if (strEqual(configItem, "logLevel")) {
			if (!parseDelimiter(":")) {
				raiseError("Expected ':' after item identifier");
			}
			if (logLevel.has_value()) {
				raiseError("Duplicate allocator class item");
			}
			logLevel.emplace(parseLogLevel());
		} else {
			raiseError("Unexpected configuration item");
		}
		if (!(delimiter = parseDelimiter(",}"))) {
			raiseError("Expected ',' configuration item delimiter.");
		}
	}
	while (isSpace(*current)) {
		++current;
	}
	if (*current) {
		raiseError("Unexpected character after '}' at configuration string end");
	}
}

LogLevel ParseConfiguration::parseLogLevel() noexcept
{
	LogLevel result;
	char const* logLevelStr{parseIdentifier()};
	if (strEqual(logLevelStr, "NONE")) {
		result = LogLevel::NONE;
	} else if (strEqual(logLevelStr, "ERROR")) {
		result = LogLevel::ERROR;
	} else if (strEqual(logLevelStr, "INFO")) {
		result = LogLevel::INFO;
	} else if (strEqual(logLevelStr, "TRACE")) {
		result = LogLevel::TRACE;
	} else if (strEqual(logLevelStr, "DEBUG")) {
		result = LogLevel::DEBUG;
	} else {
		raiseError("Invalid log level");
	}
	return result;
}

SizeRange ParseConfiguration::parseSizeRange() noexcept
{
	SizeRange result;
	while (isSpace(*current)) {
		++current;
	}
	if (!parseDelimiter("[")) {
		raiseError("Expexted '[' at size range begin");
	}
	result.first = parseSize();
	if (!parseDelimiter(",")) {
		raiseError("Expexted ',' separating size range first and last");
	}
	result.last = parseSize();
	if (!parseDelimiter("]")) {
		raiseError("Expexted ']' at size range end");
	}
	if (result.first > result.last) {
		raiseError("Size range first grater than last");
	}
	return result;
}

void ParseConfiguration::parsePoolMap() noexcept
{
	if (pools.has_value()) {
		raiseError("Duplicate pools item");
	}
	pools.emplace();
	if (!parseDelimiter("{") == '{') {
		raiseError("Expected '{' at pool configuration begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		parsePool();
		if (!(delimiter = parseDelimiter(",}"))) {
			raiseError("Expected ',' pool map element delimiter");
		}
	}
}

void ParseConfiguration::parsePool() noexcept
{
	const SizeRange range{parseSizeRange()};
	if (!parseDelimiter(":") == ':') {
		raiseError("Expected ':' at pool configuration begin");
	}
	std::size_t nChunks{parseSize()};
	if (!pools.value().emplace(range, nChunks)) {
		raiseError("Expected disjunct pool size ranges");
	}
}

// Generic primitives

char ParseConfiguration::parseDelimiter(char const* delimiters) noexcept
{
	while (isSpace(*current)) {
		++current;
	}
	char result{0};
	for (char const* p{delimiters}; *p; ++p) {
		if (*current == *p) {
			result = *current++;
			break;
		}
	}
	return result;
}

char const* ParseConfiguration::parseIdentifier() noexcept
{
	while (isSpace(*current)) {
		++current;
	}
	char const* result{current};
	while (isAlpha(*current)) {
		++current;
	}
	return result;
}

std::size_t ParseConfiguration::parseSize() noexcept
{
	char* end;
	std::size_t result = std::strtoul(current, &end, 10);
	if (current == end) {
		raiseError("Invalid size value");
	}
	current = end;
	return result;
}

bool ParseConfiguration::strEqual(char const* strConfig, char const* reference) const noexcept
{
	bool result{true};
	while (strConfig != current && *reference) {
		if (*strConfig != *reference) {
			result = false;
			break;
		}
		++strConfig;
		++reference;
	}
	if (strConfig != current || *reference) {
		result = false;
	}
	return result;
}

bool ParseConfiguration::isSpace(const char c) noexcept
{
	return c == ' ' || c == '\t' || c == '\n';
}

bool ParseConfiguration::isAlpha(const char c) noexcept
{
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

void ParseConfiguration::raiseError(char const* message) noexcept
{
	std::fprintf(stderr, "ParseConfiguration: %s\n", message);
	std::abort();
}

} // namespace ArenaAllocator
