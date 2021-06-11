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
	NullLogger() = default;
	NullLogger(Logger const&) = delete;
	NullLogger& operator=(NullLogger const&) = delete;
	~NullLogger() override = default;

	void operator()(char const*, ...) const noexcept override;
	void operator()(std::chrono::nanoseconds, ArenaAllocator::OperationType, char const*, ...) const noexcept override;
	void operator()(ArenaAllocator::LogLevel, char const*, ...) const noexcept override;
	bool isLevel(ArenaAllocator::LogLevel) const noexcept override;
	void setLevel(ArenaAllocator::LogLevel) noexcept override;
};

} // namespace Mock

#endif // Mock_NullLogger_h_INCLUDED
