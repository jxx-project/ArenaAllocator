//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/Format.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string_view>
#include <unistd.h>

namespace ArenaAllocator {

namespace {

void write(std::string_view str) noexcept
{
	int propagateErrno{errno};

	std::size_t totalBytesWritten{0};
	::ssize_t bytesWritten{0};
	while ((bytesWritten = ::write(STDERR_FILENO, &str[totalBytesWritten], str.size() - totalBytesWritten)) > 0) {
		totalBytesWritten += bytesWritten;
	}

	errno = propagateErrno;
}

void write(char const* prefix, char const* fmt, std::va_list argp) noexcept
{
	std::array<char, ConsoleLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	::ssize_t prefixLengthOrError{
		std::snprintf(buffer.data(), buffer.size() - 1, "[pid:%u]\t%s\t", ::getpid(), prefix != nullptr ? prefix : "")};
	std::size_t prefixLength{std::size_t(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	// NOLINTNEXTLINE available buffer size checked in vsnprintf argument
	::ssize_t messageLengthOrError{std::vsnprintf(&buffer[prefixLength], buffer.size() - 1 - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), buffer.size() - 1)};
	buffer[totalLength++] = '\n'; // NOLINT available buffer size checked above
	write(std::string_view(buffer.data(), totalLength));
}

void write(std::chrono::nanoseconds duration, char const* fmt, std::va_list argp) noexcept
{
	std::array<char, ConsoleLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	::ssize_t prefixLengthOrError{
		std::snprintf(buffer.data(), buffer.size() - 1, "[pid:%u]\t%luns\t", ::getpid(), duration.count())};
	std::size_t prefixLength{std::size_t(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	// NOLINTNEXTLINE available buffer size checked in vsnprintf argument
	::ssize_t messageLengthOrError{std::vsnprintf(&buffer[prefixLength], buffer.size() - 1 - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), buffer.size() - 1)};
	buffer[totalLength++] = '\n'; // NOLINT available buffer size checked above
	write(std::string_view(buffer.data(), totalLength));
}

void writeWithPrefix(std::string_view prefix, std::string_view message) noexcept
{
	std::cout << "writeWithPrefix(" << prefix << ", " << message << ")" << std::endl;
	std::array<char, ConsoleLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	Format prefixFormat{"[pid:{}]\t{}\t", ::getpid(), prefix};
	std::string_view prefixStr{prefixFormat.getStringView()};
	std::size_t prefixLength{std::min(prefixStr.size(), buffer.size() - 1)};
	std::memcpy(buffer.data(), prefixStr.data(), prefixLength);
	std::size_t messageLength{std::min(message.size(), buffer.size() - 1 - prefixLength)};
	std::memcpy(&buffer[prefixLength], message.data(), messageLength);
	std::size_t totalLength{std::min(prefixLength + messageLength, buffer.size() - 1)};
	buffer[totalLength++] = '\n'; // NOLINT available buffer size checked above
	std::cout << "writeWithPrefix(" << prefix << ", " << message << ")" << std::endl;
	write(std::string_view(buffer.data(), totalLength));
}

void writeWithPrefix(std::chrono::nanoseconds duration, std::string_view message) noexcept
{
	std::array<char, ConsoleLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	Format prefixFormat{"[pid:{}]\t{}\t", ::getpid(), duration.count()};
	std::string_view prefixStr{prefixFormat.getStringView()};
	std::size_t prefixLength{std::min(prefixStr.size(), buffer.size() - 1)};
	std::memcpy(buffer.data(), prefixStr.data(), prefixLength);
	std::size_t messageLength{std::min(message.size(), buffer.size() - 1 - prefixLength)};
	std::memcpy(&buffer[prefixLength], message.data(), messageLength);
	std::size_t totalLength{std::min(prefixLength + messageLength, buffer.size() - 1)};
	buffer[totalLength++] = '\n'; // NOLINT available buffer size checked above
	write(std::string_view(buffer.data(), totalLength));
}

} // namespace

ConsoleLogger::ConsoleLogger() noexcept : logLevel{LogLevel::NONE}
{
	ConsoleLogger::operator()(LogLevel::DEBUG, "ConsoleLogger::ConsoleLogger() -> this:%p\n", this);
}

ConsoleLogger::~ConsoleLogger() noexcept
{
	ConsoleLogger::operator()(LogLevel::DEBUG, "ConsoleLogger::~ConsoleLogger(this:%p)\n", this);
}

void ConsoleLogger::operator()(char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt); // NOLINT std::va_list and ::va_start are libc defined
	write(nullptr, fmt, argp); // NOLINT std::va_list is libc defined
	::va_end(argp); // NOLINT std::va_list is libc defined
}

void ConsoleLogger::operator()(std::chrono::nanoseconds duration, OperationType, char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt); // NOLINT std::va_list, ::va_start are libc defined
	write(duration, fmt, argp); // NOLINT std::va_list is libc defined
	::va_end(argp); // NOLINT std::va_list is libc defined
}

void ConsoleLogger::operator()(LogLevel level, char const* fmt, ...) const noexcept
{
	if (isLevel(level)) {
		std::va_list argp;
		::va_start(argp, fmt); // NOLINT std::va_list and ::va_start are libc defined
		write(nullptr, fmt, argp); // NOLINT std::va_list is libc defined
		::va_end(argp); // NOLINT std::va_list is libc defined
	}
}

bool ConsoleLogger::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void ConsoleLogger::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

void ConsoleLogger::log(Formatter const& formatter) const noexcept
{
	Format message{formatter()};
	writeWithPrefix(std::string_view{}, message.getStringView());
}

void ConsoleLogger::log(std::chrono::nanoseconds duration, OperationType, Formatter const& formatter) const noexcept
{
	Format message = formatter();
	writeWithPrefix(duration, message.getStringView());
}

void ConsoleLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Format message{formatter()};
		writeWithPrefix(std::string_view{}, message.getStringView());
	}
}

void ConsoleLogger::logAbort(Formatter const& formatter) noexcept
{
	Format message{formatter()};
	writeWithPrefix(std::string_view{"ArenaAllocator abort:"}, message.getStringView());
	std::abort();
}

void ConsoleLogger::logExit(Formatter const& formatter) noexcept
{
	Format message{formatter()};
	writeWithPrefix(std::string_view{"ArenaAllocator exit:"}, message.getStringView());
	std::exit(-1);
}

} // namespace ArenaAllocator
