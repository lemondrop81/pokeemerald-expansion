#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveType(MOVE_ROCK_SLIDE) == TYPE_ROCK);
    ASSUME(GetMoveType(MOVE_TACKLE) == TYPE_NORMAL);
}

SINGLE_BATTLE_TEST("Foundry turns a Rock-type move into a Fire-type move")
{
    GIVEN {
        PLAYER(SPECIES_CHARIZARD) { Ability(ABILITY_FOUNDRY); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_ROCK_SLIDE, player);
        // damage should be calculated as if Fire-type
        HP_BAR(opponent);
    }
}

SINGLE_BATTLE_TEST("Foundry boosts power of affected moves by 30%", s16 damage)
{
    enum Ability ability;
    u32 genConfig;
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_INTIMIDATE; genConfig = GEN_6; }
    PARAMETRIZE { ability = ABILITY_FOUNDRY;   genConfig = GEN_9; }
    PARAMETRIZE { ability = ABILITY_FOUNDRY;   genConfig = GEN_6; }

    GIVEN {
        WITH_CONFIG(CONFIG_ATE_MULTIPLIER, genConfig);
        PLAYER(SPECIES_CHARIZARD) { Ability(ability); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_ROCK_SLIDE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &results[i].damage);
    } FINALLY {
        // compare baseline (intimidate) to Foundry
        EXPECT_MUL_EQ(results[2].damage, Q_4_12(1.3), results[0].damage);
        EXPECT_MUL_EQ(results[3].damage, Q_4_12(1.3), results[1].damage);
    }
}

SINGLE_BATTLE_TEST("Stealth Rock set by Foundry user deals Fire-type damage")
{
    u32 species, divisor;

    PARAMETRIZE { species = SPECIES_CHARIZARD; divisor = 2; } // Fire/Flying (weak to Fire)
    PARAMETRIZE { species = SPECIES_VENUSAUR; divisor = 4; }   // Grass/Poison (resists Fire)
    PARAMETRIZE { species = SPECIES_RATICATE; divisor = 8; }  // Normal (neutral)

    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARIZARD) { Ability(ABILITY_FOUNDRY); }
        OPPONENT(species);
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_STEALTH_ROCK); }
        TURN { SWITCH(opponentRight, 1); }
    } SCENE {
        s32 maxHP = GetMonData(&OPPONENT_PARTY[1], MON_DATA_MAX_HP);
        ANIMATION(ANIM_TYPE_MOVE, MOVE_STEALTH_ROCK, opponentLeft);
        HP_BAR(opponentRight, damage: maxHP / divisor);
    }
}

SINGLE_BATTLE_TEST("Stealth Rock laid by Foundry user is recorded as FOUNDRY_ROCK")
{
    GIVEN {
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_CHARIZARD) { Ability(ABILITY_FOUNDRY); }
    } WHEN {
        TURN { MOVE(opponent, MOVE_STEALTH_ROCK); }
    } THEN {
        EXPECT_EQ(gBattleStruct->hazardsQueue[B_SIDE_OPPONENT][0], HAZARDS_FOUNDRY_ROCK);
    }
}
