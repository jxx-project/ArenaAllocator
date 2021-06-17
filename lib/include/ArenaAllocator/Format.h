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
	Format(std::string_view fmt, Args const&... args) noexcept :
		result{format(buffer.data(), buffer.size(), fmt, std::forward<Args const&>(args)...)}
	{
	}

	template<typename... Args>
	[[nodiscard]] static constexpr std::string_view format(
		char* buffer,
		std::size_t bufferSize,
		std::string_view fmt,
		Args const&... args)
	{
		char const* first{buffer};
		BufferView bufferView{buffer, bufferSize};
		((writeToBuffer(bufferView, fmt, args)), ...);
		writeToBuffer(bufferView, fmt);
		return std::string_view{first, std::size_t(bufferView.data() - first)};
	}

	[[nodiscard]] std::string_view getResult() const noexcept;

	static constexpr std::size_t bufferSize{1024};

private:
	class BufferView
	{
	public:
		BufferView(char* first, std::size_t bufferSize) noexcept : first{first}, bufferSize{bufferSize}
		{
		}
		char* data() noexcept
		{
			return first;
		}

		std::size_t size() noexcept
		{
			return bufferSize;
		}

		void removePrefix(std::size_t n)
		{
			first += n;
			bufferSize -= n;
		}

	private:
		char* first;
		std::size_t bufferSize;
	};

	template<typename T>
	static void writeToBuffer(BufferView& bufferView, std::string_view& fmt, T const& value) noexcept
	{
		std::size_t placeholderPos{fmt.find("{}")};
		writeToBuffer(bufferView, std::string_view(fmt.data(), placeholderPos));
		fmt.remove_prefix(placeholderPos + 2);
		writeToBuffer(bufferView, value);
	}

	template<typename T, typename = typename std::enable_if<std::is_integral<T>::value && !std::is_same<T, bool>::value>::type>
	static void writeToBuffer(BufferView& bufferView, T const& value) noexcept
	{
		std::to_chars_result conversionResult{std::to_chars(bufferView.data(), bufferView.data() + bufferView.size(), value)};
		if (conversionResult.ec == std::errc{}) {
			bufferView.removePrefix(conversionResult.ptr - bufferView.data());
		}
	}

	static void writeToBuffer(BufferView& bufferView, std::string_view value) noexcept;
	static void writeToBuffer(BufferView& bufferView, bool value) noexcept;
	static void writeToBuffer(BufferView& bufferView, double value) noexcept;
	static void writeToBuffer(BufferView& bufferView, char const* ptr) noexcept;
	static void writeToBuffer(BufferView& bufferView, void const* ptr) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::nanoseconds duration) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::microseconds duration) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::milliseconds duration) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::seconds duration) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::minutes duration) noexcept;
	static void writeToBuffer(BufferView& bufferView, std::chrono::hours duration) noexcept;

	std::array<char, bufferSize> buffer;
	std::string_view result;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Format_h_INCLUDED
