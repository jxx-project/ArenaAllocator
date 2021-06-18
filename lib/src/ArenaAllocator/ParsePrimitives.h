//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_ParsePrimitives_h_INCLUDED
#define ArenaAllocator_ParsePrimitives_h_INCLUDED

#include <charconv>
#include <string_view>

namespace ArenaAllocator {

class ParsePrimitives
{
public:
	ParsePrimitives(std::string_view str) noexcept;
	virtual ~ParsePrimitives() noexcept = default;

protected:
	template<typename T>
	[[nodiscard]] T parse()
	{
		T result{};
		std::from_chars_result matchResult{std::from_chars(current.data(), current.data() + current.size(), result)};
		if (!static_cast<bool>(matchResult.ec)) {
			current.remove_prefix(matchResult.ptr - current.data());
		} else if (matchResult.ec == std::errc::result_out_of_range) {
			raiseError("value out of range");
		} else {
			raiseError("invalid value");
		}
		return result;
	}

	char parseDelimiter(std::string_view delimiters) noexcept;
	std::string_view parseIdentifier() noexcept;
	void skipSpaceChars() noexcept;
	[[nodiscard]] bool empty() const noexcept;
	[[nodiscard]] static bool isSpace(char c) noexcept;
	[[nodiscard]] static bool isAlpha(char c) noexcept;
	[[noreturn]] virtual void raiseError(std::string_view message) = 0;

private:
	std::string_view current;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_ParsePrimitives_h_INCLUDED
