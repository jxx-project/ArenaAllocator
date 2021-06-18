#include "ArenaAllocator/ParsePrimitives.h"

namespace ArenaAllocator {

ParsePrimitives::ParsePrimitives(std::string_view str) noexcept : current{str}
{
}

char ParsePrimitives::parseDelimiter(const std::string_view delimiters) noexcept
{
	skipSpaceChars();
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

std::string_view ParsePrimitives::parseIdentifier() noexcept
{
	skipSpaceChars();
	char const* const indentifierPtr{current.data()};
	while (!current.empty() && isAlpha(current.front())) {
		current.remove_prefix(1);
	}
	return std::string_view(indentifierPtr, current.data() - indentifierPtr);
}

void ParsePrimitives::skipSpaceChars() noexcept
{
	while (!current.empty() && isSpace(current.front())) {
		current.remove_prefix(1);
	}
}

bool ParsePrimitives::empty() const noexcept
{
	return current.empty();
}

bool ParsePrimitives::isSpace(const char c) noexcept
{
	return c == ' ' || c == '\t' || c == '\n';
}

bool ParsePrimitives::isAlpha(const char c) noexcept
{
	return c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z';
}

} // namespace ArenaAllocator
