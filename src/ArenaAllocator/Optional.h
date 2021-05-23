//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#ifndef ARENA_ALLOCATOR_OPTIONAL_H_INCLUDED
#define ARENA_ALLOCATOR_OPTIONAL_H_INCLUDED

#include <utility>

namespace ArenaAllocator {

template<typename T>
class Optional
{
public:
	Optional() noexcept : hasVal{false}
	{
	}

	Optional(Optional const& other) noexcept : hasVal{other.hasVal}
	{
		if (hasVal) {
			new (&val) T{other.val};
		}
	}

	Optional& operator=(Optional const&) = delete;

	~Optional() noexcept
	{
		if (hasVal) {
			val.~T();
		}
	}

	template<typename... Args>
	void emplace(Args... args) noexcept
	{
		new (&val) T{std::forward<Args>(args)...};
		hasVal = true;
	}

	bool hasValue() const noexcept
	{
		return hasVal;
	}

	T const& value() const noexcept
	{
		return val;
	}

private:
	bool hasVal;
	union
	{
		T val;
	};
};

} // namespace ArenaAllocator

#endif // ARENA_ALLOCATOR_OPTIONAL_H_INCLUDED
