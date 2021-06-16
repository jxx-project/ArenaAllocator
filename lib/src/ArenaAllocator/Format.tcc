//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Format_tcc_INCLUDED
#define ArenaAllocator_Format_tcc_INCLUDED

#include "ArenaAllocator/Format.h"
#include <cstdio>
#include <cstring>

namespace ArenaAllocator {

template<std::size_t bufferSize>
void Format<bufferSize>::writeToBuffer(std::string_view value) noexcept
{
	std::size_t copySize{std::min(value.size(), buffer.size() - length)};
	::memcpy(&buffer[length], value.data(), copySize);
	length += copySize;
}

template<std::size_t bufferSize>
void Format<bufferSize>::writeToBuffer(bool value) noexcept
{
	if (value) {
		writeToBuffer(std::string_view("true"));
	} else {
		writeToBuffer(std::string_view("false"));
	}
}

template<std::size_t bufferSize>
void Format<bufferSize>::writeToBuffer(double value) noexcept
{
	std::size_t conversionlength{std::size_t(::snprintf(&buffer[length], buffer.size() - length, "%f", double(value)))};
	length += std::min(conversionlength, buffer.size() - length);
}

template<std::size_t bufferSize>
void Format<bufferSize>::writeToBuffer(void const* ptr) noexcept
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

} // namespace ArenaAllocator

#endif // ArenaAllocator_Format_tcc_INCLUDED
