#ifndef CONSOLE_LOGGER_H_INCLUDED
#define CONSOLE_LOGGER_H_INCLUDED

#include "Logger.h"
#include <cstdarg>

class ConsoleLogger : public Logger
{
public:
	ConsoleLogger() noexcept;
	ConsoleLogger(Logger const&) = delete;
	ConsoleLogger& operator=(ConsoleLogger const&) = delete;
	virtual ~ConsoleLogger() noexcept;
	virtual void log(const char* fmt, ...) const noexcept override;

private:
	void write(const char* fmt, std::va_list argp) const noexcept;
};

#endif // CONSOLE_LOGGER_H_INCLUDED
