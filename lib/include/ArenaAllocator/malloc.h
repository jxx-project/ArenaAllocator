//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ArenaAllocator_malloc_h_INCLUDED
#define ArenaAllocator_malloc_h_INCLUDED

#include <cstddef>

namespace ArenaAllocator {
  
  void* malloc(std::size_t size);
  
  void free(void* ptr);

  void* calloc(std::size_t nmemb, std::size_t size);

  void* realloc(void* ptr, std::size_t size);

} // namespace ArenaAllocator

#endif // ArenaAllocator_malloc_h_INCLUDED
