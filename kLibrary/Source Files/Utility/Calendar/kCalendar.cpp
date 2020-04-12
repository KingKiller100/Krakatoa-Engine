#include "pch.hpp"
#include "kCalendar.hpp"

#include "../Format/kFormatToString.hpp"
#include "../Format/kStringManipulation.hpp"

#include <array>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#	include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

namespace klib::kCalendar
{
	using namespace kFormat;

	static const auto localStartTime = GetLocalDateAndTime();
	static const auto systemStartTime = GetSystemDateAndTime();

	_SYSTEMTIME& GetLocalDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_Local_DateTime;
		GetLocalTime(&kCalendar_Local_DateTime);
		return kCalendar_Local_DateTime;
	}

	_SYSTEMTIME& GetSystemDateAndTime()  noexcept
	{
		static _SYSTEMTIME kCalendar_System_DateTime;
		GetSystemTime(&kCalendar_System_DateTime);
		return kCalendar_System_DateTime;
	}

	unsigned short GetComponentOfTime(const TimeComponent timeComponent)  noexcept
	{
		const auto now = GetLocalDateAndTime();

		switch (timeComponent)
		{
		case TimeComponent::hour:		return now.wHour;
		case TimeComponent::min:		return now.wMinute;
		case TimeComponent::sec:		return now.wSecond;
		case TimeComponent::millisec:	return now.wMilliseconds;
		default: return MAXWORD;
		}
	}

	// ASCII

	std::string GetTimeText()  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		return ToString("%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::string GetDateInTextFormat(const bool fullDayname)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		std::string day = GetDayOfTheWeek(dateTime.wDayOfWeek).data();
		day = fullDayname ? day : day.substr(0, 3);
		const auto dateStr = ToString("%s %d %s %04d", day.data(), dateTime.wDay, GetMonth(dateTime.wMonth).data(), dateTime.wYear);
		return dateStr;
	}

	constexpr std::string_view GetMonth(const unsigned short month)  noexcept
	{
		constexpr std::array<const char*, 12> kCalendar_MonthsArray =
		{ "January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October",
			"November", "December" };

		if (month <= kCalendar_MonthsArray.size() || month > 0)
			return kCalendar_MonthsArray[month - 1];

		return "Value entered does not index to a month of the year";
	}

	constexpr std::string_view GetDayOfTheWeek(const unsigned short day)  noexcept
	{
		constexpr std::array<const char*, 7> kCalendar_DaysOfTheWeek =
		{ "Sunday", "Monday", "Tuesday", "Wednesday",
		"Thursday", "Friday", "Saturday" };

		if (day < kCalendar_DaysOfTheWeek.size())
			return kCalendar_DaysOfTheWeek[day];

		return "Value entered does not index to a day of the week";
	}

	std::string GetDateInNumericalFormat(const bool slash)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto dateFormat = slash ? "%02d/%02d/%02d" : "%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	std::string CreateTime(unsigned short hour, unsigned short minute, unsigned short second)  noexcept
	{
		const auto time = ToString("%02d:%02d:%02d", hour, minute, second);
		return time;
	}

	std::string GetLocalStartTimeStr() noexcept
	{
		static const auto localTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::string GetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString("%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::string GetLocalStartDateStr(const bool slash) noexcept
	{
		static std::string localDate;
		if (localDate.empty())
		{
			localDate = GetDateInNumericalFormat(slash);
			return localDate;
		}

		if (slash)
		{
			if (localDate.find('/') == std::string::npos)
				localDate = String::Replace(std::string_view(localDate.data()), '-', '/');
		}
		else
		{
			if (localDate.find('-') == std::string::npos)
				localDate = String::Replace(std::string_view(localDate.data()), '/', '-');
		}

		return localDate;
	}

	// WIDE MULTI-BYTE CHAR

	std::wstring wGetTimeText()  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		return ToString(L"%02d:%02d:%02d:%03d", dateTime.wHour, dateTime.wMinute, dateTime.wSecond, dateTime.wMilliseconds);
	}

	std::wstring wGetDateInTextFormat(const bool fullDayname)  noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		std::wstring day = wGetDayOfTheWeek(dateTime.wDayOfWeek).data();
		day = fullDayname ? day : day.substr(0, 3);
		return ToString(L"%s %d %s %04d", day.data(), dateTime.wDay, GetMonth(dateTime.wMonth), dateTime.wYear);
	}

	constexpr std::wstring_view wGetMonth(const unsigned short month) noexcept
	{
		constexpr  std::array<const wchar_t*, 12> kCalendar_MonthsArray =
		{ L"January", L"February", L"March", L"April", L"May",
		L"June", L"July", L"August", L"September", L"October",
			L"November", L"December" };

		if (month < kCalendar_MonthsArray.size())
			return kCalendar_MonthsArray[month - 1];

		return L"Value entered does not index to a month of the year";
	}

	constexpr std::wstring_view  wGetDayOfTheWeek(const unsigned short day) noexcept
	{
		constexpr  std::array<const wchar_t*, 7> kCalendar_DaysOfTheWeek =
		{ L"Sunday", L"Monday", L"Tuesday", L"Wednesday",
		L"Thursday", L"Friday", L"Saturday" };

		if (day < kCalendar_DaysOfTheWeek.size())
			return kCalendar_DaysOfTheWeek[day];

		return L"Value entered does not index to a day of the week";
	}

	std::wstring wGetDateInNumericalFormat(const bool slash) noexcept
	{
		const auto dateTime = GetLocalDateAndTime();
		const auto dateFormat = slash ? L"%02d/%02d/%02d" : L"%02d-%02d-%04d";
		return ToString(dateFormat, dateTime.wDay, dateTime.wMonth, dateTime.wYear);
	}

	std::wstring wCreateTime(unsigned short hour, unsigned short minute, unsigned short second) noexcept
	{
		const auto time = ToString(L"%02d:%02d:%02d", hour, minute, second);
		return time;
	}

	std::wstring wGetLocalStartTimeStr() noexcept
	{
		static const auto localTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				localStartTime.wHour,
				localStartTime.wMinute,
				localStartTime.wSecond,
				localStartTime.wMilliseconds);
		return localTimeStr;
	}

	std::wstring wGetSystemStartTimeStr() noexcept
	{
		static const auto systemTimeStr =
			ToString(L"%02d:%02d:%02d:%03d",
				systemStartTime.wHour,
				systemStartTime.wMinute,
				systemStartTime.wSecond,
				systemStartTime.wMilliseconds);
		return systemTimeStr;
	}

	std::wstring wGetLocalStartDateStr(const bool slash) noexcept
	{
		static std::wstring localDate;
		if (localDate.empty())
		{
			localDate = wGetDateInNumericalFormat(slash);
			return localDate;
		}

		if (slash)
		{
			if (localDate.find('/') == std::wstring::npos)
				localDate = String::Replace<wchar_t>(localDate.data(), L'-', L'/');
		}
		else
		{
			if (localDate.find('-') == std::wstring::npos)
				localDate = String::Replace<wchar_t>(localDate.data(), L'/', L'-');
		}

		return localDate;
	}
}


