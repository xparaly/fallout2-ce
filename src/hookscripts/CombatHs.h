#pragma once

// Combat-related hook scripts
#include "..\to_move_somewhere_else.h"
namespace fallout
{

void InitCombatHookScripts();

void Inject_ToHitHook();
void Inject_AfterHitRollHook();
void Inject_CalcApCostHook();
void Inject_CombatDamageHook();
void Inject_FindTargetHook();
void Inject_ItemDamageHook();
void Inject_AmmoCostHook();
void Inject_CombatTurnHook();
void Inject_OnExplosionHook();
void Inject_SubCombatDamageHook();
void Inject_TargetObjectHook();
void Inject_BestWeaponHook();
void Inject_CanUseWeaponHook();

int __fastcall AmmoCostHook_Script(DWORD hookType, fallout::Object* weapon, DWORD &rounds);

long CalcApCostHook_Invoke(fallout::Object* source, long hitMode, long isCalled, long cost, fallout::Object* weapon);
//void FindTargetHook_Invoke(fallout::Object* targets[], fallout::Object* attacker);
//fallout::Object* BestWeaponHook_Invoke(fallout::Object* bestWeapon, fallout::Object* source, fallout::Object* weapon1, fallout::Object* weapon2, fallout::Object* target);
bool CanUseWeaponHook_Invoke(bool result, fallout::Object* source, fallout::Object* weapon, long hitMode);

}
