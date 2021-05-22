//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

#include "CustomAllocators/NativeCXXAllocator.h"
#include <cstdint>
#include <map>

namespace CustomAllocators {

class Configuration
{
public:
	using MapType =
		std::map<std::size_t, std::size_t, std::less<std::size_t>, NativeCXXAllocator<std::pair<const std::size_t, std::size_t>>>;

	virtual ~Configuration() noexcept = default;
	virtual MapType const& getArenas() const noexcept = 0;
};

} // namespace CustomAllocators

#endif // CONFIGURATION_H_INCLUDED
