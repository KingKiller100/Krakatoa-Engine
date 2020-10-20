#include "pch.hpp"
#include "kLogging_Class.hpp"

#include "../Format/kFormatToString.hpp"

#include "kFileLogger.hpp"
#include "kLogEntry.hpp"
#include "kConsoleLogger.hpp"

#include "../../Maths/kAlgorithms.hpp"
#include "../../Type Traits/ToImpl.hpp"

#include "../File System/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kFormat;
	using namespace kCalendar;
	using namespace type_trait;

	const char* Logging::kLogs_Empty = "NO ENTRIES! CACHE IS EMPTY";

	Logging::Logging(const std::string& directory, const std::string& filename)
		: minimumLoggingLevel(LogLevel::NORM),
		name("logger"),
		isEnabled(false),
		subSystemLoggingEnabled(false),
		inCacheMode(false),
		constantFlushing(false)
	{
		Initialize(filename, directory);
	}

	Logging::~Logging()
	{
		if (!logEntries.empty())
			FinalOutput();
	}

	void Logging::Initialize(const std::string& directory, const std::string& filename)
	{
		loggers[LoggerType::FILE].reset(new FileLogger(name, directory, filename));
		loggers[LoggerType::SYSTEM].reset(new ConsoleLogger(name));
		
		SetMinimumLoggingLevel(LogLevel::NORM);
		ToggleLoggingEnabled();
	}

	void Logging::Open()
	{
		for (auto& logger : loggers)
		{
			if (!logger.second->Open())
			{
				const auto errMsg = ToString("Unable to open log: {0}", name);
				throw std::runtime_error(errMsg);
			}
		}
	}

	void Logging::OutputInitialized(const std::string_view& startLog)
	{
		if (!isEnabled) { return; }
		
		for (auto& logger : loggers)
		{
			logger.second->OutputInitialized(startLog);
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
		for (const auto& logger : loggers)
		{
			logger.second->SetName(newName);
		}
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleSubSystemEnabled() noexcept
	{
		subSystemLoggingEnabled = !subSystemLoggingEnabled;
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		inCacheMode = enable;
	}

	void Logging::ChangeOutputPath(const std::string_view & dir, const std::string_view & fname)
	{
		ChangeOutputDirectory(dir);
		ChangeFilename(fname);
	}

	void Logging::ChangeOutputDirectory(const std::string_view dir)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		fLogger.SetDirectory(dir);
	}

	void Logging::ChangeFilename(const std::string_view newFileName)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		const auto filename = kFileSystem::AppendFileExtension(newFileName.data(), ".log");
		fLogger.SetFileName(filename);
	}
	
	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(loggers.at(LoggerType::FILE));
		const auto path = fLogger.GetPath();
		return path;
	}

	void Logging::SuspendFileLogging()
	{
		constexpr const auto* const pauseLog = "\n************************************************************************\n";
		Close();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
	}


	void Logging::OutputToFatalFile(const LogEntry& entry)
	{
		AddEntry(entry);
		FinalOutput();
	}

	void Logging::AddEntry(const LogEntry& entry)
	{
		if (!isEnabled || !IsLoggable(entry.lvl)) 
			return;

		logEntries.push_back(entry);
	}

	void Logging::AddBanner(LogEntry& entry, const std::string& desc
	                        , const std::string& frontPadding, const std::string& backPadding, const std::uint16_t paddingCount)
	{
		if (!isEnabled)
			return;

		std::string front, back;

		for (auto i = 0; i < paddingCount; ++i)
		{
			front.append(frontPadding);
			back.append(backPadding);
		}
		
		constexpr  auto format = "[%s]: %s %s %s";
		entry.msg = ToString(format,
			desc.c_str()
			, frontPadding.c_str()
			, entry.msg.c_str()
			, backPadding.c_str()
		);
		
		logEntries.push_back(entry);
	}

	void Logging::FinalOutput()
	{
		Close();
		isEnabled = false;
	}

	void Logging::Flush()
	{
		const auto start = kmaths::Min(
			logEntries.begin()->first
			, bannerEntries.begin()->first
			);

		for (auto i = start; i < logIndex; ++i)
		{
			if (auto entry_iter = logEntries.find(i); entry_iter != logEntries.end())
			{
				for (auto& logger : loggers)
				{
					logger.second->AddEntry(entry_iter->second,);
				}
			}
			else
			{
				const auto& entry = bannerEntries.at(i);
				
				for (auto& logger : loggers)
				{
					logger.second->AddBanner(entry);
				}
			}
		}
	}

	void Logging::Close()
	{
		for (auto& logger : loggers)
		{
			logger.second->Close();
		}
	}

	bool Logging::IsLoggable(const LogLevel lvl) const
	{
		return (lvl > minimumLoggingLevel);
	}

	std::string_view Logging::GetLastCachedEntry()
	{
		if (logEntries.empty())
			return kLogs_Empty;

		if (!inCacheMode)
			return "CHECK LOGGING FILE: " + GetOutputPath();

		const auto& lastlog = logEntries.front();
		
		return lastlog.msg;
	}

	void Logging::ClearCache()
	{
		if (!logEntries.empty())
			logEntries.clear();
	}

	void Logging::EnableConstantFlush(bool enable)
	{
		constantFlushing = enable;
	}
}
