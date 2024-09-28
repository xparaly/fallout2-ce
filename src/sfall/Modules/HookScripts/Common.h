#pragma once

#include "..\..\..\to_move_somewhere_else.h"
#include "..\HookScripts.h"
#include "..\ScriptExtender.h"
#include "..\Scripting\ScriptValue.h"

// Common variables and functions for hook script implementations
//using namespace fallout;
namespace sfall
{

class HookCommon {
public:
	static fallout::DWORD GetHSArgCount();
	static fallout::ProgramValue GetHSArg();
	static fallout::ProgramValue GetHSArgAt(fallout::DWORD id);
	static void SetHSArg(fallout::DWORD id, const fallout::ProgramValue& value);
	static void SetHSReturn(const fallout::ProgramValue& value);

	static void GameModeChangeHook(fallout::DWORD exit);
	static void __stdcall KeyPressHook(fallout::DWORD* dxKey, bool pressed, fallout::DWORD vKey);
	static void __stdcall MouseClickHook(fallout::DWORD button, bool pressed);

	static void Reset();
};

// Struct for registered hook script
struct HookScript {
	fallout::ScriptProgram prog;
	int callback;        // procedure position in script's proc table
	bool isGlobalScript; // false for hs_* scripts, true for gl* scripts
};

// All currently registered hook scripts
extern std::vector<HookScript> hooks[];

extern bool allowNonIntReturn; // allow set_sfall_return with non-int values (validate value in the hook code)
extern fallout::opcode_t retTypes[]; // current hook return value types
extern fallout::DWORD args[];  // current hook arguments
extern fallout::DWORD rets[];  // current hook return values

extern fallout::DWORD argCount;
extern fallout::DWORD cRet;    // how many return values were set by current hook script
extern fallout::DWORD cRetTmp; // how many return values were set by specific hook script (when using register_hook)

void __stdcall BeginHook();
void __stdcall RunHookScript(fallout::DWORD hook);
void __stdcall EndHook();

#define HookBegin pushadc __asm call BeginHook popadc
#define HookEnd pushadc __asm call EndHook popadc

}
