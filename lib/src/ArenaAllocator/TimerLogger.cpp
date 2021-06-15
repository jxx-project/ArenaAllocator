//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/TimerLogger.h"
#include <algorithm>
#include <cerrno>
#include <cstdio>
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
	std::array<char, TimerLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations
	::ssize_t prefixLengthOrError{
		std::snprintf(buffer.data(), buffer.size() - 1, "[pid:%u]\t%s\t", ::getpid(), prefix != nullptr ? prefix : "")};
	std::size_t prefixLength{std::size_t(prefixLengthOrError >= 0 ? prefixLengthOrError : 0)};
	// NOLINTNEXTLINE available buffer size checked in vsnprintf argument
	::ssize_t messageLengthOrError{std::vsnprintf(&buffer[prefixLength], buffer.size() - 1 - prefixLength, fmt, argp)};
	std::size_t totalLength{std::min(prefixLength + (messageLengthOrError >= 0 ? messageLengthOrError : 0), buffer.size() - 1)};
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
	write(nullptr, fmt, argp); // NOLINT std::va_list is libc defined
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
		write(nullptr, fmt, argp); // NOLINT std::va_list is libc defined
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

} // namespace ArenaAllocator
