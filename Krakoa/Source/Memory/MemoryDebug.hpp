﻿#pragma once

#include "../Debug/DebugCore.hpp"
#include "../Core/EngineMacros.hpp"
#include "../Logging/MemoryLogger.hpp"

#include <Utility/Debug/kAssert.hpp>
#include <Utility/Debug/Source/kSourceInfo.hpp>


#if KRK_ENABLE_ASSERT

#	define MEM_ASSERT(cond, msg) kAssertCB(cond, msg, [](const std::string_view& m, const klib::SourceInfo& s)\
{\
	::krakoa::debug::RaiseNotice(m, s);\
})\

#else
#	define MEM_ASSERT(cond, msg) ((void)0)
#endif

#define MEM_FATAL(msg) ::krakoa::debug::RaiseException(msg, SOURCE_INFO(), memory::MemoryLogger::GetLogger())

