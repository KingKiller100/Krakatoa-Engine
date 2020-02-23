#include <pch.hpp>
#include <Testing/Utility Tests/Logging_Test.hpp>

#include <Utility/Logging/kLogging_Class.hpp>
#include <Utility/File System/kFileSystem.hpp>

namespace kTest::utility
{		
	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
	{
		using namespace klib::kLogs;

		auto testLogger = std::make_unique<Logging>();

		testLogger->InitializeLogging(LLevel::NORM);

		const auto dir = klib::kFileSystem::GetCurrentWorkingDirectory() + "Test Results\\Log Test Dir\\";
		testLogger->ChangeOutputDirectory(dir);

		const auto filename = "DiffFileName";
		testLogger->ChangeFilename(filename);
		
		testLogger->AddEntryBanner("Welcome to the Log Tests!", "Tests");
		auto last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("Welcome to the Log Tests!") != std::string::npos);
		
		testLogger->AddEntryBanner("BANNER!", "TEST");
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("BANNER!") != std::string::npos);

		testLogger->AddEntry("DEBUG!", LLevel::DBUG);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("DEBUG!") != std::string::npos);

		testLogger->AddEntry("NORMAL!");
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("NORMAL!") != std::string::npos);

		testLogger->AddEntry("INFORMATIVE!", LLevel::INFO);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("INFORMATIVE!") != std::string::npos);

		testLogger->AddEntry("Done", LLevel::WARN);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("Done") != std::string::npos);

		testLogger->AddEntry("ERROR!", LLevel::ERRR, __FILE__, __LINE__);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("ERROR!") != std::string::npos);

		testLogger->ErasePreviousEntries(1);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("ERROR!") == std::string::npos);
				
		testLogger->AddEntry("ERROR AGAIN!", LLevel::ERRR, __FILE__, __LINE__);
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("ERROR AGAIN!") != std::string::npos);
				
		testLogger->OutputToFatalFile("FATAL!", __FILE__, __LINE__);

		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("EMPTY") != std::string::npos);
		
		testLogger->Output();
		testLogger->FinalOutput();

		const auto fullFilePathToDelete = dir + filename + ".log";
		VERIFY(klib::kFileSystem::CheckFileExists(fullFilePathToDelete.c_str()) == true);
		
		testLogger->AddEntry("end");
		last = testLogger->GetLastLoggedEntry();
		VERIFY(last.find("end") == std::string::npos);

		klib::kFileSystem::RemoveFile(fullFilePathToDelete.data());
		klib::kFileSystem::DeleteDirectory(dir.data());
	}
}
