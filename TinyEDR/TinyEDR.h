#pragma once

#ifdef TINY_EDR_EXPORTS
#define TINY_EDR_API __declspec(dllexport)
#else
#define TINY_EDR_API __declspec(dllimport)
#endif

typedef DWORD(NTAPI* pNtProtectVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);
extern pNtProtectVirtualMemory pOriginalNtProtectVirtualMemory;
DWORD NTAPI NtProtectVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);


typedef DWORD(NTAPI* pNtAllocateVirtualMemory)(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN ULONG_PTR ZeroBits, IN OUT PSIZE_T   RegionSize, IN ULONG   AllocationType, IN ULONG   Protect);
extern pNtAllocateVirtualMemory pOriginalNtAllocateVirtualMemory;
DWORD NTAPI NtAllocateVirtualMemory(IN HANDLE ProcessHandle, IN OUT PVOID* BaseAddress, IN ULONG_PTR ZeroBits, IN OUT PSIZE_T   RegionSize, IN ULONG   AllocationType, IN ULONG   Protect);