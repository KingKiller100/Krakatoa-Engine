#pragma once

#include <Core/Application.hpp>

#include <memory>

extern krakoa::Application* krakoa::CreateApplication();

bool RunTestsOnkLibrary();

int main(int argc, char** argv)
{
	if (RunTestsOnkLibrary())
		return EXIT_SUCCESS;

	auto app = krakoa::CreateApplication();

	app->Initialize();

	unsigned count = 0;
	do {
		app->Run();
	} while (app->IsRunning());

	return EXIT_SUCCESS;
}




#ifdef KRAKOA_TEST
#	include <Core/TestDriver.hpp>
bool RunTestsOnkLibrary()
{
	auto testDriver = tests::TestDriver();
	testDriver.Initialize();
	testDriver.RunTests();
	testDriver.ShutDown();
	return true;
};
#else
inline bool RunTestsOnkLibrary() { return false; }
#endif // KRAKOA_TEST
