/**
 * @file misdat.cpp
 *
 * Implementation of data related to missiles.
 */
#include "misdat.h"

#include "missiles.h"
#include "engine/load_file.hpp"
#include "engine/cel_header.hpp"

#include "datatable.h"

namespace devilution {

	#define MILLISLE_STRING_TO_ID(x) if (str == #x) { return x; }
	#define MISSILE_PROC_STRING(x) if (str == #x) { return &x; }

	MissileMovementDistrubution MissileMovementDistrubutionFromString(std::string str)
	{
	
		if (str == "Disabled")
		    return MissileMovementDistrubution::Disabled;

		if (str == "Blockable")
		    return MissileMovementDistrubution::Blockable;

		if (str == "UnBlockable")
		    return MissileMovementDistrubution::Unblockable;

		devilution::app_fatal("Invalid missile movement distribution!");
		
		return MissileMovementDistrubution::Disabled;
	}

	missile_resistance MissileResistFromString(std::string str)
	{
	    MILLISLE_STRING_TO_ID(MISR_NONE)
	    MILLISLE_STRING_TO_ID(MISR_FIRE)
	    MILLISLE_STRING_TO_ID(MISR_LIGHTNING)
	    MILLISLE_STRING_TO_ID(MISR_MAGIC)
	    MILLISLE_STRING_TO_ID(MISR_ACID)

		devilution::app_fatal("Invalid missile resist!");

	    return MISR_NONE;
	}

	missile_graphic_id MissileGraphicIdFromString(std::string str)
    {
	    MILLISLE_STRING_TO_ID(MFILE_ARROWS)
	    MILLISLE_STRING_TO_ID(MFILE_FIREBA)
	    MILLISLE_STRING_TO_ID(MFILE_GUARD)
	    MILLISLE_STRING_TO_ID(MFILE_LGHNING)
	    MILLISLE_STRING_TO_ID(MFILE_FIREWAL)
	    MILLISLE_STRING_TO_ID(MFILE_MAGBLOS)
	    MILLISLE_STRING_TO_ID(MFILE_PORTAL)
	    MILLISLE_STRING_TO_ID(MFILE_BLUEXFR)
	    MILLISLE_STRING_TO_ID(MFILE_BLUEXBK)
	    MILLISLE_STRING_TO_ID(MFILE_MANASHLD)
	    MILLISLE_STRING_TO_ID(MFILE_BLOOD)
	    MILLISLE_STRING_TO_ID(MFILE_BONE)
	    MILLISLE_STRING_TO_ID(MFILE_METLHIT)
	    MILLISLE_STRING_TO_ID(MFILE_FARROW)
	    MILLISLE_STRING_TO_ID(MFILE_DOOM)
	    MILLISLE_STRING_TO_ID(MFILE_0F)
	    MILLISLE_STRING_TO_ID(MFILE_BLODBUR)
	    MILLISLE_STRING_TO_ID(MFILE_NEWEXP)
	    MILLISLE_STRING_TO_ID(MFILE_SHATTER1)
	    MILLISLE_STRING_TO_ID(MFILE_BIGEXP)
	    MILLISLE_STRING_TO_ID(MFILE_INFERNO)
	    MILLISLE_STRING_TO_ID(MFILE_THINLGHT)
	    MILLISLE_STRING_TO_ID(MFILE_FLARE)
	    MILLISLE_STRING_TO_ID(MFILE_FLAREEXP)
	    MILLISLE_STRING_TO_ID(MFILE_MAGBALL)
	    MILLISLE_STRING_TO_ID(MFILE_KRULL)
	    MILLISLE_STRING_TO_ID(MFILE_MINILTNG)
	    MILLISLE_STRING_TO_ID(MFILE_HOLY)
	    MILLISLE_STRING_TO_ID(MFILE_HOLYEXPL)
	    MILLISLE_STRING_TO_ID(MFILE_LARROW)
	    MILLISLE_STRING_TO_ID(MFILE_FIRARWEX)
	    MILLISLE_STRING_TO_ID(MFILE_ACIDBF)
	    MILLISLE_STRING_TO_ID(MFILE_ACIDSPLA)
	    MILLISLE_STRING_TO_ID(MFILE_ACIDPUD)
	    MILLISLE_STRING_TO_ID(MFILE_ETHRSHLD)
	    MILLISLE_STRING_TO_ID(MFILE_FIRERUN)
	    MILLISLE_STRING_TO_ID(MFILE_RESSUR1)
	    MILLISLE_STRING_TO_ID(MFILE_SKLBALL)
	    MILLISLE_STRING_TO_ID(MFILE_RPORTAL)
	    MILLISLE_STRING_TO_ID(MFILE_FIREPLAR)
	    MILLISLE_STRING_TO_ID(MFILE_SCUBMISB)
	    MILLISLE_STRING_TO_ID(MFILE_SCBSEXPB)
	    MILLISLE_STRING_TO_ID(MFILE_SCUBMISC)
	    MILLISLE_STRING_TO_ID(MFILE_SCBSEXPC)
	    MILLISLE_STRING_TO_ID(MFILE_SCUBMISD)
	    MILLISLE_STRING_TO_ID(MFILE_SCBSEXPD)
	    MILLISLE_STRING_TO_ID(MFILE_SPAWNS)
	    MILLISLE_STRING_TO_ID(MFILE_REFLECT)
	    MILLISLE_STRING_TO_ID(MFILE_LICH)
	    MILLISLE_STRING_TO_ID(MFILE_MSBLA)
	    MILLISLE_STRING_TO_ID(MFILE_NECROMORB)
	    MILLISLE_STRING_TO_ID(MFILE_ARCHLICH)
	    MILLISLE_STRING_TO_ID(MFILE_RUNE)
	    MILLISLE_STRING_TO_ID(MFILE_EXYEL2)
	    MILLISLE_STRING_TO_ID(MFILE_EXBL2)
	    MILLISLE_STRING_TO_ID(MFILE_EXRED3)
	    MILLISLE_STRING_TO_ID(MFILE_BONEDEMON)
	    MILLISLE_STRING_TO_ID(MFILE_EXORA1)
	    MILLISLE_STRING_TO_ID(MFILE_EXBL3)
	    MILLISLE_STRING_TO_ID(MFILE_NONE)

		devilution::app_fatal("Invalid missile gfx type!");

	    return MFILE_NONE;
	}

