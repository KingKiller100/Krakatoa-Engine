#pragma once

#include "../../HelperMacros.hpp"

#include "../../Utility/Profiler/kProfiler.hpp"

#include <map>
#include <string>
#include <vector>

#ifdef TESTING_ENABLED
namespace kTest::performance
{
	struct AverageTime
	{
		long long time = 0;
		int count = 0;
	};

	class PerformanceTestBase
	{
	public:
		PerformanceTestBase(const char* name) noexcept;
		~PerformanceTestBase();

		void Run() noexcept;

		const std::string& GetName() const;

	protected:
		virtual void Test() = 0;

		void Add(PerformanceTestBase* test);
		void SetUpParticipants(const std::vector<std::string_view>& participants) noexcept;

	private:
		void Output() noexcept;
		void AddSubTest(const std::string& subTestName, const std::string_view& participant) noexcept;
		void SendResult(const std::string_view& subTestName, const std::string_view& result, const float percentageDifference, const std::pair<std::string, long long>& fastestParticipant) noexcept;

	protected:
		std::string name;
		std::vector<klib::kProfiler::ProfilerResult> profilerResults;
		std::map<const std::string, std::map<std::string, AverageTime>> results;
	};
}

#define START_TEST(profilee) auto profiler##__LINE__ = klib::kProfiler::Profiler(profilee, [&](const klib::kProfiler::ProfilerResult& res)\
		{\
			profilerResults.push_back(res);\
		});\

#endif
