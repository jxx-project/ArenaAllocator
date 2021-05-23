//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NATIVE_CXX_ALLOCATOR_H_INCLUDED
#define NATIVE_CXX_ALLOCATOR_H_INCLUDED

#include <cstdint>

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);

namespace ArenaAllocator {

template<typename T>
class NativeCXXAllocator
{
public:
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	NativeCXXAllocator() noexcept = default;
	NativeCXXAllocator(const NativeCXXAllocator&) noexcept = default;
	template<typename U>
	NativeCXXAllocator(const NativeCXXAllocator<U>& other) noexcept
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
bool operator==(NativeCXXAllocator<T1> const&, NativeCXXAllocator<T2> const&) noexcept
{
	return true;
}

template<typename T1, typename T2>
bool operator!=(NativeCXXAllocator<T1> const&, NativeCXXAllocator<T2> const&) noexcept
{
	return false;
}

} // namespace ArenaAllocator

#endif // NATIVE_CXX_ALLOCATOR_H_INCLUDED
