#include "pch.hpp"
#include "kAssert.hpp"

#include "../File System/kFileSystem.hpp"
#include "../Format/kFormatToString.hpp"
#include "../Logging/kLogging.hpp"

#if defined(_DEBUG) ||  defined(KLIB_TEST)
namespace klib::kDebug
{
	AssertOnFailedConditionException::AssertOnFailedConditionException(const std::string_view& exp, const std::string_view& msg, const char* file, const std::int16_t line)
		: report(kFormat::ToString("Condition \"%s\" was not met! \n               [DETAILS]: %s.", exp.data(), msg.data()))
	{
		const auto currentDir = kFileSystem::GetExeDirectory();
		
		auto exceptionLog = kLogs::Logging(currentDir, "AssertOnFailCondition", "ASSERT");
		exceptionLog.ToggleConsoleEnabled();
		exceptionLog.OutputToFatalFile({report.data(), file, line, kCalendar::CalendarInfoSource::LOCAL});
	}

	AssertOnFailedConditionException::~AssertOnFailedConditionException() throw()
	{}

	char const* AssertOnFailedConditionException::what() const
	{
		return report.c_str();
	}
}
#endif
