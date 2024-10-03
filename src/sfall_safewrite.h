#pragma once

#include <initializer_list>

#include "sfall\CheckAddress.h"
#include <Windows.h>

namespace fallout
{

enum CodeType : BYTE {
	Ret       = 0xC3,
	Call      = 0xE8,
	Jump      = 0xE9,
	Nop       = 0x90,
	JumpShort = 0xEB, // 0xEB [jmp short ...]
	JumpNZ    = 0x75, // 0x75 [jnz short ...]
	JumpZ     = 0x74, // 0x74 [jz  short ...]
};

// Macros for quick replacement of assembler opcodes pushad/popad
#define pushadc __asm push eax __asm push edx __asm push ecx
#define popadc __asm pop ecx __asm pop edx __asm pop eax

template <typename T>
void __stdcall SafeWrite(DWORD addr, T data) {
	DWORD oldProtect;
	VirtualProtect((void*)addr, sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
	*((T*)addr) = data;
	VirtualProtect((void*)addr, sizeof(T), oldProtect, &oldProtect);

	sfall::AddrAddToList(addr, sizeof(T));
}

template <typename T, class ForwardIteratorType>
void __stdcall SafeWriteBatch(T data, ForwardIteratorType begin, ForwardIteratorType end) {
	for (auto it = begin; it != end; ++it) {
		SafeWrite<T>(*it, data);
	}
}

template <class T, size_t N>
void __stdcall SafeWriteBatch(T data, const DWORD (&addrs)[N]) {
	SafeWriteBatch<T>(data, std::begin(addrs), std::end(addrs));
}

template <typename T>
void __stdcall SafeWriteBatch(T data, std::initializer_list<DWORD> addrs) {
	SafeWriteBatch<T>(data, addrs.begin(), addrs.end());
}

void __stdcall SafeWrite8(DWORD addr, BYTE data);
void __stdcall SafeWrite16(DWORD addr, WORD data);
void __stdcall SafeWrite32(DWORD addr, DWORD data);
void __stdcall SafeWriteStr(DWORD addr, const char* data);

void SafeMemSet(DWORD addr, BYTE val, size_t len);
void SafeWriteBytes(DWORD addr, BYTE* data, size_t count);

void HookCall(DWORD addr, void* func);
void MakeCall(DWORD addr, void* func);
void MakeCall(DWORD addr, void* func, size_t len);
void MakeJump(DWORD addr, void* func);
void MakeJump(DWORD addr, void* func, size_t len);
void BlockCall(DWORD addr);

void HookCalls(void* func, std::initializer_list<DWORD> addrs);
void MakeCalls(void* func, std::initializer_list<DWORD> addrs);
void MakeJumps(void* func, std::initializer_list<DWORD> addrs);

}
