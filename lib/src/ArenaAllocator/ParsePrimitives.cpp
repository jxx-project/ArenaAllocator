#include "ArenaAllocator/ParsePrimitives.h"

namespace ArenaAllocator {

ParsePrimitives::ParsePrimitives(std::string_view str) noexcept : in{str}
{
}

char ParsePrimitives::parseDelimiter(const std::string_view delimiters) noexcept
{
	skipSpaceChars();
	char result{0};
	for (const char p : delimiters) {
		if (in.front() == p) {
			result = p;
			in.remove_prefix(1);
			break;
		}
	}
	return result;
}

std::string_view ParsePrimitives::parseIdentifier() noexcept
{
	skipSpaceChars();
	char const* const indentifierPtr{in.data()};
	while (!in.empty() && isAlpha(in.front())) {
		in.remove_prefix(1);
	}
	return std::string_view(indentifierPtr, in.data() - indentifierPtr);
}

void ParsePrimitives::skipSpaceChars() noexcept
{
	while (!in.empty() && isSpace(in.front())) {
		in.remove_prefix(1);
	}
}

bool ParsePrimitives::empty() const noexcept
{
	return in.empty();
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
