#include "ArenaAllocator/ParseConfiguration.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include <charconv>

namespace ArenaAllocator {

ParseConfiguration::ParseConfiguration(
	std::string_view str,
	std::optional<std::string_view>& className,
	std::optional<Configuration::PoolMapType>& pools,
	std::optional<LogLevel>& logLevel,
	std::optional<std::string_view>& loggerName) noexcept :
	current{str}, className{className}, pools{pools}, logLevel{logLevel}, loggerName{loggerName}
{
	if (parseDelimiter("{") != '{') {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: expected '{' at configuration string begin"); });
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		std::string_view configItem{parseIdentifier()};
		if (configItem == "pools") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ':' after pools item identifier"); });
			}
			parsePoolMap();
		} else if (configItem == "class") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ':' after class item identifier"); });
			}
			if (className.has_value()) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: duplicate allocator class item"); });
			}
			className.emplace(parseIdentifier());
		} else if (configItem == "logLevel") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ':' after logLevel item identifier"); });
			}
			if (logLevel.has_value()) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: duplicate allocator class item"); });
			}
			logLevel.emplace(parseLogLevel());
		} else if (configItem == "logger") {
			if (parseDelimiter(":") == 0) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ':' after logger item identifier"); });
			}
			if (loggerName.has_value()) {
				ConsoleLogger::exit([] { return Format("ParseConfiguration: duplicate logger class item"); });
			}
			loggerName.emplace(parseIdentifier());
		} else {
			ConsoleLogger::exit([] { return Format("ParseConfiguration: unexpected configuration item"); });
		}
		if ((delimiter = parseDelimiter(",}")) == 0) {
			ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ',' configuration item delimiter."); });
		}
	}
	while (!current.empty() && isSpace(current.front())) {
		current.remove_prefix(1);
	}
	if (!current.empty()) {
		ConsoleLogger::exit(
			[] { return Format("ParseConfiguration: unexpected character after '}' at configuration string end"); });
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
		ConsoleLogger::exit([] { return Format("ParseConfiguration: invalid log level"); });
	}
	return result;
}

SizeRange ParseConfiguration::parseSizeRange() noexcept
{
	SizeRange result{};
	while (!current.empty() && isSpace(current.front())) {
		current.remove_prefix(1);
	}
	if (parseDelimiter("[") == 0) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: expexted '[' at size range begin"); });
	}
	result.first = parseSize();
	if (parseDelimiter(",") == 0) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: expexted ',' separating size range first and last"); });
	}
	result.last = parseSize();
	if (parseDelimiter("]") == 0) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: expexted ']' at size range end"); });
	}
	if (result.first > result.last) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: size range first grater than last"); });
	}
	return result;
}

void ParseConfiguration::parsePoolMap() noexcept
{
	if (pools.has_value()) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: duplicate pools item"); });
	}
	pools.emplace();
	if (parseDelimiter("{") != '{') {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: expected '{' at pool configuration begin"); });
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		parsePool();
		if ((delimiter = parseDelimiter(",}")) == 0) {
			ConsoleLogger::exit([] { return Format("ParseConfiguration: expected ',' pool map element delimiter"); });
		}
	}
}

void ParseConfiguration::parsePool() noexcept
{
	const SizeRange range{parseSizeRange()};
	if (parseDelimiter(":") != ':') {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: Expected ':' at pool configuration begin"); });
	}
	std::size_t nChunks{parseSize()};
	if (!pools.value().emplace(range, nChunks)) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: Expected disjunct pool size ranges"); });
	}
}

// Generic primitives

char ParseConfiguration::parseDelimiter(const std::string_view delimiters) noexcept
{
	while (!current.empty() && isSpace(current.front())) {
		current.remove_prefix(1);
	}
	char result{0};
	for (const char p : delimiters) {
		if (current.front() == p) {
			result = p;
			current.remove_prefix(1);
			break;
		}
	}
	return result;
}

std::string_view ParseConfiguration::parseIdentifier() noexcept
{
	while (!current.empty() && isSpace(current.front())) {
		current.remove_prefix(1);
	}
	char const* const indentifierPtr{current.data()};
	while (!current.empty() && isAlpha(current.front())) {
		current.remove_prefix(1);
	}
	return std::string_view(indentifierPtr, current.data() - indentifierPtr);
}

std::size_t ParseConfiguration::parseSize() noexcept
{
	std::size_t result{0};
	std::from_chars_result matchResult{std::from_chars(current.data(), current.data() + current.size(), result)};
	if (!static_cast<bool>(matchResult.ec)) {
		current.remove_prefix(matchResult.ptr - current.data());
	} else if (matchResult.ec == std::errc::result_out_of_range) {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: size value out of range"); });
	} else {
		ConsoleLogger::exit([] { return Format("ParseConfiguration: invalid unsigned long value"); });
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

} // namespace ArenaAllocator
