//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/SizeRangeMap.tcc"

namespace ArenaAllocator {

template class SizeRangeMap<std::size_t>;
template class SizeRangeMap<Pool>;
template class SizeRangeMap<PoolStatistics>;

} // namespace ArenaAllocator
