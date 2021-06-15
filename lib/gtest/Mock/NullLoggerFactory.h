//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef Mock_NullLoggerFactory_h_INCLUDED
#define Mock_NullLoggerFactory_h_INCLUDED

#include "ArenaAllocator/LoggerFactory.h"

namespace Mock {

class NullLoggerFactory : public ArenaAllocator::LoggerFactory
{
public:
	NullLoggerFactory() = default;
	NullLoggerFactory(NullLoggerFactory const&) = delete;
	NullLoggerFactory& operator=(NullLoggerFactory const&) = delete;
	~NullLoggerFactory() override = default;

	ArenaAllocator::Logger* getLogger(std::string_view const&) noexcept override;
};

} // namespace Mock

#endif // Mock_NullLoggerFactory_h_INCLUDED
