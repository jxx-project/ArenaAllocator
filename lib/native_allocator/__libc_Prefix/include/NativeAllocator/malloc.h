//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NativeAllocator_malloc_h_INCLUDED
#define NativeAllocator_malloc_h_INCLUDED

#include <cstddef>

extern "C" void* __libc_malloc(std::size_t size);
extern "C" void __libc_free(void* ptr);
extern "C" void* __libc_calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __libc_realloc(void* ptr, std::size_t size);

namespace NativeAllocator {

inline void* malloc(std::size_t size) noexcept
{
	return __libc_malloc(size);
}

inline void free(void* ptr) noexcept
{
	__libc_free(ptr);
}

inline void* calloc(std::size_t nmemb, std::size_t size) noexcept
{
	return __libc_calloc(nmemb, size);
}

inline void* realloc(void* ptr, std::size_t size) noexcept
{
	return __libc_realloc(ptr, size);
}

} // namespace NativeAllocator

#endif // NativeAllocator_malloc_h_INCLUDED
