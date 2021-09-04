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

	ItemData *AllItemsList;

	void InitItemTable(void)
    {
	    AllItemsList = new ItemData[itemsTable->NumRows()];

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

/** Contains the data related to each unique item ID. */
const UItemStruct UniqueItemList[] = {
	// clang-format off
	// UIName,                        UIItemId,     UIMinLvl, UINumPL, UIValue,   ItemPower[0],                          ItemPower[1],                          ItemPower[2],                          ItemPower[3],                          ItemPower[4],                          ItemPower[5]
	// TRANSLATORS: Unique Item section
	{  N_("The Butcher's Cleaver"),   UITYPE_CLEAVER,      1,       3,    3650, { { IPL_STR,             10,       10 }, { IPL_SETDAM,           4,       24 }, { IPL_SETDUR,          10,       10 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Undead Crown"),        UITYPE_SKCROWN,      1,       3,   16650, { { IPL_RNDSTEALLIFE,     0,        0 }, { IPL_SETAC,            8,        8 }, { IPL_INVCURS,         77,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Empyrean Band"),           UITYPE_INFRARING,    1,       4,    8000, { { IPL_ATTRIBS,          2,        2 }, { IPL_LIGHT,            2,        2 }, { IPL_FASTRECOVER,      1,        1 }, { IPL_ABSHALFTRAP,      0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Optic Amulet"),            UITYPE_OPTAMULET,    1,       5,    9750, { { IPL_LIGHT,            2,        2 }, { IPL_LIGHTRES,        20,       20 }, { IPL_GETHIT,           1,        1 }, { IPL_MAG,              5,        5 }, { IPL_INVCURS,         44,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of Truth"),           UITYPE_TRING,        1,       4,    9100, { { IPL_LIFE,            10,       10 }, { IPL_GETHIT,           1,        1 }, { IPL_ALLRES,          10,       10 }, { IPL_INVCURS,         10,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Harlequin Crest"),         UITYPE_HARCREST,     1,       6,    4000, { { IPL_AC_CURSE,         3,        3 }, { IPL_GETHIT,           1,        1 }, { IPL_ATTRIBS,          2,        2 }, { IPL_LIFE,             7,        7 }, { IPL_MANA,             7,        7 }, { IPL_INVCURS,         81,        0 } } },
	{  N_("Veil of Steel"),           UITYPE_STEELVEIL,    1,       6,   63800, { { IPL_ALLRES,          50,       50 }, { IPL_LIGHT_CURSE,      2,        2 }, { IPL_ACP,             60,       60 }, { IPL_MANA_CURSE,      30,       30 }, { IPL_STR,             15,       15 }, { IPL_VIT,             15,       15 } } },
	{  N_("Arkaine's Valor"),         UITYPE_ARMOFVAL,     1,       4,   42000, { { IPL_SETAC,           25,       25 }, { IPL_VIT,             10,       10 }, { IPL_GETHIT,           3,        3 }, { IPL_FASTRECOVER,      3,        3 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Griswold's Edge"),         UITYPE_GRISWOLD,     1,       6,   42000, { { IPL_FIREDAM,          1,       10 }, { IPL_TOHIT,           25,       25 }, { IPL_FASTATTACK,       2,        2 }, { IPL_KNOCKBACK,        0,        0 }, { IPL_MANA,            20,       20 }, { IPL_LIFE_CURSE,      20,       20 } } },
	{  N_("Bovine Plate"),            UITYPE_BOVINE,       1,       6,     400, { { IPL_SETAC,          150,      150 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_LIGHT,            5,        5 }, { IPL_ALLRES,          30,       30 }, { IPL_MANA_CURSE,      50,       50 }, { IPL_SPLLVLADD,       -2,       -2 } } },
	{  N_("The Rift Bow"),            UITYPE_SHORTBOW,     1,       3,    1800, { { IPL_RNDARROWVEL,      0,        0 }, { IPL_DAMMOD,           2,        2 }, { IPL_DEX_CURSE,        3,        3 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Needler"),             UITYPE_SHORTBOW,     2,       4,    8900, { { IPL_TOHIT,           50,       50 }, { IPL_SETDAM,           1,        3 }, { IPL_FASTATTACK,       2,        2 }, { IPL_INVCURS,        158,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Celestial Bow"),       UITYPE_LONGBOW,      2,       4,    1200, { { IPL_NOMINSTR,         0,        0 }, { IPL_DAMMOD,           2,        2 }, { IPL_SETAC,            5,        5 }, { IPL_INVCURS,        133,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Deadly Hunter"),           UITYPE_COMPBOW,      3,       4,    8750, { { IPL_3XDAMVDEM,       10,       10 }, { IPL_TOHIT,           20,       20 }, { IPL_MAG_CURSE,        5,        5 }, { IPL_INVCURS,        108,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Bow of the Dead"),         UITYPE_COMPBOW,      5,       6,    2500, { { IPL_TOHIT,           10,       10 }, { IPL_DEX,              4,        4 }, { IPL_VIT_CURSE,        3,        3 }, { IPL_LIGHT_CURSE,      2,        2 }, { IPL_SETDUR,          30,       30 }, { IPL_INVCURS,        108,        0 } } },
	{  N_("The Blackoak Bow"),        UITYPE_LONGBOW,      5,       4,    2500, { { IPL_DEX,             10,       10 }, { IPL_VIT_CURSE,       10,       10 }, { IPL_DAMP,            50,       50 }, { IPL_LIGHT_CURSE,      1,        1 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Flamedart"),               UITYPE_HUNTBOW,     10,       4,   14250, { { IPL_FIRE_ARROWS,      0,        0 }, { IPL_FIREDAM,          1,        6 }, { IPL_TOHIT,           20,       20 }, { IPL_FIRERES,         40,       40 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Fleshstinger"),            UITYPE_LONGBOW,     13,       4,   16500, { { IPL_DEX,             15,       15 }, { IPL_TOHIT,           40,       40 }, { IPL_DAMP,            80,       80 }, { IPL_DUR,              6,        6 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Windforce"),               UITYPE_WARBOW,      17,       4,   37750, { { IPL_STR,              5,        5 }, { IPL_DAMP,           200,      200 }, { IPL_KNOCKBACK,        0,        0 }, { IPL_INVCURS,        164,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Eaglehorn"),               UITYPE_BATTLEBOW,   26,       5,   42500, { { IPL_DEX,             20,       20 }, { IPL_TOHIT,           50,       50 }, { IPL_DAMP,           100,      100 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        108,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gonnagal's Dirk"),         UITYPE_DAGGER,       1,       5,    7040, { { IPL_DEX_CURSE,        5,        5 }, { IPL_DAMMOD,           4,        4 }, { IPL_FASTATTACK,       2,        2 }, { IPL_FIRERES,         25,       25 }, { IPL_INVCURS,         54,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Defender"),            UITYPE_SABRE,        1,       3,    2000, { { IPL_SETAC,            5,        5 }, { IPL_VIT,              5,        5 }, { IPL_TOHIT_CURSE,      5,        5 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gryphons Claw"),           UITYPE_FALCHION,     1,       4,    1000, { { IPL_DAMP,           100,      100 }, { IPL_MAG_CURSE,        2,        2 }, { IPL_DEX_CURSE,        5,        5 }, { IPL_INVCURS,         68,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Black Razor"),             UITYPE_DAGGER,       1,       4,    2000, { { IPL_DAMP,           150,      150 }, { IPL_VIT,              2,        2 }, { IPL_SETDUR,           5,        5 }, { IPL_INVCURS,         53,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gibbous Moon"),            UITYPE_BROADSWR,     2,       4,    6660, { { IPL_ATTRIBS,          2,        2 }, { IPL_DAMP,            25,       25 }, { IPL_MANA,            15,       15 }, { IPL_LIGHT_CURSE,      3,        3 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ice Shank"),               UITYPE_LONGSWR,      3,       3,    5250, { { IPL_FIRERES,         40,       40 }, { IPL_SETDUR,          15,       15 }, { IPL_STR,              5,       10 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Executioner's Blade"), UITYPE_FALCHION,     3,       5,    7080, { { IPL_DAMP,           150,      150 }, { IPL_LIFE_CURSE,      10,       10 }, { IPL_LIGHT_CURSE,      1,        1 }, { IPL_DUR,            200,      200 }, { IPL_INVCURS,         58,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Bonesaw"),             UITYPE_CLAYMORE,     6,       6,    4400, { { IPL_DAMMOD,          10,       10 }, { IPL_STR,             10,       10 }, { IPL_MAG_CURSE,        5,        5 }, { IPL_DEX_CURSE,        5,        5 }, { IPL_LIFE,            10,       10 }, { IPL_MANA_CURSE,      10,       10 } } },
	{  N_("Shadowhawk"),              UITYPE_BROADSWR,     8,       4,   13750, { { IPL_LIGHT_CURSE,      2,        2 }, { IPL_STEALLIFE,        5,        5 }, { IPL_TOHIT,           15,       15 }, { IPL_ALLRES,           5,        5 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Wizardspike"),             UITYPE_DAGGER,      11,       5,   12920, { { IPL_MAG,             15,       15 }, { IPL_MANA,            35,       35 }, { IPL_TOHIT,           25,       25 }, { IPL_ALLRES,          15,       15 }, { IPL_INVCURS,         50,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Lightsabre"),              UITYPE_SABRE,       13,       4,   19150, { { IPL_LIGHT,            2,        2 }, { IPL_LIGHTDAM,         1,       10 }, { IPL_TOHIT,           20,       20 }, { IPL_LIGHTRES,        50,       50 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Falcon's Talon"),      UITYPE_SCIMITAR,    15,       5,    7867, { { IPL_FASTATTACK,       4,        4 }, { IPL_TOHIT,           20,       20 }, { IPL_DAMP_CURSE,      33,       33 }, { IPL_DEX,             10,       10 }, { IPL_INVCURS,         68,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Inferno"),                 UITYPE_LONGSWR,     17,       4,   34600, { { IPL_FIREDAM,          2,       12 }, { IPL_LIGHT,            3,        3 }, { IPL_MANA,            20,       20 }, { IPL_FIRERES,         80,       80 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Doombringer"),             UITYPE_BASTARDSWR,  19,       5,   18250, { { IPL_TOHIT,           25,       25 }, { IPL_DAMP,           250,      250 }, { IPL_ATTRIBS_CURSE,    5,        5 }, { IPL_LIFE_CURSE,      25,       25 }, { IPL_LIGHT_CURSE,      2,        2 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Grizzly"),             UITYPE_TWOHANDSWR,  23,       6,   50000, { { IPL_STR,             20,       20 }, { IPL_VIT_CURSE,        5,        5 }, { IPL_DAMP,           200,      200 }, { IPL_KNOCKBACK,        0,        0 }, { IPL_DUR,            100,      100 }, { IPL_INVCURS,        160,        0 } } },
	{  N_("The Grandfather"),         UITYPE_GREATSWR,    27,       6,  119800, { { IPL_ONEHAND,          0,        0 }, { IPL_ATTRIBS,          5,        5 }, { IPL_TOHIT,           20,       20 }, { IPL_DAMP,            70,       70 }, { IPL_LIFE,            20,       20 }, { IPL_INVCURS,        161,        0 } } },
	{  N_("The Mangler"),             UITYPE_LARGEAXE,     2,       5,    2850, { { IPL_DAMP,           200,      200 }, { IPL_DEX_CURSE,        5,        5 }, { IPL_MAG_CURSE,        5,        5 }, { IPL_MANA_CURSE,      10,       10 }, { IPL_INVCURS,        144,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Sharp Beak"),              UITYPE_LARGEAXE,     2,       4,    2850, { { IPL_LIFE,            20,       20 }, { IPL_MAG_CURSE,       10,       10 }, { IPL_MANA_CURSE,      10,       10 }, { IPL_INVCURS,        143,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("BloodSlayer"),             UITYPE_BROADAXE,     3,       5,    2500, { { IPL_DAMP,           100,      100 }, { IPL_3XDAMVDEM,       50,       50 }, { IPL_ATTRIBS_CURSE,    5,        5 }, { IPL_SPLLVLADD,       -1,       -1 }, { IPL_INVCURS,        144,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Celestial Axe"),       UITYPE_BATTLEAXE,    4,       4,   14100, { { IPL_NOMINSTR,         0,        0 }, { IPL_TOHIT,           15,       15 }, { IPL_LIFE,            15,       15 }, { IPL_STR_CURSE,       15,       15 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Wicked Axe"),              UITYPE_LARGEAXE,     5,       6,   31150, { { IPL_TOHIT,           30,       30 }, { IPL_DEX,             10,       10 }, { IPL_VIT_CURSE,       10,       10 }, { IPL_GETHIT,           1,        6 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        143,        0 } } },
	{  N_("Stonecleaver"),            UITYPE_BROADAXE,     7,       5,   23900, { { IPL_LIFE,            30,       30 }, { IPL_TOHIT,           20,       20 }, { IPL_DAMP,            50,       50 }, { IPL_LIGHTRES,        40,       40 }, { IPL_INVCURS,        104,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Aguinara's Hatchet"),      UITYPE_SMALLAXE,    12,       3,   24800, { { IPL_SPLLVLADD,        1,        1 }, { IPL_MAG,             10,       10 }, { IPL_MAGICRES,        80,       80 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Hellslayer"),              UITYPE_BATTLEAXE,   15,       5,   26200, { { IPL_STR,              8,        8 }, { IPL_VIT,              8,        8 }, { IPL_DAMP,           100,      100 }, { IPL_LIFE,            25,       25 }, { IPL_MANA_CURSE,      25,       25 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Messerschmidt's Reaver"),  UITYPE_GREATAXE,    25,       6,   58000, { { IPL_DAMP,           200,      200 }, { IPL_DAMMOD,          15,       15 }, { IPL_ATTRIBS,          5,        5 }, { IPL_LIFE_CURSE,      50,       50 }, { IPL_FIREDAM,          2,       12 }, { IPL_INVCURS,        163,        0 } } },
	{  N_("Crackrust"),               UITYPE_MACE,         1,       5,   11375, { { IPL_ATTRIBS,          2,        2 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_ALLRES,          15,       15 }, { IPL_DAMP,            50,       50 }, { IPL_SPLLVLADD,       -1,       -1 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Hammer of Jholm"),         UITYPE_MAUL,         1,       4,    8700, { { IPL_DAMP,             4,       10 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_STR,              3,        3 }, { IPL_TOHIT,           15,       15 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Civerb's Cudgel"),         UITYPE_MACE,         1,       3,    2000, { { IPL_3XDAMVDEM,       35,       35 }, { IPL_DEX_CURSE,        5,        5 }, { IPL_MAG_CURSE,        2,        2 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Celestial Star"),      UITYPE_FLAIL,        2,       5,    7810, { { IPL_NOMINSTR,         0,        0 }, { IPL_LIGHT,            2,        2 }, { IPL_DAMMOD,          10,       10 }, { IPL_AC_CURSE,         8,        8 }, { IPL_INVCURS,        131,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Baranar's Star"),          UITYPE_MORNSTAR,     5,       6,    6850, { { IPL_TOHIT,           12,       12 }, { IPL_DAMP,            80,       80 }, { IPL_FASTATTACK,       1,        1 }, { IPL_VIT,              4,        4 }, { IPL_DEX_CURSE,        4,        4 }, { IPL_SETDUR,          60,       60 } } },
	{  N_("Gnarled Root"),            UITYPE_SPIKCLUB,     9,       6,    9820, { { IPL_TOHIT,           20,       20 }, { IPL_DAMP,           300,      300 }, { IPL_DEX,             10,       10 }, { IPL_MAG,              5,        5 }, { IPL_ALLRES,          10,       10 }, { IPL_AC_CURSE,        10,       10 } } },
	{  N_("The Cranium Basher"),      UITYPE_MAUL,        12,       6,   36500, { { IPL_DAMMOD,          20,       20 }, { IPL_STR,             15,       15 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_MANA_CURSE,     150,      150 }, { IPL_ALLRES,           5,        5 }, { IPL_INVCURS,        122,        0 } } },
	{  N_("Schaefer's Hammer"),       UITYPE_WARHAMMER,   16,       6,   56125, { { IPL_DAMP_CURSE,     100,      100 }, { IPL_LIGHTDAM,         1,       50 }, { IPL_LIFE,            50,       50 }, { IPL_TOHIT,           30,       30 }, { IPL_LIGHTRES,        80,       80 }, { IPL_LIGHT,            1,        1 } } },
	{  N_("Dreamflange"),             UITYPE_MACE,        26,       5,   26450, { { IPL_MAG,             30,       30 }, { IPL_MANA,            50,       50 }, { IPL_MAGICRES,        50,       50 }, { IPL_LIGHT,            2,        2 }, { IPL_SPLLVLADD,        1,        1 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Staff of Shadows"),        UITYPE_LONGSTAFF,    2,       5,    1250, { { IPL_MAG_CURSE,       10,       10 }, { IPL_TOHIT,           10,       10 }, { IPL_DAMP,            60,       60 }, { IPL_LIGHT_CURSE,      2,        2 }, { IPL_FASTATTACK,       1,        1 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Immolator"),               UITYPE_LONGSTAFF,    4,       4,    3900, { { IPL_FIRERES,         20,       20 }, { IPL_FIREDAM,          4,        4 }, { IPL_MANA,            10,       10 }, { IPL_VIT_CURSE,        5,        5 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Storm Spire"),             UITYPE_WARSTAFF,     8,       4,   22500, { { IPL_LIGHTRES,        50,       50 }, { IPL_LIGHTDAM,         2,        8 }, { IPL_STR,             10,       10 }, { IPL_MAG_CURSE,       10,       10 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gleamsong"),               UITYPE_SHORTSTAFF,   8,       4,    6520, { { IPL_MANA,            25,       25 }, { IPL_STR_CURSE,        3,        3 }, { IPL_VIT_CURSE,        3,        3 }, { IPL_SPELL,           10,       76 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Thundercall"),             UITYPE_COMPSTAFF,   14,       5,   22250, { { IPL_TOHIT,           35,       35 }, { IPL_LIGHTDAM,         1,       10 }, { IPL_SPELL,            3,       76 }, { IPL_LIGHTRES,        30,       30 }, { IPL_LIGHT,            2,        2 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Protector"),           UITYPE_SHORTSTAFF,  16,       6,   17240, { { IPL_VIT,              5,        5 }, { IPL_GETHIT,           5,        5 }, { IPL_SETAC,           40,       40 }, { IPL_SPELL,            2,       86 }, { IPL_THORNS,           1,        3 }, { IPL_INVCURS,        162,        0 } } },
	{  N_("Naj's Puzzler"),           UITYPE_LONGSTAFF,   18,       5,   34000, { { IPL_MAG,             20,       20 }, { IPL_DEX,             10,       10 }, { IPL_ALLRES,          20,       20 }, { IPL_SPELL,           23,       57 }, { IPL_LIFE_CURSE,      25,       25 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Mindcry"),                 UITYPE_QUARSTAFF,   20,       4,   41500, { { IPL_MAG,             15,       15 }, { IPL_SPELL,           13,       69 }, { IPL_ALLRES,          15,       15 }, { IPL_SPLLVLADD,        1,        1 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Rod of Onan"),             UITYPE_WARSTAFF,    22,       3,   44167, { { IPL_SPELL,           21,       50 }, { IPL_DAMP,           100,      100 }, { IPL_ATTRIBS,          5,        5 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Helm of Sprits"),          UITYPE_HELM,         1,       2,    7525, { { IPL_STEALLIFE,        5,        5 }, { IPL_INVCURS,         77,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Thinking Cap"),            UITYPE_SKULLCAP,     6,       5,    2020, { { IPL_MANA,            30,       30 }, { IPL_SPLLVLADD,        2,        2 }, { IPL_ALLRES,          20,       20 }, { IPL_SETDUR,           1,        1 }, { IPL_INVCURS,         93,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("OverLord's Helm"),         UITYPE_HELM,         7,       6,   12500, { { IPL_STR,             20,       20 }, { IPL_DEX,             15,       15 }, { IPL_VIT,              5,        5 }, { IPL_MAG_CURSE,       20,       20 }, { IPL_SETDUR,          15,       15 }, { IPL_INVCURS,         99,        0 } } },
	{  N_("Fool's Crest"),            UITYPE_HELM,        12,       5,   10150, { { IPL_ATTRIBS_CURSE,    4,        4 }, { IPL_LIFE,           100,      100 }, { IPL_GETHIT_CURSE,     1,        6 }, { IPL_THORNS,           1,        3 }, { IPL_INVCURS,         80,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gotterdamerung"),          UITYPE_GREATHELM,   21,       6,   54900, { { IPL_ATTRIBS,         20,       20 }, { IPL_SETAC,           60,       60 }, { IPL_GETHIT,           4,        4 }, { IPL_ALLRESZERO,       0,        0 }, { IPL_LIGHT_CURSE,      4,        4 }, { IPL_INVCURS,         85,        0 } } },
	{  N_("Royal Circlet"),           UITYPE_CROWN,       27,       5,   24875, { { IPL_ATTRIBS,         10,       10 }, { IPL_MANA,            40,       40 }, { IPL_SETAC,           40,       40 }, { IPL_LIGHT,            1,        1 }, { IPL_INVCURS,         79,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Torn Flesh of Souls"),     UITYPE_RAGS,         2,       5,    4825, { { IPL_SETAC,            8,        8 }, { IPL_VIT,             10,       10 }, { IPL_GETHIT,           1,        1 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,         92,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Gladiator's Bane"),    UITYPE_STUDARMOR,    6,       4,    3450, { { IPL_SETAC,           25,       25 }, { IPL_GETHIT,           2,        2 }, { IPL_DUR,            200,      200 }, { IPL_ATTRIBS_CURSE,    3,        3 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Rainbow Cloak"),       UITYPE_CLOAK,        2,       6,    4900, { { IPL_SETAC,           10,       10 }, { IPL_ATTRIBS,          1,        1 }, { IPL_ALLRES,          10,       10 }, { IPL_LIFE,             5,        5 }, { IPL_DUR,             50,       50 }, { IPL_INVCURS,        138,        0 } } },
	{  N_("Leather of Aut"),          UITYPE_LEATHARMOR,   4,       5,   10550, { { IPL_SETAC,           15,       15 }, { IPL_STR,              5,        5 }, { IPL_MAG_CURSE,        5,        5 }, { IPL_DEX,              5,        5 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Wisdom's Wrap"),           UITYPE_ROBE,         5,       6,    6200, { { IPL_MAG,              5,        5 }, { IPL_MANA,            10,       10 }, { IPL_LIGHTRES,        25,       25 }, { IPL_SETAC,           15,       15 }, { IPL_GETHIT,           1,        1 }, { IPL_INVCURS,        138,        0 } } },
	{  N_("Sparking Mail"),           UITYPE_CHAINMAIL,    9,       2,   15750, { { IPL_SETAC,           30,       30 }, { IPL_LIGHTDAM,         1,       10 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Scavenger Carapace"),      UITYPE_BREASTPLATE, 13,       4,   14000, { { IPL_GETHIT,          15,       15 }, { IPL_AC_CURSE,        30,       30 }, { IPL_DEX,              5,        5 }, { IPL_LIGHTRES,        40,       40 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Nightscape"),              UITYPE_CAPE,        16,       6,   11600, { { IPL_FASTRECOVER,      2,        2 }, { IPL_LIGHT_CURSE,      4,        4 }, { IPL_SETAC,           15,       15 }, { IPL_DEX,              3,        3 }, { IPL_ALLRES,          20,       20 }, { IPL_INVCURS,        138,        0 } } },
	{  N_("Naj's Light Plate"),       UITYPE_PLATEMAIL,   19,       6,   78700, { { IPL_NOMINSTR,         0,        0 }, { IPL_MAG,              5,        5 }, { IPL_MANA,            20,       20 }, { IPL_ALLRES,          20,       20 }, { IPL_SPLLVLADD,        1,        1 }, { IPL_INVCURS,        159,        0 } } },
	{  N_("Demonspike Coat"),         UITYPE_FULLPLATE,   25,       5,  251175, { { IPL_SETAC,          100,      100 }, { IPL_GETHIT,           6,        6 }, { IPL_STR,             10,       10 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_FIRERES,         50,       50 }, { IPL_INVALID,          0,        0 } } },
	{  N_("The Deflector"),           UITYPE_BUCKLER,      1,       5,    1500, { { IPL_SETAC,            7,        7 }, { IPL_ALLRES,          10,       10 }, { IPL_DAMP_CURSE,      20,       20 }, { IPL_TOHIT_CURSE,      5,        5 }, { IPL_INVCURS,         83,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Split Skull Shield"),      UITYPE_BUCKLER,      1,       6,    2025, { { IPL_SETAC,           10,       10 }, { IPL_LIFE,            10,       10 }, { IPL_STR,              2,        2 }, { IPL_LIGHT_CURSE,      1,        1 }, { IPL_SETDUR,          15,       15 }, { IPL_INVCURS,        116,        0 } } },
	{  N_("Dragon's Breach"),         UITYPE_KITESHIELD,   2,       6,   19200, { { IPL_FIRERES,         25,       25 }, { IPL_STR,              5,        5 }, { IPL_SETAC,           20,       20 }, { IPL_MAG_CURSE,        5,        5 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        117,        0 } } },
	{  N_("Blackoak Shield"),         UITYPE_SMALLSHIELD,  4,       6,    5725, { { IPL_DEX,             10,       10 }, { IPL_VIT_CURSE,       10,       10 }, { IPL_SETAC,           18,       18 }, { IPL_LIGHT_CURSE,      1,        1 }, { IPL_DUR,            150,      150 }, { IPL_INVCURS,        146,        0 } } },
	{  N_("Holy Defender"),           UITYPE_LARGESHIELD, 10,       6,   13800, { { IPL_SETAC,           15,       15 }, { IPL_GETHIT,           2,        2 }, { IPL_FIRERES,         20,       20 }, { IPL_DUR,            200,      200 }, { IPL_FASTBLOCK,        1,        1 }, { IPL_INVCURS,        146,        0 } } },
	{  N_("Stormshield"),             UITYPE_GOTHSHIELD,  24,       6,   49000, { { IPL_SETAC,           40,       40 }, { IPL_GETHIT_CURSE,     4,        4 }, { IPL_STR,             10,       10 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_FASTBLOCK,        1,        1 }, { IPL_INVCURS,        148,        0 } } },
	{  N_("Bramble"),                 UITYPE_RING,         1,       4,    1000, { { IPL_ATTRIBS_CURSE,    2,        2 }, { IPL_DAMMOD,           3,        3 }, { IPL_MANA,            10,       10 }, { IPL_INVCURS,          9,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of Regha"),           UITYPE_RING,         1,       6,    4175, { { IPL_MAG,             10,       10 }, { IPL_MAGICRES,        10,       10 }, { IPL_LIGHT,            1,        1 }, { IPL_STR_CURSE,        3,        3 }, { IPL_DEX_CURSE,        3,        3 }, { IPL_INVCURS,         11,        0 } } },
	{  N_("The Bleeder"),             UITYPE_RING,         2,       4,    8500, { { IPL_MAGICRES,        20,       20 }, { IPL_MANA,            30,       30 }, { IPL_LIFE_CURSE,      10,       10 }, { IPL_INVCURS,          8,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Constricting Ring"),       UITYPE_RING,         5,       3,   62000, { { IPL_ALLRES,          75,       75 }, { IPL_DRAINLIFE,        0,        0 }, { IPL_INVCURS,         14,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of Engagement"),      UITYPE_RING,        11,       5,   12476, { { IPL_GETHIT,           1,        2 }, { IPL_THORNS,           1,        3 }, { IPL_SETAC,            5,        5 }, { IPL_TARGAC,           4,       12 }, { IPL_INVCURS,         13,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Giant's Knuckle"),         UITYPE_RING,         8,       3,    8000, { { IPL_STR,             60,       60 }, { IPL_DEX_CURSE,       30,       30 }, { IPL_INVCURS,        179,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Mercurial Ring"),          UITYPE_RING,         8,       3,    8000, { { IPL_DEX,             60,       60 }, { IPL_STR_CURSE,       30,       30 }, { IPL_INVCURS,        176,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Xorine's Ring"),           UITYPE_RING,         8,       3,    8000, { { IPL_MAG,             60,       60 }, { IPL_STR_CURSE,       30,       30 }, { IPL_INVCURS,        168,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Karik's Ring"),            UITYPE_RING,         8,       3,    8000, { { IPL_VIT,             60,       60 }, { IPL_MAG_CURSE,       30,       30 }, { IPL_INVCURS,        173,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of Magma"),           UITYPE_RING,         8,       4,    8000, { { IPL_FIRERES,         60,       60 }, { IPL_LIGHTRES_CURSE,  30,       30 }, { IPL_MAGICRES_CURSE,  30,       30 }, { IPL_INVCURS,        184,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of the Mystics"),     UITYPE_RING,         8,       4,    8000, { { IPL_MAGICRES,        60,       60 }, { IPL_FIRERES_CURSE,   30,       30 }, { IPL_LIGHTRES_CURSE,  30,       30 }, { IPL_INVCURS,        181,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Ring of Thunder"),         UITYPE_RING,         8,       4,    8000, { { IPL_LIGHTRES,        60,       60 }, { IPL_FIRERES_CURSE,   30,       30 }, { IPL_MAGICRES_CURSE,  30,       30 }, { IPL_INVCURS,        177,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Amulet of Warding"),       UITYPE_AMULET,      12,       3,   30000, { { IPL_ALLRES,          40,       40 }, { IPL_LIFE_CURSE,     100,      100 }, { IPL_INVCURS,        170,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Gnat Sting"),              UITYPE_HUNTBOW,     15,       5,   30000, { { IPL_MULT_ARROWS,      3,        3 }, { IPL_SETDAM,           1,        2 }, { IPL_FASTATTACK,       1,        1 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        210,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Flambeau"),                UITYPE_COMPBOW,     11,       4,   30000, { { IPL_FIREBALL,        15,       20 }, { IPL_SETDAM,           0,        0 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        209,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Armor of Gloom"),          UITYPE_FULLPLATE,   25,       5,  200000, { { IPL_NOMINSTR,         0,        0 }, { IPL_SETAC,          225,      225 }, { IPL_ALLRESZERO,       0,        0 }, { IPL_LIGHT_CURSE,      2,        2 }, { IPL_INVCURS,        203,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Blitzen"),                 UITYPE_COMPBOW,     13,       4,   30000, { { IPL_ADDACLIFE,       10,       15 }, { IPL_SETDAM,           0,        0 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        219,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Thunderclap"),             UITYPE_WARHAMMER,   13,       6,   30000, { { IPL_ADDMANAAC,        3,        6 }, { IPL_STR,             20,       20 }, { IPL_LIGHTRES,        30,       30 }, { IPL_LIGHT,            2,        2 }, { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_INVCURS,        205,        0 } } },
	{  N_("Shirotachi"),              UITYPE_GREATSWR,    21,       4,   36000, { { IPL_ONEHAND,          0,        0 }, { IPL_FASTATTACK,       4,        4 }, { IPL_TARGAC,           2,        2 }, { IPL_LIGHTDAM,         6,        6 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Eater of Souls"),          UITYPE_TWOHANDSWR,  23,       6,   42000, { { IPL_INDESTRUCTIBLE,   0,        0 }, { IPL_LIFE,            50,       50 }, { IPL_STEALLIFE,        5,        5 }, { IPL_STEALMANA,        5,        5 }, { IPL_DRAINLIFE,        0,        0 }, { IPL_INVCURS,        200,        0 } } },
	{  N_("Diamondedge"),             UITYPE_LONGSWR,     17,       6,   42000, { { IPL_SETDUR,          10,       10 }, { IPL_TOHIT,           50,       50 }, { IPL_DAMP,           100,      100 }, { IPL_LIGHTRES,        50,       50 }, { IPL_SETAC,           10,       10 }, { IPL_INVCURS,        206,        0 } } },
	{  N_("Bone Chain Armor"),        UITYPE_CHAINMAIL,   13,       3,   36000, { { IPL_SETAC,           40,       40 }, { IPL_ACUNDEAD,         0,        0 }, { IPL_INVCURS,        204,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Demon Plate Armor"),       UITYPE_FULLPLATE,   25,       3,   80000, { { IPL_SETAC,           80,       80 }, { IPL_ACDEMON,          0,        0 }, { IPL_INVCURS,        225,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  N_("Acolyte's Amulet"),        UITYPE_AMULET,      10,       2,   10000, { { IPL_MANATOLIFE,      50,       50 }, { IPL_INVCURS,        183,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	// TRANSLATORS: Unique Item section end.
	{  N_("Gladiator's Ring"),        UITYPE_RING,        10,       2,   10000, { { IPL_LIFETOMANA,      40,       40 }, { IPL_INVCURS,        186,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	{  "",                            UITYPE_INVALID,      0,       0,       0, { { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 }, { IPL_INVALID,          0,        0 } } },
	// clang-format on
};

} // namespace devilution
