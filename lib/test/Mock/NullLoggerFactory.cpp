//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "Mock/NullLoggerFactory.h"

namespace Mock {

ArenaAllocator::Logger* NullLoggerFactory::getLogger(std::string_view const&) noexcept
{
	return nullptr;
}

} // namespace Mock
