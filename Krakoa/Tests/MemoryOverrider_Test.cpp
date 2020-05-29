﻿#include "Precompile.hpp"
#include "MemoryOverrider_Test.hpp"

#include "../Source Files/PointerTypes.hpp"
#include "../Source Files/Patterns/MemoryOverrider.hpp"

#include "../Source Files/Core/Logging/MemoryLogger.hpp"

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	MemoryOverriderTester::MemoryOverriderTester() noexcept
		: Tester("Memory Overrider Test")
	{}

	MemoryOverriderTester::~MemoryOverriderTester()
		= default;

	class TestMemType : public patterns::MemoryOverrider<TestMemType>
	{

	};

	void MemoryOverriderTester::Test()
	{
		MEM_INIT_LOGS();

		for (auto i = 0; i < 5; ++i)
			const auto* const lifeTime = new TestMemType();

		const auto scope = Make_Solo<TestMemType>();
		VERIFY(scope != nullptr);

		auto str = TestMemType::GetHeapStatus();
		
	}
}
#endif