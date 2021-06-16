//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_Logger_h_INCLUDED
#define ArenaAllocator_Logger_h_INCLUDED

#include "ArenaAllocator/LogLevel.h"
#include "ArenaAllocator/OperationType.h"
#include <chrono>

namespace ArenaAllocator {

class Logger
{
public:
	virtual ~Logger() = default;

	template<typename F>
	void operator()(F callback) const noexcept
	{
		log(FormattingCallback(std::forward<F>(callback)));
	}

	template<typename F>
	void operator()(std::chrono::nanoseconds duration, OperationType opType, F callback) const noexcept
	{
		log(duration, opType, FormattingCallback(std::forward<F>(callback)));
	}

	template<typename F>
	void operator()(LogLevel level, F callback) const noexcept
	{
		log(level, FormattingCallback(std::forward<F>(callback)));
	}

	virtual void operator()(char const* fmt, ...) const noexcept = 0;
	virtual void operator()(std::chrono::nanoseconds duration, OperationType opType, char const* fmt, ...) const noexcept = 0;
	virtual void operator()(LogLevel level, char const* fmt, ...) const noexcept = 0;
	[[nodiscard]] virtual bool isLevel(LogLevel level) const noexcept = 0;
	virtual void setLevel(LogLevel level) noexcept = 0;

protected:
	class Formatter
	{
	public:
		virtual ~Formatter() noexcept = default;
		virtual std::string_view getStringView() const noexcept = 0;
	};

	template<typename F>
	class FormattingCallback : public Formatter
	{
	public:
		FormattingCallback(F callback) : callback{callback}
		{
		}
		~FormattingCallback() noexcept override = default;

		std::string_view getStringView() const noexcept override
		{
			return callback().getStringView();
		}

	private:
		F callback;
	};

	virtual void log(Formatter const& formatter) const noexcept = 0;
	virtual void log(std::chrono::nanoseconds duration, OperationType operationType, Formatter const& formatter) const noexcept = 0;
	virtual void log(LogLevel level, Formatter const& formatter) const noexcept = 0;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_Logger_h_INCLUDED
