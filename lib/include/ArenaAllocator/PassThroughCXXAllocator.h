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
	PassThroughCXXAllocator& operator=(const PassThroughCXXAllocator&) noexcept = default;
	~PassThroughCXXAllocator() noexcept = default;

	template<typename U>
	explicit PassThroughCXXAllocator(const PassThroughCXXAllocator<U>& other) noexcept
	{
	}

	T* allocate(std::size_t size)
	{
		return static_cast<T*>(__libc_malloc(size * sizeof(T)));
	}

	void deallocate(T* ptr, std::size_t)
	{
		__libc_free(ptr);
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
