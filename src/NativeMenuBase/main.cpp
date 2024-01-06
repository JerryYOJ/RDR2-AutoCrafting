#include "..\..\inc\main.h"
#include "script.h"
#include "keyboard.h"
#include <iostream>

#include "../../inc/MinHook.h"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		MH_Initialize();
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)NativeInit, 0, 0, 0);
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();
		break;
	}

	return TRUE;
}
