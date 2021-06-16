//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/ConsoleLogger.h"
#include "ArenaAllocator/Format.h"
#include "Mock/NullAllocatorFactory.h"
#include "Mock/NullLogger.h"
#include <chrono>
#include <iostream> //DEBUG

using namespace std::chrono_literals;

TEST(ConsoleLogger, abort)
{
	ASSERT_DEATH(
		ArenaAllocator::ConsoleLogger::abort([] { return ArenaAllocator::Format("testing {}", "abort"); }), "testing abort");
}

TEST(ConsoleLogger, exit)
{
	ASSERT_DEATH(ArenaAllocator::ConsoleLogger::exit([] { return ArenaAllocator::Format("testing {}", "exit"); }), "testing exit");
}

TEST(ConsoleLogger, logPlain)
{
	ArenaAllocator::ConsoleLogger log;
	testing::internal::CaptureStderr();
	log([] { return ArenaAllocator::Format("testing {}", "plain"); });
	std::string capturedStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing plain", capturedStderr));
}

TEST(ConsoleLogger, logTimerMessage)
{
	ArenaAllocator::ConsoleLogger log;
	testing::internal::CaptureStderr();
	log(4711ns, ArenaAllocator::OperationType::FREE, [] { return ArenaAllocator::Format("testing {}", "timer message"); });
	std::string capturedStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "4711ns", capturedStderr));
	EXPECT_TRUE(testing::IsSubstring("", "", "testing timer message", capturedStderr));
}

TEST(ConsoleLogger, logLevelNone)
{
	ArenaAllocator::ConsoleLogger log;
	log.setLevel(ArenaAllocator::LogLevel::NONE);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Format("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Format("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Format("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Format("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(ConsoleLogger, logLevelError)
{
	ArenaAllocator::ConsoleLogger log;
	log.setLevel(ArenaAllocator::LogLevel::ERROR);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, "testing %s", "error message");
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, "testing %s", "info message");
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, "testing %s", "trace message");
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, "testing %s", "debug message");
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(ConsoleLogger, logLevelInfo)
{
	ArenaAllocator::ConsoleLogger log;
	log.setLevel(ArenaAllocator::LogLevel::INFO);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, "testing %s", "error message");
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, "testing %s", "info message");
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, "testing %s", "trace message");
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, "testing %s", "debug message");
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(ConsoleLogger, logLevelTrace)
{
	ArenaAllocator::ConsoleLogger log;
	log.setLevel(ArenaAllocator::LogLevel::TRACE);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, "testing %s", "error message");
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, "testing %s", "info message");
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, "testing %s", "trace message");
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, "testing %s", "debug message");
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(ConsoleLogger, logLevelDebug)
{
	ArenaAllocator::ConsoleLogger log;
	log.setLevel(ArenaAllocator::LogLevel::DEBUG);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, "testing %s", "error message");
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, "testing %s", "info message");
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, "testing %s", "trace message");
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, "testing %s", "debug message");
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
