﻿#pragma once

#include <Utility/Stopwatch/kStopwatch.hpp>

namespace krakoa::time
{
	class TimeStep
	{
	public:
		TimeStep(const float targetMilliseconds = 1.f) noexcept;

		USE_RESULT float GetLifeTime() const noexcept;
		USE_RESULT float GetDeltaTime() const noexcept;

		void SetSpeedMultiplier(float speed) noexcept;
		
	private:
		USE_RESULT float CalculateLifeTime() const noexcept;

	private:
		float targetIncrement;
		float speedMultiplier;
		bool isFixedIncrement;

		klib::kStopwatch::AccurateStopwatch stopwatch;
	};
}

