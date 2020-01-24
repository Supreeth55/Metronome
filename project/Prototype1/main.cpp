#include "EngineLibrary.h"

extern "C"
{
	__declspec(dllexport) int           AmdPowerXpressRequestHighPerformance = 1;
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* System;
	bool result;

	//if (AllocConsole())
	//{
	//	FILE* file;
	//	freopen_s(&file, "CONOUT$", "wt", stdout);
	//	freopen_s(&file, "CONOUT$", "wt", stderr);
	//	freopen_s(&file, "CONOUT$", "wt", stdin);
	//}
	// Create the system object.
	System = new Engine;
	if (!System)return 0;


	// Initialize and run the system object.
	result = System->Initialize(0);
	if (result)System->Run();


	// Shutdown and release the system object.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}