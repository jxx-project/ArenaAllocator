//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "Mock/NullAllocatorFactory.h"
#include "Mock/NullLoggerFactory.h"

TEST(EnvironmentConfiguration, EnvironmentVariableNotFound)
{
	Mock::NullAllocatorFactory allocatorFactory{};
	ArenaAllocator::Allocator* allocator{nullptr};
	Mock::NullLoggerFactory loggerFactory{};
	ArenaAllocator::Logger* logger{nullptr};

	ASSERT_DEATH(
		ArenaAllocator::EnvironmentConfiguration(nullptr, allocatorFactory, allocator, loggerFactory, logger),
		"ArenaAllocator exit");
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
