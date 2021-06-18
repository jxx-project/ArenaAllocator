//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Format_h_INCLUDED
#define ArenaAllocator_Format_h_INCLUDED

#include <array>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <string_view>
#include <type_traits>

namespace ArenaAllocator {

class Format
{
public:
	template<typename... Args>
	// NOLINTNEXTLINE no read on buffer until getResult() returns the corresponding std::string_view
	explicit Format(std::string_view fmt, Args const&... args) noexcept :
		result{format(buffer.data(), buffer.size(), fmt, std::forward<Args const&>(args)...)} // NOLINT buffer used write-only
	{
	}

	Format(Format const& other) noexcept;
	Format& operator=(Format const& other) noexcept;
	~Format() noexcept = default;

	template<typename... Args>
	[[nodiscard]] static std::string_view format(char* buffer, std::size_t bufferSize, std::string_view fmt, Args const&... args)
	{
		Span out{buffer, bufferSize};
		((write(out, fmt, args)), ...);
		write(out, fmt);
		return std::string_view{buffer, std::size_t(out.data() - buffer)};
	}

	[[nodiscard]] std::string_view getResult() const noexcept;

	static constexpr std::size_t bufferSize{1024};

private:
	class Span
	{
	public:
		Span(char* first, std::size_t count) noexcept;

		[[nodiscard]] char* data() noexcept;
		[[nodiscard]] std::size_t size() const noexcept;
		[[nodiscard]] Span subspan(std::size_t offset) noexcept;

	private:
		char* first;
		std::size_t extent;
	};

	template<typename T>
	static void write(Span& out, std::string_view& fmt, T const& value) noexcept
	{
		std::size_t placeholderPos{fmt.find("{}")};
		write(out, std::string_view(fmt.data(), placeholderPos));
		fmt.remove_prefix(placeholderPos + 2);
		write(out, value);
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type>
	static void write(Span& out, T const& value) noexcept
	{
		std::to_chars_result conversionResult{std::to_chars(out.data(), out.data() + out.size(), value)};
		if (conversionResult.ec == std::errc{}) {
			out = out.subspan(conversionResult.ptr - out.data());
		}
	}

	static void write(Span& out, std::string_view value) noexcept;
	static void write(Span& out, bool value) noexcept;
	static void write(Span& out, double value) noexcept;
	static void write(Span& out, char const* ptr) noexcept;
	static void write(Span& out, void const* ptr) noexcept;
	static void write(Span& out, std::chrono::nanoseconds duration) noexcept;
	static void write(Span& out, std::chrono::microseconds duration) noexcept;
	static void write(Span& out, std::chrono::milliseconds duration) noexcept;
	static void write(Span& out, std::chrono::seconds duration) noexcept;
	static void write(Span& out, std::chrono::minutes duration) noexcept;
	static void write(Span& out, std::chrono::hours duration) noexcept;

	std::array<char, bufferSize> buffer;
	std::string_view result;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Format_h_INCLUDED
