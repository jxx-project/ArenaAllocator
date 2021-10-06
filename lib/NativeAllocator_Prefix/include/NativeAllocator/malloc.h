//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NativeAllocator_malloc_h_INCLUDED
#define NativeAllocator_malloc_h_INCLUDED

#include <cstddef>

extern "C" void* _malloc(std::size_t size);
extern "C" void _free(void* ptr);
extern "C" void* _calloc(std::size_t nmemb, std::size_t size);
extern "C" void* _realloc(void* ptr, std::size_t size);

namespace NativeAllocator {

inline void* malloc(std::size_t size) noexcept
{
	return _malloc(size);
}

inline void free(void* ptr) noexcept
{
	_free(ptr);
}

inline void* calloc(std::size_t nmemb, std::size_t size) noexcept
{
	return _calloc(nmemb, size);
}

inline void* realloc(void* ptr, std::size_t size) noexcept
{
	return _realloc(ptr, size);
}

} // namespace NativeAllocator

#endif // NativeAllocator_malloc_h_INCLUDED
