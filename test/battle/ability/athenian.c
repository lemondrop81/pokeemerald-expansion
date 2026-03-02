#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveType(MOVE_SURF) == TYPE_WATER);
    ASSUME(GetMoveType(MOVE_ROCK_SLIDE) == TYPE_ROCK);
}

SINGLE_BATTLE_TEST("Athenian doubles power of special moves", s16 damage)
{
    enum Ability ability;
    u32 genConfig;
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_ATHENIAN; genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_ATHENIAN; genConfig = GEN_6; }

    GIVEN {
        WITH_CONFIG(CONFIG_ATE_MULTIPLIER, genConfig);
        PLAYER(SPECIES_CHARIZARD) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_SURF); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(2.0), results[0].damage);
        EXPECT_MUL_EQ(results[3].damage, Q_4_12(2.0), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Athenian has no effect on physical moves")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_ATHENIAN); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, player);
        HP_BAR(opponent);
    }
}
