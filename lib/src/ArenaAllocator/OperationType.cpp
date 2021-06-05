//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//

#include <map>

#include "ArenaAllocator/OperationType.h"

namespace ArenaAllocator {

char const* to_string(OperationType operationType)
{
	static const std::map<OperationType, char const*> names{
		{OperationType::MALLOC, "malloc"},
		{OperationType::FREE, "free"},
		{OperationType::CALLOC, "calloc"},
		{OperationType::REALLOC, "realloc"},
		{OperationType::REALLOCARRAY, "reallocarray"},
		{OperationType::POSIX_MEMALIGN, "posix_memalign"},
		{OperationType::ALIGNED_ALLOC, "align_alloc"},
		{OperationType::VALLOC, "valloc"},
		{OperationType::MEMALIGN, "memalign"},
		{OperationType::PVALLOC, "pvalloc"}};
	return names.at(operationType);
}

} // namespace ArenaAllocator
