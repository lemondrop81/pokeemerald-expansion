#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Vampiric heals 25% of contact damage", s16 damage, s16 heal)
{
    u16 species;

    PARAMETRIZE { species = SPECIES_RATTATA; }
    PARAMETRIZE { species = SPECIES_ZUBAT; }
    PARAMETRIZE { species = SPECIES_TYRANITAR; }
    PARAMETRIZE { species = SPECIES_HYDREIGON; }
    PARAMETRIZE { species = SPECIES_BISHARP; }
    PARAMETRIZE { species = SPECIES_AEGISLASH_SHIELD; }

    GIVEN {
        PLAYER(species) { Ability(ABILITY_VAMPIRIC); HP(100); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &heal);
        ABILITY_POPUP(player, ABILITY_VAMPIRIC);
    } FINALLY {
        EXPECT_EQ(heal, damage / 4);
    }
}

SINGLE_BATTLE_TEST("Vampiric healing unaffected by Big Root", s16 damage, s16 heal)
{
    u32 item;

    PARAMETRIZE { item = ITEM_NONE; }
    PARAMETRIZE { item = ITEM_BIG_ROOT; }

    GIVEN {
        PLAYER(SPECIES_RATTATA) { Ability(ABILITY_VAMPIRIC); Item(item); HP(100); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { HP(100); }
    } WHEN {
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &heal);
    } FINALLY {
        EXPECT_EQ(heal, damage / 4);
    }
}

SINGLE_BATTLE_TEST("Vampiric doesn't heal under Heal Block")
{
    GIVEN {
        PLAYER(SPECIES_RATTATA) { Ability(ABILITY_VAMPIRIC); HP(100); MaxHP(100); }
        OPPONENT(SPECIES_WOBBUFFET) { }
    } WHEN {
        TURN { MOVE(opponent, MOVE_HEAL_BLOCK); }
        TURN { MOVE(player, MOVE_TACKLE); }
    } SCENE {
        NONE_OF {
            HP_BAR(player, damage: -ANY);
            ABILITY_POPUP(player, ABILITY_VAMPIRIC);
        }
    }
}
