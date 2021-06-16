//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/TimerLogger.h"
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

void write(char const* fmt, std::va_list argp) noexcept
{
	std::array<char, TimerLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	::ssize_t prefixLengthOrError{std::snprintf(buffer.data(), buffer.size() - 1, "[pid:%u]\t\t", ::getpid())};
	std::size_t prefixLength{std::size_t(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	// NOLINTNEXTLINE available buffer size checked in vsnprintf argument
	::ssize_t messageLengthOrError{std::vsnprintf(&buffer[prefixLength], buffer.size() - 1 - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), buffer.size() - 1)};
	buffer[totalLength++] = '\n'; // NOLINT available buffer size checked above
	write(std::string_view(buffer.data(), totalLength));
}

void writeWithPrefix(std::string_view message) noexcept
{
	std::array<char, TimerLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	Format prefixFormat{"[pid:{}]\t\t", ::getpid()};
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

TimerLogger::TimerLogger() noexcept : logLevel{LogLevel::NONE}
{
	TimerLogger::operator()(LogLevel::DEBUG, "TimerLogger::TimerLogger() -> this:%p\n", this);
}

TimerLogger::~TimerLogger() noexcept
{
	TimerLogger::operator()(LogLevel::DEBUG, "TimerLogger::~TimerLogger(this:%p)\n", this);
}

void TimerLogger::operator()(char const* fmt, ...) const noexcept
{
	std::va_list argp;
	::va_start(argp, fmt); // NOLINT std::va_list and ::va_start are libc defined
	write(fmt, argp); // NOLINT std::va_list is libc defined
	::va_end(argp); // NOLINT std::va_list is libc defined
}

void TimerLogger::operator()(std::chrono::nanoseconds duration, OperationType operationType, char const*, ...) const noexcept
{
	std::array<char, TimerLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	::ssize_t lengthOrError{std::snprintf(
		buffer.data(),
		buffer.size(),
		"[pid:%u]\tTimerLogger:%s,%lu\n",
		::getpid(),
		to_string(operationType).data(),
		duration.count())};
	write(std::string_view(buffer.data(), lengthOrError > 0 ? std::size_t(lengthOrError) : 0));
}

void TimerLogger::operator()(LogLevel level, char const* fmt, ...) const noexcept
{
	if (isLevel(level)) {
		std::va_list argp;
		::va_start(argp, fmt); // NOLINT std::va_list and ::va_start are libc defined
		write(fmt, argp); // NOLINT std::va_list is libc defined
		::va_end(argp); // NOLINT std::va_list is libc defined
	}
}

bool TimerLogger::isLevel(LogLevel level) const noexcept
{
	return logLevel >= level;
}

void TimerLogger::setLevel(LogLevel level) noexcept
{
	logLevel = level;
}

void TimerLogger::log(Formatter const& formatter) const noexcept
{
	writeWithPrefix(formatter.getStringView());
}

void TimerLogger::log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept
{
	write(Format{"[pid:{}]\tTimerLogger:{},{}", ::getpid(), to_string(operationType), duration.count()}.getStringView());
}

void TimerLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		writeWithPrefix(formatter.getStringView());
	}
}

} // namespace ArenaAllocator
