//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NativeMmap_malloc_h_INCLUDED
#define NativeMmap_malloc_h_INCLUDED

#include <cstddef>
#include <unistd.h>

extern "C" void* _mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset);
extern "C" int _munmap(void* addr, std::size_t length);

namespace NativeMmap {

inline void* mmap(void* addr, std::size_t length, int prot, int flags, int fd, off_t offset) noexcept
{
	return _mmap(addr, length, prot, flags, fd, offset);
}

// inline int munmap(void* addr, std::size_t length) noexcept
// {
// 	return _munmap(addr, length);
// }

} // namespace NativeMmap

#endif // NativeMmap_malloc_h_INCLUDED