	void *MissileGetAddProcFromString(std::string str)
	{
		MISSILE_PROC_STRING(AddHiveExplosion)
		MISSILE_PROC_STRING(AddFireRune)
		MISSILE_PROC_STRING(AddLightningRune)
		MISSILE_PROC_STRING(AddGreatLightningRune)
		MISSILE_PROC_STRING(AddImmolationRune)
		MISSILE_PROC_STRING(AddStoneRune)
		MISSILE_PROC_STRING(AddReflection)
		MISSILE_PROC_STRING(AddBerserk)
		MISSILE_PROC_STRING(AddHorkSpawn)
		MISSILE_PROC_STRING(AddJester)
		MISSILE_PROC_STRING(AddStealPotions)
		MISSILE_PROC_STRING(AddManaTrap)
		MISSILE_PROC_STRING(AddSpecArrow)
		MISSILE_PROC_STRING(AddWarp)
		MISSILE_PROC_STRING(AddLightningWall)
		MISSILE_PROC_STRING(AddRuneExplosion)
		MISSILE_PROC_STRING(AddFireNova)
		MISSILE_PROC_STRING(AddLightningArrow)
		MISSILE_PROC_STRING(AddMana)
		MISSILE_PROC_STRING(AddMagi)
		MISSILE_PROC_STRING(AddRing)
		MISSILE_PROC_STRING(AddSearch)
		MISSILE_PROC_STRING(AddCboltArrow)
		MISSILE_PROC_STRING(AddLArrow)
		MISSILE_PROC_STRING(AddArrow)
		MISSILE_PROC_STRING(AddRndTeleport)
		MISSILE_PROC_STRING(AddFirebolt)
		MISSILE_PROC_STRING(AddMagmaball)
		MISSILE_PROC_STRING(AddTeleport)
		MISSILE_PROC_STRING(AddLightball)
		MISSILE_PROC_STRING(AddFirewall)
		MISSILE_PROC_STRING(AddFireball)
		MISSILE_PROC_STRING(AddLightctrl)
		MISSILE_PROC_STRING(AddLightning)
		MISSILE_PROC_STRING(AddMisexp)
		MISSILE_PROC_STRING(AddWeapexp)
		MISSILE_PROC_STRING(AddTown)
		MISSILE_PROC_STRING(AddFlash)
		MISSILE_PROC_STRING(AddFlash2)
		MISSILE_PROC_STRING(AddManashield)
		MISSILE_PROC_STRING(AddFiremove)
		MISSILE_PROC_STRING(AddGuardian)
		MISSILE_PROC_STRING(AddChain)
		MISSILE_PROC_STRING(AddRhino)
		MISSILE_PROC_STRING(AddFlare)
		MISSILE_PROC_STRING(AddAcid)
		MISSILE_PROC_STRING(AddAcidpud)
		MISSILE_PROC_STRING(AddStone)
		MISSILE_PROC_STRING(AddGolem)
		MISSILE_PROC_STRING(AddBoom)
		MISSILE_PROC_STRING(AddHeal)
		MISSILE_PROC_STRING(AddHealOther)
		MISSILE_PROC_STRING(AddElement)
		MISSILE_PROC_STRING(AddIdentify)
		MISSILE_PROC_STRING(AddFirewallC)
		MISSILE_PROC_STRING(AddInfra)
		MISSILE_PROC_STRING(AddWave)
		MISSILE_PROC_STRING(AddNova)
		MISSILE_PROC_STRING(AddBlodboil)
		MISSILE_PROC_STRING(AddRepair)
		MISSILE_PROC_STRING(AddRecharge)
		MISSILE_PROC_STRING(AddDisarm)
		MISSILE_PROC_STRING(AddApoca)
		MISSILE_PROC_STRING(AddFlame)
		MISSILE_PROC_STRING(AddFlamec)
		MISSILE_PROC_STRING(AddCbolt)
		MISSILE_PROC_STRING(AddHbolt)
		MISSILE_PROC_STRING(AddResurrect)
		MISSILE_PROC_STRING(AddResurrectBeam)
		MISSILE_PROC_STRING(AddTelekinesis)
		MISSILE_PROC_STRING(AddBoneSpirit)
		MISSILE_PROC_STRING(AddRportal)
		MISSILE_PROC_STRING(AddDiabApoca)

		devilution::app_fatal("Invalid missile addproc!");
	    return nullptr;
	}

