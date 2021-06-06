//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/ConsoleLogger.h"
#include "Mock/NullAllocatorFactory.h"
#include "Mock/NullLogger.h"

TEST(ConsoleLogger, abort)
{
	ASSERT_DEATH(ArenaAllocator::ConsoleLogger::abort("testing %s", "abort"), "testing abort");
}

TEST(ConsoleLogger, exit)
{
	ASSERT_DEATH(ArenaAllocator::ConsoleLogger::exit("testing %s", "exit"), "testing exit");
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
