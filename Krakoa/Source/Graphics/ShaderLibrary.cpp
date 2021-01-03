﻿#include "Precompile.hpp"
#include "ShaderLibrary.hpp"

#include "Resources/iShader.hpp"

#include "../Logging/CoreLogger.hpp"
#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/String/kStringTricks.hpp>

namespace krakoa::graphics
{
	ShaderLibrary::ShaderLibrary(Token &&)
	{}

	ShaderLibrary::~ShaderLibrary()
		= default;

	USE_RESULT std::weak_ptr<iShader> ShaderLibrary::Get(const std::string & name) const
	{
		KRK_PROFILE_FUNCTION();
		KRK_ASSERT(Exists(name), klib::kString::ToString("Trying to retrieve shader that does not currently exist:\n{0}", name));
		return shadersUMap.at(name);
	}

	std::weak_ptr<iShader> ShaderLibrary::Load(const std::string_view& filepath)
	{
		KRK_PROFILE_FUNCTION();
		static auto no_name_shader_count = 0ull;
		const auto& name = klib::kString::ToString("Shader {0}", no_name_shader_count++);
		const auto shader = iShader::Create(name, filepath);
		return Add(name, shader);
	}

	std::weak_ptr<iShader> ShaderLibrary::Load(const std::string& name, const std::string_view& filepath)
	{
		KRK_PROFILE_FUNCTION();
		const auto shader = iShader::Create(name, filepath);
		return Add(name, shader);
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		KRK_PROFILE_FUNCTION();
		return shadersUMap.find(name) != shadersUMap.end();
	}

	bool ShaderLibrary::Delete(const std::string& name)
	{
		KRK_PROFILE_FUNCTION();
		const auto pathName = klib::kString::Replace(name, '/', '\\');
		if (Exists(pathName))
		{
			shadersUMap.erase(pathName);
			return true;
		}
		return false;
	}

	std::shared_ptr<iShader> ShaderLibrary::Add(const std::string& name, iShader* shader)
	{
		KRK_PROFILE_FUNCTION();
		KRK_ASSERT(!Exists(name)
			, klib::kString::ToString("Adding shader that already exists in the library:\n{0}", name));
		const auto shaderS_Ptr = std::shared_ptr<iShader>(shader);
		shadersUMap.insert(std::make_pair(name, shaderS_Ptr));
		return shaderS_Ptr;
	}

}
