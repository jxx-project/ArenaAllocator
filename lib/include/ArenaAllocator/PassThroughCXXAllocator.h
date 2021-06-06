//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED
#define ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED

#include <cstddef>

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);

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

	template<typename U>
	PassThroughCXXAllocator(const PassThroughCXXAllocator<U>& other) noexcept
	{
	}

	T* allocate(std::size_t size)
	{
		return static_cast<T*>(__libc_malloc(size * sizeof(T)));
	}

	void deallocate(T* ptr, std::size_t n)
	{
		__libc_free(ptr);
	}
};

template<typename T1, typename T2>
bool operator==(PassThroughCXXAllocator<T1> const&, PassThroughCXXAllocator<T2> const&) noexcept
{
	return true;
}

template<typename T1, typename T2>
bool operator!=(PassThroughCXXAllocator<T1> const&, PassThroughCXXAllocator<T2> const&) noexcept
{
	return false;
}

} // namespace ArenaAllocator

#endif // ArenaAllocator_PassThroughCXXAllocator_h_INCLUDED
