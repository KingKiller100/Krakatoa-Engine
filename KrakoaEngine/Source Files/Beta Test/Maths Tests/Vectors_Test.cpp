#include "Precompile.h"
#include "Vectors_Test.h"

#include "../../Utility/Maths/Vectors/Vector2.h"
#include "../../Utility/Maths/Vectors/Vector3.h"
#include "../../Utility/Maths/Vectors/Vector4.h"

namespace kTest::Maths
{
	using namespace ::util::kMaths;
	
	VectorsTester::VectorsTester()
		: Tester("Vectors 2/3/4 Test")
	{}
	
	VectorsTester::~VectorsTester()
		= default;

	bool VectorsTester::Test()
	{
		VerifyResults(Math_Vector2_Test() == true);
		VerifyResults(Math_Vector3_Test() == true);
		VerifyResults(Math_Vector4_Test() == true);
	}

	bool VectorsTester::Math_Vector2_Test()
	{
		auto v2 = Vector2d(5.999, -10.675);
		
		v2.ToPositives();
		
		if ((v2.X() > 0 || v2.Y() > 0) == false) { this->name = util::kFormatToString::FormatToString("%s Test:  Line %d     Function: %s", this->name, __LINE__, __FUNCSIG__).c_str(); return false; }			

		v2.ReverseVector();
		v2.X();
		v2.Y();
		auto g = v2 *= 8;
		g = v2 /= 8;
		v2.Truncate(30);
		const auto temp = v2.Magnitude();
		const auto temp1 = v2.Perpendicular();
		v2.Distance(temp1);
		const auto temp2 = -v2;
		const auto temp3 = v2.Magnitude();
		auto res = v2 != temp2;
		v2.Zero();

		return true;
	}

	bool VectorsTester::Math_Vector3_Test()
	{
		auto v3 = Vector3f(12.6f, 3.4f, 10.345f);
		const auto tempV3 = Vector3s(static_cast<int>(v3.Z()));

		auto v = CrossProduct(v3, tempV3);

		auto ref = v3 [1];

		// operator tests		
		auto result = tempV3 + v3;
		result = tempV3 * v3;
		result =  tempV3 - v3;
		result =  tempV3 / v3;
		v3 *= tempV3;
		v3 /= tempV3;
		v3 *= 2;
		v3 = tempV3;

		return true;
	}

	bool VectorsTester::Math_Vector4_Test()
	{
		const auto v4 = Vector4s(5, 7, 3, 1);

		auto p = v4.W();

		return true;
	}
}
