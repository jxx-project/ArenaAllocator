#include "ConsoleLogger.h"
#include <cstdio>

ConsoleLogger::ConsoleLogger() noexcept
{
}

ConsoleLogger::~ConsoleLogger() noexcept
{
}

void Logger::write(const char* fmt, va_list argp) noexcept
{
	std::vfprintf(stderr, fmt, argp);
}
