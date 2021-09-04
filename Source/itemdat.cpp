/**
 * @file itemdat.cpp
 *
 * Implementation of all item data.
 */

#include "itemdat.h"
#include "utils/language.h"

#include "datatable.h"

namespace devilution {
	#define ITEM_STRING_TO_ID(x) \
	if (str == #x) {        \
		return x;           \
	}

	item_drop_rate ItemDropRateFromText(std::string str)
	{
		ITEM_STRING_TO_ID(IDROP_NEVER)
		ITEM_STRING_TO_ID(IDROP_REGULAR)
		ITEM_STRING_TO_ID(IDROP_DOUBLE)

		devilution::app_fatal("Invalid item drop rate! %s", str.c_str());
	    return IDROP_NEVER;
	}

	item_class ItemClassFromText(std::string str)
    {
	    ITEM_STRING_TO_ID(ICLASS_NONE)
	    ITEM_STRING_TO_ID(ICLASS_WEAPON)
	    ITEM_STRING_TO_ID(ICLASS_ARMOR)
	    ITEM_STRING_TO_ID(ICLASS_MISC)
	    ITEM_STRING_TO_ID(ICLASS_GOLD)
		ITEM_STRING_TO_ID(ICLASS_QUEST)

		devilution::app_fatal("Invalid item class! %s", str.c_str());
	    return ICLASS_NONE;
	}

	item_equip_type ItemEquipTypeFromText(std::string str)
    {
	    ITEM_STRING_TO_ID(ILOC_NONE)
	    ITEM_STRING_TO_ID(ILOC_ONEHAND)
	    ITEM_STRING_TO_ID(ILOC_TWOHAND)
	    ITEM_STRING_TO_ID(ILOC_ARMOR)
	    ITEM_STRING_TO_ID(ILOC_HELM)
	    ITEM_STRING_TO_ID(ILOC_RING)
	    ITEM_STRING_TO_ID(ILOC_AMULET)
	    ITEM_STRING_TO_ID(ILOC_UNEQUIPABLE)
	    ITEM_STRING_TO_ID(ILOC_BELT)
	    ITEM_STRING_TO_ID(ILOC_INVALID)

		devilution::app_fatal("Invalid equip type! %s", str.c_str());
	    return ILOC_NONE;
	}

	item_type ItemTypeFromText(std::string str)
    {
	    ITEM_STRING_TO_ID(ITYPE_MISC)
	    ITEM_STRING_TO_ID(ITYPE_SWORD)
	    ITEM_STRING_TO_ID(ITYPE_AXE)
	    ITEM_STRING_TO_ID(ITYPE_BOW)
	    ITEM_STRING_TO_ID(ITYPE_MACE)
	    ITEM_STRING_TO_ID(ITYPE_SHIELD)
	    ITEM_STRING_TO_ID(ITYPE_LARMOR)
	    ITEM_STRING_TO_ID(ITYPE_HELM)
	    ITEM_STRING_TO_ID(ITYPE_MARMOR)
	    ITEM_STRING_TO_ID(ITYPE_HARMOR)
	    ITEM_STRING_TO_ID(ITYPE_STAFF)
	    ITEM_STRING_TO_ID(ITYPE_GOLD)
	    ITEM_STRING_TO_ID(ITYPE_RING)
	    ITEM_STRING_TO_ID(ITYPE_AMULET)
		ITEM_STRING_TO_ID(ITYPE_NONE)

		devilution::app_fatal("Invalid item tpye! %s", str.c_str());
	    return ITYPE_NONE;
	}

