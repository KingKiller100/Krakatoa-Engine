﻿#include "Precompile.hpp"
#include "DebugWindows.hpp"

#include <iostream>

#include <Utility/Enum/kEnum.hpp>
#include <Utility/String/kToString.hpp>
#include <Utility/Debug/Source/kSourceInfoToString.hpp>

#if KRAKOA_OS_WINDOWS

#include <Windows.h>

namespace krakoa::debug::windows
{
	ENUM_CLASS(MessageBoxResponse, long,
		MSG_OKAY = 1,
		MSG_CANCEL,
		MSG_ABORT,
		MSG_RETRY,
		MSG_IGNORE,
		MSG_YES,
		MSG_NO,
		MSG_TRY_AGAIN = 10,
		MSG_CONTINUE
	);

	void RaiseNoticeImpl(const std::string_view& msg, const klib::SourceInfo& sourceInfo)
	{
		const auto caption = klib::ToString("{0}\n Source: {1}", msg, sourceInfo);

		const auto ret = MessageBoxA(nullptr, caption.data(), "ERROR OCCURRED", MB_COMPOSITE);
		const auto response = static_cast<MessageBoxResponse>(ret);
		const auto valueStr = response.ToString();
		std::cout << valueStr;
	}
}

#endif

