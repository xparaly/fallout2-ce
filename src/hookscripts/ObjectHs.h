#pragma once
#include "..\to_move_somewhere_else.h"
namespace fallout
{

void InitObjectHookScripts();

void Inject_UseObjOnHook();
void Inject_UseObjHook();
void Inject_UseAnimateObjHook();
void Inject_DescriptionObjHook();
void Inject_SetLightingHook();
void Inject_ScriptProcedureHook();
void Inject_ScriptProcedureHook2();
void Inject_AdjustPoisonHook();
void Inject_AdjustRadsHook();

long UseObjOnHook_Invoke(fallout::Object* source, fallout::Object* item, fallout::Object* target);

}
