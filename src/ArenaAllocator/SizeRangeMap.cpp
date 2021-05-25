//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "ArenaAllocator/SizeRangeMap.h"
#include "ArenaAllocator/Pool.h"

namespace ArenaAllocator {

namespace {

bool rangeBelow(SizeRange const& lhs, SizeRange const& rhs) noexcept
{
	return lhs.last < rhs.first;
}

} // namespace

template<typename T>
SizeRangeMap<T>::SizeRangeMap() noexcept : delegate{rangeBelow}
{
}

template<typename T>
SizeRangeMap<T>::~SizeRangeMap() noexcept
{
}

template<typename T>
typename SizeRangeMap<T>::iterator SizeRangeMap<T>::begin() noexcept
{
	return delegate.begin();
}

template<typename T>
typename SizeRangeMap<T>::const_iterator SizeRangeMap<T>::begin() const noexcept
{
	return delegate.begin();
}

template<typename T>
typename SizeRangeMap<T>::iterator SizeRangeMap<T>::end() noexcept
{
	return delegate.end();
}

template<typename T>
typename SizeRangeMap<T>::const_iterator SizeRangeMap<T>::end() const noexcept
{
	return delegate.end();
}

template<typename T>
T* SizeRangeMap<T>::at(std::size_t size) noexcept
{
	T* result(nullptr);
	typename DelegateType::iterator it{delegate.find(SizeRange{size, size})};
	if (it != delegate.end()) {
		result = &it->second;
	}
	return result;
}

template class SizeRangeMap<Pool>;
template class SizeRangeMap<std::size_t>;

} // namespace ArenaAllocator