	void *MissileGetProcFromString(std::string str)
    {
	    //MISSILE_PROC_STRING(MI_Golem)
	    //MISSILE_PROC_STRING(MI_Manashield)
	    MISSILE_PROC_STRING(MI_LArrow)
	    MISSILE_PROC_STRING(MI_Arrow)
	    MISSILE_PROC_STRING(MI_Firebolt)
	    MISSILE_PROC_STRING(MI_Lightball)
	    MISSILE_PROC_STRING(MI_Acidpud)
	    MISSILE_PROC_STRING(MI_Firewall)
	    MISSILE_PROC_STRING(MI_Fireball)
	    MISSILE_PROC_STRING(MI_HorkSpawn)
	    MISSILE_PROC_STRING(MI_Rune)
	    MISSILE_PROC_STRING(MI_LightningWall)
	    MISSILE_PROC_STRING(MI_HiveExplode)
	    MISSILE_PROC_STRING(MI_LightningArrow)
	    MISSILE_PROC_STRING(MI_FireRing)
	    MISSILE_PROC_STRING(MI_Search)
	    MISSILE_PROC_STRING(MI_LightningWallC)
	    MISSILE_PROC_STRING(MI_FireNova)
	    MISSILE_PROC_STRING(MI_SpecArrow)
	    MISSILE_PROC_STRING(MI_Lightctrl)
	    MISSILE_PROC_STRING(MI_Lightning)
	    MISSILE_PROC_STRING(MI_Town)
	    MISSILE_PROC_STRING(MI_Flash)
	    MISSILE_PROC_STRING(MI_Flash2)
	    MISSILE_PROC_STRING(MI_Firemove)
	    MISSILE_PROC_STRING(MI_Guardian)
	    MISSILE_PROC_STRING(MI_Chain)
	    MISSILE_PROC_STRING(MI_Weapexp)
	    MISSILE_PROC_STRING(MI_Misexp)
	    MISSILE_PROC_STRING(MI_Acidsplat)
	    MISSILE_PROC_STRING(MI_Teleport)
	    MISSILE_PROC_STRING(MI_Stone)
	    MISSILE_PROC_STRING(MI_Boom)
	    MISSILE_PROC_STRING(MI_Rhino)
	    MISSILE_PROC_STRING(MI_FirewallC)
	    MISSILE_PROC_STRING(MI_Infra)
	    MISSILE_PROC_STRING(MI_Apoca)
	    MISSILE_PROC_STRING(MI_Wave)
	    MISSILE_PROC_STRING(MI_Nova)
	    MISSILE_PROC_STRING(MI_Blodboil)
	    MISSILE_PROC_STRING(MI_Flame)
	    MISSILE_PROC_STRING(MI_Flamec)
	    MISSILE_PROC_STRING(MI_Cbolt)
	    MISSILE_PROC_STRING(MI_Hbolt)
	    MISSILE_PROC_STRING(MI_Element)
	    MISSILE_PROC_STRING(MI_Bonespirit)
	    MISSILE_PROC_STRING(MI_ResurrectBeam)
	    MISSILE_PROC_STRING(MI_Rportal)

	    devilution::app_fatal("Invalid missile proc!");
	    return nullptr;
    }

