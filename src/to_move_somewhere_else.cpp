#include "to_move_somewhere_else.h"
#include "interpreter.h"
#include "scripts.h"
#include <unordered_map>

namespace fallout {

// a map of all sfall programs (global and hook scripts) by thier scriptPtr

// void AddProgramToMap(Script prog)
//{
//     sfallProgsMap[prog.program] = prog;
// }
//
//
// Script GetGlobalScriptProgram(Program* scriptPtr)
//{
//     SfallProgsMap::iterator it = sfallProgsMap.find(scriptPtr);
//     return (it == sfallProgsMap.end()) ? nullptr : &it->second; // prog
// }

} // namespace fallout
