#pragma once
#include "..\to_move_somewhere_else.h"

namespace sfall
{

void PrintAddrList();
void CheckConflict(fallout::DWORD addr, long len);
void AddrAddToList(fallout::DWORD addr, long len);

}
