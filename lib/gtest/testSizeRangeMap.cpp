//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/SizeRangeMap.tcc"

class Sample
{
public:
	Sample(int a, float b, char const* c) : a{a}, b{b}, c{c}
	{
	}

	int a;
	float b;
	char const* c;
};

class SizeRangeMapFixture : public ::testing::Test
{
protected:
	SizeRangeMapFixture()
	{
		testee.emplace({1, 3}, 1, 1.3F, "[1,3]");
		testee.emplace({4, 7}, 4, 4.7F, "[4,7]");
		testee.emplace({10, 11}, 10, 10.11F, "[10,11]");
	}

	ArenaAllocator::SizeRangeMap<Sample> testee;
};

TEST_F(SizeRangeMapFixture, InitializeAndLookupElements)
{
	EXPECT_EQ(3, testee.size());

	Sample* response1{testee.at(1)};
	ASSERT_NE(nullptr, response1);
	EXPECT_EQ(1, response1->a);
	EXPECT_EQ(1.3F, response1->b);
	EXPECT_STREQ("[1,3]", response1->c);

	Sample* response2{testee.at(5)};
	ASSERT_NE(nullptr, response2);
	EXPECT_EQ(4, response2->a);
	EXPECT_EQ(4.7F, response2->b);
	EXPECT_STREQ("[4,7]", response2->c);

	Sample* response3{testee.at(11)};
	ASSERT_NE(nullptr, response3);
	EXPECT_EQ(10, response3->a);
	EXPECT_EQ(10.11F, response3->b);
	EXPECT_STREQ("[10,11]", response3->c);
}

TEST_F(SizeRangeMapFixture, LookupInGap)
{
	Sample* response{testee.at(8)};
	EXPECT_EQ(nullptr, response);
}

TEST_F(SizeRangeMapFixture, Emplace)
{
	EXPECT_TRUE(testee.emplace({12, 17}, 12, 12.17F, "[12,17]"));
	Sample* response{testee.at(15)};
	ASSERT_NE(nullptr, response);
	EXPECT_EQ(12, response->a);
	EXPECT_EQ(12.17F, response->b);
	EXPECT_STREQ("[12,17]", response->c);
	EXPECT_EQ(4, testee.size());
}

TEST_F(SizeRangeMapFixture, EmplaceIntoGap)
{
	EXPECT_TRUE(testee.emplace({8, 9}, 8, 8.9F, "[8,9]"));
	EXPECT_EQ(4, testee.size());

	Sample* response{testee.at(8)};
	ASSERT_NE(nullptr, response);
	EXPECT_EQ(8, response->a);
	EXPECT_EQ(8.9F, response->b);
	EXPECT_STREQ("[8,9]", response->c);
}

TEST_F(SizeRangeMapFixture, EmplaceOverlapBegin)
{
	EXPECT_FALSE(testee.emplace({7, 9}, 7, 7.9F, "[7,9]"));
	EXPECT_EQ(3, testee.size());

	Sample* response{testee.at(7)};
	ASSERT_NE(nullptr, response);
	EXPECT_EQ(4, response->a);
	EXPECT_EQ(4.7F, response->b);
	EXPECT_STREQ("[4,7]", response->c);
}

TEST_F(SizeRangeMapFixture, EmplaceOverlapEnd)
{
	EXPECT_FALSE(testee.emplace({8, 11}, 8, 8.11F, "[8,11]"));
	EXPECT_EQ(3, testee.size());

	Sample* response{testee.at(10)};
	ASSERT_NE(nullptr, response);
	EXPECT_EQ(10, response->a);
	EXPECT_EQ(10.11F, response->b);
	EXPECT_STREQ("[10,11]", response->c);
}

TEST_F(SizeRangeMapFixture, EmplaceInvalidRange)
{
	EXPECT_FALSE(testee.emplace({9, 8}, 9, 9.8F, "[9,8]"));
	EXPECT_EQ(3, testee.size());

	Sample* response{testee.at(8)};
	EXPECT_EQ(nullptr, response);
}

// TEST(SizeRangeMap, Abort)
// {
// 	ASSERT_DEATH(sample(), "abort");
// }

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
