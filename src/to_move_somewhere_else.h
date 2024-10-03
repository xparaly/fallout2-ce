#ifndef TO_MOVE_SOMEWHERE_ELSE_H
#define TO_MOVE_SOMEWHERE_ELSE_H

//#include <Windows.h>

// global flag, indicating that debugging features of Sfall are enabled
#ifndef NO_SFALL_DEBUG
    extern bool isDebug;
#else
    #define isDebug false
#endif

//#define min(a, b) (((a) < (b)) ? (a) : (b))

namespace fallout {
    typedef unsigned long DWORD;

    enum class DataType : DWORD {
        NONE = 0,
        INT = 1,
        FLOAT = 2,
        STR = 3,
    }; 

    __inline long getInt(fallout::DWORD addr)
    {
        return *reinterpret_cast<fallout::DWORD*>(addr);
    }

}

namespace sfall {
    namespace script {
    }
}

namespace fo {
    namespace func {
    }
    namespace funcoffs {
    }
    namespace var {
    }
}

//#include "scripts.h"
namespace sfall {

//void ToLowerCase(std::string& line)
//{
//    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
//}

//long protoId = 0x64;


namespace Fields {
    enum CommonObj : long {
        id = 0x00, // saveable
        tile = 0x04, // saveable
        x = 0x08, // saveable
        y = 0x0C, // saveable
        sx = 0x10, // saveable
        sy = 0x14, // saveable
        currFrame = 0x18, // saveable
        rotation = 0x1C, // saveable
        artFid = 0x20, // saveable
        flags = 0x24, // saveable
        elevation = 0x28, // saveable
        inventory = 0x2C, // saveable

        protoId = 0x64, // saveable
        cid = 0x68, // saveable (critter CombatID, don't change while in combat)
        lightDistance = 0x6C, // saveable
        lightIntensity = 0x70, // saveable
        outline = 0x74, // saveable
        scriptId = 0x78, // saveable
        owner = 0x7C,
        scriptIndex = 0x80, // saveable
    };

    enum ItemObj : long {
        miscFlags = 0x38, // saveable
        charges = 0x3C, // saveable
        ammoPid = 0x40, // saveable
    };

} // namespace Fields







// Macros for quick replacement of assembler opcodes pushad/popad
#define pushadc __asm push eax __asm push edx __asm push ecx
#define popadc __asm pop ecx __asm pop edx __asm pop eax


__inline long getInt(fallout::DWORD addr)
{
    return *reinterpret_cast<fallout::DWORD*>(addr);
}

//__inline BYTE getByte(fallout::DWORD addr)
//{
//    return *reinterpret_cast<BYTE*>(addr);
//}
//
//__inline long& setInt(fallout::DWORD addr)
//{
//    return *reinterpret_cast<long*>(addr);
//}
//
//__inline BYTE& setByte(fallout::DWORD addr)
//{
//    return *reinterpret_cast<BYTE*>(addr);
//}



//__inline long getInt(fallout::DWORD addr)
//{
//    return *reinterpret_cast<fallout::DWORD*>(addr);
//}

} // namespace sfall

#endif /* TO_MOVE_SOMEWHERE_ELSE_H */
