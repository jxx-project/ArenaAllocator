#include "Logger.h"
#include <cstdio>

Logger::Logger() noexcept
{
}

void Logger::write(const char* fmt, va_list argp) noexcept
{
	std::vfprintf(stderr, fmt, argp);
}
