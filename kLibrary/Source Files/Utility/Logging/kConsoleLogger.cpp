﻿#include <pch.hpp>
#include "kConsoleLogger.hpp"
#include "kLogLevel.hpp"


#include <consoleapi2.h>
#include <processenv.h>
#include <winbase.h>

#include "../Calendar/kCalendar.hpp"
#include "../Format/kFormatToString.hpp"

#include <unordered_map>


namespace klib
{
	using namespace kFormat;
	
	namespace kLogs
	{
		ConsoleLogger::ConsoleLogger(const std::string& newName)
			: active(false)
			, name(newName)
			, currentConsoleColour(ConsoleColour::WHITE)
		{}

		ConsoleLogger::~ConsoleLogger() noexcept
			= default;

		void ConsoleLogger::UpdateConsoleColour(const LogLevel lvl)
		{

			switch (lvl.ToEnum())
			{
			case LogLevel::DBUG: 
				currentConsoleColour = ConsoleColour::AQUA_BLUE;
				break;
			case LogLevel::NORM:
				currentConsoleColour = ConsoleColour::LIGHT_GREY;
				break;
			case LogLevel::INFO: 
				currentConsoleColour = ConsoleColour::LIGHT_GREEN;
				break;
			case LogLevel::WARN: 
				currentConsoleColour = ConsoleColour::YELLOW;
					break;
			case LogLevel::ERRR: 
				currentConsoleColour = ConsoleColour::SCARLET_RED;
				break;
			case LogLevel::FATL: 
				currentConsoleColour = ConsoleColour::RED_BG_WHITE_TEXT;
				break;
			default: 
				throw std::runtime_error("Unknown log level");
			}
		}

		std::string_view ConsoleLogger::GetName() const
		{
			return name;
		}

		void ConsoleLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		void ConsoleLogger::OutputInitialized(const std::string_view& openingMsg)
		{
			const std::string msg(openingMsg);
			const std::string padding = "************************************************************************\n";
			auto format = padding + "     {0} activated: ";
			format += openingMsg;
			format += "\n    " + GetDateInTextFormat(Date::DateTextLength::SHORT) + "    " + GetTimeText();
			format += "\n" + padding + "\n";
			const auto startLog = ToString(format, name);
			OutputToConsole(startLog);
		}

		void ConsoleLogger::AddEntry(const LogEntry& entry, const LogLevel& lvl)
		{
			if (!active)
				return;

			auto logLine = ToString("[%s] [%s] [%d]:  %s",
				entry.time.ToString().data(),
				name.data(),
				lvl.ToUnderlying(),
				entry.msg.data());

			if (lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: %s
               [LINE]: %d)",
					entry.file,
					entry.line)
				);
			}

			logLine.push_back('\n');
			
			UpdateConsoleColour(lvl);
			Flush(logLine);
		}

		void ConsoleLogger::AddBanner(const LogEntry& entry, const std::string_view& type
		                              , const std::string_view& frontPadding, const std::string_view& backPadding, const std::uint16_t paddingCount)
		{
			if (!active)
				return;

			std::string front, back;

			for (auto i = 0; i < paddingCount; ++i)
			{
				front.append(frontPadding);
				back.append(backPadding);
			}

			const auto format = "[%s] [%s] [%s]: " + front + " %s" + back;
			auto bannerLine = ToString(format
				, entry.time.ToString().data()
				, name.data()
				, type.data()
				, entry.msg.data()
			);
			
			bannerLine.push_back('\n');

			Flush(bannerLine);
		}

		bool ConsoleLogger::Open()
		{
			active = true;
			return active;
		}

		void ConsoleLogger::Close()
		{
			const auto endLogLine
				= ToString(R"(
************************************************************************
               {0} Console Logger Concluded                            
************************************************************************
)"
,  name
);
			OutputToConsole(endLogLine);
			active = false;
		}

		void ConsoleLogger::Flush(const std::string_view& msg)
		{
			OutputToConsole(msg);
			OutputToDebugString(msg);
		}

		void ConsoleLogger::OutputToDebugString(const std::string_view& msg)
		{
#if defined(KLIB_DEBUG) || defined(_DEBUG)
			OutputDebugStringA(msg.data());
#endif
		}

		void ConsoleLogger::OutputToConsole(const std::string_view& msg)
		{
			static constexpr auto whiteText = CAST(WORD, ConsoleColour::WHITE);

			const auto msgColour = CAST(WORD, currentConsoleColour);
			currentConsoleColour = ConsoleColour::WHITE;

			auto* handle = GetStdHandle(STD_OUTPUT_HANDLE);

			SetConsoleTextAttribute(handle, msgColour);
			std::printf("%s", msg.data());
			SetConsoleTextAttribute(handle, whiteText);
		}
	}
}