	devilution::missile_id MissileIdFromString(std::string str)
	{
	    MILLISLE_STRING_TO_ID(MIS_NULL)
	    MILLISLE_STRING_TO_ID(MIS_ARROW)
		MILLISLE_STRING_TO_ID(MIS_FIREBOLT)
		MILLISLE_STRING_TO_ID(MIS_GUARDIAN)
		MILLISLE_STRING_TO_ID(MIS_RNDTELEPORT)
		MILLISLE_STRING_TO_ID(MIS_LIGHTBALL)
		MILLISLE_STRING_TO_ID(MIS_FIREWALL)
		MILLISLE_STRING_TO_ID(MIS_FIREBALL)
		MILLISLE_STRING_TO_ID(MIS_LIGHTCTRL)
		MILLISLE_STRING_TO_ID(MIS_LIGHTNING)
		MILLISLE_STRING_TO_ID(MIS_MISEXP)
		MILLISLE_STRING_TO_ID(MIS_TOWN)
		MILLISLE_STRING_TO_ID(MIS_FLASH)
		MILLISLE_STRING_TO_ID(MIS_FLASH2)
		MILLISLE_STRING_TO_ID(MIS_MANASHIELD)
		MILLISLE_STRING_TO_ID(MIS_FIREMOVE)
		MILLISLE_STRING_TO_ID(MIS_CHAIN)
		MILLISLE_STRING_TO_ID(MIS_SENTINAL)
		MILLISLE_STRING_TO_ID(MIS_BLODSTAR)
		MILLISLE_STRING_TO_ID(MIS_BONE)
		MILLISLE_STRING_TO_ID(MIS_METLHIT)
		MILLISLE_STRING_TO_ID(MIS_RHINO)
		MILLISLE_STRING_TO_ID(MIS_MAGMABALL)
		MILLISLE_STRING_TO_ID(MIS_LIGHTCTRL2)
		MILLISLE_STRING_TO_ID(MIS_LIGHTNING2)
		MILLISLE_STRING_TO_ID(MIS_FLARE)
		MILLISLE_STRING_TO_ID(MIS_MISEXP2)
		MILLISLE_STRING_TO_ID(MIS_TELEPORT)
		MILLISLE_STRING_TO_ID(MIS_FARROW)
		MILLISLE_STRING_TO_ID(MIS_DOOMSERP)
		MILLISLE_STRING_TO_ID(MIS_FIREWALLA)
		MILLISLE_STRING_TO_ID(MIS_STONE)
		MILLISLE_STRING_TO_ID(MIS_NULL_1F)
		MILLISLE_STRING_TO_ID(MIS_INVISIBL)
		MILLISLE_STRING_TO_ID(MIS_GOLEM)
		MILLISLE_STRING_TO_ID(MIS_ETHEREALIZE)
		MILLISLE_STRING_TO_ID(MIS_BLODBUR)
		MILLISLE_STRING_TO_ID(MIS_BOOM)
		MILLISLE_STRING_TO_ID(MIS_HEAL)
		MILLISLE_STRING_TO_ID(MIS_FIREWALLC)
		MILLISLE_STRING_TO_ID(MIS_INFRA)
		MILLISLE_STRING_TO_ID(MIS_IDENTIFY)
		MILLISLE_STRING_TO_ID(MIS_WAVE)
		MILLISLE_STRING_TO_ID(MIS_NOVA)
		MILLISLE_STRING_TO_ID(MIS_BLODBOIL)
		MILLISLE_STRING_TO_ID(MIS_APOCA)
		MILLISLE_STRING_TO_ID(MIS_REPAIR)
		MILLISLE_STRING_TO_ID(MIS_RECHARGE)
		MILLISLE_STRING_TO_ID(MIS_DISARM)
		MILLISLE_STRING_TO_ID(MIS_FLAME)
		MILLISLE_STRING_TO_ID(MIS_FLAMEC)
		MILLISLE_STRING_TO_ID(MIS_FIREMAN)
		MILLISLE_STRING_TO_ID(MIS_KRULL)
		MILLISLE_STRING_TO_ID(MIS_CBOLT)
		MILLISLE_STRING_TO_ID(MIS_HBOLT)
		MILLISLE_STRING_TO_ID(MIS_RESURRECT)
		MILLISLE_STRING_TO_ID(MIS_TELEKINESIS)
		MILLISLE_STRING_TO_ID(MIS_LARROW)
		MILLISLE_STRING_TO_ID(MIS_ACID)
		MILLISLE_STRING_TO_ID(MIS_MISEXP3)
		MILLISLE_STRING_TO_ID(MIS_ACIDPUD)
		MILLISLE_STRING_TO_ID(MIS_HEALOTHER)
		MILLISLE_STRING_TO_ID(MIS_ELEMENT)
		MILLISLE_STRING_TO_ID(MIS_RESURRECTBEAM)
		MILLISLE_STRING_TO_ID(MIS_BONESPIRIT)
		MILLISLE_STRING_TO_ID(MIS_WEAPEXP)
		MILLISLE_STRING_TO_ID(MIS_RPORTAL)
		MILLISLE_STRING_TO_ID(MIS_BOOM2)
		MILLISLE_STRING_TO_ID(MIS_DIABAPOCA)
		MILLISLE_STRING_TO_ID(MIS_MANA)
		MILLISLE_STRING_TO_ID(MIS_MAGI)
		MILLISLE_STRING_TO_ID(MIS_LIGHTWALL)
		MILLISLE_STRING_TO_ID(MIS_LIGHTNINGWALL)
		MILLISLE_STRING_TO_ID(MIS_IMMOLATION)
		MILLISLE_STRING_TO_ID(MIS_SPECARROW)
		MILLISLE_STRING_TO_ID(MIS_FIRENOVA)
		MILLISLE_STRING_TO_ID(MIS_LIGHTARROW)
		MILLISLE_STRING_TO_ID(MIS_CBOLTARROW)
		MILLISLE_STRING_TO_ID(MIS_HBOLTARROW)
		MILLISLE_STRING_TO_ID(MIS_WARP)
		MILLISLE_STRING_TO_ID(MIS_REFLECT)
		MILLISLE_STRING_TO_ID(MIS_BERSERK)
		MILLISLE_STRING_TO_ID(MIS_FIRERING)
		MILLISLE_STRING_TO_ID(MIS_STEALPOTS)
		MILLISLE_STRING_TO_ID(MIS_MANATRAP)
		MILLISLE_STRING_TO_ID(MIS_LIGHTRING)
		MILLISLE_STRING_TO_ID(MIS_SEARCH)
		MILLISLE_STRING_TO_ID(MIS_FLASHFR)
		MILLISLE_STRING_TO_ID(MIS_FLASHBK)
		MILLISLE_STRING_TO_ID(MIS_IMMOLATION2)
		MILLISLE_STRING_TO_ID(MIS_RUNEFIRE)
		MILLISLE_STRING_TO_ID(MIS_RUNELIGHT)
		MILLISLE_STRING_TO_ID(MIS_RUNENOVA)
		MILLISLE_STRING_TO_ID(MIS_RUNEIMMOLAT)
		MILLISLE_STRING_TO_ID(MIS_RUNESTONE)
		MILLISLE_STRING_TO_ID(MIS_HIVEEXP)
		MILLISLE_STRING_TO_ID(MIS_HORKDMN)
		MILLISLE_STRING_TO_ID(MIS_JESTER)
		MILLISLE_STRING_TO_ID(MIS_HIVEEXP2)
		MILLISLE_STRING_TO_ID(MIS_LICH)
		MILLISLE_STRING_TO_ID(MIS_PSYCHORB)
		MILLISLE_STRING_TO_ID(MIS_NECROMORB)
		MILLISLE_STRING_TO_ID(MIS_ARCHLICH)
		MILLISLE_STRING_TO_ID(MIS_BONEDEMON)
		MILLISLE_STRING_TO_ID(MIS_EXYEL2)
		MILLISLE_STRING_TO_ID(MIS_EXRED3)
		MILLISLE_STRING_TO_ID(MIS_EXBL2)
		MILLISLE_STRING_TO_ID(MIS_EXBL3)
		MILLISLE_STRING_TO_ID(MIS_EXORA1)

		devilution::app_fatal("Invalid missile tpye!");
	    return MIS_EXORA1;
	}


