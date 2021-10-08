//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED
#define ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED

#include "ArenaAllocator/BuildConfiguration.h"
#include "NativeAllocator/malloc.h"
#include <cstddef>
#include <sys/mman.h>

namespace ArenaAllocator {

template<typename T>
class PassThroughCXXAllocator
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	PassThroughCXXAllocator() noexcept = default;
	PassThroughCXXAllocator(const PassThroughCXXAllocator&) noexcept = default;
	PassThroughCXXAllocator& operator=(const PassThroughCXXAllocator&) noexcept = default;
	~PassThroughCXXAllocator() noexcept = default;

	template<typename U>
	explicit PassThroughCXXAllocator(const PassThroughCXXAllocator<U>& other) noexcept
	{
	}

	T* allocate(std::size_t size)
	{
		T* result{static_cast<T*>(NativeAllocator::malloc(size * sizeof(T)))};
		if constexpr (BuildConfiguration::useMlock) {
			::mlock(result, size * sizeof(T));
		}
		return result;
	}

	void deallocate(T* ptr, std::size_t)
	{
		NativeAllocator::free(ptr);
	}
};

template<typename T, typename U>
bool operator==(PassThroughCXXAllocator<T> const&, PassThroughCXXAllocator<U> const&) noexcept
{
	return true;
}

template<typename T, typename U>
bool operator!=(PassThroughCXXAllocator<T> const&, PassThroughCXXAllocator<U> const&) noexcept
{
	return false;
}

} // namespace ArenaAllocator

#endif // ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED
