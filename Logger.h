#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdarg.h>

class Logger
{
public:
	Logger(Logger const&) = delete;
	void operator=(Logger const&) = delete;
	static void log(const char* fmt, ...) noexcept
	{
		va_list argp;
		va_start(argp, fmt);
		getInstance().write(fmt, argp);
		va_end(argp);
	}

private:
	Logger() noexcept;
	static Logger& getInstance() noexcept
	{
		static Logger instance;
		return instance;
	}
	void write(const char* fmt, va_list argp) noexcept;
};

#endif // LOGGER_H_INCLUDED
