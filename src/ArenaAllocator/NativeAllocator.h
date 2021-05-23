//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef NATIVE_ALLOCATOR_H_INCLUDED
#define NATIVE_ALLOCATOR_H_INCLUDED

#include "ArenaAllocator/Allocator.h"
#include "ArenaAllocator/Logger.h"
#include <cstdint>

namespace ArenaAllocator {

class NativeAllocator : public Allocator
{
public:
	explicit NativeAllocator(Logger const& logger) noexcept;
	NativeAllocator(NativeAllocator const&) = delete;
	NativeAllocator& operator=(NativeAllocator const&) = delete;
	virtual ~NativeAllocator() noexcept;
	virtual void* malloc(std::size_t size) noexcept override;
	virtual void free(void* ptr) noexcept override;
	virtual void* calloc(std::size_t nmemb, std::size_t size) noexcept override;
	virtual void* realloc(void* ptr, std::size_t size) noexcept override;
	virtual void* reallocarray(void* ptr, std::size_t nmemb, std::size_t size) noexcept override;

private:
	Logger const& logger;
};

} // namespace ArenaAllocator

#endif // NATIVE_ALLOCATOR_H_INCLUDED
