//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//

#include <array>

#include "ArenaAllocator/OperationType.h"

namespace ArenaAllocator {

std::string_view to_string(OperationType operationType)
{
	static const std::array<const std::string_view, unsigned(OperationType::UNKNOWN) + 1> names{
		"malloc", "free", "calloc", "realloc", "mmap", "munmap", "unknown"};
	return names.at(unsigned(operationType));
}

} // namespace ArenaAllocator
