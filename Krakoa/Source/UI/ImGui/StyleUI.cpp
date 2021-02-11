﻿#include "Precompile.hpp"
#include "StyleUI.hpp"

#include "../../Graphics/Helpers/Colour.hpp"
#include "../../Graphics/Fonts/Font.hpp"

#include <imgui.h>

namespace krakoa::ui
{
	void PushStyleVar(StyleVarFlags index, float val)
	{
		ImGui::PushStyleVar(index, val);
	}

	void PushStyleVar(StyleVarFlags index, kmaths::Vector2f val)
	{
		ImGui::PushStyleVar(index, { val.x, val.y });
	}

	void PopStyleVar(size_t count)
	{
		ImGui::PopStyleVar(static_cast<int>(count));
	}

	void StyleUI(StyleVarFlags index, kmaths::Vector2f val, const UICallBack& callback)
	{
		PushStyleVar(index, val);
		callback();
		PopStyleVar();
	}

	void PushStyleColour(StyleColourFlags index, const graphics::Colour& colour)
	{
		ImGui::PushStyleColor(index.ToUnderlying(), colour.ToU32());
	}

	void PopStyleColour(size_t count)
	{
		ImGui::PopStyleVar(static_cast<int>(count));
	}

	void StyleUI(StyleColourFlags index, const graphics::Colour& colour, const UICallBack& callback)
	{
		PushStyleColour(index, colour);
		callback();
		PopStyleColour();
	}

	void PushFont(const graphics::Font& font)
	{
		ImGui::PushFont(font);
	}

	void PopFont()
	{
		ImGui::PopFont();
	}
}
