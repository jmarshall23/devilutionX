/**
 * @file spelldat.cpp
 *
 * Implementation of all spell data.
 */
#include "spelldat.h"
#include "utils/language.h"

#include "datatable.h"

namespace devilution {

	/** Data related to each spell ID. */
	SpellData *spelldata;

	#define BIND_SPELL_VALUE(x) spelldata[i].##x = spellTable->GetValue(#x, i);
	#define BIND_SPELL_INT_CONV(x, y) spelldata[i].##x = ##y(spellTable->GetValue(#x, i));
	#define BIND_SPELL_INT(x) spelldata[i].##x = spellTable->GetInt(#x, i);

	#define SPELL_STRING_TO_ID(x) if (str == #x) { return x; }

	devilution::magic_type SpellTypeFromString(std::string str)
    {
	    SPELL_STRING_TO_ID(STYPE_FIRE)
	    SPELL_STRING_TO_ID(STYPE_LIGHTNING)
	    SPELL_STRING_TO_ID(STYPE_MAGIC)
	    SPELL_STRING_TO_ID(STYPE_FIRE)
	    SPELL_STRING_TO_ID(STYPE_LIGHTNING)
	    SPELL_STRING_TO_ID(STYPE_MAGIC)

		devilution::app_fatal("Invalid magic tpye!");
	    return STYPE_FIRE;
	}

	devilution::spell_id SpellIdFromString(std::string str)
    {
	    SPELL_STRING_TO_ID(SPL_INVALID)
	    SPELL_STRING_TO_ID(SPL_NULL)
	    SPELL_STRING_TO_ID(SPL_FIREBOLT)
	    SPELL_STRING_TO_ID(SPL_HEAL)
	    SPELL_STRING_TO_ID(SPL_LIGHTNING)
	    SPELL_STRING_TO_ID(SPL_FLASH)
	    SPELL_STRING_TO_ID(SPL_IDENTIFY)
	    SPELL_STRING_TO_ID(SPL_FIREWALL)
	    SPELL_STRING_TO_ID(SPL_TOWN)
	    SPELL_STRING_TO_ID(SPL_STONE)
	    SPELL_STRING_TO_ID(SPL_INFRA)
	    SPELL_STRING_TO_ID(SPL_RNDTELEPORT)
	    SPELL_STRING_TO_ID(SPL_MANASHIELD)
	    SPELL_STRING_TO_ID(SPL_FIREBALL)
	    SPELL_STRING_TO_ID(SPL_GUARDIAN)
	    SPELL_STRING_TO_ID(SPL_CHAIN)
	    SPELL_STRING_TO_ID(SPL_WAVE)
	    SPELL_STRING_TO_ID(SPL_DOOMSERP)
	    SPELL_STRING_TO_ID(SPL_BLODRIT)
	    SPELL_STRING_TO_ID(SPL_NOVA)
	    SPELL_STRING_TO_ID(SPL_INVISIBIL)
	    SPELL_STRING_TO_ID(SPL_FLAME)
	    SPELL_STRING_TO_ID(SPL_GOLEM)
	    SPELL_STRING_TO_ID(SPL_BLODBOIL)
	    SPELL_STRING_TO_ID(SPL_TELEPORT)
		SPELL_STRING_TO_ID(SPL_APOCA)
		SPELL_STRING_TO_ID(SPL_ETHEREALIZE)
		SPELL_STRING_TO_ID(SPL_REPAIR)
		SPELL_STRING_TO_ID(SPL_RECHARGE)
		SPELL_STRING_TO_ID(SPL_DISARM)
		SPELL_STRING_TO_ID(SPL_ELEMENT)
		SPELL_STRING_TO_ID(SPL_CBOLT)
		SPELL_STRING_TO_ID(SPL_HBOLT)
		SPELL_STRING_TO_ID(SPL_RESURRECT)
		SPELL_STRING_TO_ID(SPL_TELEKINESIS)
		SPELL_STRING_TO_ID(SPL_LASTDIABLO)
		SPELL_STRING_TO_ID(SPL_HEALOTHER)
		SPELL_STRING_TO_ID(SPL_FLARE)
		SPELL_STRING_TO_ID(SPL_BONESPIRIT)
		SPELL_STRING_TO_ID(SPL_LASTDIABLO)
		SPELL_STRING_TO_ID(SPL_MANA)
		SPELL_STRING_TO_ID(SPL_MAGI)
		SPELL_STRING_TO_ID(SPL_JESTER)
		SPELL_STRING_TO_ID(SPL_LIGHTWALL)
		SPELL_STRING_TO_ID(SPL_IMMOLAT)
		SPELL_STRING_TO_ID(SPL_WARP)
		SPELL_STRING_TO_ID(SPL_REFLECT)
		SPELL_STRING_TO_ID(SPL_BERSERK)
		SPELL_STRING_TO_ID(SPL_FIRERING)
		SPELL_STRING_TO_ID(SPL_SEARCH)
		SPELL_STRING_TO_ID(SPL_RUNEFIRE)
		SPELL_STRING_TO_ID(SPL_RUNELIGHT)
		SPELL_STRING_TO_ID(SPL_RUNENOVA)
		SPELL_STRING_TO_ID(SPL_RUNEIMMOLAT)
		SPELL_STRING_TO_ID(SPL_RUNESTONE)

		devilution::app_fatal("Invalid spell id!");
	    return SPL_NULL;
	}


	void InitSpellDataTable(void)
	{
	    spelldata = new SpellData[spellTable->NumRows()];

	    for (int i = 0; i < spellTable->NumRows(); i++) {
		    BIND_SPELL_INT_CONV(sName, SpellIdFromString)
		    BIND_SPELL_INT(sManaCost)
		    BIND_SPELL_INT_CONV(sType, SpellTypeFromString)
		    BIND_SPELL_VALUE(sNameText)
		    BIND_SPELL_VALUE(sSkillText)
		    BIND_SPELL_INT(sBookLvl)
		    BIND_SPELL_INT(sStaffLvl)
		    BIND_SPELL_INT(sTargeted)
		    BIND_SPELL_INT(sTownSpell)
		    BIND_SPELL_INT(sMinInt)
		    spelldata[i].sSFX = SfxIdFromString(spellTable->GetValue("sSFX", i));

			spelldata[i].sMissiles[0] = MissileIdFromString(spellTable->GetValue("sMissiles0", i));
		    spelldata[i].sMissiles[1] = MissileIdFromString(spellTable->GetValue("sMissiles1", i));
		    spelldata[i].sMissiles[2] = MissileIdFromString(spellTable->GetValue("sMissiles2", i));

		    BIND_SPELL_INT(sManaAdj)
		    BIND_SPELL_INT(sMinMana)
		    BIND_SPELL_INT(sStaffMin)
		    BIND_SPELL_INT(sStaffMax)
		    BIND_SPELL_INT(sBookCost)
		    BIND_SPELL_INT(sStaffCost)
	    }
	}

} // namespace devilution
