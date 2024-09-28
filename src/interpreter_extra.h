#ifndef INTERPRETER_EXTRA_H
#define INTERPRETER_EXTRA_H

#include "interpreter.h"

namespace fallout {

enum InvenType : long {
    INVEN_TYPE_WORN = 0,
    INVEN_TYPE_RIGHT_HAND = 1,
    INVEN_TYPE_LEFT_HAND = 2,
    INVEN_TYPE_INV_COUNT = -2,
};

void _intExtraClose_();
void _initIntExtra();
void intExtraUpdate();
void intExtraRemoveProgramReferences(Program* program);
int _correctFidForRemovedItem(Object* a1, Object* a2, int flags);

} // namespace fallout

#endif /* INTERPRETER_EXTRA_H */
