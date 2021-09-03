/**
 * @file monstdat.cpp
 *
 * Implementation of all monster data.
 */
#include "items.h"
#include "monstdat.h"

#include "monster.h"
#include "textdat.h"
#include "utils/language.h"

#include "datatable.h"

namespace devilution {

/** Contains the data related to each monster ID. */
MonsterData *MonstersData;

#define BIND_MONSTER_VALUE(x) MonstersData[i].##x = monsterTable->GetValue(#x, i);
#define BIND_MONSTER_INT(x) MonstersData[i].##x = monsterTable->GetInt(#x, i);
#define BIND_MONSTER_INT_ARRAY(x) MonstersData[i].##x = monsterTable->GetInt(#x, i);

#define MONTABLE_STRING_TYPE(x) if (str == #x) {  return x; }
#define MONTABLE_STRING_TYPE_VALSHIFT(x) if (tokens[i] == #x) {  v |= x; continue; }

_mc_id MonsterTable_GetAIClassType(std::string str)
{
	MONTABLE_STRING_TYPE(MC_UNDEAD)
	MONTABLE_STRING_TYPE(MC_DEMON)
	MONTABLE_STRING_TYPE(MC_ANIMAL)

		
	devilution::app_fatal("Invalid monster class type %s", str.c_str());

	return MC_UNDEAD;
}

_mai_id MonsterTable_GetAIType(std::string str)
{
	MONTABLE_STRING_TYPE(AI_ZOMBIE)
	MONTABLE_STRING_TYPE(AI_FAT)
	MONTABLE_STRING_TYPE(AI_SKELSD)
	MONTABLE_STRING_TYPE(AI_SKELBOW)
	MONTABLE_STRING_TYPE(AI_SCAV)
	MONTABLE_STRING_TYPE(AI_RHINO)
	MONTABLE_STRING_TYPE(AI_GOATMC)
	MONTABLE_STRING_TYPE(AI_GOATBOW)
	MONTABLE_STRING_TYPE(AI_FALLEN)
	MONTABLE_STRING_TYPE(AI_MAGMA)
	MONTABLE_STRING_TYPE(AI_SKELKING)
	MONTABLE_STRING_TYPE(AI_BAT)
	MONTABLE_STRING_TYPE(AI_GARG)
	MONTABLE_STRING_TYPE(AI_CLEAVER)
	MONTABLE_STRING_TYPE(AI_SUCC)
	MONTABLE_STRING_TYPE(AI_SNEAK)
	MONTABLE_STRING_TYPE(AI_STORM)
	MONTABLE_STRING_TYPE(AI_FIREMAN)
	MONTABLE_STRING_TYPE(AI_GARBUD)
	MONTABLE_STRING_TYPE(AI_ACID)
	MONTABLE_STRING_TYPE(AI_ACIDUNIQ)
	MONTABLE_STRING_TYPE(AI_GOLUM)
	MONTABLE_STRING_TYPE(AI_ZHAR)
	MONTABLE_STRING_TYPE(AI_SNOTSPIL)
	MONTABLE_STRING_TYPE(AI_SNAKE)
	MONTABLE_STRING_TYPE(AI_COUNSLR)
	MONTABLE_STRING_TYPE(AI_MEGA)
	MONTABLE_STRING_TYPE(AI_DIABLO)
	MONTABLE_STRING_TYPE(AI_LAZARUS)
	MONTABLE_STRING_TYPE(AI_LAZHELP)
	MONTABLE_STRING_TYPE(AI_LACHDAN)
	MONTABLE_STRING_TYPE(AI_WARLORD)
	MONTABLE_STRING_TYPE(AI_FIREBAT)
	MONTABLE_STRING_TYPE(AI_TORCHANT)
	MONTABLE_STRING_TYPE(AI_HORKDMN)
	MONTABLE_STRING_TYPE(AI_LICH)
	MONTABLE_STRING_TYPE(AI_ARCHLICH)
	MONTABLE_STRING_TYPE(AI_PSYCHORB)
	MONTABLE_STRING_TYPE(AI_NECROMORB)
	MONTABLE_STRING_TYPE(AI_BONEDEMON)

	devilution::app_fatal("Invalid monster AI type %s", str.c_str());

	return AI_LICH;
}

int MonsterTable_ParseResistance(std::string str)
{
	std::vector<std::string> tokens = split(str, '|');
	int v = 0;

	if (tokens[0] == "0")
		return 0;


	for (int i = 0; i < tokens.size(); i++) {
		MONTABLE_STRING_TYPE_VALSHIFT(RESIST_MAGIC);
		MONTABLE_STRING_TYPE_VALSHIFT(RESIST_FIRE);
		MONTABLE_STRING_TYPE_VALSHIFT(RESIST_LIGHTNING);
		MONTABLE_STRING_TYPE_VALSHIFT(IMMUNE_MAGIC);
		MONTABLE_STRING_TYPE_VALSHIFT(IMMUNE_FIRE);
		MONTABLE_STRING_TYPE_VALSHIFT(IMMUNE_LIGHTNING);
		MONTABLE_STRING_TYPE_VALSHIFT(IMMUNE_NULL_40);
		MONTABLE_STRING_TYPE_VALSHIFT(IMMUNE_ACID);

		devilution::app_fatal("Invalid resistance type %s", tokens[i].c_str());
	}

	return v;
}

#define MONTABLE_LOOT_TO_ID(x)  if (tokens[1] == #x) {  v += x; return v; }
int MonsterTable_ParseLoot(std::string str)
{
	int v = 0;

	if (str == "0")
		return 0;

	if (str == "T_NODROP")
		return T_NODROP;

	std::vector<std::string> tokens = split(str, '+');

	if (tokens[0] == "T_UNIQ") {
		v += T_UNIQ;
	} else {
		devilution::app_fatal("Invalid resistance type %s", tokens[0].c_str());
	}

	MONTABLE_LOOT_TO_ID(UITEM_CLEAVER)
	MONTABLE_LOOT_TO_ID(UITEM_SKCROWN)
	MONTABLE_LOOT_TO_ID(UITEM_INFRARING)
	MONTABLE_LOOT_TO_ID(UITEM_OPTAMULET)
	MONTABLE_LOOT_TO_ID(UITEM_TRING)
	MONTABLE_LOOT_TO_ID(UITEM_HARCREST)
	MONTABLE_LOOT_TO_ID(UITEM_STEELVEIL)
	MONTABLE_LOOT_TO_ID(UITEM_ARMOFVAL)
	MONTABLE_LOOT_TO_ID(UITEM_GRISWOLD)
	MONTABLE_LOOT_TO_ID(UITEM_BOVINE)
	MONTABLE_LOOT_TO_ID(UITEM_RIFTBOW)
	MONTABLE_LOOT_TO_ID(UITEM_NEEDLER)
	MONTABLE_LOOT_TO_ID(UITEM_CELESTBOW)
	MONTABLE_LOOT_TO_ID(UITEM_DEADLYHUNT)
	MONTABLE_LOOT_TO_ID(UITEM_BOWOFDEAD)
	MONTABLE_LOOT_TO_ID(UITEM_BLKOAKBOW)
	MONTABLE_LOOT_TO_ID(UITEM_FLAMEDART)
	MONTABLE_LOOT_TO_ID(UITEM_FLESHSTING)
	MONTABLE_LOOT_TO_ID(UITEM_WINDFORCE)
	MONTABLE_LOOT_TO_ID(UITEM_EAGLEHORN)
	MONTABLE_LOOT_TO_ID(UITEM_GONNAGALDIRK)
	MONTABLE_LOOT_TO_ID(UITEM_DEFENDER)
	MONTABLE_LOOT_TO_ID(UITEM_GRYPHONCLAW)
	MONTABLE_LOOT_TO_ID(UITEM_BLACKRAZOR)
	MONTABLE_LOOT_TO_ID(UITEM_GIBBOUSMOON)
	MONTABLE_LOOT_TO_ID(UITEM_ICESHANK)
	MONTABLE_LOOT_TO_ID(UITEM_EXECUTIONER)
	MONTABLE_LOOT_TO_ID(UITEM_BONESAW)
	MONTABLE_LOOT_TO_ID(UITEM_SHADHAWK)
	MONTABLE_LOOT_TO_ID(UITEM_WIZSPIKE)
	MONTABLE_LOOT_TO_ID(UITEM_LGTSABRE)
	MONTABLE_LOOT_TO_ID(UITEM_FALCONTALON)
	MONTABLE_LOOT_TO_ID(UITEM_INFERNO)
	MONTABLE_LOOT_TO_ID(UITEM_DOOMBRINGER)
	MONTABLE_LOOT_TO_ID(UITEM_GRIZZLY)
	MONTABLE_LOOT_TO_ID(UITEM_GRANDFATHER)
	MONTABLE_LOOT_TO_ID(UITEM_MANGLER)
	MONTABLE_LOOT_TO_ID(UITEM_SHARPBEAK)
	MONTABLE_LOOT_TO_ID(UITEM_BLOODLSLAYER)
	MONTABLE_LOOT_TO_ID(UITEM_CELESTAXE)
	MONTABLE_LOOT_TO_ID(UITEM_WICKEDAXE)
	MONTABLE_LOOT_TO_ID(UITEM_STONECLEAV)
	MONTABLE_LOOT_TO_ID(UITEM_AGUHATCHET)
	MONTABLE_LOOT_TO_ID(UITEM_HELLSLAYER)
	MONTABLE_LOOT_TO_ID(UITEM_MESSERREAVER)
	MONTABLE_LOOT_TO_ID(UITEM_CRACKRUST)
	MONTABLE_LOOT_TO_ID(UITEM_JHOLMHAMM)
	MONTABLE_LOOT_TO_ID(UITEM_CIVERBS)
	MONTABLE_LOOT_TO_ID(UITEM_CELESTSTAR)
	MONTABLE_LOOT_TO_ID(UITEM_BARANSTAR)
	MONTABLE_LOOT_TO_ID(UITEM_GNARLROOT)
	MONTABLE_LOOT_TO_ID(UITEM_CRANBASH)
	MONTABLE_LOOT_TO_ID(UITEM_SCHAEFHAMM)
	MONTABLE_LOOT_TO_ID(UITEM_DREAMFLANGE)
	MONTABLE_LOOT_TO_ID(UITEM_STAFFOFSHAD)
	MONTABLE_LOOT_TO_ID(UITEM_IMMOLATOR)
	MONTABLE_LOOT_TO_ID(UITEM_STORMSPIRE)
	MONTABLE_LOOT_TO_ID(UITEM_GLEAMSONG)
	MONTABLE_LOOT_TO_ID(UITEM_THUNDERCALL)
	MONTABLE_LOOT_TO_ID(UITEM_PROTECTOR)
	MONTABLE_LOOT_TO_ID(UITEM_NAJPUZZLE)
	MONTABLE_LOOT_TO_ID(UITEM_MINDCRY)
	MONTABLE_LOOT_TO_ID(UITEM_RODOFONAN)
	MONTABLE_LOOT_TO_ID(UITEM_SPIRITSHELM)
	MONTABLE_LOOT_TO_ID(UITEM_THINKINGCAP)
	MONTABLE_LOOT_TO_ID(UITEM_OVERLORDHELM)
	MONTABLE_LOOT_TO_ID(UITEM_FOOLSCREST)
	MONTABLE_LOOT_TO_ID(UITEM_GOTTERDAM)
	MONTABLE_LOOT_TO_ID(UITEM_ROYCIRCLET)
	MONTABLE_LOOT_TO_ID(UITEM_TORNFLESH)
	MONTABLE_LOOT_TO_ID(UITEM_GLADBANE)
	MONTABLE_LOOT_TO_ID(UITEM_RAINCLOAK)
	MONTABLE_LOOT_TO_ID(UITEM_LEATHAUT)
	MONTABLE_LOOT_TO_ID(UITEM_WISDWRAP)
	MONTABLE_LOOT_TO_ID(UITEM_SPARKMAIL)
	MONTABLE_LOOT_TO_ID(UITEM_SCAVCARAP)
	MONTABLE_LOOT_TO_ID(UITEM_NIGHTSCAPE)
	MONTABLE_LOOT_TO_ID(UITEM_NAJPLATE)
	MONTABLE_LOOT_TO_ID(UITEM_DEMONSPIKE)
	MONTABLE_LOOT_TO_ID(UITEM_DEFLECTOR)
	MONTABLE_LOOT_TO_ID(UITEM_SKULLSHLD)
	MONTABLE_LOOT_TO_ID(UITEM_DRAGONBRCH)
	MONTABLE_LOOT_TO_ID(UITEM_BLKOAKSHLD)
	MONTABLE_LOOT_TO_ID(UITEM_HOLYDEF)
	MONTABLE_LOOT_TO_ID(UITEM_STORMSHLD)
	MONTABLE_LOOT_TO_ID(UITEM_BRAMBLE)
	MONTABLE_LOOT_TO_ID(UITEM_REGHA)
	MONTABLE_LOOT_TO_ID(UITEM_BLEEDER)
	MONTABLE_LOOT_TO_ID(UITEM_CONSTRICT)
	MONTABLE_LOOT_TO_ID(UITEM_ENGAGE)



	devilution::app_fatal("Invalid loot id!");
}

void InitMonsterTable(void)
{
	MonstersData = new MonsterData[monsterTable->NumRows()];

	for (int i = 0; i < monsterTable->NumRows(); i++) {
		BIND_MONSTER_VALUE(mName);
		BIND_MONSTER_VALUE(GraphicType);
		BIND_MONSTER_VALUE(sndfile);
		BIND_MONSTER_VALUE(TransFile);
		BIND_MONSTER_INT(width);
		BIND_MONSTER_INT(mImage);
		BIND_MONSTER_INT(has_special);
		BIND_MONSTER_INT(snd_special);
		BIND_MONSTER_INT(has_trans);
		//BIND_MONSTER_INT(Frames[6]);
		//BIND_MONSTER_INT(Rate[6]);
		MonstersData[i].Frames[0] = monsterTable->GetInt("Frames0", i);
		MonstersData[i].Frames[1] = monsterTable->GetInt("Frames1", i);
		MonstersData[i].Frames[2] = monsterTable->GetInt("Frames2", i);
		MonstersData[i].Frames[3] = monsterTable->GetInt("Frames3", i);
		MonstersData[i].Frames[4] = monsterTable->GetInt("Frames4", i);
		MonstersData[i].Frames[5] = monsterTable->GetInt("Frames5", i);

		MonstersData[i].Rate[0] = monsterTable->GetInt("Rate0", i);
		MonstersData[i].Rate[1] = monsterTable->GetInt("Rate1", i);
		MonstersData[i].Rate[2] = monsterTable->GetInt("Rate2", i);
		MonstersData[i].Rate[3] = monsterTable->GetInt("Rate3", i);
		MonstersData[i].Rate[4] = monsterTable->GetInt("Rate4", i);
		MonstersData[i].Rate[5] = monsterTable->GetInt("Rate5", i);

		BIND_MONSTER_INT(mMinDLvl);
		BIND_MONSTER_INT(mMaxDLvl);
		BIND_MONSTER_INT(mLevel);
		BIND_MONSTER_INT(mMinHP);
		BIND_MONSTER_INT(mMaxHP);
		//BIND_MONSTER_INT(mAi);
		MonstersData[i].mAi = MonsterTable_GetAIType(monsterTable->GetValue("mAi", i));
		BIND_MONSTER_INT(mFlags);
		BIND_MONSTER_INT(mInt);
		BIND_MONSTER_INT(mHit);
		BIND_MONSTER_INT(mAFNum);
		BIND_MONSTER_INT(mMinDamage);
		BIND_MONSTER_INT(mMaxDamage);
		BIND_MONSTER_INT(mHit2);
		BIND_MONSTER_INT(mAFNum2);
		BIND_MONSTER_INT(mMinDamage2);
		BIND_MONSTER_INT(mMaxDamage2);
		BIND_MONSTER_INT(mArmorClass);
		//BIND_MONSTER_INT(mMonstClass);
		MonstersData[i].mMonstClass = MonsterTable_GetAIClassType(monsterTable->GetValue("mMonstClass", i));
		MonstersData[i].mMagicRes = MonsterTable_ParseResistance(monsterTable->GetValue("mMagicRes", i));
		MonstersData[i].mMagicRes2 = MonsterTable_ParseResistance(monsterTable->GetValue("mMagicRes", i));		
		BIND_MONSTER_INT(mSelFlag);
		//BIND_MONSTER_INT(mTreasure);
		MonstersData[i].mTreasure = MonsterTable_ParseLoot(monsterTable->GetValue("mTreasure", i));		
		BIND_MONSTER_INT(mExp);
	}
}

/**
 * Map between .DUN file value and monster type enum
 */
const _monster_id MonstConvTbl[] = {
	MT_NZOMBIE,
	MT_BZOMBIE,
	MT_GZOMBIE,
	MT_YZOMBIE,
	MT_RFALLSP,
	MT_DFALLSP,
	MT_YFALLSP,
	MT_BFALLSP,
	MT_WSKELAX,
	MT_TSKELAX,
	MT_RSKELAX,
	MT_XSKELAX,
	MT_RFALLSD,
	MT_DFALLSD,
	MT_YFALLSD,
	MT_BFALLSD,
	MT_NSCAV,
	MT_BSCAV,
	MT_WSCAV,
	MT_YSCAV,
	MT_WSKELBW,
	MT_TSKELBW,
	MT_RSKELBW,
	MT_XSKELBW,
	MT_WSKELSD,
	MT_TSKELSD,
	MT_RSKELSD,
	MT_XSKELSD,
	MT_SNEAK,
	MT_STALKER,
	MT_UNSEEN,
	MT_ILLWEAV,
	MT_NGOATMC,
	MT_BGOATMC,
	MT_RGOATMC,
	MT_GGOATMC,
	MT_FIEND,
	MT_GLOOM,
	MT_BLINK,
	MT_FAMILIAR,
	MT_NGOATBW,
	MT_BGOATBW,
	MT_RGOATBW,
	MT_GGOATBW,
	MT_NACID,
	MT_RACID,
	MT_BACID,
	MT_XACID,
	MT_SKING,
	MT_FAT,
	MT_MUDMAN,
	MT_TOAD,
	MT_FLAYED,
	MT_WYRM,
	MT_CAVSLUG,
	MT_DEVOUR,
	MT_DVLWYRM,
	MT_NMAGMA,
	MT_YMAGMA,
	MT_BMAGMA,
	MT_WMAGMA,
	MT_HORNED,
	MT_MUDRUN,
	MT_FROSTC,
	MT_OBLORD,
	MT_BONEDMN,
	MT_REDDTH,
	MT_LTCHDMN,
	MT_UDEDBLRG,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID,
	MT_INCIN,
	MT_FLAMLRD,
	MT_DOOMFIRE,
	MT_HELLBURN,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID,
	MT_RSTORM,
	MT_STORM,
	MT_STORML,
	MT_MAEL,
	MT_WINGED,
	MT_GARGOYLE,
	MT_BLOODCLW,
	MT_DEATHW,
	MT_MEGA,
	MT_GUARD,
	MT_VTEXLRD,
	MT_BALROG,
	MT_NSNAKE,
	MT_RSNAKE,
	MT_GSNAKE,
	MT_BSNAKE,
	MT_NBLACK,
	MT_RTBLACK,
	MT_BTBLACK,
	MT_RBLACK,
	MT_UNRAV,
	MT_HOLOWONE,
	MT_PAINMSTR,
	MT_REALWEAV,
	MT_SUCCUBUS,
	MT_SNOWWICH,
	MT_HLSPWN,
	MT_SOLBRNR,
	MT_COUNSLR,
	MT_MAGISTR,
	MT_CABALIST,
	MT_ADVOCATE,
	MT_INVALID,
	MT_DIABLO,
	MT_INVALID,
	MT_GOLEM,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID, // Monster from blood1.dun and blood2.dun
	MT_INVALID,
	MT_INVALID,
	MT_INVALID,
	MT_INVALID, // Snotspill from banner2.dun
	MT_INVALID,
	MT_INVALID,
	MT_BIGFALL,
	MT_DARKMAGE,
	MT_HELLBOAR,
	MT_STINGER,
	MT_PSYCHORB,
	MT_ARACHNON,
	MT_FELLTWIN,
	MT_HORKSPWN,
	MT_STINGER,
	MT_PSYCHORB,
	MT_ARACHNON,
	MT_LASHWORM,
	MT_TORCHANT,
	MT_HORKDMN,
	MT_DEFILER,
	MT_GRAVEDIG,
	MT_TOMBRAT,
	MT_FIREBAT,
	MT_SKLWING,
	MT_LICH,
	MT_CRYPTDMN,
	MT_FIREBAT,
	MT_SKLWING,
	MT_LICH,
	MT_BICLOPS,
	MT_FLESTHNG,
	MT_REAPER,
	MT_NAKRUL,
	MT_CLEAVER,
	MT_INVILORD,
	MT_LRDSAYTR,
};

/**
 * Define what version a monster type is available in
 */
const char MonstAvailTbl[] = {
	MAT_ALWAYS, // Zombie
	MAT_ALWAYS, // Ghoul
	MAT_ALWAYS, // Rotting Carcass
	MAT_ALWAYS, // Black Death
	MAT_ALWAYS, // Fallen One
	MAT_ALWAYS, // Carver
	MAT_ALWAYS, // Devil Kin
	MAT_ALWAYS, // Dark One
	MAT_ALWAYS, // Skeleton
	MAT_ALWAYS, // Corpse Axe
	MAT_ALWAYS, // Burning Dead
	MAT_ALWAYS, // Horror
	MAT_ALWAYS, // Fallen One
	MAT_ALWAYS, // Carver
	MAT_ALWAYS, // Devil Kin
	MAT_ALWAYS, // Dark One
	MAT_ALWAYS, // Scavenger
	MAT_ALWAYS, // Plague Eater
	MAT_ALWAYS, // Shadow Beast
	MAT_ALWAYS, // Bone Gasher
	MAT_ALWAYS, // Skeleton
	MAT_ALWAYS, // Corpse Bow
	MAT_ALWAYS, // Burning Dead
	MAT_ALWAYS, // Horror
	MAT_ALWAYS, // Skeleton Captain
	MAT_ALWAYS, // Corpse Captain
	MAT_ALWAYS, // Burning Dead Captain
	MAT_ALWAYS, // Horror Captain
	MAT_NEVER,  // Invisible Lord
	MAT_RETAIL, // Hidden
	MAT_RETAIL, // Stalker
	MAT_RETAIL, // Unseen
	MAT_RETAIL, // Illusion Weaver
	MAT_RETAIL, // Satyr Lord
	MAT_RETAIL, // Flesh Clan
	MAT_RETAIL, // Stone Clan
	MAT_RETAIL, // Fire Clan
	MAT_RETAIL, // Night Clan
	MAT_ALWAYS, // Fiend
	MAT_ALWAYS, // Blink
	MAT_ALWAYS, // Gloom
	MAT_ALWAYS, // Familiar
	MAT_RETAIL, // Flesh Clan
	MAT_RETAIL, // Stone Clan
	MAT_RETAIL, // Fire Clan
	MAT_RETAIL, // Night Clan
	MAT_RETAIL, // Acid Beast
	MAT_RETAIL, // Poison Spitter
	MAT_RETAIL, // Pit Beast
	MAT_RETAIL, // Lava Maw
	MAT_NEVER,  // Skeleton King
	MAT_NEVER,  // The Butcher
	MAT_RETAIL, // Overlord
	MAT_RETAIL, // Mud Man
	MAT_RETAIL, // Toad Demon
	MAT_RETAIL, // Flayed One
	MAT_NEVER,  // Wyrm
	MAT_NEVER,  // Cave Slug
	MAT_NEVER,  // Devil Wyrm
	MAT_NEVER,  // Devourer
	MAT_RETAIL, // Magma Demon
	MAT_RETAIL, // Blood Stone
	MAT_RETAIL, // Hell Stone
	MAT_RETAIL, // Lava Lord
	MAT_RETAIL, // Horned Demon
	MAT_RETAIL, // Mud Runner
	MAT_RETAIL, // Frost Charger
	MAT_RETAIL, // Obsidian Lord
	MAT_NEVER,  // Bone Demon (oldboned in Hellfire)
	MAT_NEVER,  // Red Death
	MAT_NEVER,  // Litch Demon
	MAT_NEVER,  // Undead Balrog
	MAT_NEVER,  // Incinerator
	MAT_NEVER,  // Flame Lord
	MAT_NEVER,  // Doom Fire
	MAT_NEVER,  // Hell Burner
	MAT_RETAIL, // Red Storm
	MAT_RETAIL, // Storm Rider
	MAT_RETAIL, // Storm Lord
	MAT_RETAIL, // Maelstorm
	MAT_RETAIL, // Devil Kin Brute
	MAT_RETAIL, // Winged-Demon
	MAT_RETAIL, // Gargoyle
	MAT_RETAIL, // Blood Claw
	MAT_RETAIL, // Death Wing
	MAT_RETAIL, // Slayer
	MAT_RETAIL, // Guardian
	MAT_RETAIL, // Vortex Lord
	MAT_RETAIL, // Balrog
	MAT_RETAIL, // Cave Viper
	MAT_RETAIL, // Fire Drake
	MAT_RETAIL, // Gold Viper
	MAT_RETAIL, // Azure Drake
	MAT_RETAIL, // Black Knight
	MAT_RETAIL, // Doom Guard
	MAT_RETAIL, // Steel Lord
	MAT_RETAIL, // Blood Knight
	MAT_RETAIL, // The Shredded
	MAT_NEVER,  // Hollow One
	MAT_NEVER,  // Pain Master
	MAT_NEVER,  // Reality Weaver
	MAT_RETAIL, // Succubus
	MAT_RETAIL, // Snow Witch
	MAT_RETAIL, // Hell Spawn
	MAT_RETAIL, // Soul Burner
	MAT_RETAIL, // Counselor
	MAT_RETAIL, // Magistrate
	MAT_RETAIL, // Cabalist
	MAT_RETAIL, // Advocate
	MAT_NEVER,  // Golem
	MAT_NEVER,  // The Dark Lord
	MAT_NEVER,  // The Arch-Litch Malignus
	MAT_RETAIL, // Hellboar
	MAT_RETAIL, // Stinger
	MAT_RETAIL, // Psychorb
	MAT_RETAIL, // Arachnon
	MAT_RETAIL, // Felltwin
	MAT_RETAIL, // Hork Spawn
	MAT_RETAIL, // Venomtail
	MAT_RETAIL, // Necromorb
	MAT_RETAIL, // Spider Lord
	MAT_RETAIL, // Lashworm
	MAT_RETAIL, // Torchant
	MAT_NEVER,  // Hork Demon
	MAT_NEVER,  // Hell Bug
	MAT_RETAIL, // Gravedigger
	MAT_RETAIL, // Tomb Rat
	MAT_RETAIL, // Firebat
	MAT_RETAIL, // Skullwing
	MAT_RETAIL, // Lich
	MAT_RETAIL, // Crypt Demon
	MAT_RETAIL, // Hellbat
	MAT_RETAIL, // Bone Demon
	MAT_RETAIL, // Arch Lich
	MAT_RETAIL, // Biclops
	MAT_RETAIL, // Flesh Thing
	MAT_RETAIL, // Reaper
	MAT_NEVER,  // Na-Krul
};
/** Contains the data related to each unique monster ID. */
const UniqMonstStruct UniqMonst[] = {
	// clang-format off
	// mtype,       mName,                          mTrnName,    mlevel, mmaxhp, mAi,      mint, mMinDamage, mMaxDamage, mMagicRes,                                                    monsterPack, customHitpoints, customArmorClass, mtalkmsg
	// TRANSLATORS: Unique Monster Block start
	{  MT_NGOATMC,  N_("Gharbad the Weak"),         "BSDB",        4,    120, AI_GARBUD,   3,          8,         16,                              IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_GARBUD1   },
	{  MT_SKING,    N_("Skeleton King"),            "GENRL",       0,    240, AI_SKELKING, 3,          6,         16, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Independent, 0,        0, TEXT_NONE      },
	{  MT_COUNSLR,  N_("Zhar the Mad"),             "GENERAL",     8,    360, AI_ZHAR,     3,         16,         40, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_ZHAR1     },
	{  MT_BFALLSP,  N_("Snotspill"),                "BNG",         4,    220, AI_SNOTSPIL, 3,         10,         18,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_BANNER10  },
	{  MT_ADVOCATE, N_("Arch-Bishop Lazarus"),      "GENERAL",     0,    600, AI_LAZARUS,  3,         30,         50, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_VILE13    },
	{  MT_HLSPWN,   N_("Red Vex"),                  "REDV",        0,    400, AI_LAZHELP,  3,         30,         50, IMMUNE_MAGIC | RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_VILE13    },
	{  MT_HLSPWN,   N_("Black Jade"),               "BLKJD",       0,    400, AI_LAZHELP,  3,         30,         50, IMMUNE_MAGIC |               RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_VILE13    },
	{  MT_RBLACK,   "Lachdanan",                    "BHKA",       14,    500, AI_LACHDAN,  3,          0,          0, 0                                                             ,  UniqueMonsterPack::None,        0,        0, TEXT_VEIL9     },
	{  MT_BTBLACK,  N_("Warlord of Blood"),         "GENERAL",    13,    850, AI_WARLORD,  3,         35,         50, IMMUNE_MAGIC | IMMUNE_FIRE | IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_WARLRD9   },
	{  MT_CLEAVER,  N_("The Butcher"),              "GENRL",       0,    220, AI_CLEAVER,  3,          6,         12,                RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_HORKDMN,  N_("Hork Demon"),               "GENRL",      19,    300, AI_HORKDMN,  3,         20,         35,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_DEFILER,  N_("The Defiler"),              "GENRL",      20,    480, AI_SKELSD,   3,         30,         40, RESIST_MAGIC | RESIST_FIRE | IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_NAKRUL,   "Na-Krul",                      "GENRL",       0,   1332, AI_SKELSD,   3,         40,         50, IMMUNE_MAGIC | IMMUNE_FIRE | IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_TSKELAX,  N_("Bonehead Keenaxe"),         "BHKA",        2,     91, AI_SKELSD,   2,          4,         10, IMMUNE_MAGIC |                                  IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,   100,        0, TEXT_NONE      },
	{  MT_RFALLSD,  N_("Bladeskin the Slasher"),    "BSTS",        2,     51, AI_FALLEN,   0,          6,         18,                RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,       45, TEXT_NONE      },
	{  MT_NZOMBIE,  N_("Soulpus"),                  "GENERAL",     2,    133, AI_ZOMBIE,   0,          4,          8,                RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_RFALLSP,  N_("Pukerat the Unclean"),      "PTU",         2,     77, AI_FALLEN,   3,          1,          5,                RESIST_FIRE                                    ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_WSKELAX,  N_("Boneripper"),               "BR",          2,     54, AI_BAT,      0,          6,         15, IMMUNE_MAGIC | IMMUNE_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NZOMBIE,  N_("Rotfeast the Hungry"),      "ETH",         2,     85, AI_SKELSD,   3,          4,         12, IMMUNE_MAGIC |                                  IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_DFALLSD,  N_("Gutshank the Quick"),       "GTQ",         3,     66, AI_BAT,      2,          6,         16,                RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_TSKELSD,  N_("Brokenhead Bangshield"),    "BHBS",        3,    108, AI_SKELSD,   3,         12,         20, IMMUNE_MAGIC |               RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_YFALLSP,  "Bongo",                        "BNG",         3,    178, AI_FALLEN,   3,          9,         21, 0                                                             ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BZOMBIE,  N_("Rotcarnage"),               "RCRN",        3,    102, AI_ZOMBIE,   3,          9,         24, IMMUNE_MAGIC |               RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,       45, TEXT_NONE      },
	{  MT_NSCAV,    N_("Shadowbite"),               "SHBT",        2,     60, AI_SKELSD,   3,          3,         20,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_WSKELBW,  N_("Deadeye"),                  "DE",          2,     49, AI_GOATBOW,  0,          6,          9, IMMUNE_MAGIC | RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_RSKELAX,  N_("Madeye the Dead"),          "MTD",         4,     75, AI_BAT,      0,          9,         21, IMMUNE_MAGIC | IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,       30, TEXT_NONE      },
	{  MT_BSCAV,    "El Chupacabras",               "GENERAL",     3,    120, AI_GOATMC,   0,         10,         18,                RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_TSKELBW,  N_("Skullfire"),                "SKFR",        3,    125, AI_GOATBOW,  1,          6,         10,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_SNEAK,    N_("Warpskull"),                "TSPO",        3,    117, AI_SNEAK,    2,          6,         18,                RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_GZOMBIE,  N_("Goretongue"),               "PMR",         3,    156, AI_SKELSD,   1,         15,         30, IMMUNE_MAGIC |                                  IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_WSCAV,    N_("Pulsecrawler"),             "BHKA",        4,    150, AI_SCAV,     0,         16,         20,                IMMUNE_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,       45, TEXT_NONE      },
	{  MT_BLINK,    N_("Moonbender"),               "GENERAL",     4,    135, AI_BAT,      0,          9,         27,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BLINK,    N_("Wrathraven"),               "GENERAL",     5,    135, AI_BAT,      2,          9,         22,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_YSCAV,    N_("Spineeater"),               "GENERAL",     4,    180, AI_SCAV,     1,         18,         25,                              IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RSKELBW,  N_("Blackash the Burning"),     "BASHTB",      4,    120, AI_GOATBOW,  0,          6,         16, IMMUNE_MAGIC | IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BFALLSD,  N_("Shadowcrow"),               "GENERAL",     5,    270, AI_SNEAK,    2,         12,         25, 0                                                             ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_LRDSAYTR, N_("Blightstone the Weak"),     "BHKA",        4,    360, AI_SKELSD,   0,          4,         12, IMMUNE_MAGIC |               RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,    70,        0, TEXT_NONE      },
	{  MT_FAT,      N_("Bilefroth the Pit Master"), "BFTP",        6,    210, AI_BAT,      1,         16,         23, IMMUNE_MAGIC | IMMUNE_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NGOATBW,  N_("Bloodskin Darkbow"),        "BSDB",        5,    207, AI_GOATBOW,  0,          3,         16,                RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,       55, TEXT_NONE      },
	{  MT_GLOOM,    N_("Foulwing"),                 "DB",          5,    246, AI_RHINO,    3,         12,         28,                RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_XSKELSD,  N_("Shadowdrinker"),            "SHDR",        5,    300, AI_SNEAK,    1,         18,         26, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,       45, TEXT_NONE      },
	{  MT_UNSEEN,   N_("Hazeshifter"),              "BHKA",        5,    285, AI_SNEAK,    3,         18,         30,                              IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NACID,    N_("Deathspit"),                "BFDS",        6,    303, AI_ACIDUNIQ, 0,         12,         32,                RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RGOATMC,  N_("Bloodgutter"),              "BGBL",        6,    315, AI_BAT,      1,         24,         34,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BGOATMC,  N_("Deathshade Fleshmaul"),     "DSFM",        6,    276, AI_RHINO,    0,         12,         24, IMMUNE_MAGIC | RESIST_FIRE                                    ,  UniqueMonsterPack::None,        0,       65, TEXT_NONE      },
	{  MT_WYRM,     N_("Warmaggot the Mad"),        "GENERAL",     6,    246, AI_BAT,      3,         15,         30,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_STORM,    N_("Glasskull the Jagged"),     "BHKA",        7,    354, AI_STORM,    0,         18,         30, IMMUNE_MAGIC | IMMUNE_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RGOATBW,  N_("Blightfire"),               "BLF",         7,    321, AI_SUCC,     2,         13,         21,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_GARGOYLE, N_("Nightwing the Cold"),       "GENERAL",     7,    342, AI_BAT,      1,         18,         26, IMMUNE_MAGIC |               RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_GGOATBW,  N_("Gorestone"),                "GENERAL",     7,    303, AI_GOATBOW,  1,         15,         28,                              RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,    70,        0, TEXT_NONE      },
	{  MT_BMAGMA,   N_("Bronzefist Firestone"),     "GENERAL",     8,    360, AI_MAGMA,    0,         30,         36, IMMUNE_MAGIC | RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_INCIN,    N_("Wrathfire the Doomed"),     "WFTD",        8,    270, AI_SKELSD,   2,         20,         30, IMMUNE_MAGIC | RESIST_FIRE |  RESIST_LIGHTNING                ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NMAGMA,   N_("Firewound the Grim"),       "BHKA",        8,    303, AI_MAGMA,    0,         18,         22, IMMUNE_MAGIC | RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_MUDMAN,   N_("Baron Sludge"),             "BSM",         8,    315, AI_SNEAK,    3,         25,         34, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,       75, TEXT_NONE      },
	{  MT_GGOATMC,  N_("Blighthorn Steelmace"),     "BHSM",        7,    250, AI_RHINO,    0,         20,         28,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,       45, TEXT_NONE      },
	{  MT_RACID,    N_("Chaoshowler"),              "GENERAL",     8,    240, AI_ACIDUNIQ, 0,         12,         20, 0                                                             ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_REDDTH,   N_("Doomgrin the Rotting"),     "GENERAL",     8,    405, AI_STORM,    3,         25,         50, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_FLAMLRD,  N_("Madburner"),                "GENERAL",     9,    270, AI_STORM,    0,         20,         40, IMMUNE_MAGIC | IMMUNE_FIRE | IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_LTCHDMN,  N_("Bonesaw the Litch"),        "GENERAL",     9,    495, AI_STORM,    2,         30,         55, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_MUDRUN,   N_("Breakspine"),               "GENERAL",     9,    351, AI_RHINO,    0,         25,         34,                RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_REDDTH,   N_("Devilskull Sharpbone"),     "GENERAL",     9,    444, AI_STORM,    1,         25,         40,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_STORM,    N_("Brokenstorm"),              "GENERAL",     9,    411, AI_STORM,    2,         25,         36,                              IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RSTORM,   N_("Stormbane"),                "GENERAL",     9,    555, AI_STORM,    3,         30,         30,                              IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_TOAD,     N_("Oozedrool"),                "GENERAL",     9,    483, AI_FAT,      3,         25,         30,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BLOODCLW, N_("Goldblight of the Flame"),  "GENERAL",    10,    405, AI_GARG,     0,         15,         35, IMMUNE_MAGIC | IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,       80, TEXT_NONE      },
	{  MT_OBLORD,   N_("Blackstorm"),               "GENERAL",    10,    525, AI_RHINO,    3,         20,         40, IMMUNE_MAGIC |               IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,       90, TEXT_NONE      },
	{  MT_RACID,    N_("Plaguewrath"),              "GENERAL",    10,    450, AI_ACIDUNIQ, 2,         20,         30, IMMUNE_MAGIC | RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RSTORM,   N_("The Flayer"),               "GENERAL",    10,    501, AI_STORM,    1,         20,         35, RESIST_MAGIC | RESIST_FIRE | IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_FROSTC,   N_("Bluehorn"),                 "GENERAL",    11,    477, AI_RHINO,    1,         25,         30, IMMUNE_MAGIC | RESIST_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,       90, TEXT_NONE      },
	{  MT_HELLBURN, N_("Warpfire Hellspawn"),       "GENERAL",    11,    525, AI_FIREMAN,  3,         10,         40, RESIST_MAGIC | IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NSNAKE,   N_("Fangspeir"),                "GENERAL",    11,    444, AI_SKELSD,   1,         15,         32,                IMMUNE_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_UDEDBLRG, N_("Festerskull"),              "GENERAL",    11,    600, AI_STORM,    2,         15,         30, IMMUNE_MAGIC |                                  IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_NBLACK,   N_("Lionskull the Bent"),       "GENERAL",    12,    525, AI_SKELSD,   2,         25,         25, IMMUNE_MAGIC | IMMUNE_FIRE | IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_COUNSLR,  N_("Blacktongue"),              "GENERAL",    12,    360, AI_COUNSLR,  3,         15,         30,                RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_DEATHW,   N_("Viletouch"),                "GENERAL",    12,    525, AI_GARG,     3,         20,         40,                              IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RSNAKE,   N_("Viperflame"),               "GENERAL",    12,    570, AI_SKELSD,   1,         25,         35,               IMMUNE_FIRE  | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BSNAKE,   N_("Fangskin"),                 "BHKA",       14,    681, AI_SKELSD,   2,         15,         50, IMMUNE_MAGIC |               RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_SUCCUBUS, N_("Witchfire the Unholy"),     "GENERAL",    12,    444, AI_SUCC,     3,         10,         20, IMMUNE_MAGIC | IMMUNE_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_BALROG,   N_("Blackskull"),               "BHKA",       13,    750, AI_SKELSD,   3,         25,         40, IMMUNE_MAGIC |               RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_UNRAV,    N_("Soulslash"),                "GENERAL",    12,    450, AI_SKELSD,   0,         25,         25, IMMUNE_MAGIC |                                  IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_VTEXLRD,  N_("Windspawn"),                "GENERAL",    12,    711, AI_SKELSD,   1,         35,         40, IMMUNE_MAGIC | IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_GSNAKE,   N_("Lord of the Pit"),          "GENERAL",    13,    762, AI_SKELSD,   2,         25,         42,                RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_RTBLACK,  N_("Rustweaver"),               "GENERAL",    13,    400, AI_SKELSD,   3,          1,         60, IMMUNE_MAGIC | IMMUNE_FIRE | IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_HOLOWONE, N_("Howlingire the Shade"),     "GENERAL",    13,    450, AI_SKELSD,   2,         40,         75,                RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_MAEL,     N_("Doomcloud"),                "GENERAL",    13,    612, AI_STORM,    1,          1,         60,                RESIST_FIRE | IMMUNE_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_PAINMSTR, N_("Bloodmoon Soulfire"),       "GENERAL",    13,    684, AI_SKELSD,   1,         15,         40, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_SNOWWICH, N_("Witchmoon"),                "GENERAL",    13,    310, AI_SUCC,     3,         30,         40,                              RESIST_LIGHTNING                 ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_VTEXLRD,  N_("Gorefeast"),                "GENERAL",    13,    771, AI_SKELSD,   3,         20,         55,                RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_RTBLACK,  N_("Graywar the Slayer"),       "GENERAL",    14,    672, AI_SKELSD,   1,         30,         50,                              RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_MAGISTR,  N_("Dreadjudge"),               "GENERAL",    14,    540, AI_COUNSLR,  1,         30,         40, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING                 ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_HLSPWN,   N_("Stareye the Witch"),        "GENERAL",    14,    726, AI_SUCC,     2,         30,         50,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_BTBLACK,  N_("Steelskull the Hunter"),    "GENERAL",    14,    831, AI_SKELSD,   3,         40,         50,                              RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_RBLACK,   N_("Sir Gorash"),               "GENERAL",    16,   1050, AI_SKELSD,   1,         20,         60,                                                 IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_CABALIST, N_("The Vizier"),               "GENERAL",    15,    850, AI_COUNSLR,  2,         25,         40,                IMMUNE_FIRE                                    ,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_REALWEAV, "Zamphir",                      "GENERAL",    15,    891, AI_SKELSD,   2,         30,         50, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_HLSPWN,   N_("Bloodlust"),                "GENERAL",    15,    825, AI_SUCC,     1,         20,         55, IMMUNE_MAGIC |               IMMUNE_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_HLSPWN,   "Webwidow",                     "GENERAL",    16,    774, AI_SUCC,     1,         20,         50, IMMUNE_MAGIC | IMMUNE_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_SOLBRNR,  N_("Fleshdancer"),              "GENERAL",    16,    999, AI_SUCC,     3,         30,         50, IMMUNE_MAGIC | RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	{  MT_OBLORD,   N_("Grimspike"),                "GENERAL",    19,    534, AI_SNEAK,    1,         25,         40, IMMUNE_MAGIC | RESIST_FIRE |                    IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
// TRANSLATORS: Unique Monster Block end
	{  MT_STORML,   N_("Doomlock"),                 "GENERAL",    28,    534, AI_SNEAK,    1,         35,         55, IMMUNE_MAGIC | RESIST_FIRE | RESIST_LIGHTNING | IMMUNE_NULL_40,  UniqueMonsterPack::Leashed,     0,        0, TEXT_NONE      },
	{  MT_INVALID,  nullptr,                        nullptr,       0,      0, AI_INVALID,  0,          0,          0, 0                                                             ,  UniqueMonsterPack::None,        0,        0, TEXT_NONE      },
	// clang-format on
};

} // namespace devilution
