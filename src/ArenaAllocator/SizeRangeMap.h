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

	SizeRangeMap() noexcept;
	SizeRangeMap(SizeRangeMap const&) = delete;
	SizeRangeMap& operator=(SizeRangeMap const&) = delete;
	~SizeRangeMap() noexcept;

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	T* at(std::size_t size) noexcept;

	template<typename... Args>
	bool emplace(SizeRange const& range, Args&&... args) noexcept
	{
		return delegate
			.emplace(std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(std::forward<Args>(args)...))
			.second;
	}

private:
	DelegateType delegate;
};

} // namespace ArenaAllocator
#endif // ARENA_ALLOCATOR_SIZE_RANGE_MAP_H_INCLUDED
