// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "minhook/include/MinHook.h"
#include "TinyEDR.h"
#include <stdio.h>
#include <iostream>

DWORD WINAPI InitHooksThread(LPVOID param) {

    if (MH_Initialize() != MH_OK) {
        OutputDebugString(TEXT("Failed to initalize\n"));
        return EXIT_FAILURE;
    }

    MH_STATUS status = MH_CreateHookApi(TEXT("ntdll"), "NtProtectVirtualMemory", NtProtectVirtualMemory,
        reinterpret_cast<LPVOID*>(&pOriginalNtProtectVirtualMemory));

     //MH_STATUS status = MH_CreateHookApi(TEXT("ntdll"), "NtAllocateVirtualMemory", NtAllocateVirtualMemory,
     //   reinterpret_cast<LPVOID*>(&pOriginalNtAllocateVirtualMemory));

    status = MH_EnableHook(MH_ALL_HOOKS);

    return status;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    OutputDebugString(TEXT("INJECTING DLL\n"));

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {

        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, InitHooksThread, nullptr, 0, nullptr);
        if (hThread != nullptr) {
            CloseHandle(hThread);
        }
        break;
    }
    case DLL_PROCESS_DETACH:

        break;
    }
    return TRUE;
}
