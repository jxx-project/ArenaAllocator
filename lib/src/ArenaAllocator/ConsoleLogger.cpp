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

template<typename T>
void writeToBuffer(T prefix, std::string_view message) noexcept
{
	std::array<char, ConsoleLogger::bufferSize> buffer; // NOLINT initialized by subsequent write operations

	Format prefixFormat{"[pid:{}]\t{}\t", ::getpid(), prefix};
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
	ConsoleLogger::operator()(LogLevel::DEBUG, [&] { return Format("ConsoleLogger::ConsoleLogger() -> this:{}\n", this); });
}

ConsoleLogger::~ConsoleLogger() noexcept
{
	ConsoleLogger::operator()(LogLevel::DEBUG, [&] { return Format("ConsoleLogger::~ConsoleLogger(this:{})\n", this); });
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
	writeToBuffer(std::string_view{}, message.getStringView());
}

void ConsoleLogger::log(std::chrono::nanoseconds duration, OperationType, Formatter const& formatter) const noexcept
{
	Format message = formatter();
	writeToBuffer(duration, message.getStringView());
}

void ConsoleLogger::log(LogLevel level, Formatter const& formatter) const noexcept
{
	if (isLevel(level)) {
		Format message{formatter()};
		writeToBuffer(std::string_view{}, message.getStringView());
	}
}

void ConsoleLogger::logAbort(Formatter const& formatter) noexcept
{
	Format message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator abort:"}, message.getStringView());
	std::abort();
}

void ConsoleLogger::logExit(Formatter const& formatter) noexcept
{
	Format message{formatter()};
	writeToBuffer(std::string_view{"ArenaAllocator exit:"}, message.getStringView());
	std::exit(-1);
}

} // namespace ArenaAllocator
