#pragma once
#include "..\..\..\to_move_somewhere_else.h"
namespace sfall
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

long UseObjOnHook_Invoke(fo::GameObject* source, fo::GameObject* item, fo::GameObject* target);

}
