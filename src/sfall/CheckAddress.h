#pragma once
#include "..\to_move_somewhere_else.h"

namespace sfall
{

void PrintAddrList();
void CheckConflict(DWORD addr, long len);
void AddrAddToList(DWORD addr, long len);

}
