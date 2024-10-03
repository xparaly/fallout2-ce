#ifndef INVENTORY_HS_H
#define INVENTORY_HS_H
#include "..\to_move_somewhere_else.h"
// Inventory-Related hook scripts

namespace fallout
{

void InitInventoryHookScripts();

void Inject_RemoveInvenObjHook();
void Inject_MoveCostHook();
void Inject_InventoryMoveHook();
void Inject_InvenWieldHook();

long InvenWieldHook_Invoke(fallout::Object* critter, fallout::Object* item, long flags);

void InvenUnwield_HookDrop();
void InvenUnwield_HookMove();

void __stdcall SetRemoveObjectType(long rmType);

} // sfall

#endif /* INVENTORY_HS_H */
