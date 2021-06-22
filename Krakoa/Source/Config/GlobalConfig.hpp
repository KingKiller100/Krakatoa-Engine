﻿#pragma once

#include "ConfigValueMap.hpp"

#include "../Patterns/ManagerBase.hpp"
#include "../Core/PointerTypes.hpp"

#include <Utility/String/kStringTricks.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::configurations
{
	class GlobalConfig : public patterns::ManagerBase<GlobalConfig>
	{
		using ConfigMap = std::unordered_map<std::string, Solo_Ptr<ConfigValueMap>>;

	public:
		GlobalConfig(Token);
		~GlobalConfig() noexcept override;

		template<typename T>
		T Get(const std::string& context, const std::string& key) const
		{
			const auto& valueMap = GetValueMap(klib::ToLower(context));
			const auto& value = valueMap.ReadAs<T>(key);
			return value;
		}

		template<typename T>
		T TryGet(const std::string& context, const std::string& key, T defaultValue) const
		{
			try
			{
				return Get<T>(context, key);
			}
			catch (...)
			{
				return defaultValue;
			}
		}

		const ConfigValueMap& GetValueMap(const std::string& context) const;

		template<typename T>
		void Set(const std::string& context, const std::string& key, const T& value, const klib::MutSourceInfo& source)
		{
			const auto valueStr = util::Fmt("{0}", value);
			auto& valueMap = configMap[SanitizeKey(context)];
			if (!valueMap)
			{
				valueMap = Make_Solo<ConfigValueMap>("");
			}
			valueMap->Set(SanitizeKey(key), valueStr, source);
			KRK_LOG("Config", util::Fmt("Setting configuration: {0}: [{1}, {2}]", context, key, valueStr));
			KRK_LOG("Config", util::Fmt("Source: {0}", source));
		}

		void AddRemap(const std::string& redirectKey, const std::string& key);
	
	private:
		void Initialize();
		USE_RESULT std::string ResolveKey(const std::string& key) const;
		USE_RESULT std::string SanitizeKey(const std::string& key) const;
	
	private:
		ConfigMap configMap;
		std::map<std::string, std::string> remapKeys;
	};

	template<typename T>
	T GetConfiguration(const std::string& context, const std::string& key)
	{
		const auto& gConf = GlobalConfig::Reference();
		return gConf.Get<T>(context, key);
	}

	template<typename T>
	T GetConfiguration(const std::string& context, const std::string& key, T defaultValue)
	{
		const auto& gConf = GlobalConfig::Reference();
		return gConf.TryGet<T>(context, key, defaultValue);
	}
}