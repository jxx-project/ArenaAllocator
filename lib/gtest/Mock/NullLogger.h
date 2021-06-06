//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef Mock_NullLogger_h_INCLUDED
#define Mock_NullLogger_h_INCLUDED

#include "ArenaAllocator/Logger.h"

namespace Mock {

class NullLogger : public ArenaAllocator::Logger
{
public:
	NullLogger() noexcept;
	NullLogger(Logger const&) = delete;
	NullLogger& operator=(NullLogger const&) = delete;
	virtual ~NullLogger() noexcept;

	virtual void operator()(char const*, ...) const noexcept override;
	virtual void operator()(std::chrono::nanoseconds, ArenaAllocator::OperationType, char const*, ...) const noexcept override;
	virtual void operator()(ArenaAllocator::LogLevel, char const*, ...) const noexcept override;
	virtual bool isLevel(ArenaAllocator::LogLevel) const noexcept override;
	virtual void setLevel(ArenaAllocator::LogLevel) noexcept override;
};

} // namespace Mock

#endif // Mock_NullLogger_h_INCLUDED
