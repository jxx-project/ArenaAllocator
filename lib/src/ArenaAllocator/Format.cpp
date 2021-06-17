//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Format.h"
#include <cstdio>
#include <cstring>

namespace ArenaAllocator {

void Format::writeToBuffer(std::string_view value) noexcept
{
	std::size_t copySize{std::min(value.size(), buffer.size() - length)};
	::memcpy(&buffer[length], value.data(), copySize);
	length += copySize;
}

void Format::writeToBuffer(bool value) noexcept
{
	if (value) {
		writeToBuffer(std::string_view("true"));
	} else {
		writeToBuffer(std::string_view("false"));
	}
}

void Format::writeToBuffer(double value) noexcept
{
	std::size_t conversionlength{std::size_t(::snprintf(&buffer[length], buffer.size() - length, "%f", double(value)))};
	length += std::min(conversionlength, buffer.size() - length);
}

void Format::writeToBuffer(char const* ptr) noexcept
{
	if (ptr == nullptr) {
		writeToBuffer(std::string_view("(nil)"));
	} else {
		writeToBuffer(std::string_view(ptr));
	}
}

void Format::writeToBuffer(void const* ptr) noexcept
{
	if (ptr == nullptr) {
		writeToBuffer(std::string_view("(nil)"));
	} else {
		writeToBuffer(std::string_view("0x"));
		std::to_chars_result conversionResult{
			std::to_chars(&buffer[length], buffer.data() + buffer.size(), std::ptrdiff_t(ptr), 16)};
		if (conversionResult.ec == std::errc{}) {
			length += conversionResult.ptr - &buffer[length];
		}
	}
}

void Format::writeToBuffer(std::chrono::nanoseconds duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("ns"));
}

void Format::writeToBuffer(std::chrono::microseconds duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("us"));
}

void Format::writeToBuffer(std::chrono::milliseconds duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("ms"));
}

void Format::writeToBuffer(std::chrono::seconds duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("s"));
}

void Format::writeToBuffer(std::chrono::minutes duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("min"));
}

void Format::writeToBuffer(std::chrono::hours duration) noexcept
{
	writeToBuffer(duration.count());
	writeToBuffer(std::string_view("h"));
}

} // namespace ArenaAllocator
