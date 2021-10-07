//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NativeAllocator_malloc_h_INCLUDED
#define NativeAllocator_malloc_h_INCLUDED

#include <cstddef>

extern "C" void* __malloc(std::size_t size);
extern "C" void __free(void* ptr);
extern "C" void* __calloc(std::size_t nmemb, std::size_t size);
extern "C" void* __realloc(void* ptr, std::size_t size);

namespace NativeAllocator {

inline void* malloc(std::size_t size) noexcept
{
	return __malloc(size);
}

inline void free(void* ptr) noexcept
{
	__free(ptr);
}

inline void* calloc(std::size_t nmemb, std::size_t size) noexcept
{
	return __calloc(nmemb, size);
}

inline void* realloc(void* ptr, std::size_t size) noexcept
{
	return __realloc(ptr, size);
}

} // namespace NativeAllocator

#endif // NativeAllocator_malloc_h_INCLUDED