	item_cursor_graphic ItemCursorGraphicFromText(std::string str)
    {
		ITEM_STRING_TO_ID(ICURS_POTION_OF_FULL_MANA)
		ITEM_STRING_TO_ID(ICURS_SCROLL_OF)
		ITEM_STRING_TO_ID(ICURS_GOLD_SMALL)
		ITEM_STRING_TO_ID(ICURS_GOLD_MEDIUM)
		ITEM_STRING_TO_ID(ICURS_GOLD_LARGE)
		ITEM_STRING_TO_ID(ICURS_RING_OF_TRUTH)
		ITEM_STRING_TO_ID(ICURS_RING)
		ITEM_STRING_TO_ID(ICURS_SPECTRAL_ELIXIR)
		ITEM_STRING_TO_ID(ICURS_GOLDEN_ELIXIR)
		ITEM_STRING_TO_ID(ICURS_EMPYREAN_BAND)
		ITEM_STRING_TO_ID(ICURS_EAR_SORCERER)
		ITEM_STRING_TO_ID(ICURS_EAR_WARRIOR)
		ITEM_STRING_TO_ID(ICURS_EAR_ROGUE)
		ITEM_STRING_TO_ID(ICURS_BLOOD_STONE)
		ITEM_STRING_TO_ID(ICURS_OIL)
		ITEM_STRING_TO_ID(ICURS_ELIXIR_OF_VITALITY)
		ITEM_STRING_TO_ID(ICURS_POTION_OF_HEALING)
		ITEM_STRING_TO_ID(ICURS_POTION_OF_FULL_REJUVENATION)
		ITEM_STRING_TO_ID(ICURS_ELIXIR_OF_MAGIC)
		ITEM_STRING_TO_ID(ICURS_POTION_OF_FULL_HEALING)
		ITEM_STRING_TO_ID(ICURS_ELIXIR_OF_DEXTERITY)
		ITEM_STRING_TO_ID(ICURS_POTION_OF_REJUVENATION)
		ITEM_STRING_TO_ID(ICURS_ELIXIR_OF_STRENGTH)
		ITEM_STRING_TO_ID(ICURS_POTION_OF_MANA)
		ITEM_STRING_TO_ID(ICURS_BRAIN)
		ITEM_STRING_TO_ID(ICURS_OPTIC_AMULET)
		ITEM_STRING_TO_ID(ICURS_AMULET)
		ITEM_STRING_TO_ID(ICURS_DAGGER)
		ITEM_STRING_TO_ID(ICURS_BLADE)
		ITEM_STRING_TO_ID(ICURS_BASTARD_SWORD)
		ITEM_STRING_TO_ID(ICURS_MACE)
		ITEM_STRING_TO_ID(ICURS_LONG_SWORD)
		ITEM_STRING_TO_ID(ICURS_BROAD_SWORD)
		ITEM_STRING_TO_ID(ICURS_FALCHION)
		ITEM_STRING_TO_ID(ICURS_MORNING_STAR)
		ITEM_STRING_TO_ID(ICURS_SHORT_SWORD)
		ITEM_STRING_TO_ID(ICURS_CLAYMORE)
		ITEM_STRING_TO_ID(ICURS_CLUB)
		ITEM_STRING_TO_ID(ICURS_SABRE)
		ITEM_STRING_TO_ID(ICURS_SPIKED_CLUB)
		ITEM_STRING_TO_ID(ICURS_SCIMITAR)
		ITEM_STRING_TO_ID(ICURS_FULL_HELM)
		ITEM_STRING_TO_ID(ICURS_MAGIC_ROCK)
		ITEM_STRING_TO_ID(ICURS_THE_UNDEAD_CROWN)
		ITEM_STRING_TO_ID(ICURS_HELM)
		ITEM_STRING_TO_ID(ICURS_BUCKLER)
		ITEM_STRING_TO_ID(ICURS_VIEL_OF_STEEL)
		ITEM_STRING_TO_ID(ICURS_BOOK_GREY)
		ITEM_STRING_TO_ID(ICURS_BOOK_RED)
		ITEM_STRING_TO_ID(ICURS_BOOK_BLUE)
		ITEM_STRING_TO_ID(ICURS_BLACK_MUSHROOM)
		ITEM_STRING_TO_ID(ICURS_SKULL_CAP)
		ITEM_STRING_TO_ID(ICURS_CAP)
		ITEM_STRING_TO_ID(ICURS_HARLEQUIN_CREST)
		ITEM_STRING_TO_ID(ICURS_CROWN)
		ITEM_STRING_TO_ID(ICURS_MAP_OF_THE_STARS)
		ITEM_STRING_TO_ID(ICURS_FUNGAL_TOME)
		ITEM_STRING_TO_ID(ICURS_GREAT_HELM)
		ITEM_STRING_TO_ID(ICURS_BATTLE_AXE)
		ITEM_STRING_TO_ID(ICURS_HUNTERS_BOW)
		ITEM_STRING_TO_ID(ICURS_FIELD_PLATE)
		ITEM_STRING_TO_ID(ICURS_SMALL_SHIELD)
		ITEM_STRING_TO_ID(ICURS_CLEAVER)
		ITEM_STRING_TO_ID(ICURS_STUDDED_LEATHER_ARMOR)
		ITEM_STRING_TO_ID(ICURS_SHORT_STAFF)
		ITEM_STRING_TO_ID(ICURS_TWO_HANDED_SWORD)
		ITEM_STRING_TO_ID(ICURS_CHAIN_MAIL)
		ITEM_STRING_TO_ID(ICURS_SMALL_AXE)
		ITEM_STRING_TO_ID(ICURS_KITE_SHIELD)
		ITEM_STRING_TO_ID(ICURS_SCALE_MAIL)
		ITEM_STRING_TO_ID(ICURS_SHORT_BOW)
		ITEM_STRING_TO_ID(ICURS_LONG_WAR_BOW)
		ITEM_STRING_TO_ID(ICURS_WAR_HAMMER)
		ITEM_STRING_TO_ID(ICURS_MAUL)
		ITEM_STRING_TO_ID(ICURS_LONG_STAFF)
		ITEM_STRING_TO_ID(ICURS_WAR_STAFF)
		ITEM_STRING_TO_ID(ICURS_TAVERN_SIGN)
		ITEM_STRING_TO_ID(ICURS_HARD_LEATHER_ARMOR)
		ITEM_STRING_TO_ID(ICURS_RAGS)
		ITEM_STRING_TO_ID(ICURS_QUILTED_ARMOR)
		ITEM_STRING_TO_ID(ICURS_FLAIL)
		ITEM_STRING_TO_ID(ICURS_TOWER_SHIELD)
		ITEM_STRING_TO_ID(ICURS_COMPOSITE_BOW)
		ITEM_STRING_TO_ID(ICURS_GREAT_SWORD)
		ITEM_STRING_TO_ID(ICURS_LEATHER_ARMOR)
		ITEM_STRING_TO_ID(ICURS_SPLINT_MAIL)
		ITEM_STRING_TO_ID(ICURS_ROBE)
		ITEM_STRING_TO_ID(ICURS_ANVIL_OF_FURY)
		ITEM_STRING_TO_ID(ICURS_BROAD_AXE)
		ITEM_STRING_TO_ID(ICURS_LARGE_AXE)
		ITEM_STRING_TO_ID(ICURS_GREAT_AXE)
		ITEM_STRING_TO_ID(ICURS_AXE)
		ITEM_STRING_TO_ID(ICURS_LARGE_SHIELD)
		ITEM_STRING_TO_ID(ICURS_GOTHIC_SHIELD)
		ITEM_STRING_TO_ID(ICURS_CLOAK)
		ITEM_STRING_TO_ID(ICURS_CAPE)
		ITEM_STRING_TO_ID(ICURS_FULL_PLATE_MAIL)
		ITEM_STRING_TO_ID(ICURS_GOTHIC_PLATE)
		ITEM_STRING_TO_ID(ICURS_BREAST_PLATE)
		ITEM_STRING_TO_ID(ICURS_RING_MAIL)
		ITEM_STRING_TO_ID(ICURS_STAFF_OF_LAZARUS)
		ITEM_STRING_TO_ID(ICURS_ARKAINES_VALOR)
		ITEM_STRING_TO_ID(ICURS_SHORT_WAR_BOW)
		ITEM_STRING_TO_ID(ICURS_COMPOSITE_STAFF)
		ITEM_STRING_TO_ID(ICURS_SHORT_BATTLE_BOW)
		ITEM_STRING_TO_ID(ICURS_GOLD)
		ITEM_STRING_TO_ID(ICURS_AURIC_AMULET)
		ITEM_STRING_TO_ID(ICURS_RUNE_BOMB)
		ITEM_STRING_TO_ID(ICURS_THEODORE)
		ITEM_STRING_TO_ID(ICURS_TORN_NOTE_1)
		ITEM_STRING_TO_ID(ICURS_TORN_NOTE_2)
		ITEM_STRING_TO_ID(ICURS_TORN_NOTE_3)
		ITEM_STRING_TO_ID(ICURS_RECONSTRUCTED_NOTE)
		ITEM_STRING_TO_ID(ICURS_RUNE_OF_FIRE)
		ITEM_STRING_TO_ID(ICURS_GREATER_RUNE_OF_FIRE)
		ITEM_STRING_TO_ID(ICURS_RUNE_OF_LIGHTNING)
		ITEM_STRING_TO_ID(ICURS_GREATER_RUNE_OF_LIGHTNING)
		ITEM_STRING_TO_ID(ICURS_RUNE_OF_STONE)
		ITEM_STRING_TO_ID(ICURS_GREY_SUIT)
		ITEM_STRING_TO_ID(ICURS_BROWN_SUIT)
		ITEM_STRING_TO_ID(ICURS_BOVINE)

		devilution::app_fatal("Invalid item graphic tpye! %s", str.c_str());
	    return ICURS_BOVINE;
	}

	item_misc_id ItemMisicIdFromText(std::string str)
    {
	    ITEM_STRING_TO_ID(IMISC_NONE)
	    ITEM_STRING_TO_ID(IMISC_USEFIRST)
	    ITEM_STRING_TO_ID(IMISC_FULLHEAL)
	    ITEM_STRING_TO_ID(IMISC_HEAL)
	    ITEM_STRING_TO_ID(IMISC_OLDHEAL)
	    ITEM_STRING_TO_ID(IMISC_DEADHEAL)
	    ITEM_STRING_TO_ID(IMISC_MANA)
	    ITEM_STRING_TO_ID(IMISC_FULLMANA)
	    ITEM_STRING_TO_ID(IMISC_POTEXP)
	    ITEM_STRING_TO_ID(IMISC_POTFORG)
	    ITEM_STRING_TO_ID(IMISC_ELIXSTR)
	    ITEM_STRING_TO_ID(IMISC_ELIXMAG)
	    ITEM_STRING_TO_ID(IMISC_ELIXDEX)
	    ITEM_STRING_TO_ID(IMISC_ELIXVIT)
	    ITEM_STRING_TO_ID(IMISC_ELIXWEAK)
	    ITEM_STRING_TO_ID(IMISC_ELIXDIS)
	    ITEM_STRING_TO_ID(IMISC_ELIXCLUM)
	    ITEM_STRING_TO_ID(IMISC_ELIXSICK)
	    ITEM_STRING_TO_ID(IMISC_REJUV)
	    ITEM_STRING_TO_ID(IMISC_FULLREJUV)
	    ITEM_STRING_TO_ID(IMISC_USELAST)
	    ITEM_STRING_TO_ID(IMISC_SCROLL)
	    ITEM_STRING_TO_ID(IMISC_SCROLLT)
	    ITEM_STRING_TO_ID(IMISC_STAFF)
	    ITEM_STRING_TO_ID(IMISC_BOOK)
	    ITEM_STRING_TO_ID(IMISC_RING)
	    ITEM_STRING_TO_ID(IMISC_AMULET)
	    ITEM_STRING_TO_ID(IMISC_UNIQUE)
	    ITEM_STRING_TO_ID(IMISC_FOOD)
	    ITEM_STRING_TO_ID(IMISC_OILFIRST)
	    ITEM_STRING_TO_ID(IMISC_OILOF)
	    ITEM_STRING_TO_ID(IMISC_OILACC)
	    ITEM_STRING_TO_ID(IMISC_OILMAST)
	    ITEM_STRING_TO_ID(IMISC_OILSHARP)
	    ITEM_STRING_TO_ID(IMISC_OILDEATH)
	    ITEM_STRING_TO_ID(IMISC_OILSKILL)
	    ITEM_STRING_TO_ID(IMISC_OILBSMTH)
	    ITEM_STRING_TO_ID(IMISC_OILFORT)
	    ITEM_STRING_TO_ID(IMISC_OILPERM)
	    ITEM_STRING_TO_ID(IMISC_OILHARD)
	    ITEM_STRING_TO_ID(IMISC_OILIMP)
	    ITEM_STRING_TO_ID(IMISC_OILLAST)
	    ITEM_STRING_TO_ID(IMISC_MAPOFDOOM)
	    ITEM_STRING_TO_ID(IMISC_EAR)
	    ITEM_STRING_TO_ID(IMISC_SPECELIX)
		ITEM_STRING_TO_ID(IMISC_0x2D)
		ITEM_STRING_TO_ID(IMISC_RUNEFIRST)
		ITEM_STRING_TO_ID(IMISC_RUNEF)
		ITEM_STRING_TO_ID(IMISC_RUNEL)
		ITEM_STRING_TO_ID(IMISC_GR_RUNEL)
		ITEM_STRING_TO_ID(IMISC_GR_RUNEF)
		ITEM_STRING_TO_ID(IMISC_RUNES)
		ITEM_STRING_TO_ID(IMISC_RUNELAST)
		ITEM_STRING_TO_ID(IMISC_AURIC)
		ITEM_STRING_TO_ID(IMISC_NOTE)

	    devilution::app_fatal("Invalid item misc tpye! %s", str.c_str());
	    return IMISC_NONE;
	}

