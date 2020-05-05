﻿#pragma once
#include "../Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::maths
{
	class AlgorithmsTester : public Tester
	{
	public:
		AlgorithmsTester();
		~AlgorithmsTester() override;

	private:
		void Test() override;

		bool ConstantsTest();
		bool ConversionTest();
		bool SignTest();
		bool AbsTest();
		bool MinMaxTest();
		bool FloorTest();
		bool RealToFractionTest();
		bool BinarySearchTest();
		bool BinarySearchClosestTest();
		bool RoundingTest();
		bool PowerOfTest();
		bool SwapTest();
		bool ToDegreesTest();
		bool ToRadiansTest();
		bool SquareRootTest();
		bool InverseSquareRootTest();
		bool RootTest();
		bool WhatPowerOf10Test();
		bool LogTest();
		bool PowerOfFractionTest();
		bool FloatingPointRemainderTest();
		bool ModulusTest();
	};
}
#endif