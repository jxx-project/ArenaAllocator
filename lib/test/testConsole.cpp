//
// Copyright (C) 2021 Dr. Michael Steffens
//
// SPDX-License-Identifier:     BSL-1.0
//


#include "gtest/gtest.h"

#include "ArenaAllocator/Console.h"
#include "Mock/NullAllocatorFactory.h"
#include "Mock/NullLogger.h"
#include <chrono>

using namespace std::chrono_literals;

TEST(Console, abort)
{
	ASSERT_DEATH(ArenaAllocator::Console::abort([] { return ArenaAllocator::Message("testing {}", "abort"); }), "testing abort");
}

TEST(Console, exit)
{
	ASSERT_DEATH(ArenaAllocator::Console::exit([] { return ArenaAllocator::Message("testing {}", "exit"); }), "testing exit");
}

TEST(Console, logPlain)
{
	ArenaAllocator::Console log;
	testing::internal::CaptureStderr();
	log([] { return ArenaAllocator::Message("testing {}", "plain"); });
	std::string capturedStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing plain", capturedStderr));
}

TEST(Console, logTimerMessage)
{
	ArenaAllocator::Console log;
	testing::internal::CaptureStderr();
	log(4711ns, ArenaAllocator::OperationType::FREE, [] { return ArenaAllocator::Message("testing {}", "timer message"); });
	std::string capturedStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "4711ns", capturedStderr));
	EXPECT_TRUE(testing::IsSubstring("", "", "testing timer message", capturedStderr));
}

TEST(Console, logLevelNone)
{
	ArenaAllocator::Console log;
	log.setLevel(ArenaAllocator::LogLevel::NONE);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Message("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Message("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Message("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Message("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(Console, logLevelError)
{
	ArenaAllocator::Console log;
	log.setLevel(ArenaAllocator::LogLevel::ERROR);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Message("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Message("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Message("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Message("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(Console, logLevelInfo)
{
	ArenaAllocator::Console log;
	log.setLevel(ArenaAllocator::LogLevel::INFO);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Message("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Message("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Message("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Message("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(Console, logLevelTrace)
{
	ArenaAllocator::Console log;
	log.setLevel(ArenaAllocator::LogLevel::TRACE);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Message("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Message("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Message("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Message("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_FALSE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_FALSE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

TEST(Console, logLevelDebug)
{
	ArenaAllocator::Console log;
	log.setLevel(ArenaAllocator::LogLevel::DEBUG);

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::ERROR, [] { return ArenaAllocator::Message("testing {}", "error message"); });
	std::string capturedErrorStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing error message", capturedErrorStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::ERROR));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::INFO, [] { return ArenaAllocator::Message("testing {}", "info message"); });
	std::string capturedInfoStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing info message", capturedInfoStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::INFO));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::TRACE, [] { return ArenaAllocator::Message("testing {}", "trace message"); });
	std::string capturedTraceStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing trace message", capturedTraceStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::TRACE));

	testing::internal::CaptureStderr();
	log(ArenaAllocator::LogLevel::DEBUG, [] { return ArenaAllocator::Message("testing {}", "debug message"); });
	std::string capturedDebugStderr{testing::internal::GetCapturedStderr()};
	EXPECT_TRUE(testing::IsSubstring("", "", "testing debug message", capturedDebugStderr));
	EXPECT_TRUE(log.isLevel(ArenaAllocator::LogLevel::DEBUG));
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
