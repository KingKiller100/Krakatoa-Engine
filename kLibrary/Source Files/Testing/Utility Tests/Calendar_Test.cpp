#include "pch.hpp"
#include "Calendar_Test.hpp"

#include "../../Utility/Calendar/kCalendar.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	
	CalendarTester::CalendarTester()
		: Tester("Calendar Test")
	{
	}

	CalendarTester::~CalendarTester()
		= default;

	void CalendarTester::Test()
	{
		using namespace klib;
		
		const auto& localDateTimeTest = kCalendar::GetLocalDateAndTime();
		const auto& systemDateTimeTest = kCalendar::GetSystemDateAndTime();
		auto time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::hour);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::min);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::sec);
		time = kCalendar::GetComponentOfTime(kCalendar::TimeComponent::millisec);
		auto calendarTest = kCalendar::GetDateInNumericalFormat();
		calendarTest = kCalendar::GetDateInNumericalFormat(false);
		calendarTest = kCalendar::GetDateInTextFormat();
		calendarTest = kCalendar::GetTimeText();
		calendarTest = kCalendar::GetDayOfTheWeek(3);
		calendarTest = kCalendar::GetMonth(3);	
	}
}
#endif

