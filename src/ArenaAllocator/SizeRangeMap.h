//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_SIZE_RANGE_MAP_H_INCLUDED
#define ARENA_ALLOCATOR_SIZE_RANGE_MAP_H_INCLUDED

#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cassert>
#include <map>

namespace ArenaAllocator {

template<typename T>
class SizeRangeMap
{
public:
	using DelegateType = std::
		map<SizeRange, T, bool (*)(SizeRange const&, SizeRange const&), PassThroughCXXAllocator<std::pair<const SizeRange, T>>>;
	using value_type = typename DelegateType::value_type;
	using iterator = typename DelegateType::iterator;
	using const_iterator = typename DelegateType::const_iterator;

	iterator begin() noexcept
	{
		return delegate.begin();
	}

	const_iterator begin() const noexcept
	{
		return delegate.begin();
	}

	iterator end() noexcept
	{
		return delegate.end();
	}

	const_iterator end() const noexcept
	{
		return delegate.end();
	}

	SizeRangeMap() noexcept : delegate{rangeBelow}
	{
	}

	T* at(std::size_t size) noexcept
	{
		T* result(nullptr);
		typename DelegateType::iterator it{delegate.find(SizeRange{size, size})};
		if (it != delegate.end()) {
			result = &it->second;
		}
		return result;
	}

	template<typename... Args>
	void emplace(SizeRange const& range, Args&&... args) noexcept
	{
		std::pair<typename DelegateType::iterator, bool> emplaceResult{delegate.emplace(
			std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(std::forward<Args>(args)...))};
		assert(emplaceResult.second);
	}

private:
	static bool rangeBelow(SizeRange const& lhs, SizeRange const& rhs) noexcept
	{
		return lhs.last < rhs.first;
	}

	DelegateType delegate;
};

} // namespace ArenaAllocator
#endif // ARENA_ALLOCATOR_SIZE_RANGE_MAP_H_INCLUDED
