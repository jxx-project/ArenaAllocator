//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_SizeRangeMap_tcc_INCLUDED
#define ArenaAllocator_SizeRangeMap_tcc_INCLUDED

#include "ArenaAllocator/FreeList.h"
#include "ArenaAllocator/PoolStatistics.h"
#include "ArenaAllocator/SizeRangeMap.h"

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

template<typename T>
std::size_t SizeRangeMap<T>::size() const noexcept
{
	return aggregate.size();
}

} // namespace ArenaAllocator

#endif // ArenaAllocator_SizeRangeMap_tcc_INCLUDED
