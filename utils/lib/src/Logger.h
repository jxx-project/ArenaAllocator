//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef Logger_h_INCLUDED
#define Logger_h_INCLUDED

#include <Static/BasicLogger.h>

using Message = Static::Format::Buffer<Static::BasicLogger::maxLength>;

class Logger : private Static::BasicLogger
{
public:
	Logger() : enabled{false}
	{
	}

	template<typename F>
	void operator()(F callback) const noexcept
	{
		if (enabled) {
			log(FormattingCallback{callback});
		}
	}

	void enable(bool enabled) noexcept
	{
		this->enabled = enabled;
	}

private:
	bool enabled;
};

#endif // ArenaAllocator_Logger_h_INCLUDED
