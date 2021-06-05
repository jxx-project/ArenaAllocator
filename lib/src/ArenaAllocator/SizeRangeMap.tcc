//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_SIZE_RANGE_MAP_TCC_INCLUDED
#define ARENA_ALLOCATOR_SIZE_RANGE_MAP_TCC_INCLUDED

#include "ArenaAllocator/SizeRangeMap.h"
#include "ArenaAllocator/Pool.h"
#include "ArenaAllocator/PoolStatistics.h"

namespace ArenaAllocator {

namespace {

bool rangeBelow(SizeRange const& lhs, SizeRange const& rhs) noexcept
{
	return lhs.last < rhs.first;
}

} // namespace

template<typename T>
SizeRangeMap<T>::SizeRangeMap() noexcept : aggregate{rangeBelow}
{
}

template<typename T>
SizeRangeMap<T>::~SizeRangeMap() noexcept
{
}

template<typename T>
typename SizeRangeMap<T>::iterator SizeRangeMap<T>::begin() noexcept
{
	return aggregate.begin();
}

template<typename T>
typename SizeRangeMap<T>::const_iterator SizeRangeMap<T>::begin() const noexcept
{
	return aggregate.begin();
}

template<typename T>
typename SizeRangeMap<T>::iterator SizeRangeMap<T>::end() noexcept
{
	return aggregate.end();
}

template<typename T>
typename SizeRangeMap<T>::const_iterator SizeRangeMap<T>::end() const noexcept
{
	return aggregate.end();
}

template<typename T>
T* SizeRangeMap<T>::at(std::size_t size) noexcept
{
	T* result(nullptr);
	iterator it{aggregate.find(SizeRange{size, size})};
	if (it != aggregate.end()) {
		result = &it->second;
	}
	return result;
}

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_SIZE_RANGE_MAP_TCC_INCLUDED
