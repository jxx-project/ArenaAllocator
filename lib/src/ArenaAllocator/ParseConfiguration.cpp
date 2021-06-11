#include "ArenaAllocator/ParseConfiguration.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include <cstdlib>

namespace ArenaAllocator {

ParseConfiguration::ParseConfiguration(
	std::string_view str,
	std::optional<std::string_view>& className,
	std::optional<Configuration::PoolMapType>& pools,
	std::optional<LogLevel>& logLevel) noexcept :
	current{str.begin()}, className{className}, pools{pools}, logLevel{logLevel}
{
	if (parseDelimiter("{") != '{') {
		ConsoleLogger::exit("ParseConfiguration: expected '{' at configuration string begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		std::string_view configItem{parseIdentifier()};
		if (configItem == "pools") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit("ParseConfiguration: expected ':' after pools item identifier");
			}
			parsePoolMap();
		} else if (configItem == "class") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit("ParseConfiguration: expected ':' after class item identifier");
			}
			if (className.has_value()) {
				ConsoleLogger::exit("ParseConfiguration: duplicate allocator class item");
			}
			className.emplace(parseIdentifier());
		} else if (configItem == "logLevel") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit("ParseConfiguration: expected ':' after logLevel item identifier");
			}
			if (logLevel.has_value()) {
				ConsoleLogger::exit("ParseConfiguration: duplicate allocator class item");
			}
			logLevel.emplace(parseLogLevel());
		} else {
			ConsoleLogger::exit("ParseConfiguration: unexpected configuration item");
		}
		if ((delimiter = parseDelimiter(",}")) == 0) {
			ConsoleLogger::exit("ParseConfiguration: expected ',' configuration item delimiter.");
		}
	}
	while (isSpace(*current)) {
		++current;
	}
	if (*current != 0) {
		ConsoleLogger::exit("ParseConfiguration: unexpected character after '}' at configuration string end");
	}
}

LogLevel ParseConfiguration::parseLogLevel() noexcept
{
	LogLevel result{};
	std::string_view logLevel{parseIdentifier()};
	if (logLevel == "NONE") {
		result = LogLevel::NONE;
	} else if (logLevel == "ERROR") {
		result = LogLevel::ERROR;
	} else if (logLevel == "INFO") {
		result = LogLevel::INFO;
	} else if (logLevel == "TRACE") {
		result = LogLevel::TRACE;
	} else if (logLevel == "DEBUG") {
		result = LogLevel::DEBUG;
	} else {
		ConsoleLogger::exit("ParseConfiguration: invalid log level");
	}
	return result;
}

SizeRange ParseConfiguration::parseSizeRange() noexcept
{
	SizeRange result{};
	while (isSpace(*current)) {
		++current;
	}
	if (parseDelimiter("[") == 0) {
		ConsoleLogger::exit("ParseConfiguration: expexted '[' at size range begin");
	}
	result.first = parseUnsignedLong();
	if (parseDelimiter(",") == 0) {
		ConsoleLogger::exit("ParseConfiguration: expexted ',' separating size range first and last");
	}
	result.last = parseUnsignedLong();
	if (parseDelimiter("]") == 0) {
		ConsoleLogger::exit("ParseConfiguration: expexted ']' at size range end");
	}
	if (result.first > result.last) {
		ConsoleLogger::exit("ParseConfiguration: size range first grater than last");
	}
	return result;
}

void ParseConfiguration::parsePoolMap() noexcept
{
	if (pools.has_value()) {
		ConsoleLogger::exit("ParseConfiguration: duplicate pools item");
	}
	pools.emplace();
	if (parseDelimiter("{") != '{') {
		ConsoleLogger::exit("ParseConfiguration: expected '{' at pool configuration begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		parsePool();
		if ((delimiter = parseDelimiter(",}")) == 0) {
			ConsoleLogger::exit("ParseConfiguration: expected ',' pool map element delimiter");
		}
	}
}

void ParseConfiguration::parsePool() noexcept
{
	const SizeRange range{parseSizeRange()};
	if (parseDelimiter(":") != ':') {
		ConsoleLogger::exit("ParseConfiguration: Expected ':' at pool configuration begin");
	}
	std::size_t nChunks{parseUnsignedLong()};
	if (!pools.value().emplace(range, nChunks)) {
		ConsoleLogger::exit("ParseConfiguration: Expected disjunct pool size ranges");
	}
}

// Generic primitives

char ParseConfiguration::parseDelimiter(const std::string_view delimiters) noexcept
{
	while (isSpace(*current)) {
		++current;
	}
	char result{0};
	for (const char p : delimiters) {
		if (*current == p) {
			result = *current++;
			break;
		}
	}
	return result;
}

std::string_view ParseConfiguration::parseIdentifier() noexcept
{
	while (isSpace(*current)) {
		++current;
	}
	std::string_view::const_iterator indentifierStr{current};
	while (isAlpha(*current)) {
		++current;
	}
	return std::string_view(indentifierStr, current - indentifierStr);
}

std::size_t ParseConfiguration::parseUnsignedLong() noexcept
{
	char* end{nullptr};
	std::size_t result{std::strtoul(current, &end, 10)};
	if (current == end) {
		ConsoleLogger::exit("ParseConfiguration: invalid unsigned long value");
	}
	current = end;
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

} // namespace ArenaAllocator