	item_special_effect ItemSpecialEffectFromText(std::string str)
    {
		ITEM_STRING_TO_ID(ISPL_NONE)
		ITEM_STRING_TO_ID(ISPL_INFRAVISION)
		ITEM_STRING_TO_ID(ISPL_RNDSTEALLIFE)
		ITEM_STRING_TO_ID(ISPL_RNDARROWVEL)
		ITEM_STRING_TO_ID(ISPL_FIRE_ARROWS)
		ITEM_STRING_TO_ID(ISPL_FIREDAM)
		ITEM_STRING_TO_ID(ISPL_LIGHTDAM)
		ITEM_STRING_TO_ID(ISPL_DRAINLIFE)
		ITEM_STRING_TO_ID(ISPL_UNKNOWN_1)
		ITEM_STRING_TO_ID(ISPL_NOHEALPLR)
		ITEM_STRING_TO_ID(ISPL_MULT_ARROWS)
		ITEM_STRING_TO_ID(ISPL_UNKNOWN_3)
		ITEM_STRING_TO_ID(ISPL_KNOCKBACK)
		ITEM_STRING_TO_ID(ISPL_NOHEALMON)
		ITEM_STRING_TO_ID(ISPL_STEALMANA_3)
		ITEM_STRING_TO_ID(ISPL_STEALMANA_5)
		ITEM_STRING_TO_ID(ISPL_STEALLIFE_3)
		ITEM_STRING_TO_ID(ISPL_STEALLIFE_5)
		ITEM_STRING_TO_ID(ISPL_QUICKATTACK)
		ITEM_STRING_TO_ID(ISPL_FASTATTACK)
		ITEM_STRING_TO_ID(ISPL_FASTERATTACK)
		ITEM_STRING_TO_ID(ISPL_FASTESTATTACK)
		ITEM_STRING_TO_ID(ISPL_FASTRECOVER)
		ITEM_STRING_TO_ID(ISPL_FASTERRECOVER)
		ITEM_STRING_TO_ID(ISPL_FASTESTRECOVER)
		ITEM_STRING_TO_ID(ISPL_FASTBLOCK)
		ITEM_STRING_TO_ID(ISPL_LIGHT_ARROWS)
		ITEM_STRING_TO_ID(ISPL_THORNS)
		ITEM_STRING_TO_ID(ISPL_NOMANA)
		ITEM_STRING_TO_ID(ISPL_ABSHALFTRAP)
		ITEM_STRING_TO_ID(ISPL_UNKNOWN_4)
		ITEM_STRING_TO_ID(ISPL_3XDAMVDEM)
		ITEM_STRING_TO_ID(ISPL_ALLRESZERO)

		devilution::app_fatal("Invalid item special effect! %s", str.c_str());
	    return ISPL_NONE;
	}

	unique_base_item UniqueBaseItemFromText(std::string str)
    {
		ITEM_STRING_TO_ID(UITYPE_NONE)
	    ITEM_STRING_TO_ID(UITYPE_INVALID)
		ITEM_STRING_TO_ID(UITYPE_SHORTBOW)
		ITEM_STRING_TO_ID(UITYPE_LONGBOW)
		ITEM_STRING_TO_ID(UITYPE_HUNTBOW)
		ITEM_STRING_TO_ID(UITYPE_COMPBOW)
		ITEM_STRING_TO_ID(UITYPE_WARBOW)
		ITEM_STRING_TO_ID(UITYPE_BATTLEBOW)
		ITEM_STRING_TO_ID(UITYPE_DAGGER)
		ITEM_STRING_TO_ID(UITYPE_FALCHION)
		ITEM_STRING_TO_ID(UITYPE_CLAYMORE)
		ITEM_STRING_TO_ID(UITYPE_BROADSWR)
		ITEM_STRING_TO_ID(UITYPE_SABRE)
		ITEM_STRING_TO_ID(UITYPE_SCIMITAR)
		ITEM_STRING_TO_ID(UITYPE_LONGSWR)
		ITEM_STRING_TO_ID(UITYPE_BASTARDSWR)
		ITEM_STRING_TO_ID(UITYPE_TWOHANDSWR)
		ITEM_STRING_TO_ID(UITYPE_GREATSWR)
		ITEM_STRING_TO_ID(UITYPE_CLEAVER)
		ITEM_STRING_TO_ID(UITYPE_LARGEAXE)
		ITEM_STRING_TO_ID(UITYPE_BROADAXE)
		ITEM_STRING_TO_ID(UITYPE_SMALLAXE)
		ITEM_STRING_TO_ID(UITYPE_BATTLEAXE)
		ITEM_STRING_TO_ID(UITYPE_GREATAXE)
		ITEM_STRING_TO_ID(UITYPE_MACE)
		ITEM_STRING_TO_ID(UITYPE_MORNSTAR)
		ITEM_STRING_TO_ID(UITYPE_SPIKCLUB)
		ITEM_STRING_TO_ID(UITYPE_MAUL)
		ITEM_STRING_TO_ID(UITYPE_WARHAMMER)
		ITEM_STRING_TO_ID(UITYPE_FLAIL)
		ITEM_STRING_TO_ID(UITYPE_LONGSTAFF)
		ITEM_STRING_TO_ID(UITYPE_SHORTSTAFF)
		ITEM_STRING_TO_ID(UITYPE_COMPSTAFF)
		ITEM_STRING_TO_ID(UITYPE_QUARSTAFF)
		ITEM_STRING_TO_ID(UITYPE_WARSTAFF)
		ITEM_STRING_TO_ID(UITYPE_SKULLCAP)
		ITEM_STRING_TO_ID(UITYPE_HELM)
		ITEM_STRING_TO_ID(UITYPE_GREATHELM)
		ITEM_STRING_TO_ID(UITYPE_CROWN)
		ITEM_STRING_TO_ID(UITYPE_38)
		ITEM_STRING_TO_ID(UITYPE_RAGS)
		ITEM_STRING_TO_ID(UITYPE_STUDARMOR)
		ITEM_STRING_TO_ID(UITYPE_CLOAK)
		ITEM_STRING_TO_ID(UITYPE_ROBE)
		ITEM_STRING_TO_ID(UITYPE_CHAINMAIL)
		ITEM_STRING_TO_ID(UITYPE_LEATHARMOR)
		ITEM_STRING_TO_ID(UITYPE_BREASTPLATE)
		ITEM_STRING_TO_ID(UITYPE_CAPE)
		ITEM_STRING_TO_ID(UITYPE_PLATEMAIL)
		ITEM_STRING_TO_ID(UITYPE_FULLPLATE)
		ITEM_STRING_TO_ID(UITYPE_BUCKLER)
		ITEM_STRING_TO_ID(UITYPE_SMALLSHIELD)
		ITEM_STRING_TO_ID(UITYPE_LARGESHIELD)
		ITEM_STRING_TO_ID(UITYPE_KITESHIELD)
		ITEM_STRING_TO_ID(UITYPE_GOTHSHIELD)
		ITEM_STRING_TO_ID(UITYPE_RING)
		ITEM_STRING_TO_ID(UITYPE_55)
		ITEM_STRING_TO_ID(UITYPE_AMULET)
		ITEM_STRING_TO_ID(UITYPE_SKCROWN)
		ITEM_STRING_TO_ID(UITYPE_INFRARING)
		ITEM_STRING_TO_ID(UITYPE_OPTAMULET)
		ITEM_STRING_TO_ID(UITYPE_TRING)
		ITEM_STRING_TO_ID(UITYPE_HARCREST)
		ITEM_STRING_TO_ID(UITYPE_MAPOFDOOM)
		ITEM_STRING_TO_ID(UITYPE_ELIXIR)
		ITEM_STRING_TO_ID(UITYPE_ARMOFVAL)
		ITEM_STRING_TO_ID(UITYPE_STEELVEIL)
		ITEM_STRING_TO_ID(UITYPE_GRISWOLD)
		ITEM_STRING_TO_ID(UITYPE_LGTFORGE)
		ITEM_STRING_TO_ID(UITYPE_LAZSTAFF)
		ITEM_STRING_TO_ID(UITYPE_BOVINE)

			
		devilution::app_fatal("Invalid unique item type! %s", str.c_str());
	    return UITYPE_NONE;
	}

