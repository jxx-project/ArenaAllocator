//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/EnvironmentConfiguration.h"
#include "Mock/NullAllocatorFactory.h"
#include "Mock/NullLogger.h"

TEST(EnvironmentConfiguration, EnvironmentVariableNotFound)
{
	Mock::NullAllocatorFactory allocatorFactory{};
	ArenaAllocator::Allocator* allocator{nullptr};
	Mock::NullLogger logger{};

	ASSERT_DEATH(ArenaAllocator::EnvironmentConfiguration(nullptr, allocatorFactory, allocator, logger), "ArenaAllocator exit");
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
