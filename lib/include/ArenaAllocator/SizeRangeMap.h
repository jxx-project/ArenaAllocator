//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_SizeRangeMap_h_INCLUDED
#define ArenaAllocator_SizeRangeMap_h_INCLUDED

#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRange.h"
#include <cassert>
#include <map>

namespace ArenaAllocator {

template<typename T>
class SizeRangeMap
{
public:
	using AggregateType = std::
		map<SizeRange, T, bool (*)(SizeRange const&, SizeRange const&), PassThroughCXXAllocator<std::pair<const SizeRange, T>>>;
	using value_type = typename AggregateType::value_type;
	using iterator = typename AggregateType::iterator;
	using const_iterator = typename AggregateType::const_iterator;

	SizeRangeMap() noexcept;
	SizeRangeMap(SizeRangeMap const&) = delete;
	SizeRangeMap& operator=(SizeRangeMap const&) = delete;
	~SizeRangeMap() = default;

	iterator begin() noexcept;
	[[nodiscard]] const_iterator begin() const noexcept;
	iterator end() noexcept;
	[[nodiscard]] const_iterator end() const noexcept;
	T* at(std::size_t size) noexcept;

	template<typename... Args>
	bool emplace(SizeRange const& range, Args&&... args) noexcept
	{
		return range.first <= range.last &&
			   aggregate
				   .emplace(
					   std::piecewise_construct, std::forward_as_tuple(range), std::forward_as_tuple(std::forward<Args>(args)...))
				   .second;
	}

	[[nodiscard]] std::size_t size() const noexcept;

private:
	AggregateType aggregate;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_SizeRangeMap_h_INCLUDED
