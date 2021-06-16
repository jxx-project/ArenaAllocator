//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Format_h_INCLUDED
#define ArenaAllocator_Format_h_INCLUDED

#include <array>
#include <charconv>
#include <cstddef>
#include <string_view>
#include <type_traits>

namespace ArenaAllocator {

template<std::size_t bufferSize = 1024>
class Format
{
public:
	template<typename... Args>
	Format(std::string_view fmt, Args const&... args)
	{
		if constexpr (sizeof...(args) > 0) {
			format(fmt, std::forward<Args const&>(args)...);
		} else {
			writeToBuffer(fmt);
		}
	}

	[[nodiscard]] std::string_view getStringView() const noexcept
	{
		return std::string_view(buffer.data(), length);
	}

private:
	template<typename First, typename... Rest>
	void format(std::string_view fmt, First const& first, Rest const&... rest)
	{
		std::size_t placeholderPos{fmt.find("{}")};
		if (placeholderPos == std::string_view::npos) {
			writeToBuffer(fmt);
		} else {
			writeToBuffer(std::string_view(fmt.data(), placeholderPos));
			fmt.remove_prefix(placeholderPos + 2);
			writeToBuffer(first);
			if constexpr (sizeof...(rest) > 0) {
				format(fmt, rest...);
			} else if (!fmt.empty()) {
				writeToBuffer(fmt);
			}
		}
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type>
	void writeToBuffer(T const& value) noexcept
	{
		std::to_chars_result conversionResult{std::to_chars(&buffer[length], buffer.data() + buffer.size(), value)};
		if (conversionResult.ec == std::errc{}) {
			length += conversionResult.ptr - &buffer[length];
		}
	}

	void writeToBuffer(std::string_view value) noexcept;
	void writeToBuffer(bool value) noexcept;
	void writeToBuffer(double value) noexcept;
	void writeToBuffer(void const* ptr) noexcept;

	std::array<char, bufferSize> buffer;
	std::size_t length{0};
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Format_h_INCLUDED
