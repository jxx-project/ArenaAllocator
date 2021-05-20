#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

class Logger
{
public:
	virtual ~Logger() = default;
	virtual void log(const char* fmt, ...) const noexcept = 0;
};

#endif // LOGGER_H_INCLUDED
