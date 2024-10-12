//#include "..\..\FalloutEngine\Fallout2.h"
#include "..\sfall_safewrite.h"
#include "..\combat.h"
//#include "..\DamageMod.h"
#include "..\sfall_hook_scripts.h"
#include "Common.h"
#include "CombatHs.h"
#include "..\random.h"
#include "..\item.h"
#include "..\sfall\FalloutEngine\Variables.h"
#include "..\sfall\FalloutEngine\FunctionOffsets.h"
#include "..\obj_types.h"

namespace fallout
{

static __declspec(naked) void ToHitHook() {
	__asm {
		HookBegin;
		mov  args[4], eax;    // attacker
		mov  args[8], ebx;    // target
		mov  args[12], ecx;   // body part
		mov  args[16], edx;   // source tile
		mov  eax, [esp + 8];
		mov  args[24], eax;   // is ranged
		push eax;
		mov  eax, [esp + 8];
		mov  args[20], eax;   // attack type
		push eax;
		mov  eax, args[4];    // restore
		call fo::funcoffs::determine_to_hit_func_;
		mov  args[0], eax;
		mov  ebx, eax;
	}
	argCount = 8;

	args[7] = fallout::Combat::determineHitChance;
	RunHookScript(HOOK_TOHIT);

	__asm {
		cmp  cRet, 1;
		cmovge ebx, rets[0];
		mov  eax, 999; // max
		cmp  ebx, eax;
		cmovg ebx, eax;
		mov  eax, -99; // min
		cmp  ebx, eax;
		cmovl ebx, eax;
		call EndHook;
		mov  eax, ebx;
		retn 8;
	}
}

static DWORD __fastcall AfterHitRollHook_Script(fallout::Attack& ctd, DWORD hitChance, DWORD hit)
{
	BeginHook();
	argCount = 5;

	args[0] = hit;
	args[1] = (DWORD)ctd.attacker;   // Attacker
	args[2] = (DWORD)ctd.defender;     // Target
	args[3] = ctd.defenderHitLocation;          // bodypart
	args[4] = hitChance;

	RunHookScript(HOOK_AFTERHITROLL);
	if (cRet > 0) {
		hit = rets[0];
		if (cRet > 1) {
			ctd.defenderHitLocation = rets[1];
			if (cRet > 2) ctd.defender = (fallout::Object*)rets[2];
		}
	}
	EndHook();

	return hit;
}

static __declspec(naked) void AfterHitRollHook() {
	using namespace fo;
	__asm {
		mov  ecx, esi;              // ctd
		mov  edx, [esp + 0x18 + 4]; // hit chance
		push eax;                   // was it a hit?
		call AfterHitRollHook_Script;
		// engine code
		mov  ebx, eax;
		cmp  eax, ROLL_FAILURE;
		retn;
	}
}

static long CalcApCostHook_Script(fallout::Object* source, long hitMode, long isCalled, long cost, fallout::Object* weapon) {
	BeginHook();
	argCount = 5;

	args[0] = (DWORD)source;
	args[1] = hitMode;
	args[2] = isCalled;
	args[3] = cost;
	args[4] = (DWORD)weapon;

	RunHookScript(HOOK_CALCAPCOST);

	if (cRet > 0) cost = rets[0];
	EndHook();

	return cost;
}

long CalcApCostHook_Invoke(fallout::Object* source, long hitMode, long isCalled, long cost, fallout::Object* weapon) {
	return (HookScripts::HookHasScript(HOOK_CALCAPCOST))
	       ? CalcApCostHook_Script(source, hitMode, isCalled, cost, weapon)
	       : cost;
}
/*
static __declspec(naked) void CalcApCostHook() {
	__asm {
		HookBegin;
		mov  args[0], eax;
		mov  args[4], edx;
		mov  args[8], ebx;
		call fo::funcoffs::item_w_mp_cost_;
		mov  args[12], eax;
		mov  ebx, eax;
		push ecx;
	}

	argCount = 5;
	args[4] = 0;

	RunHookScript(HOOK_CALCAPCOST);

	__asm {
		cmp  cRet, 1;
		cmovge ebx, rets[0];
		call EndHook;
		mov  eax, ebx;
		pop  ecx;
		retn;
	}
}
*/
// this is for using non-weapon items, always 2 AP in vanilla
static __declspec(naked) void CalcApCostHook2() {
	__asm {
		HookBegin;
		mov args[0], ecx; // critter
		mov args[4], edx; // attack type (to determine hand)
		mov args[8], ebx;
		mov ebx, 2;       // vanilla cost value
		mov args[12], ebx;
		//push ecx;
	}

	argCount = 5;
	args[4] = 0;

	RunHookScript(HOOK_CALCAPCOST);

	__asm {
		cmp  cRet, 1;
		cmovge ebx, rets[0];
		call EndHook;
		mov  eax, ebx;
		//pop  ecx;
		retn;
	}
}

static void __fastcall ComputeDamageHook_Script(fallout::Attack& ctd, DWORD rounds, DWORD multiplier)
{
    BeginHook();
    argCount = 13;

    args[0] = (DWORD)ctd.defender; // Target
    args[1] = (DWORD)ctd.attacker; // Attacker
    args[2] = ctd.defenderDamage; // amountTarget
    args[3] = ctd.attackerDamage; // amountSource
    args[4] = ctd.defenderFlags; // flagsTarget
    args[5] = ctd.attackerFlags; // flagsSource
    args[6] = (DWORD)ctd.weapon;
    args[7] = ctd.defenderHitLocation;
    args[8] = multiplier; // damage multiplier
    args[9] = rounds; // number of rounds
    args[10] = ctd.defenderKnockback;
    args[11] = ctd.hitMode; // attack type
    args[12] = (DWORD)&ctd; // main_ctd/shoot_ctd/explosion_ctd

    RunHookScript(HOOK_COMBATDAMAGE);

    if (cRet > 0) {
        ctd.defenderDamage = rets[0];
        if (cRet > 1) {
            ctd.attackerDamage = rets[1];
            if (cRet > 2) {
                ctd.defenderFlags = rets[2]; // flagsTarget
                if (cRet > 3) {
                    ctd.attackerFlags = rets[3]; // flagsSource
                    if (cRet > 4) ctd.defenderKnockback = rets[4];
                }
            }
        }
        // prevent hook from setting zero damage for instadeath criticals (bug fix)
        if ((ctd.defenderFlags & fallout::DAM_DEAD) && ctd.defenderDamage <= 0) {
            ctd.defenderDamage = 1; // set 1 hp damage
        }
    }
    EndHook();
}

static __declspec(naked) void ComputeDamageHook() {
	__asm {
		push ecx;
		push ebx;         // store dmg multiplier  args[8]
		push edx;         // store num of rounds   args[9]
		push eax;         // store ctd
		call fo::funcoffs::compute_damage_;
		pop  ecx;         // restore ctd (eax)
		pop  edx;         // restore num of rounds
		call ComputeDamageHook_Script;  // stack - arg multiplier
		pop  ecx;
		retn;
	}
}

static void __fastcall SubComputeDamageHook_Script(fallout::DamageCalculationContext& context)
{
    // calculated internal ammo mod damage
    switch (fallout::gDamageCalculationType) {
    case 1:
    case 2:
        // DamageMod::DamageGlovz(ctd, accumulatedDamage, rounds, armorDT, armorDR, bonusRangedDamage, multiplyDamage, difficulty);
        damageModCalculateGlovz(&context);
        return;
    case 5:
        // DamageMod::DamageYAAM(ctd, accumulatedDamage, rounds, armorDT, armorDR, bonusRangedDamage, multiplyDamage, difficulty);
        damageModCalculateYaam(&context);
        return;
    }

    BeginHook();
    argCount = 12;

    // Attack* attack = context.attack;
    args[0] = (DWORD)context.attack;
    args[1] = (DWORD)context.attack->defender;
    args[2] = (DWORD)context.attack->weapon;
    args[3] = (DWORD)context.attack->hitMode;
    args[4] = (DWORD)context.ammoQuantity;
    args[5] = (DWORD)context.damageResistance;
    args[6] = (DWORD)context.damageThreshold;
    args[7] = (DWORD)context.damageBonus;
    args[8] = (DWORD)context.bonusDamageMultiplier;
    args[9] = (DWORD)context.combatDifficultyDamageModifier;
    args[10] = (DWORD)context.damagePtr;
    args[11] = (DWORD)&context; // main_ctd/shoot_ctd/explosion_ctd

    RunHookScript(HOOK_SUBCOMBATDAMAGE);

    if (cRet == 1) *context.damagePtr = rets[0];
    EndHook();
}

static __declspec(naked) void SubComputeDamageHook() {
	__asm {
		mov  ecx, dword ptr [esp + 0x20];      // Difficulty
		push ecx;
		mov  eax, dword ptr [esp + 0x24 + 4];  // MultiplyDamage
		push eax;
		mov  ecx, dword ptr [esp + 0x1C + 8];  // Rounds
		push ecx;
		mov  eax, dword ptr [esp + 0x18 + 12]; // BonusRangedDamage
		push eax;
		mov  ecx, dword ptr [esp + 0x28 + 16]; // ResistDT
		push ecx;
		push edi;                              // AccumulatedDamage
		mov  ecx, esi;                         // cdt
		push 0x424A63;                         // return to address
		jmp  SubComputeDamageHook_Script;      // edx - ResistDR
	}
}

static void __fastcall FindTargetHook_Script(DWORD* target, fallout::Object* attacker) {
	BeginHook();
	argCount = 5;

	args[0] = (DWORD)attacker;
	args[1] = target[0];
	args[2] = target[1];
	args[3] = target[2];
	args[4] = target[3];

	RunHookScript(HOOK_FINDTARGET);

	if (cRet > 0) {
		if (rets[0] != -1) target[0] = rets[0];
		if (cRet > 1 && rets[1] != -1) target[1] = rets[1];
		if (cRet > 2 && rets[2] != -1) target[2] = rets[2];
		if (cRet > 3 && rets[3] != -1) target[3] = rets[3];
	}
	EndHook();
}
/*
void FindTargetHook_Invoke(fallout::Object* targets[], fallout::Object* attacker) {
	if (HookScripts::HookHasScript(HOOK_FINDTARGET)) FindTargetHook_Script((DWORD*)targets, attacker);
}
*/
static __declspec(naked) void FindTargetHook() {
	__asm {
		push eax;
		call fo::funcoffs::qsort_;
		pop  ecx;          // targets (base)
		mov  edx, esi;     // attacker
		jmp  FindTargetHook_Script;
	}
}

static __declspec(naked) void ItemDamageHook() {
	__asm {
		HookBegin;
		mov args[0], eax;  // min
		mov args[4], edx;  // max
		mov args[8], edi;  // weapon
		mov args[12], ecx; // critter
		mov args[16], esi; // type
		mov args[20], ebp; // non-zero for weapon melee attack (add to min/max melee damage)
		pushadc;
	}
	argCount = 6;

	// tweak for 0x4784AA (obsolete)
	//if (args[2] == 0) { // weapon arg
	//	args[4] += 8;     // type arg
	//}

	RunHookScript(HOOK_ITEMDAMAGE);

	__asm popadc;
	if (cRet > 0) {
		__asm mov eax, rets[0];     // set min
		if (cRet > 1) {
			__asm mov edx, rets[4]; // set max
		} else {
			HookEnd;
			__asm retn;             // no calc random
		}
	}
	HookEnd;
	__asm jmp fo::funcoffs::roll_random_;
}

int __fastcall AmmoCostHook_Script(DWORD hookType, fallout::Object* weapon, DWORD &rounds) {
	int result = 0;

	BeginHook();
	argCount = 4;

	args[0] = (DWORD)weapon;
	args[1] = rounds;           // rounds in attack
	args[3] = hookType;

	if (hookType == 2) {        // burst hook
		rounds = 1;             // set default multiply for check burst attack
	} else {
            result = fallout::_item_w_compute_ammo_cost(weapon, (int*) & rounds);
		if (result == -1) goto failed; // computation failed
	}
	args[2] = rounds;           // rounds as computed by game (cost)

	RunHookScript(HOOK_AMMOCOST);

	if (cRet > 0 && (long)rets[0] >= 0) rounds = rets[0]; // override rounds

failed:
	EndHook();
	return result;
}

static __declspec(naked) void AmmoCostHook() {
	using namespace fo;
	__asm {
		xor  ecx, ecx;             // type of hook (0)
		cmp  dword ptr [esp + 0x1C + 4], ANIM_fire_burst;
		jl   skip;
		cmp  dword ptr [esp + 0x1C + 4], ANIM_fire_continuous;
		jg   skip;
		mov  ecx, 3;               // hook type burst
skip:
		xchg eax, edx;
		push eax;                  // rounds in attack ref
		call AmmoCostHook_Script;  // edx - weapon
		retn;
	}
}

static long combatTurnResult = 0;

// hooks combat_turn function
static long __fastcall CombatTurnHook_Script(fallout::Object* critter, long dudeBegin) {
	BeginHook();
	argCount = 3;

	args[0] = 1;              // turn begin
	args[1] = (DWORD)critter; // who begins turn
	args[2] = dudeBegin;      // true - dude begins/ends turn after loading a game saved in combat mode

	RunHookScript(HOOK_COMBATTURN); // Start of turn

	combatTurnResult = 0;
	if (cRet > 0 && rets[0] == 1) { // skip turn
		goto endHook;               // exit hook
	}

	// set_sfall_return is not used, proceed normally
	combatTurnResult =  _combat_turn(critter, dudeBegin);
	args[0] = combatTurnResult;

	if (fallout::_combat_end_due_to_load && combatTurnResult == -1) goto endHook; // don't run end of turn hook when the game was loaded during the combat

	//cRet = 0; // reset number of return values
	RunHookScript(HOOK_COMBATTURN); // End of turn

	if (cRet > 0 && rets[0] == -1) combatTurnResult = -1; // override result of turn

endHook:
	EndHook();
	return combatTurnResult;
}

static __declspec(naked) void CombatTurnHook() {
	__asm {
		push ecx;
		mov  ecx, eax;
		call CombatTurnHook_Script; // edx - dudeBegin
		pop  ecx;
		retn;
	}
}

static __declspec(naked) void CombatTurnHook_End() {
	if (combatTurnResult >= 0) {
		BeginHook();
		argCount = 3;

		args[0] = -2; // combat ended normally
		args[1] = getInt(FO_VAR_combat_turn_obj);
		args[2] = 0;

		RunHookScript(HOOK_COMBATTURN);
		EndHook();
	}
	__asm jmp  fo::funcoffs::combat_over_;
}

// hack to exit from combat_add_noncoms function without crashing when you load game during PM/NPC turn
static long countCombat = 0;
static __declspec(naked) void CombatTurnHook_AddNoncoms() {
	__asm {
		push ecx;
		mov  ecx, eax;
		call CombatTurnHook_Script; // edx - dudeBegin
		pop  ecx;
		cmp  eax, -1;
		je   endCombat;
		retn;
endCombat:
		mov  ecx, [esp + 4]; // list
		xor  edx, edx;
		cmp  ds:[FO_VAR_combat_end_due_to_load], edx;
		jz   skip;
		mov  eax, ds:[FO_VAR_list_com];
		test eax, eax;
		jz   skip;
		mov  countCombat, eax;
skip:
		mov  ds:[FO_VAR_list_com], edx; // edx = 0
		retn;
	}
}

static __declspec(naked) void combat_hook_fix_load() {
	static const DWORD combat_hook_end_combat = 0x422E91;
	__asm {
		call fo::funcoffs::combat_sequence_;
		mov  eax, countCombat;
		test eax, eax;
		jnz  forceEndCombat;
		retn;
forceEndCombat:
		mov  ds:[FO_VAR_list_com], eax;
		mov  countCombat, 0;
		add  esp, 4;
		jmp  combat_hook_end_combat;
	}
}

fallout::Object* __fastcall ComputeExplosionOnExtrasHook_Script(fallout::Object* object, DWORD isCheck, DWORD checkTile, fo::ComputeAttackResult* ctdSource, DWORD isThrowing, fallout::Object* who) {
	fallout::Object* result = object;

	BeginHook();
	argCount = 7;

	args[0] = isCheck;
	args[1] = (DWORD)ctdSource->attacker;
	args[2] = ctdSource->targetTile;
	args[3] = checkTile;
	args[4] = (DWORD)object;
	args[5] = (DWORD)who;
	args[6] = isThrowing;

	RunHookScript(HOOK_ONEXPLOSION);

	if (cRet > 0) result = (fallout::Object*)rets[0]; // override object

	EndHook();
	return result;
}

static __declspec(naked) void ComputeExplosionOnExtrasHook() {
	__asm {
		cmp  dword ptr [esp + 0x34 + 4], 0x429533;  // skip hook when AI assesses the situation in choosing the best weapon
		jz   end;
		cmp  dword ptr [esp + 0x34 + 4], 0x4296BC;
		jnz  hook;
end:
		jmp  fo::funcoffs::obj_blocking_at_;
hook:
		push ecx;
		push eax;                                   // who (target/source)
		call fo::funcoffs::obj_blocking_at_;
		push [esp + 0x34 - 0x24 + 12];              // isThrowing
		xor  edx, edx;
		cmp  dword ptr [esp + 0x34 + 16], 0x412F11; // check called from action_explode_
		jz   skip;
		mov  edx, [esp + 0x34 - 0x34 + 16];         // isCheck (bypass damage)
skip:
		push esi;                                   // source ctd
		push edi;                                   // check tile
		mov  ecx, eax;                              // object
		call ComputeExplosionOnExtrasHook_Script;   // edx - isCheck
		pop  ecx;
		retn;
	}
}

static DWORD targetRet = 0;
static bool targetObjHookHasRet = false;

static long __fastcall TargetObjectHook(DWORD isValid, DWORD object, long type) {
	if (isValid > 1) isValid = 1;

	BeginHook();
	argCount = 3;

	args[0] = type;    // 0 - mouse hovering over target, 1 - mouse clicking on target
	args[1] = isValid; // 1 - target is valid
	args[2] = object;  // target object

	if (isValid == 0) object = 0; // it is necessary for the proper operation of the engine code
	if (type == 0) targetRet = 0; // unset ret from the previous execution of the hook

	RunHookScript(HOOK_TARGETOBJECT);

	if (cRet > 0) {
		targetRet = (rets[0] != 0) ? rets[0] : object; // 0 - default object, -1 - invalid target, or object override
		object = (targetRet != -1) ? targetRet : 0;    // object can't be -1
		targetObjHookHasRet = true;
	} else if (targetObjHookHasRet && type == 1) {
		targetObjHookHasRet = false;
		if (targetRet != -1) object = targetRet;
	}
	EndHook();
	return object; // null or object
}

static __declspec(naked) void gmouse_bk_process_hook() {
	__asm {
		push 0;        // type
		mov  ecx, eax; // 1 - valid(object) or 0 - invalid
		mov  edx, edi; // object under mouse
		call TargetObjectHook;
		mov  edi, eax;
		retn;
	}
}

static __declspec(naked) void gmouse_handle_event_hook() {
	__asm {
		push 1;        // type
		mov  ecx, eax; // 1 - valid(object) or 0 - invalid
		cmp  dword ptr ds:[targetRet], 0;
		je   default;
		// override
		mov  ecx, 1;
		xor  eax, eax;
		cmp  dword ptr ds:[targetRet], -1;
		cmove  ecx, eax;       // if true - set invalid
		cmovne edx, targetRet; // if false - set override object
default:
		call TargetObjectHook;
		mov  edx, eax;
		retn;
	}
}

static fallout::Object* __stdcall BestWeaponHook_Script(fallout::Object* bestWeapon, fallout::Object* source, fallout::Object* weapon1, fallout::Object* weapon2, fallout::Object* target) {
	BeginHook();
	argCount = 5;

	args[0] = (DWORD)source;
	args[1] = (DWORD)bestWeapon;
	args[2] = (DWORD)weapon1;
	args[3] = (DWORD)weapon2;
	args[4] = (DWORD)target;

	RunHookScript(HOOK_BESTWEAPON);

	if (cRet > 0) bestWeapon = (fallout::Object*)rets[0];

	EndHook();
	return bestWeapon;
}

static __declspec(naked) void ai_search_inven_weap_hook() {
	__asm {
		push ecx; // target
		push ebx; // weapon2 (secondary)
		push edx; // weapon1 (primary)
		push eax; // source
		call fo::funcoffs::ai_best_weapon_;
		push eax; // bestWeapon
		call BestWeaponHook_Script;
		retn;
	}
}
/*
fallout::Object* BestWeaponHook_Invoke(fallout::Object* bestWeapon, fallout::Object* source, fallout::Object* weapon1, fallout::Object* weapon2, fallout::Object* target) {
	return (HookScripts::HookHasScript(HOOK_BESTWEAPON))
	       ? BestWeaponHook_Script(bestWeapon, source, weapon1, weapon2, target)
	       : bestWeapon;
}
*/
static bool __stdcall CanUseWeaponHook_Script(bool result, fallout::Object* source, fallout::Object* weapon, long hitMode) {
	BeginHook();
	argCount = 4;

	args[0] = (DWORD)source;
	args[1] = (DWORD)weapon;
	args[2] = hitMode;
	args[3] = result;

	RunHookScript(HOOK_CANUSEWEAPON);

	if (cRet > 0) result = (rets[0]) ? true : false;

	EndHook();
	return result; // only 0 and 1
}

static __declspec(naked) void AICanUseWeaponHook() {
	__asm {
		push ecx;
		push ebx; // hitMode
		push edx; // weapon
		push eax; // source
		call fo::funcoffs::ai_can_use_weapon_;
		push eax; // result
		call CanUseWeaponHook_Script;
		//and  eax, 1;
		pop  ecx;
		retn;
	}
}

static __declspec(naked) void CanUseWeaponHook() {
	__asm {
		push ecx;
		push edx;
		push -1;  // hitMode (indefinite)
		push eax; // weapon
		push ds:[FO_VAR_obj_dude]; // source
		call fo::funcoffs::can_use_weapon_; // 1 - can't use
		xor  al, 1;
		push eax; // result
		call CanUseWeaponHook_Script;
		xor  al, 1; // invert
		pop  edx;
		pop  ecx;
		retn;
	}
}

bool CanUseWeaponHook_Invoke(bool result, fallout::Object* source, fallout::Object* weapon, long hitMode) {
	return (HookScripts::HookHasScript(HOOK_CANUSEWEAPON))
	       ? CanUseWeaponHook_Script(result, source, weapon, hitMode)
	       : result;
}

////////////////////////////////////////////////////////////////////////////////

void Inject_ToHitHook() {
	HookCalls(ToHitHook, {
		0x421686, // combat_safety_invalidate_weapon_func_
		0x4231D9, // check_ranged_miss_
		0x42331F, // shoot_along_path_
		0x4237FC, // compute_attack_
		0x424379, // determine_to_hit_
		0x42438D, // determine_to_hit_no_range_
		0x42439C, // determine_to_hit_from_tile_
		0x42679A  // combat_to_hit_
	});
}

void Inject_AfterHitRollHook() {
	MakeCall(0x423893, AfterHitRollHook);
}

void Inject_CalcApCostHook() {
	//HookCalls(CalcApCostHook, {
	//	0x42307A,
	//	0x42669F,
	//	0x42687B,
	//	0x42A625,
	//	0x42A655,
	//	0x42A686,
	//	0x42AE32,
	//	0x42AE71,
	//	0x460048,
	//	0x47807B
	//});
	MakeCall(0x478083, CalcApCostHook2);
}

void Inject_CombatDamageHook() {
	HookCalls(ComputeDamageHook, {
		0x42326C, // check_ranged_miss()
		0x4233E3, // shoot_along_path() - for extra burst targets
		0x423AB7, // compute_attack()
		0x423BBF, // compute_attack()
//		0x423DE7, // compute_explosion_on_extras()
		0x423E69, // compute_explosion_on_extras()
		0x424220, // attack_crit_failure()
		0x4242FB  // attack_crit_failure()
	});
	MakeCall(0x423DEB, ComputeDamageHook); // compute_explosion_on_extras() - fix for the attacker
}

void Inject_FindTargetHook() {
	HookCall(0x429143, FindTargetHook);
}

void Inject_ItemDamageHook() {
	HookCall(0x478560, ItemDamageHook);
}

void Inject_AmmoCostHook() {
	HookCall(0x423A7C, AmmoCostHook); // compute_attack_
}

void Inject_CombatTurnHook() {
	HookCall(0x422354, CombatTurnHook_AddNoncoms); // 0x4222A8 _combat_add_noncoms()
	HookCalls(CombatTurnHook, { 0x422D87, 0x422E20 });
	HookCalls(CombatTurnHook_End, {
		0x422E85, // combat_ //0x422D2C void _combat(CombatStartData* csd)
		0x422196  // combat_over_from_load_ //0x422194 void _combat_over_from_load()
	});

	HookCall(0x422E4D, combat_hook_fix_load);
}

void Inject_OnExplosionHook() {
	HookCall(0x423D70, ComputeExplosionOnExtrasHook);
}

void Inject_SubCombatDamageHook() {
	MakeJump(0x42499C, SubComputeDamageHook);
}

void Inject_TargetObjectHook() {
	MakeCall(0x44BB16, gmouse_bk_process_hook, 1);
	SafeWrite8(0x44BB00, 0x15);

	MakeCall(0x44C286, gmouse_handle_event_hook, 1);
	SafeWrite8(0x44C26E, 0x17);
}

void Inject_BestWeaponHook() {
	HookCall(0x429A59, ai_search_inven_weap_hook);
}

void Inject_CanUseWeaponHook() {
	HookCalls(AICanUseWeaponHook, {
		0x429A1B, // ai_search_inven_weap_
		0x429CF2, // ai_search_environ_
		0x429E1C  // ai_pick_hit_mode_
	});
	HookCalls(CanUseWeaponHook, { 0x45F05E, 0x45F1C1, 0x45F203, 0x45F36A }); // intface_update_items_
}

void InitCombatHookScripts() {
	HookScripts::LoadHookScript("hs_tohit", HOOK_TOHIT);
	HookScripts::LoadHookScript("hs_afterhitroll", HOOK_AFTERHITROLL);
	HookScripts::LoadHookScript("hs_calcapcost", HOOK_CALCAPCOST);
	HookScripts::LoadHookScript("hs_combatdamage", HOOK_COMBATDAMAGE);
	HookScripts::LoadHookScript("hs_findtarget", HOOK_FINDTARGET);
	HookScripts::LoadHookScript("hs_itemdamage", HOOK_ITEMDAMAGE);
	HookScripts::LoadHookScript("hs_ammocost", HOOK_AMMOCOST);
	HookScripts::LoadHookScript("hs_combatturn", HOOK_COMBATTURN);
	HookScripts::LoadHookScript("hs_onexplosion", HOOK_ONEXPLOSION);
	HookScripts::LoadHookScript("hs_subcombatdmg", HOOK_SUBCOMBATDAMAGE);
	HookScripts::LoadHookScript("hs_targetobject", HOOK_TARGETOBJECT);
	HookScripts::LoadHookScript("hs_bestweapon", HOOK_BESTWEAPON);
	HookScripts::LoadHookScript("hs_canuseweapon", HOOK_CANUSEWEAPON);
}

}
