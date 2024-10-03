#pragma once
#include "..\to_move_somewhere_else.h"
namespace fallout
{

void InitMiscHookScripts();
void SourceUseSkillOnInit();

void Inject_BarterPriceHook();
void Inject_UseSkillOnHook();
void Inject_UseSkillHook();
void Inject_StealCheckHook();
void Inject_SneakCheckHook();
void Inject_WithinPerceptionHook();
void Inject_CarTravelHook();
void Inject_SetGlobalVarHook();
void Inject_RestTimerHook();
void Inject_ExplosiveTimerHook();
void Inject_EncounterHook();
void Inject_RollCheckHook();

long PerceptionRangeHook_Invoke(fallout::Object* watcher, fallout::Object* target, long type, long result);

void BarterPriceHook_GetLastCosts(long& outPcTableCost, long& outNpcTableCost);

}
