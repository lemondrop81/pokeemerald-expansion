#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Nuclear type interactions are correct")
{
    enum Type defType;

    // ensure the new type exists and table entries are initialized
    ASSUME(TYPE_NUCLEAR < NUMBER_OF_MON_TYPES);

    for (defType = TYPE_NONE; defType < NUMBER_OF_MON_TYPES; defType++) {
        if (defType == TYPE_NUCLEAR) {
            EXPECT_EQ(gTypeEffectivenessTable[TYPE_NUCLEAR][defType], UQ_4_12(0.5));
        } else {
            EXPECT_EQ(gTypeEffectivenessTable[TYPE_NUCLEAR][defType], UQ_4_12(2.0));
        }
    }
}