	item_effect_type ItemEffectTypeToString(std::string str)
    {
		ITEM_STRING_TO_ID(IPL_TOHIT)
		ITEM_STRING_TO_ID(IPL_TOHIT_CURSE)
		ITEM_STRING_TO_ID(IPL_DAMP)
		ITEM_STRING_TO_ID(IPL_DAMP_CURSE)
		ITEM_STRING_TO_ID(IPL_TOHIT_DAMP)
		ITEM_STRING_TO_ID(IPL_TOHIT_DAMP_CURSE)
		ITEM_STRING_TO_ID(IPL_ACP)
		ITEM_STRING_TO_ID(IPL_ACP_CURSE)
		ITEM_STRING_TO_ID(IPL_FIRERES)
		ITEM_STRING_TO_ID(IPL_LIGHTRES)
		ITEM_STRING_TO_ID(IPL_MAGICRES)
		ITEM_STRING_TO_ID(IPL_ALLRES)
		ITEM_STRING_TO_ID(IPL_SPLCOST)
		ITEM_STRING_TO_ID(IPL_SPLDUR)
		ITEM_STRING_TO_ID(IPL_SPLLVLADD)
		ITEM_STRING_TO_ID(IPL_CHARGES)
		ITEM_STRING_TO_ID(IPL_FIREDAM)
		ITEM_STRING_TO_ID(IPL_LIGHTDAM)
		ITEM_STRING_TO_ID(IPL_0x12)
		ITEM_STRING_TO_ID(IPL_STR)
		ITEM_STRING_TO_ID(IPL_STR_CURSE)
		ITEM_STRING_TO_ID(IPL_MAG)
		ITEM_STRING_TO_ID(IPL_MAG_CURSE)
		ITEM_STRING_TO_ID(IPL_DEX)
		ITEM_STRING_TO_ID(IPL_DEX_CURSE)
		ITEM_STRING_TO_ID(IPL_VIT)
		ITEM_STRING_TO_ID(IPL_VIT_CURSE)
		ITEM_STRING_TO_ID(IPL_ATTRIBS)
		ITEM_STRING_TO_ID(IPL_ATTRIBS_CURSE)
		ITEM_STRING_TO_ID(IPL_GETHIT_CURSE)
		ITEM_STRING_TO_ID(IPL_GETHIT)
		ITEM_STRING_TO_ID(IPL_LIFE)
		ITEM_STRING_TO_ID(IPL_LIFE_CURSE)
		ITEM_STRING_TO_ID(IPL_MANA)
		ITEM_STRING_TO_ID(IPL_MANA_CURSE)
		ITEM_STRING_TO_ID(IPL_DUR)
		ITEM_STRING_TO_ID(IPL_DUR_CURSE)
		ITEM_STRING_TO_ID(IPL_INDESTRUCTIBLE)
		ITEM_STRING_TO_ID(IPL_LIGHT)
		ITEM_STRING_TO_ID(IPL_LIGHT_CURSE)
		ITEM_STRING_TO_ID(IPL_0x28)
		ITEM_STRING_TO_ID(IPL_MULT_ARROWS)
		ITEM_STRING_TO_ID(IPL_FIRE_ARROWS)
		ITEM_STRING_TO_ID(IPL_LIGHT_ARROWS)
		ITEM_STRING_TO_ID(IPL_INVCURS)
		ITEM_STRING_TO_ID(IPL_THORNS)
		ITEM_STRING_TO_ID(IPL_NOMANA)
		ITEM_STRING_TO_ID(IPL_NOHEALPLR)
		ITEM_STRING_TO_ID(IPL_0x30)
		ITEM_STRING_TO_ID(IPL_0x31)
		ITEM_STRING_TO_ID(IPL_FIREBALL)
		ITEM_STRING_TO_ID(IPL_0x33)
		ITEM_STRING_TO_ID(IPL_ABSHALFTRAP)
		ITEM_STRING_TO_ID(IPL_KNOCKBACK)
		ITEM_STRING_TO_ID(IPL_NOHEALMON)
		ITEM_STRING_TO_ID(IPL_STEALMANA)
		ITEM_STRING_TO_ID(IPL_STEALLIFE)
		ITEM_STRING_TO_ID(IPL_TARGAC)
		ITEM_STRING_TO_ID(IPL_FASTATTACK)
		ITEM_STRING_TO_ID(IPL_FASTRECOVER)
		ITEM_STRING_TO_ID(IPL_FASTBLOCK)
		ITEM_STRING_TO_ID(IPL_DAMMOD)
		ITEM_STRING_TO_ID(IPL_RNDARROWVEL)
		ITEM_STRING_TO_ID(IPL_SETDAM)
		ITEM_STRING_TO_ID(IPL_SETDUR)
		ITEM_STRING_TO_ID(IPL_NOMINSTR)
		ITEM_STRING_TO_ID(IPL_SPELL)
		ITEM_STRING_TO_ID(IPL_FASTSWING)
		ITEM_STRING_TO_ID(IPL_ONEHAND)
		ITEM_STRING_TO_ID(IPL_3XDAMVDEM)
		ITEM_STRING_TO_ID(IPL_ALLRESZERO)
		ITEM_STRING_TO_ID(IPL_0x47)
		ITEM_STRING_TO_ID(IPL_DRAINLIFE)
		ITEM_STRING_TO_ID(IPL_RNDSTEALLIFE)
		ITEM_STRING_TO_ID(IPL_INFRAVISION)
		ITEM_STRING_TO_ID(IPL_SETAC)
		ITEM_STRING_TO_ID(IPL_ADDACLIFE)
		ITEM_STRING_TO_ID(IPL_ADDMANAAC)
		ITEM_STRING_TO_ID(IPL_FIRERESCLVL)
		ITEM_STRING_TO_ID(IPL_AC_CURSE)
		ITEM_STRING_TO_ID(IDI_LASTDIABLO)
		ITEM_STRING_TO_ID(IPL_FIRERES_CURSE)
		ITEM_STRING_TO_ID(IPL_LIGHTRES_CURSE)
		ITEM_STRING_TO_ID(IPL_MAGICRES_CURSE)
		ITEM_STRING_TO_ID(IPL_ALLRES_CURSE)
		ITEM_STRING_TO_ID(IPL_DEVASTATION)
		ITEM_STRING_TO_ID(IPL_DECAY)
		ITEM_STRING_TO_ID(IPL_PERIL)
		ITEM_STRING_TO_ID(IPL_JESTERS)
		ITEM_STRING_TO_ID(IPL_CRYSTALLINE)
		ITEM_STRING_TO_ID(IPL_DOPPELGANGER)
		ITEM_STRING_TO_ID(IPL_ACDEMON)
		ITEM_STRING_TO_ID(IPL_ACUNDEAD)
		ITEM_STRING_TO_ID(IPL_MANATOLIFE)
		ITEM_STRING_TO_ID(IPL_LIFETOMANA)
		ITEM_STRING_TO_ID(IPL_INVALID)

		devilution::app_fatal("Invalid item effect! %s", str.c_str());
	    return IPL_INVALID;
	}

