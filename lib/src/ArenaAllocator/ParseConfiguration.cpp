#include "ArenaAllocator/ParseConfiguration.h"
#include "ArenaAllocator/ConsoleLogger.h"
#include <charconv>

namespace ArenaAllocator {

ParseConfiguration::ParseConfiguration(std::string_view str, std::optional<Configuration::PoolMapType>& pools) noexcept :
	ParsePrimitives{str}, pools{pools}
{
}

void ParseConfiguration::operator()(
	std::optional<std::string_view>& className,
	std::optional<LogLevel>& logLevel,
	std::optional<std::string_view>& loggerName) noexcept
{
	if (parseDelimiter("{") != '{') {
		raiseError("expected '{' at configuration string begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		std::string_view configItem{parseIdentifier()};
		if (configItem == "pools") {
			if (parseDelimiter(":") == 0) {
				raiseError("expected ':' after pools item identifier");
			}
			if (pools.has_value()) {
				raiseError("duplicate pools item");
			}
			parsePoolMap();
		} else if (configItem == "class") {
			if (parseDelimiter(":") == 0) {
				raiseError("expected ':' after class item identifier");
			}
			if (className.has_value()) {
				raiseError("duplicate allocator class item");
			}
			className.emplace(parseIdentifier());
		} else if (configItem == "logLevel") {
			if (parseDelimiter(":") == 0) {
				raiseError("expected ':' after logLevel item identifier");
			}
			if (logLevel.has_value()) {
				raiseError("duplicate allocator class item");
			}
			logLevel.emplace(parseLogLevel());
		} else if (configItem == "logger") {
			if (parseDelimiter(":") == 0) {
				raiseError("expected ':' after logger item identifier");
			}
			if (loggerName.has_value()) {
				raiseError("duplicate logger class item");
			}
			loggerName.emplace(parseIdentifier());
		} else {
			raiseError("unexpected configuration item");
		}
		if ((delimiter = parseDelimiter(",}")) == 0) {
			raiseError("expected ',' configuration item delimiter.");
		}
	}
	skipSpaceChars();
	if (!empty()) {
		raiseError("unexpected character after '}' at configuration string end");
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
		raiseError("invalid log level");
	}
	return result;
}

SizeRange ParseConfiguration::parseSizeRange() noexcept
{
	SizeRange result{};
	skipSpaceChars();
	if (parseDelimiter("[") == 0) {
		raiseError("expexted '[' at size range begin");
	}
	result.first = parse<std::size_t>();
	if (parseDelimiter(",") == 0) {
		raiseError("expexted ',' separating size range first and last");
	}
	result.last = parse<std::size_t>();
	if (parseDelimiter("]") == 0) {
		raiseError("expexted ']' at size range end");
	}
	if (result.first > result.last) {
		raiseError("size range first grater than last");
	}
	return result;
}

void ParseConfiguration::parsePoolMap() noexcept
{
	pools.emplace();
	if (parseDelimiter("{") != '{') {
		raiseError("expected '{' at pool configuration begin");
	}
	char delimiter{parseDelimiter("}")};
	while (delimiter != '}') {
		parsePool();
		if ((delimiter = parseDelimiter(",}")) == 0) {
			raiseError("expected ',' pool map element delimiter");
		}
	}
}

void ParseConfiguration::parsePool() noexcept
{
	const SizeRange range{parseSizeRange()};
	if (parseDelimiter(":") != ':') {
		raiseError("expected ':' at pool configuration begin");
	}
	std::size_t nChunks{parse<std::size_t>()};
	if (!pools.value().emplace(range, nChunks)) {
		raiseError("expected disjunct pool size ranges");
	}
}

void ParseConfiguration::raiseError(std::string_view message)
{
	ConsoleLogger::exit([&] { return Format("ParseConfiguration: {}", message); });
}

} // namespace ArenaAllocator
