#pragma once

// Hook scripts related to critter Death

namespace fallout
{

void InitDeathHookScripts();

void Inject_DeathAnim1Hook();
void Inject_DeathAnim2Hook();
void Inject_OnDeathHook();

}
