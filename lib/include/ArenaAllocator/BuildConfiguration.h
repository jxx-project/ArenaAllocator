//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_BuildConfiguration_h_INCLUDED
#define ArenaAllocator_BuildConfiguration_h_INCLUDED

namespace ArenaAllocator {

class BuildConfiguration
{
public:
	constexpr static bool useMlock{true};
	constexpr static bool discardTimingAcrossContextSwitch{true};
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_BuildConfiguration_h_INCLUDED