	ItemData *AllItemsList;
    UItemStruct *UniqueItemList;

	void InitItemTable(void)
    {
	    AllItemsList = new ItemData[itemsTable->NumRows()];
	    UniqueItemList = new UItemStruct[uniqueItemTable->NumRows()];

		// Load in the unique item table.
		for (int i = 0; i < uniqueItemTable->NumRows(); i++) {
		    UniqueItemList[i].UIName = uniqueItemTable->GetValue("UIName", i);
		    UniqueItemList[i].UIItemId = UniqueBaseItemFromText(uniqueItemTable->GetValue("UIItemId", i));
		    UniqueItemList[i].UIMinLvl = uniqueItemTable->GetInt("UIMinLvl", i);
		    UniqueItemList[i].UINumPL = uniqueItemTable->GetInt("UINumPL", i);
		    UniqueItemList[i].UIValue = uniqueItemTable->GetInt("UIValue", i);

			for (int d = 0; d < 6; d++) {
			    char PowerTypeName[512];
			    char PowerParam1Name[512];
			    char PowerParam2Name[512];

				sprintf(PowerTypeName, "ItemPower%dType", d);
			    sprintf(PowerParam1Name, "ItemPower%dParam1", d);
			    sprintf(PowerParam2Name, "ItemPower%dParam2", d);

				UniqueItemList[i].powers[d].type = ItemEffectTypeToString(uniqueItemTable->GetValue(PowerTypeName, i));
			    UniqueItemList[i].powers[d].param1 = uniqueItemTable->GetInt(PowerParam1Name, i);
			    UniqueItemList[i].powers[d].param2 = uniqueItemTable->GetInt(PowerParam2Name, i);
			}
		}

		// Load in the base item table.
		for (int i = 0; i < itemsTable->NumRows(); i++) {
		    AllItemsList[i].iRnd = ItemDropRateFromText(itemsTable->GetValue("iRnd", i));
		    AllItemsList[i].iClass = ItemClassFromText(itemsTable->GetValue("iClass", i));
		    AllItemsList[i].iLoc = ItemEquipTypeFromText(itemsTable->GetValue("iLoc", i));
		    AllItemsList[i].iCurs = ItemCursorGraphicFromText(itemsTable->GetValue("iCurs", i));
		    AllItemsList[i].itype = ItemTypeFromText(itemsTable->GetValue("itype", i));
		    AllItemsList[i].iItemId = UniqueBaseItemFromText(itemsTable->GetValue("iItemId", i));
		    AllItemsList[i].iName = itemsTable->GetValue("iName", i);
		    AllItemsList[i].iSName = itemsTable->GetValue("iSName", i);
		    AllItemsList[i].iMinMLvl = itemsTable->GetInt("iMinMLvl", i);
		    AllItemsList[i].iDurability = itemsTable->GetInt("iDurability", i);
		    AllItemsList[i].iMinDam = itemsTable->GetInt("iMinDam", i);
		    AllItemsList[i].iMaxDam = itemsTable->GetInt("iMaxDam", i);
		    AllItemsList[i].iMinAC = itemsTable->GetInt("iMinAC", i);
		    AllItemsList[i].iMaxAC = itemsTable->GetInt("iMaxAC", i);
		    AllItemsList[i].iMinStr = itemsTable->GetInt("iMinStr", i);
		    AllItemsList[i].iMinMag = itemsTable->GetInt("iMinMag", i);
		    AllItemsList[i].iMinDex = itemsTable->GetInt("iMinDex", i);
		    AllItemsList[i].iFlags = ItemSpecialEffectFromText(itemsTable->GetValue("iFlags", i));
		    AllItemsList[i].iMiscId = ItemMisicIdFromText(itemsTable->GetValue("iMiscId", i));
		    AllItemsList[i].iSpell = SpellIdFromString(itemsTable->GetValue("iSpell", i));
		    AllItemsList[i].iUsable = itemsTable->GetInt("iUsable", i);
		    AllItemsList[i].iValue = itemsTable->GetInt("iValue", i);
		}
	}

/** Contains the data related to each item prefix. */
const PLStruct ItemPrefixes[] = {
	// clang-format off
	// PLName,              { type,                 param1, param2 }, PLMinLvl, PLIType,                                                         PLGOE,    PLDouble, PLOk,  minVal, maxVal, multVal
	// TRANSLATORS: Item prefix section.
	{ N_("Tin"),            { IPL_TOHIT_CURSE,           6,     10 },        3,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  true,     false,      0,      0,      -3 },
	{ N_("Brass"),          { IPL_TOHIT_CURSE,           1,      5 },        1,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  true,     false,      0,      0,      -2 },
	{ N_("Bronze"),         { IPL_TOHIT,                 1,      5 },        1,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  true,     true,     100,    500,       2 },
	{ N_("Iron"),           { IPL_TOHIT,                 6,     10 },        4,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  true,     true,     600,   1000,       3 },
	{ N_("Steel"),          { IPL_TOHIT,                11,     15 },        6,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  true,     true,    1100,   1500,       5 },
	{ N_("Silver"),         { IPL_TOHIT,                16,     20 },        9,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_GOOD, true,     true,    1600,   2000,       7 },
	{ N_("Gold"),           { IPL_TOHIT,                21,     30 },       12,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_GOOD, true,     true,    2100,   3000,       9 },
	{ N_("Platinum"),       { IPL_TOHIT,                31,     40 },       16,                       PLT_WEAP |             PLT_BOW           , GOE_GOOD, true,     true,    3100,   4000,      11 },
	{ N_("Mithril"),        { IPL_TOHIT,                41,     60 },       20,                       PLT_WEAP |             PLT_BOW           , GOE_GOOD, true,     true,    4100,   6000,      13 },
	{ N_("Meteoric"),       { IPL_TOHIT,                61,     80 },       23,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,    6100,  10000,      15 },
	{ N_("Weird"),          { IPL_TOHIT,                81,    100 },       35,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,   10100,  14000,      17 },
	{ N_("Strange"),        { IPL_TOHIT,               101,    150 },       60,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,   14100,  20000,      20 },
	{ N_("Useless"),        { IPL_DAMP_CURSE,          100,    100 },        5,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,      0,      0,      -8 },
	{ N_("Bent"),           { IPL_DAMP_CURSE,           50,     75 },        3,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,      0,      0,      -4 },
	{ N_("Weak"),           { IPL_DAMP_CURSE,           25,     45 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,      0,      0,      -3 },
	{ N_("Jagged"),         { IPL_DAMP,                 20,     35 },        4,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,     250,    450,       3 },
	{ N_("Deadly"),         { IPL_DAMP,                 36,     50 },        6,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,     500,    700,       4 },
	{ N_("Heavy"),          { IPL_DAMP,                 51,     65 },        9,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,     750,    950,       5 },
	{ N_("Vicious"),        { IPL_DAMP,                 66,     80 },       12,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_EVIL, true,     true,    1000,   1450,       8 },
	{ N_("Brutal"),         { IPL_DAMP,                 81,     95 },       16,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,    1500,   1950,      10 },
	{ N_("Massive"),        { IPL_DAMP,                 96,    110 },       20,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,    2000,   2450,      13 },
	{ N_("Savage"),         { IPL_DAMP,                111,    125 },       23,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,    2500,   3000,      15 },
	{ N_("Ruthless"),       { IPL_DAMP,                126,    150 },       35,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,   10100,  15000,      17 },
	{ N_("Merciless"),      { IPL_DAMP,                151,    175 },       60,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  true,     true,   15000,  20000,      20 },
	{ N_("Clumsy"),         { IPL_TOHIT_DAMP_CURSE,     50,     75 },        5,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,      0,      0,      -7 },
	{ N_("Dull"),           { IPL_TOHIT_DAMP_CURSE,     25,     45 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,      0,      0,      -5 },
	{ N_("Sharp"),          { IPL_TOHIT_DAMP,           20,     35 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     false,    350,    950,       5 },
	{ N_("Fine"),           { IPL_TOHIT_DAMP,           36,     50 },        6,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,    1100,   1700,       7 },
	{ N_("Warrior's"),      { IPL_TOHIT_DAMP,           51,     65 },       10,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  true,     true,    1850,   2450,      13 },
	{ N_("Soldier's"),      { IPL_TOHIT_DAMP,           66,     80 },       15,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,    2600,   3950,      17 },
	{ N_("Lord's"),         { IPL_TOHIT_DAMP,           81,     95 },       19,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,    4100,   5950,      21 },
	{ N_("Knight's"),       { IPL_TOHIT_DAMP,           96,    110 },       23,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,    6100,   8450,      26 },
	{ N_("Master's"),       { IPL_TOHIT_DAMP,          111,    125 },       28,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,    8600,  13000,      30 },
	{ N_("Champion's"),     { IPL_TOHIT_DAMP,          126,    150 },       40,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,   15200,  24000,      33 },
	{ N_("King's"),         { IPL_TOHIT_DAMP,          151,    175 },       28,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  true,     true,   24100,  35000,      38 },
	{ N_("Vulnerable"),     { IPL_ACP_CURSE,            51,    100 },        3, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     false,      0,      0,      -3 },
	{ N_("Rusted"),         { IPL_ACP_CURSE,            25,     50 },        1, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     false,      0,      0,      -2 },
	{ N_("Fine"),           { IPL_ACP,                  20,     30 },        1, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     true,      20,    100,       2 },
	{ N_("Strong"),         { IPL_ACP,                  31,     40 },        3, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     true,     120,    200,       3 },
	{ N_("Grand"),          { IPL_ACP,                  41,     55 },        6, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     true,     220,    300,       5 },
	{ N_("Valiant"),        { IPL_ACP,                  56,     70 },       10, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  true,     true,     320,    400,       7 },
	{ N_("Glorious"),       { IPL_ACP,                  71,     90 },       14, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,     420,    600,       9 },
	{ N_("Blessed"),        { IPL_ACP,                  91,    110 },       19, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,     620,    800,      11 },
	{ N_("Saintly"),        { IPL_ACP,                 111,    130 },       24, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,     820,   1200,      13 },
	{ N_("Awesome"),        { IPL_ACP,                 131,    150 },       28, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,    1220,   2000,      15 },
	{ N_("Holy"),           { IPL_ACP,                 151,    170 },       35, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,    5200,   6000,      17 },
	{ N_("Godly"),          { IPL_ACP,                 171,    200 },       60, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, true,     true,    6200,   7000,      20 },
	{ N_("Red"),            { IPL_FIRERES,              10,     20 },        4, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     500,   1500,       2 },
	{ N_("Crimson"),        { IPL_FIRERES,              21,     30 },       10, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2100,   3000,       2 },
	{ N_("Crimson"),        { IPL_FIRERES,              31,     40 },       16, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3100,   4000,       2 },
	{ N_("Garnet"),         { IPL_FIRERES,              41,     50 },       20, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    8200,  12000,       3 },
	{ N_("Ruby"),           { IPL_FIRERES,              51,     60 },       26, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   17100,  20000,       5 },
	{ N_("Blue"),           { IPL_LIGHTRES,             10,     20 },        4, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     500,   1500,       2 },
	{ N_("Azure"),          { IPL_LIGHTRES,             21,     30 },       10, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2100,   3000,       2 },
	{ N_("Lapis"),          { IPL_LIGHTRES,             31,     40 },       16, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3100,   4000,       2 },
	{ N_("Cobalt"),         { IPL_LIGHTRES,             41,     50 },       20, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    8200,  12000,       3 },
	{ N_("Sapphire"),       { IPL_LIGHTRES,             51,     60 },       26, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   17100,  20000,       5 },
	{ N_("White"),          { IPL_MAGICRES,             10,     20 },        4, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     500,   1500,       2 },
	{ N_("Pearl"),          { IPL_MAGICRES,             21,     30 },       10, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2100,   3000,       2 },
	{ N_("Ivory"),          { IPL_MAGICRES,             31,     40 },       16, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3100,   4000,       2 },
	{ N_("Crystal"),        { IPL_MAGICRES,             41,     50 },       20, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    8200,  12000,       3 },
	{ N_("Diamond"),        { IPL_MAGICRES,             51,     60 },       26, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   17100,  20000,       5 },
	{ N_("Topaz"),          { IPL_ALLRES,               10,     15 },        8, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2000,   5000,       3 },
	{ N_("Amber"),          { IPL_ALLRES,               16,     20 },       12, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    7400,  10000,       3 },
	{ N_("Jade"),           { IPL_ALLRES,               21,     30 },       18, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   11000,  15000,       3 },
	{ N_("Obsidian"),       { IPL_ALLRES,               31,     40 },       24, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   24000,  40000,       4 },
	{ N_("Emerald"),        { IPL_ALLRES,               41,     50 },       31,            PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,   61000,  75000,       7 },
	{ N_("Hyena's"),        { IPL_MANA_CURSE,           11,     25 },        4,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    false,    100,   1000,      -2 },
	{ N_("Frog's"),         { IPL_MANA_CURSE,            1,     10 },        1,                                  PLT_STAFF |           PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{ N_("Spider's"),       { IPL_MANA,                 10,     15 },        1,                                  PLT_STAFF |           PLT_MISC, GOE_EVIL, false,    true,     500,   1000,       2 },
	{ N_("Raven's"),        { IPL_MANA,                 15,     20 },        5,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,    1100,   2000,       3 },
	{ N_("Snake's"),        { IPL_MANA,                 21,     30 },        9,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,    2100,   4000,       5 },
	{ N_("Serpent's"),      { IPL_MANA,                 30,     40 },       15,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,    4100,   6000,       7 },
	{ N_("Drake's"),        { IPL_MANA,                 41,     50 },       21,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,    6100,  10000,       9 },
	{ N_("Dragon's"),       { IPL_MANA,                 51,     60 },       27,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,   10100,  15000,      11 },
	{ N_("Wyrm's"),         { IPL_MANA,                 61,     80 },       35,                                  PLT_STAFF                     , GOE_ANY,  false,    true,   15100,  19000,      12 },
	{ N_("Hydra's"),        { IPL_MANA,                 81,    100 },       60,                                  PLT_STAFF                     , GOE_ANY,  false,    true,   19100,  30000,      13 },
	{ N_("Angel's"),        { IPL_SPLLVLADD,             1,      1 },       15,                                  PLT_STAFF                     , GOE_GOOD, false,    true,   25000,  25000,       2 },
	{ N_("Arch-Angel's"),   { IPL_SPLLVLADD,             2,      2 },       25,                                  PLT_STAFF                     , GOE_GOOD, false,    true,   50000,  50000,       3 },
	{ N_("Plentiful"),      { IPL_CHARGES,               2,      2 },        4,                                  PLT_STAFF                     , GOE_ANY,  false,    true,    2000,   2000,       2 },
	{ N_("Bountiful"),      { IPL_CHARGES,               3,      3 },        9,                                  PLT_STAFF                     , GOE_ANY,  false,    true,    3000,   3000,       3 },
	{ N_("Flaming"),        { IPL_FIREDAM,               1,     10 },        7,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,    5000,   5000,       2 },
	{ N_("Lightning"),      { IPL_LIGHTDAM,              2,     20 },       18,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,   10000,  10000,       2 },
	{ N_("Jester's"),       { IPL_JESTERS,               1,      1 },        7,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    1200,   1200,       3 },
	{ N_("Crystalline"),    { IPL_CRYSTALLINE,          30,     70 },        5,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    1000,   3000,       3 },
	// TRANSLATORS: Item prefix section end.
	{ N_("Doppelganger's"), { IPL_DOPPELGANGER,         81,     95 },       11,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,    2000,   2400,      10 },
	{ "",                   { IPL_INVALID,               0,      0 },        0, 0                                                              , GOE_ANY,  false,    false,      0,      0,       0 },
	// clang-format on
};

/** Contains the data related to each item suffix. */
const PLStruct ItemSuffixes[] = {
	// clang-format off
	// PLName,              { type,               param1, param2 },   PLMinLvl, PLIType,                                                         PLGOE,    PLDouble, PLOk,  minVal, maxVal, multVal
	// TRANSLATORS: Item suffix section. All items will have a word binding word. (Format: {:s} of {:s} - e.g. Rags of Valor)
	{  N_("quality"),       { IPL_DAMMOD,              1,        2 },        2,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,     100,    200,       2 },
	{  N_("maiming"),       { IPL_DAMMOD,              3,        5 },        7,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,    1300,   1500,       3 },
	{  N_("slaying"),       { IPL_DAMMOD,              6,        8 },       15,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    2600,   3000,       5 },
	{  N_("gore"),          { IPL_DAMMOD,              9,       12 },       25,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    4100,   5000,       8 },
	{  N_("carnage"),       { IPL_DAMMOD,             13,       16 },       35,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    5100,  10000,      10 },
	{  N_("slaughter"),     { IPL_DAMMOD,             17,       20 },       60,                       PLT_WEAP                                 , GOE_ANY,  false,    true,   10100,  15000,      13 },
	{  N_("pain"),          { IPL_GETHIT_CURSE,        2,        4 },        4, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -4 },
	{  N_("tears"),         { IPL_GETHIT_CURSE,        1,        1 },        2, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("health"),        { IPL_GETHIT,              1,        1 },        2, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_GOOD, false,    true,     200,    200,       2 },
	{  N_("protection"),    { IPL_GETHIT,              2,        2 },        6, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, false,    true,     400,    800,       4 },
	{  N_("absorption"),    { IPL_GETHIT,              3,        3 },       12, PLT_ARMO | PLT_SHLD                                            , GOE_GOOD, false,    true,    1001,   2500,      10 },
	{  N_("deflection"),    { IPL_GETHIT,              4,        4 },       20, PLT_ARMO                                                       , GOE_GOOD, false,    true,    2500,   6500,      15 },
	{  N_("osmosis"),       { IPL_GETHIT,              5,        6 },       50, PLT_ARMO                                                       , GOE_GOOD, false,    true,    7500,  10000,      20 },
	{  N_("frailty"),       { IPL_STR_CURSE,           6,       10 },        3, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -3 },
	{  N_("weakness"),      { IPL_STR_CURSE,           1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("strength"),      { IPL_STR,                 1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     200,   1000,       2 },
	{  N_("might"),         { IPL_STR,                 6,       10 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    1200,   2000,       3 },
	{  N_("power"),         { IPL_STR,                11,       15 },       11, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2200,   3000,       4 },
	{  N_("giants"),        { IPL_STR,                16,       20 },       17, PLT_ARMO |            PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3200,   5000,       7 },
	{  N_("titans"),        { IPL_STR,                21,       30 },       23,                       PLT_WEAP |                       PLT_MISC, GOE_ANY,  false,    true,    5200,  10000,      10 },
	{  N_("paralysis"),     { IPL_DEX_CURSE,           6,       10 },        3, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -3 },
	{  N_("atrophy"),       { IPL_DEX_CURSE,           1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("dexterity"),     { IPL_DEX,                 1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     200,   1000,       2 },
	{  N_("skill"),         { IPL_DEX,                 6,       10 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    1200,   2000,       3 },
	{  N_("accuracy"),      { IPL_DEX,                11,       15 },       11, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2200,   3000,       4 },
	{  N_("precision"),     { IPL_DEX,                16,       20 },       17, PLT_ARMO |            PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3200,   5000,       7 },
	{  N_("perfection"),    { IPL_DEX,                21,       30 },       23,                                              PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    5200,  10000,      10 },
	{  N_("the fool"),      { IPL_MAG_CURSE,           6,       10 },        3, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -3 },
	{  N_("dyslexia"),      { IPL_MAG_CURSE,           1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("magic"),         { IPL_MAG,                 1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     200,   1000,       2 },
	{  N_("the mind"),      { IPL_MAG,                 6,       10 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    1200,   2000,       3 },
	{  N_("brilliance"),    { IPL_MAG,                11,       15 },       11, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    2200,   3000,       4 },
	{  N_("sorcery"),       { IPL_MAG,                16,       20 },       17, PLT_ARMO |            PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    3200,   5000,       7 },
	{  N_("wizardry"),      { IPL_MAG,                21,       30 },       23,                                  PLT_STAFF |           PLT_MISC, GOE_ANY,  false,    true,    5200,  10000,      10 },
	{  N_("illness"),       { IPL_VIT_CURSE,           6,       10 },        3, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -3 },
	{  N_("disease"),       { IPL_VIT_CURSE,           1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("vitality"),      { IPL_VIT,                 1,        5 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_GOOD, false,    true,     200,   1000,       2 },
	{  N_("zest"),          { IPL_VIT,                 6,       10 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_GOOD, false,    true,    1200,   2000,       3 },
	{  N_("vim"),           { IPL_VIT,                11,       15 },       11, PLT_ARMO | PLT_SHLD | PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_GOOD, false,    true,    2200,   3000,       4 },
	{  N_("vigor"),         { IPL_VIT,                16,       20 },       17, PLT_ARMO | PLT_WEAP |                        PLT_BOW | PLT_MISC, GOE_GOOD, false,    true,    3200,   5000,       7 },
	{  N_("life"),          { IPL_VIT,                21,       30 },       23,                                                        PLT_MISC, GOE_GOOD, false,    true,    5200,  10000,      10 },
	{  N_("trouble"),       { IPL_ATTRIBS_CURSE,       6,       10 },       12, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,     -10 },
	{  N_("the pit"),       { IPL_ATTRIBS_CURSE,       1,        5 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -5 },
	{  N_("the sky"),       { IPL_ATTRIBS,             1,        3 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,     800,   4000,       5 },
	{  N_("the moon"),      { IPL_ATTRIBS,             4,        7 },       11, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF | PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    4800,   8000,      10 },
	{  N_("the stars"),     { IPL_ATTRIBS,             8,       11 },       17, PLT_ARMO |            PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,    8800,  12000,      15 },
	{  N_("the heavens"),   { IPL_ATTRIBS,            12,       15 },       25,                       PLT_WEAP |             PLT_BOW | PLT_MISC, GOE_ANY,  false,    true,   12800,  20000,      20 },
	{  N_("the zodiac"),    { IPL_ATTRIBS,            16,       20 },       30,                                                        PLT_MISC, GOE_ANY,  false,    true,   20800,  40000,      30 },
	{  N_("the vulture"),   { IPL_LIFE_CURSE,         11,       25 },        4, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -4 },
	{  N_("the jackal"),    { IPL_LIFE_CURSE,          1,       10 },        1, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("the fox"),       { IPL_LIFE,               10,       15 },        1, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,     100,   1000,       2 },
	{  N_("the jaguar"),    { IPL_LIFE,               16,       20 },        5, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,    1100,   2000,       3 },
	{  N_("the eagle"),     { IPL_LIFE,               21,       30 },        9, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,    2100,   4000,       5 },
	{  N_("the wolf"),      { IPL_LIFE,               30,       40 },       15, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,    4100,   6000,       7 },
	{  N_("the tiger"),     { IPL_LIFE,               41,       50 },       21, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,    6100,  10000,       9 },
	{  N_("the lion"),      { IPL_LIFE,               51,       60 },       27, PLT_ARMO |                                             PLT_MISC, GOE_ANY,  false,    true,   10100,  15000,      11 },
	{  N_("the mammoth"),   { IPL_LIFE,               61,       80 },       35, PLT_ARMO                                                       , GOE_ANY,  false,    true,   15100,  19000,      12 },
	{  N_("the whale"),     { IPL_LIFE,               81,      100 },       60, PLT_ARMO                                                       , GOE_ANY,  false,    true,   19100,  30000,      13 },
	{  N_("fragility"),     { IPL_DUR_CURSE,         100,      100 },        3, PLT_ARMO | PLT_SHLD | PLT_WEAP                                 , GOE_EVIL, false,    false,      0,      0,      -4 },
	{  N_("brittleness"),   { IPL_DUR_CURSE,          26,       75 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP                                 , GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("sturdiness"),    { IPL_DUR,                26,       75 },        1, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,     100,    100,       2 },
	{  N_("craftsmanship"), { IPL_DUR,                51,      100 },        6, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,     200,    200,       2 },
	{  N_("structure"),     { IPL_DUR,               101,      200 },       12, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,     300,    300,       2 },
	{  N_("the ages"),      { IPL_INDESTRUCTIBLE,      0,        0 },       25, PLT_ARMO | PLT_SHLD | PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,     600,    600,       5 },
	{  N_("the dark"),      { IPL_LIGHT_CURSE,         4,        4 },        6, PLT_ARMO |            PLT_WEAP |                       PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -3 },
	{  N_("the night"),     { IPL_LIGHT_CURSE,         2,        2 },        3, PLT_ARMO |            PLT_WEAP |                       PLT_MISC, GOE_EVIL, false,    false,      0,      0,      -2 },
	{  N_("light"),         { IPL_LIGHT,               2,        2 },        4, PLT_ARMO |            PLT_WEAP |                       PLT_MISC, GOE_GOOD, false,    true,     750,    750,       2 },
	{  N_("radiance"),      { IPL_LIGHT,               4,        4 },        8, PLT_ARMO |            PLT_WEAP |                       PLT_MISC, GOE_GOOD, false,    true,    1500,   1500,       3 },
	{  N_("flame"),         { IPL_FIRE_ARROWS,         1,        3 },        1,                                              PLT_BOW           , GOE_ANY,  false,    true,    2000,   2000,       2 },
	{  N_("fire"),          { IPL_FIRE_ARROWS,         1,        6 },       11,                                              PLT_BOW           , GOE_ANY,  false,    true,    4000,   4000,       4 },
	{  N_("burning"),       { IPL_FIRE_ARROWS,         1,       16 },       35,                                              PLT_BOW           , GOE_ANY,  false,    true,    6000,   6000,       6 },
	{  N_("shock"),         { IPL_LIGHT_ARROWS,        1,        6 },       13,                                              PLT_BOW           , GOE_ANY,  false,    true,    6000,   6000,       2 },
	{  N_("lightning"),     { IPL_LIGHT_ARROWS,        1,       10 },       21,                                              PLT_BOW           , GOE_ANY,  false,    true,    8000,   8000,       4 },
	{  N_("thunder"),       { IPL_LIGHT_ARROWS,        1,       20 },       60,                                              PLT_BOW           , GOE_ANY,  false,    true,   12000,  12000,       6 },
	{  N_("many"),          { IPL_DUR,               100,      100 },        3,                                              PLT_BOW           , GOE_ANY,  false,    true,     750,    750,       2 },
	{  N_("plenty"),        { IPL_DUR,               200,      200 },        7,                                              PLT_BOW           , GOE_ANY,  false,    true,    1500,   1500,       3 },
	{  N_("thorns"),        { IPL_THORNS,              1,        3 },        1, PLT_ARMO | PLT_SHLD                                            , GOE_ANY,  false,    true,     500,    500,       2 },
	{  N_("corruption"),    { IPL_NOMANA,              0,        0 },        5, PLT_ARMO | PLT_SHLD | PLT_WEAP                                 , GOE_EVIL, false,    false,  -1000,  -1000,       2 },
	{  N_("thieves"),       { IPL_ABSHALFTRAP,         0,        0 },       11, PLT_ARMO | PLT_SHLD |                                  PLT_MISC, GOE_ANY,  false,    true,    1500,   1500,       2 },
	{  N_("the bear"),      { IPL_KNOCKBACK,           0,        0 },        5,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_EVIL, false,    true,     750,    750,       2 },
	{  N_("the bat"),       { IPL_STEALMANA,           3,        3 },        8,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    7500,   7500,       3 },
	{  N_("vampires"),      { IPL_STEALMANA,           5,        5 },       19,                       PLT_WEAP                                 , GOE_ANY,  false,    true,   15000,  15000,       3 },
	{  N_("the leech"),     { IPL_STEALLIFE,           3,        3 },        8,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    7500,   7500,       3 },
	{  N_("blood"),         { IPL_STEALLIFE,           5,        5 },       19,                       PLT_WEAP                                 , GOE_ANY,  false,    true,   15000,  15000,       3 },
	{  N_("piercing"),      { IPL_TARGAC,              1,        1 },        1,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  false,    true,    1000,   1000,       3 },
	{  N_("puncturing"),    { IPL_TARGAC,              2,        2 },        9,                       PLT_WEAP |             PLT_BOW           , GOE_ANY,  false,    true,    2000,   2000,       6 },
	{  N_("bashing"),       { IPL_TARGAC,              3,        3 },       17,                       PLT_WEAP                                 , GOE_ANY,  false,    true,    4000,   4000,      12 },
	{  N_("readiness"),     { IPL_FASTATTACK,          1,        1 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,    2000,   2000,       2 },
	{  N_("swiftness"),     { IPL_FASTATTACK,          2,        2 },       10,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,    4000,   4000,       4 },
	{  N_("speed"),         { IPL_FASTATTACK,          3,        3 },       19,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,    8000,   8000,       8 },
	{  N_("haste"),         { IPL_FASTATTACK,          4,        4 },       27,                       PLT_WEAP | PLT_STAFF                     , GOE_ANY,  false,    true,   16000,  16000,      16 },
	{  N_("balance"),       { IPL_FASTRECOVER,         1,        1 },        1, PLT_ARMO |                                             PLT_MISC, GOE_ANY,  false,    true,    2000,   2000,       2 },
	{  N_("stability"),     { IPL_FASTRECOVER,         2,        2 },       10, PLT_ARMO |                                             PLT_MISC, GOE_ANY,  false,    true,    4000,   4000,       4 },
	{  N_("harmony"),       { IPL_FASTRECOVER,         3,        3 },       20, PLT_ARMO |                                             PLT_MISC, GOE_ANY,  false,    true,    8000,   8000,       8 },
	{  N_("blocking"),      { IPL_FASTBLOCK,           1,        1 },        5,            PLT_SHLD                                            , GOE_ANY,  false,    true,    4000,   4000,       4 },
	{  N_("devastation"),   { IPL_DEVASTATION,         1,        1 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,    1200,   1200,       3 },
	{  N_("decay"),         { IPL_DECAY,             150,      250 },        1,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,     200,    200,       2 },
	// TRANSLATORS: Item suffix section end.
	{  N_("peril"),         { IPL_PERIL,               1,        1 },        5,                       PLT_WEAP | PLT_STAFF | PLT_BOW           , GOE_ANY,  false,    true,     500,    500,       1 },
	{  "",                  { IPL_INVALID,             0,        0 },        0, 0                                                              , GOE_ANY,  false,    false,      0,      0,       0 },
	// clang-format on
};

} // namespace devilution
