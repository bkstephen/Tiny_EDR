#include <iostream>
#include <string>

#include "pch.h"
#include "framework.h"
#include "TinyEDR.h"

pNtProtectVirtualMemory pOriginalNtProtectVirtualMemory = nullptr;
pNtAllocateVirtualMemory pOriginalNtAllocateVirtualMemory = nullptr;

LPCWSTR ConvertInt(ULONG _long){
	std::string _val = std::to_string(_long);
	std::wstring stemp = std::wstring(_val.begin(), _val.end());
	return stemp.c_str();
}

DWORD NTAPI NtProtectVirtualMemory(
	IN HANDLE ProcessHandle, 
	IN OUT PVOID* BaseAddress, 
	IN OUT PULONG NumberOfBytesToProtect, 
	IN ULONG NewAccessProtection, 
	OUT PULONG OldAccessProtection) {
	
	//OutputDebugString(TEXT("NEW_PROT"));
	//OutputDebugString(ConvertInt(NewAccessProtection));
	//OutputDebugString(TEXT("OLD_PROT"));
	//OutputDebugString(ConvertInt(*OldAccessProtection));

	if (NewAccessProtection == PAGE_READWRITE && *OldAccessProtection == PAGE_READONLY) {
		
		OutputDebugString(TEXT("EDR_DLL: Malicious Action detected \n"));		
		TerminateProcess(GetCurrentProcess(), 0xdead1337);
		return 0;
	}

	return pOriginalNtProtectVirtualMemory(ProcessHandle, BaseAddress, NumberOfBytesToProtect, NewAccessProtection, OldAccessProtection);
}

DWORD NTAPI NtAllocateVirtualMemory(
	IN HANDLE ProcessHandle,
	IN OUT PVOID* BaseAddress,
	IN ULONG_PTR ZeroBits,
	IN OUT PSIZE_T   RegionSize,
	IN ULONG   AllocationType,
	IN ULONG   Protect) {

	OutputDebugString(TEXT("Called"));

	//OutputDebugString(TEXT("AllocationType"));
	//OutputDebugString(ConvertInt(AllocationType));

	//OutputDebugString(TEXT("Protect"));
	//OutputDebugString(ConvertInt(Protect));

	return pOriginalNtAllocateVirtualMemory(ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
}