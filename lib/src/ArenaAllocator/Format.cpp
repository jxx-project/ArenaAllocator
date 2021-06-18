//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/Format.h"
#include <cstdio>
#include <cstring>

namespace ArenaAllocator {

Format::Format(Format const& other) noexcept
{
	std::memcpy(buffer.data(), other.buffer.data(), other.result.size());
	result = std::string_view(buffer.data(), other.result.size());
}

Format& Format::operator=(Format const& other) noexcept
{
	if (this != &other) {
		std::memcpy(buffer.data(), other.buffer.data(), other.result.size());
		result = std::string_view(buffer.data(), other.result.size());
	}
	return *this;
}

std::string_view Format::getResult() const noexcept
{
	return result;
}

Format::Span::Span(char* first, std::size_t count) noexcept : first{first}, extent{count}
{
}

char* Format::Span::data() noexcept
{
	return first;
}

std::size_t Format::Span::size() const noexcept
{
	return extent;
}

Format::Span Format::Span::subspan(std::size_t offset) noexcept
{
	return Span{first + offset, extent - offset};
}

void Format::write(Span& out, std::string_view value) noexcept
{
	std::size_t length{std::min(value.size(), out.size())};
	std::memcpy(out.data(), value.data(), length);
	out = out.subspan(length);
}

void Format::write(Span& out, bool value) noexcept
{
	if (value) {
		write(out, std::string_view("true"));
	} else {
		write(out, std::string_view("false"));
	}
}

void Format::write(Span& out, double value) noexcept
{
	std::size_t length{std::size_t(::snprintf(out.data(), out.size(), "%f", double(value)))};
	out = out.subspan(std::min(length, out.size()));
}

void Format::write(Span& out, char const* ptr) noexcept
{
	if (ptr == nullptr) {
		write(out, std::string_view("(nil)"));
	} else {
		write(out, std::string_view(ptr));
	}
}

void Format::write(Span& out, void const* ptr) noexcept
{
	if (ptr == nullptr) {
		write(out, std::string_view("(nil)"));
	} else {
		write(out, std::string_view("0x"));
		std::to_chars_result conversionResult{std::to_chars(out.data(), out.data() + out.size(), std::uintptr_t(ptr), 16)};
		if (conversionResult.ec == std::errc{}) {
			out = out.subspan(conversionResult.ptr - out.data());
		}
	}
}

void Format::write(Span& out, std::chrono::nanoseconds duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("ns"));
}

void Format::write(Span& out, std::chrono::microseconds duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("us"));
}

void Format::write(Span& out, std::chrono::milliseconds duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("ms"));
}

void Format::write(Span& out, std::chrono::seconds duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("s"));
}

void Format::write(Span& out, std::chrono::minutes duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("min"));
}

void Format::write(Span& out, std::chrono::hours duration) noexcept
{
	write(out, duration.count());
	write(out, std::string_view("h"));
}

} // namespace ArenaAllocator
