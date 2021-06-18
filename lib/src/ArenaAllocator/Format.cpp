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

Format::BufferView::BufferView(char* first, std::size_t bufferSize) noexcept : first{first}, bufferSize{bufferSize}
{
}

char* Format::BufferView::data() noexcept
{
	return first;
}

std::size_t Format::BufferView::size() const noexcept
{
	return bufferSize;
}

void Format::BufferView::removePrefix(std::size_t n) noexcept
{
	first += n;
	bufferSize -= n;
}

void Format::writeToBuffer(BufferView& bufferView, std::string_view value) noexcept
{
	std::size_t size{std::min(value.size(), bufferView.size())};
	std::memcpy(bufferView.data(), value.data(), size);
	bufferView.removePrefix(size);
}

void Format::writeToBuffer(BufferView& bufferView, bool value) noexcept
{
	if (value) {
		writeToBuffer(bufferView, std::string_view("true"));
	} else {
		writeToBuffer(bufferView, std::string_view("false"));
	}
}

void Format::writeToBuffer(BufferView& bufferView, double value) noexcept
{
	std::size_t length{std::size_t(::snprintf(bufferView.data(), bufferView.size(), "%f", double(value)))};
	bufferView.removePrefix(std::min(length, bufferView.size()));
}

void Format::writeToBuffer(BufferView& bufferView, char const* ptr) noexcept
{
	if (ptr == nullptr) {
		writeToBuffer(bufferView, std::string_view("(nil)"));
	} else {
		writeToBuffer(bufferView, std::string_view(ptr));
	}
}

void Format::writeToBuffer(BufferView& bufferView, void const* ptr) noexcept
{
	if (ptr == nullptr) {
		writeToBuffer(bufferView, std::string_view("(nil)"));
	} else {
		writeToBuffer(bufferView, std::string_view("0x"));
		std::to_chars_result conversionResult{
			std::to_chars(bufferView.data(), bufferView.data() + bufferView.size(), std::uintptr_t(ptr), 16)};
		if (conversionResult.ec == std::errc{}) {
			bufferView.removePrefix(conversionResult.ptr - bufferView.data());
		}
	}
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::nanoseconds duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("ns"));
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::microseconds duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("us"));
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::milliseconds duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("ms"));
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::seconds duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("s"));
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::minutes duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("min"));
}

void Format::writeToBuffer(BufferView& bufferView, std::chrono::hours duration) noexcept
{
	writeToBuffer(bufferView, duration.count());
	writeToBuffer(bufferView, std::string_view("h"));
}

} // namespace ArenaAllocator
