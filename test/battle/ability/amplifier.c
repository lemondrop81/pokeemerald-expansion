#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(IsSoundMove(MOVE_BOOMBURST));
    ASSUME(!IsSoundMove(MOVE_ROCK_SLIDE));
}

SINGLE_BATTLE_TEST("Amplifier boosts sound moves by 25%", s16 damage)
{
    enum Ability ability;
    u32 genConfig;
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_AMPLIFIER; genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_AMPLIFIER; genConfig = GEN_6; }

    GIVEN {
        WITH_CONFIG(CONFIG_ATE_MULTIPLIER, genConfig);
        PLAYER(SPECIES_CHARIZARD) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BOOMBURST); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(1.25), results[0].damage);
        EXPECT_MUL_EQ(results[3].damage, Q_4_12(1.25), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Amplifier does not affect non-sound moves")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_AMPLIFIER); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, player);
        HP_BAR(opponent);
    }
}
