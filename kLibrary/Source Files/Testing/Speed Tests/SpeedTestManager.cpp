﻿#include "pch.hpp"
#include "SpeedTestManager.hpp"

#include "SpeedTestBase.hpp"

#include "Maths/Algorithms_SpeedTest.hpp"

// File System to output test results
#include "../../Utility/File System/kFileSystem.hpp"

#include <filesystem>


#ifdef TESTING_ENABLED
namespace kTest::speed
{
	SpeedTestManager::SpeedTestManager(const Token&)
		: Tester("Speed Run Tests")
	{}

	SpeedTestManager::~SpeedTestManager()
		= default;

	void SpeedTestManager::Initialize()
	{
		using namespace klib;
		directory = kFileSystem::GetExeDirectory<char>() + "Speed Results\\";
		const auto isMade = kFileSystem::CreateNewDirectory(directory.c_str());

		if (!kFileSystem::CheckDirectoryExists(directory))
		{
			throw std::runtime_error("Test Results directory could not be created/found. Please check why!");
		}

		std::filesystem::path startPoint(directory);
		for (auto iter = std::filesystem::directory_iterator(startPoint);
			iter != std::filesystem::directory_iterator();
			++iter)
		{
			const auto dir = *iter;
			const auto isFile = dir.is_regular_file();
			if (isFile)
			{
				const auto currentPath = dir.path().string();
				kFileSystem::RemoveFile(currentPath);
			}
		}

		InitializeMaths();
	}

	void SpeedTestManager::ShutDown()
	{
		ClearAll();
	}

	void SpeedTestManager::Run()
	{
		for (auto& test : tests)
		{
			test->Run();
			OutputResult(test->GetName());
		}
	}

	void SpeedTestManager::Add(SpeedTestBase* test)
	{
		tests.insert(test);
	}

	void SpeedTestManager::ClearAll()
	{
		for (auto& test : tests)
		{
			delete test;
		}
		tests.clear();
	}

	void SpeedTestManager::CollectResult(const std::string_view& result)
	{
		results.append(result);
	}

	void SpeedTestManager::InitializeMaths()
	{
		Add(new maths::AlgorithmsSpeedTest());
	}

	void SpeedTestManager::OutputResult(const std::string& name)
	{
		using namespace klib;
		const auto filename = kFileSystem::AppendFileExtension(name, "txt");
		const auto fullPath = directory + filename;
		kFileSystem::OutputToFile(fullPath, results);
		results.clear();
	}

	void SpeedTestManager::Test()
	{
		Initialize();
		Run();
		ShutDown();
	}

	SpeedTestManager& SpeedTestManager::Get()
	{
		static Token t;
		static SpeedTestManager instance(t);

		return instance;
	}
}
#endif
