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

void writeToBuffer(std::string_view message) noexcept
{
	std::array<char, TimerLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations

	Format prefixFormat{"[pid:{}]\t\t", ::getpid()};
	std::string_view prefixStr{prefixFormat.getResult()};
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
	TimerLogger::log(LogLevel::DEBUG, FormattingCallback{[&] { return Format("TimerLogger::TimerLogger() -> this:{}\n", this); }});
}

TimerLogger::~TimerLogger() noexcept
{
	TimerLogger::log(LogLevel::DEBUG, FormattingCallback{[&] { return Format("TimerLogger::~TimerLogger(this:{})\n", this); }});
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
	Format message = formatter();
	writeToBuffer(message.getResult());
}

void TimerLogger::log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept
{
	write(Format{"[pid:{}]\tTimerLogger:{},{}\n", ::getpid(), to_string(operationType), duration.count()}.getResult());
}

void TimerLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Format message = formatter();
		writeToBuffer(message.getResult());
	}
}

} // namespace ArenaAllocator
