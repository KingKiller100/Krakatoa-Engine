#include "pch.hpp"
#include "FileSystem_Test.hpp"

#include "../../Utility/File System/kFileSystem.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{

	FileSystemTester::FileSystemTester()
		: Tester("File System Test")
	{	}

	FileSystemTester::~FileSystemTester()
		= default;

	void FileSystemTester::Test()
	{
		using namespace klib::kFileSystem;

		const auto exeDir = GetExeDirectory<char>();
		const auto w_exeDir = GetExeDirectory<wchar_t>();
		const auto exeDir16 = GetExeDirectory<char16_t>();
		const auto exeDir32 = GetExeDirectory<char32_t>();
#if __cpp_char8_t
		const auto exeDir8 = GetExeDirectory<char8_t>();
#endif

		const auto compareExeC_W = SwitchStringFormat<wchar_t>(exeDir);
		VERIFY(compareExeC_W == w_exeDir);
		const auto compareExeC_16 = SwitchStringFormat<char16_t>(exeDir);
		VERIFY(compareExeC_16 == exeDir16);
		const auto compareExeC_32 = SwitchStringFormat<char32_t>(exeDir);
		VERIFY(compareExeC_32 == exeDir32);

#if __cpp_char8_t
		const auto compareExeC_8 = SwitchStringFormat<char8_t>(exeDir);
		VERIFY(compareExeC_8 == exeDir8);
#endif


		const auto cwd = GetCurrentWorkingDirectory<char>();
		const auto w_cwd = GetCurrentWorkingDirectory<wchar_t>();
		const auto cwd16 = GetCurrentWorkingDirectory<char16_t>();
		const auto cwd32 = GetCurrentWorkingDirectory<char32_t>();
#if __cpp_char8_t
		const auto cwd8 = GetCurrentWorkingDirectory<char8_t>();
#endif

		const auto compareCwdC_W = SwitchStringFormat<wchar_t>(cwd);
		VERIFY(compareCwdC_W == w_cwd);
		const auto compareCwdC_16 = SwitchStringFormat<char16_t>(cwd);
		VERIFY(compareCwdC_16 == cwd16);
		const auto compareCwdC_32 = SwitchStringFormat<char32_t>(cwd);
		VERIFY(compareCwdC_32 == cwd32);

#if __cpp_char8_t
		const auto compareCwdC_8 = SwitchStringFormat<char8_t>(cwd);
		VERIFY(compareCwdC_8 == cwd8);
#endif

		auto content = std::string();
		OutputToFile((exeDir + "FS_File_Test").c_str(), content.c_str());

		const auto good = CheckFileExists((exeDir + "FS_File_Test").c_str());
		VERIFY(good == true);

		const auto bad = CheckFileExists((exeDir + "NULL").c_str());
		VERIFY(bad == false);

		const auto firstSubDir = (exeDir + "Create Directory Test\\");

		const auto singleCreated = CreateNewDirectory(firstSubDir.c_str());
		VERIFY(singleCreated == true);

		const auto checkDirExist = CheckDirectoryExists(firstSubDir);
		VERIFY(checkDirExist == true);

		const auto checkDirNotExist = CheckDirectoryExists(exeDir + "None-Existing-Directory\\");
		VERIFY(checkDirNotExist == false);

		const auto wExeDir = klib::kString::Convert<wchar_t>(exeDir);

		const auto wCheckDirExist = CheckDirectoryExists<wchar_t>(wExeDir);
		VERIFY(wCheckDirExist == true);

		const auto wCheckDirNotExist = CheckDirectoryExists<wchar_t>(wExeDir + L"None-Existing-Directory\\");
		VERIFY(wCheckDirNotExist == false);

		const auto multipleCreated = CreateNewDirectories((exeDir + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(multipleCreated == true);

		const auto isDirDeleted = DeleteDirectory((exeDir + "Create Directories Test\\Success1\\Success2\\").c_str());
		VERIFY(isDirDeleted == true);

		OutputToFile((exeDir + "Create Directory Test\\Test.txt").c_str(), "Success\n");
		VERIFY(CheckFileExists((exeDir + "Create Directory Test\\Test.txt").c_str()) == true);

		const auto fileData = klib::kFileSystem::ParseFileData((exeDir + "Create Directory Test\\Test.txt").c_str());
		VERIFY(fileData.front().compare("Success") == 0);

		const auto isFileDeleted = RemoveFile((exeDir + "Create Directory Test\\Test.txt").c_str());
		VERIFY(isFileDeleted == true);

		VERIFY(RemoveFile((exeDir + "FS_File_Test").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directory Test\\").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directories Test\\Success1\\").c_str()) == true);
		VERIFY(DeleteDirectory((exeDir + "Create Directories Test\\").c_str()) == true);

		const auto path = exeDir + "Arbitrary Folder Name\\File.txt";
		const auto folders = GetPath(path);
		const auto file = GetFileName(path);
		const auto fileNoExt = GetFileNameWithoutExtension(path);

		VERIFY(folders.compare(exeDir + "Arbitrary Folder Name") == 0);
		VERIFY(file.compare("File.txt") == 0);
		VERIFY(fileNoExt.compare("File") == 0);

		const auto fileTestExt = AppendFileExtension(fileNoExt, ".test");
		VERIFY(fileTestExt.compare("File.test") == 0);
		const auto fileTestExt2 = AppendFileExtension(fileNoExt, "test");
		VERIFY(fileTestExt2.compare("File.test") == 0)
	}
}
#endif
