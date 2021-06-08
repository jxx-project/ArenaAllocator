//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PoolMap_h_INCLUDED
#define ArenaAllocator_PoolMap_h_INCLUDED

#include "ArenaAllocator/Configuration.h"
#include "ArenaAllocator/FreeList.h"
#include "ArenaAllocator/Logger.h"
#include "ArenaAllocator/PassThroughCXXAllocator.h"
#include "ArenaAllocator/SizeRangeMap.h"

namespace ArenaAllocator {

template<typename T>
class PoolMap
{
public:
	PoolMap(Configuration const& configuration, Logger const& log) noexcept;

	T* at(std::size_t chunkSize) noexcept;

	template<typename U = T, typename = typename std::enable_if<std::is_same<U, FreeList>::value>::type>
	std::size_t nChunks() const noexcept
	{
		std::size_t result{0};
		for (typename AggregateType::value_type const& element : aggregate) {
			result += element.second.nChunks();
		}
		return result;
	}

	template<typename F, typename U = T, typename = typename std::enable_if<std::is_same<U, FreeList>::value>::type>
	void forEachChunk(F f) noexcept
	{
		for (typename AggregateType::value_type& element : aggregate) {
			element.second.forEachChunk(f);
		}
	}

	void dump() const noexcept;

private:
	using AggregateType = SizeRangeMap<T>;

	void insert(SizeRange const& range, std::size_t nChunks) noexcept;

	AggregateType aggregate;
	Logger const& log;
};

} // namespace ArenaAllocator

#endif // ArenaAllocator_PoolMap_h_INCLUDED