    MissileDataStruct *MissileData;

	void InitMissileDataTable(void)
    {
	    MissileData = new MissileDataStruct[missileTable->NumRows()];

		for (int i = 0; i < missileTable->NumRows(); i++) {
		    const char *mAddProc = missileTable->GetValue("mAddProc", i);
		    const char *mProc = missileTable->GetValue("mProc", i);

			if (mAddProc) {
			    MissileData[i].mAddProc = (void(__cdecl *)(devilution::Missile &, devilution::Point, devilution::Direction))MissileGetAddProcFromString(mAddProc);
		    } else {
			    MissileData[i].mAddProc = nullptr;
			}

			if (mProc) {
			    MissileData[i].mProc = (void(__cdecl *)(devilution::Missile &))MissileGetProcFromString(mProc);
		    } else {
			    MissileData[i].mProc = nullptr;
			}
		    MissileData[i].mName = MissileIdFromString(missileTable->GetValue("mName", i));
		    MissileData[i].mDraw = missileTable->GetInt("mDraw", i);
		    MissileData[i].mType = missileTable->GetInt("mType", i);
		    MissileData[i].mResist = MissileResistFromString(missileTable->GetValue("mResist", i));
		    MissileData[i].mFileNum = MissileGraphicIdFromString(missileTable->GetValue("mFileNum", i));
		    MissileData[i].mlSFX = SfxIdFromString(missileTable->GetValue("mlSFX", i));
		    MissileData[i].miSFX = SfxIdFromString(missileTable->GetValue("miSFX", i));
		    MissileData[i].MovementDistribution = MissileMovementDistrubutionFromString(missileTable->GetValue("MovementDistribution", i));
		}
	}

/** Data related to each missile graphic ID. */
MissileFileData MissileSpriteData[] = {
	// clang-format off
	// name,      animName,        animFAmt, flags,                          animDelay[16], animLen[16],                     animWidth, animWidth2
	{ "Arrows",   MFILE_ARROWS,     1,       MissileDataFlags::NotAnimated,  { 0 },         { 16 },                                 96,         16 },
	{ "Fireba",   MFILE_FIREBA,    16,       MissileDataFlags::None,         { 0 },         { 14 },                                 96,         16 },
	{ "Guard",    MFILE_GUARD,      3,       MissileDataFlags::None,         { 1 },         { 15, 14,  3 },                         96,         16 },
	{ "Lghning",  MFILE_LGHNING,    1,       MissileDataFlags::None,         { 0 },         {  8 },                                 96,         16 },
	{ "Firewal",  MFILE_FIREWAL,    2,       MissileDataFlags::None,         { 0 },         { 13, 11 },                            128,         32 },
	{ "MagBlos",  MFILE_MAGBLOS,    1,       MissileDataFlags::None,         { 1 },         { 10 },                                128,         32 },
	{ "Portal",   MFILE_PORTAL,     2,       MissileDataFlags::None,         { 0, 1 },      { 16 },                                 96,         16 },
	{ "Bluexfr",  MFILE_BLUEXFR,    1,       MissileDataFlags::None,         { 0 },         { 19 },                                160,         48 },
	{ "Bluexbk",  MFILE_BLUEXBK,    1,       MissileDataFlags::None,         { 0 },         { 19 },                                160,         48 },
	{ "Manashld", MFILE_MANASHLD,   1,       MissileDataFlags::NotAnimated,  { 0 },         {  1 },                                 96,         16 },
	{ nullptr,    MFILE_BLOOD,      4,       MissileDataFlags::None,         { 0 },         { 15 },                                 96,         16 },
	{ nullptr,    MFILE_BONE,       3,       MissileDataFlags::None,         { 2 },         {  8 },                                128,         32 },
	{ nullptr,    MFILE_METLHIT,    3,       MissileDataFlags::None,         { 2 },         { 10 },                                 96,         16 },
	{ "Farrow",   MFILE_FARROW,    16,       MissileDataFlags::None,         { 0 },         {  4 },                                 96,         16 },
	{ "Doom",     MFILE_DOOM,       9,       MissileDataFlags::MonsterOwned, { 1 },         { 15 },                                 96,         16 },
	{ nullptr,    MFILE_0F,         1,       MissileDataFlags::MonsterOwned, { 0 },         {  0 },                                  0,          0 },
	{ nullptr,    MFILE_BLODBUR,    2,       MissileDataFlags::None,         { 2 },         {  8 },                                128,         32 },
	{ "Newexp",   MFILE_NEWEXP,     1,       MissileDataFlags::None,         { 1 },         { 15 },                                 96,         16 },
	{ "Shatter1", MFILE_SHATTER1,   1,       MissileDataFlags::None,         { 1 },         { 12 },                                128,         32 },
	{ "Bigexp",   MFILE_BIGEXP,     1,       MissileDataFlags::None,         { 0 },         { 15 },                                160,         48 },
	{ "Inferno",  MFILE_INFERNO,    1,       MissileDataFlags::None,         { 0 },         { 20 },                                 96,         16 },
	{ "Thinlght", MFILE_THINLGHT,   1,       MissileDataFlags::MonsterOwned, { 0 },         {  8 },                                 96,         16 },
	{ "Flare",    MFILE_FLARE,      1,       MissileDataFlags::None,         { 0 },         { 16 },                                128,         32 },
	{ "Flareexp", MFILE_FLAREEXP,   1,       MissileDataFlags::None,         { 0 },         {  7 },                                128,         32 },
	{ "Magball",  MFILE_MAGBALL,    8,       MissileDataFlags::MonsterOwned, { 1 },         { 16 },                                128,         32 },
	{ "Krull",    MFILE_KRULL,      1,       MissileDataFlags::MonsterOwned, { 0 },         { 14 },                                 96,         16 },
	{ "Miniltng", MFILE_MINILTNG,   1,       MissileDataFlags::None,         { 1 },         {  8 },                                 64,          0 },
	{ "Holy",     MFILE_HOLY,      16,       MissileDataFlags::None,         { 1, 0 },      { 14 },                                 96,         16 },
	{ "Holyexpl", MFILE_HOLYEXPL,   1,       MissileDataFlags::None,         { 0 },         {  8 },                                160,         48 },
	{ "Larrow",   MFILE_LARROW,    16,       MissileDataFlags::None,         { 0 },         {  4 },                                 96,         16 },
	{ nullptr,    MFILE_FIRARWEX,   1,       MissileDataFlags::None,         { 0 },         {  6 },                                 64,          0 },
	{ "Acidbf",   MFILE_ACIDBF,    16,       MissileDataFlags::MonsterOwned, { 0 },         {  8 },                                 96,         16 },
	{ "Acidspla", MFILE_ACIDSPLA,   1,       MissileDataFlags::MonsterOwned, { 0 },         {  8 },                                 96,         16 },
	{ "Acidpud",  MFILE_ACIDPUD,    2,       MissileDataFlags::MonsterOwned, { 0 },         {  9,  4 },                             96,         16 },
	{ nullptr,    MFILE_ETHRSHLD,   1,       MissileDataFlags::None,         { 0 },         {  1 },                                 96,         16 },
	{ "Firerun",  MFILE_FIRERUN,    8,       MissileDataFlags::None,         { 1 },         { 12 },                                 96,         16 },
	{ "Ressur1",  MFILE_RESSUR1,    1,       MissileDataFlags::None,         { 0 },         { 16 },                                 96,         16 },
	{ "Sklball",  MFILE_SKLBALL,    9,       MissileDataFlags::None,         { 1 },         { 16, 16, 16, 16, 16, 16, 16, 16, 8 },  96,         16 },
	{ "Rportal",  MFILE_RPORTAL,    2,       MissileDataFlags::None,         { 0 },         { 16 },                                 96,         16 },
	{ "Fireplar", MFILE_FIREPLAR,   1,       MissileDataFlags::MonsterOwned, { 1 },         { 17 },                                160,         48 },
	{ "Scubmisb", MFILE_SCUBMISB,   1,       MissileDataFlags::MonsterOwned, { 0 },         { 16 },                                 96,         16 },
	{ "Scbsexpb", MFILE_SCBSEXPB,   1,       MissileDataFlags::MonsterOwned, { 0 },         {  6 },                                128,         32 },
	{ "Scubmisc", MFILE_SCUBMISC,   1,       MissileDataFlags::MonsterOwned, { 0 },         { 16 },                                 96,         16 },
	{ "Scbsexpc", MFILE_SCBSEXPC,   1,       MissileDataFlags::MonsterOwned, { 0 },         {  6 },                                128,         32 },
	{ "Scubmisd", MFILE_SCUBMISD,   1,       MissileDataFlags::MonsterOwned, { 0 },         { 16 },                                 96,         16 },
	{ "Scbsexpd", MFILE_SCBSEXPD,   1,       MissileDataFlags::MonsterOwned, { 0 },         {  6 },                                128,         32 },
	{ "spawns",   MFILE_SPAWNS,     8,       MissileDataFlags::MonsterOwned, { 0 },         {  9 },                                 96,         16 },
	{ "reflect",  MFILE_REFLECT,    1,       MissileDataFlags::NotAnimated,  { 0 },         {  1 },                                160,         64 },
	{ "ms_ora",   MFILE_LICH,      16,       MissileDataFlags::MonsterOwned, { 0 },         { 15 },                                 96,          8 },
	{ "ms_bla",   MFILE_MSBLA,     16,       MissileDataFlags::MonsterOwned, { 0 },         { 15 },                                 96,          8 },
	{ "ms_reb",   MFILE_NECROMORB, 16,       MissileDataFlags::MonsterOwned, { 0 },         { 15 },                                 96,          8 },
	{ "ms_yeb",   MFILE_ARCHLICH,  16,       MissileDataFlags::MonsterOwned, { 0 },         { 15 },                                 96,          8 },
	{ "rglows1",  MFILE_RUNE,       1,       MissileDataFlags::None,         { 0 },         { 10 },                                 96,          8 },
	{ "ex_yel2",  MFILE_EXYEL2,     1,       MissileDataFlags::MonsterOwned, { 0 },         { 10 },                                220,         78 },
	{ "ex_blu2",  MFILE_EXBL2,      1,       MissileDataFlags::MonsterOwned, { 0 },         { 10 },                                212,         86 },
	{ "ex_red3",  MFILE_EXRED3,     1,       MissileDataFlags::MonsterOwned, { 0 },         {  7 },                                292,        114 },
	{ "ms_blb",   MFILE_BONEDEMON, 16,       MissileDataFlags::MonsterOwned, { 0 },         { 15 },                                 96,          8 },
	{ "ex_ora1",  MFILE_EXORA1,     1,       MissileDataFlags::MonsterOwned, { 0 },         { 13 },                                 96,        -12 },
	{ "ex_blu3",  MFILE_EXBL3,      1,       MissileDataFlags::MonsterOwned, { 0 },         {  7 },                                292,        114 },
	{ "",         MFILE_NONE,       0,       MissileDataFlags::None,         {  },          { },                                     0,          0 },
	// clang-format on
};

namespace {

template <typename T>
std::array<T, 16> maybeAutofill(std::initializer_list<T> list)
{
	assert(list.size() <= 16);

	std::array<T, 16> ret = {};

	if (list.size() == 1) {
		ret.fill(*list.begin());
	} else {
		int i = 0;
		for (T x : list)
			ret[i++] = x;
	}
	return ret;
}

} // namespace

MissileFileData::MissileFileData(const char *name, uint8_t animName, uint8_t animFAmt, MissileDataFlags flags,
    std::initializer_list<uint8_t> animDelay, std::initializer_list<uint8_t> animLen,
    int16_t animWidth, int16_t animWidth2)
    : name(name)
    , animName(animName)
    , animFAmt(animFAmt)
    , flags(flags)
    , animDelay(maybeAutofill(animDelay))
    , animLen(maybeAutofill(animLen))
    , animWidth(animWidth)
    , animWidth2(animWidth2)
{
}

void MissileFileData::LoadGFX()
{
	if (animData[0] != nullptr)
		return;

	if (name == nullptr)
		return;

	char pszName[256];
	if (animFAmt == 1) {
		sprintf(pszName, "Missiles\\%s.CL2", name);
		animData[0] = LoadFileInMem(pszName);
	} else {
		for (unsigned i = 0; i < animFAmt; i++) {
			sprintf(pszName, "Missiles\\%s%u.CL2", name, i + 1);
			animData[i] = LoadFileInMem(pszName);
		}
	}
}

} // namespace devilution
