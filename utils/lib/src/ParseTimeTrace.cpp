#include "ParseTimeTrace.h"
#include <string>

ParseTimeTrace::Error::Error(std::string_view message) noexcept : std::runtime_error{std::string{message}}
{
}

ParseTimeTrace::ParseTimeTrace(std::string_view str) noexcept : ArenaAllocator::ParsePrimitives{str}
{
}

void ParseTimeTrace::operator()(::pid_t& pid, ArenaAllocator::OperationType& operationType, unsigned long& nanoseconds)
{
	if (parseDelimiter("[") != '[') {
		raiseError("expected '[' at log line begin");
	}
	if (parseIdentifier() != "pid") {
		raiseError("expected 'pid' in log line prefix");
	}
	if (parseDelimiter(":") != ':') {
		raiseError("expected ':' after pid in log line prefix");
	}
	pid = parse<::pid_t>();
	if (parseDelimiter("]") != ']') {
		raiseError("expected ']' at log line prefix end");
	}
	if (parseIdentifier() != "TimerLogger") {
		raiseError("expected 'TimerLogger' log line entry prefix");
	}
	if (parseDelimiter(":") != ':') {
		raiseError("expected ':' after log line entry prefix");
	}
	operationType = parseOperationType();
	if (parseDelimiter(",") != ',') {
		raiseError("expected ',' after log line operation type");
	}
	nanoseconds = parse<unsigned long>();
	skipSpaceChars();
	if (!empty()) {
		raiseError("unexpected character after nanoseconds at log line end");
	}
}

ArenaAllocator::OperationType ParseTimeTrace::parseOperationType()
{
	std::string_view operationTypeStr{parseIdentifier()};
	ArenaAllocator::OperationType result{ArenaAllocator::OperationType::UNKNOWN};
	for (unsigned typeIndex{0}; typeIndex != static_cast<unsigned>(ArenaAllocator::OperationType::UNKNOWN); typeIndex) {
		ArenaAllocator::OperationType operationType{typeIndex};
		if (operationTypeStr == to_string(operationType)) {
			result = operationType;
			break;
		}
	}
	return result;
}

void ParseTimeTrace::raiseError(std::string_view message)
{
	throw Error{message};
}
