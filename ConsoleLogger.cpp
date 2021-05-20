#include "ConsoleLogger.h"
#include <cstdio>

ConsoleLogger::ConsoleLogger() noexcept
{
}

ConsoleLogger::~ConsoleLogger() noexcept
{
}

void ConsoleLogger::log(const char* fmt, ...) const noexcept
{
	std::va_list argp;
	va_start(argp, fmt);
	write(fmt, argp);
	va_end(argp);
}

void ConsoleLogger::write(const char* fmt, std::va_list argp) const noexcept
{
	std::vfprintf(stderr, fmt, argp);
}
