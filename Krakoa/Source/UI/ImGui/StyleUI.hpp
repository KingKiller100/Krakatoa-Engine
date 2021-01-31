﻿#pragma once
#include "../Flags/StyleVarFlags.hpp"
#include "../Flags/StyleColourFlags.hpp"

#include <Maths/Vectors/Vector2.hpp>

#include "../UITypes.hpp"


namespace krakoa
{
	namespace graphics
	{
		class Colour;
	}

	namespace ui
	{
		void PushStyleVar(StyleVarFlags index, kmaths::Vector2f val = kmaths::Vector2f());
		void PopStyleVar(size_t count = 1);
		void StyleUI(StyleVarFlags index, kmaths::Vector2f val, const UICallBack& callback);

		void PushStyleColour(StyleColourFlags index, const graphics::Colour& colour);
		void PopStyleColour(size_t count = 1);
		void StyleUI(StyleColourFlags index, const graphics::Colour& colour, const UICallBack& callback);
	}
}

