/**
 * @file interfac.cpp
 *
 * Implementation of load screens.
 */

#include <cstdint>

#include "DiabloUI/art_draw.h"
#include "control.h"
#include "dx.h"
#include "engine.h"
#include "hwcursor.hpp"
#include "init.h"
#include "loadsave.h"
#include "palette.h"
#include "pfile.h"
#include "plrmsg.h"
#include "utils/sdl_geometry.h"
#include "utils/stdcompat/optional.hpp"

#include "datatable.h"
#include "../rhi/image.h"

namespace devilution {

namespace {

StormImage *sgpBackCel;

uint32_t sgdwProgress;
int progress_id = 1;

/** The color used for the progress bar as an index into the palette. */
const BYTE BarColor[3] = { 138, 43, 254 };
/** The screen position of the top left corner of the progress bar. */
const int BarPos[3][2] = { { 53, 37 }, { 53, 421 }, { 53, 37 } };

void FreeInterface()
{

}

Cutscenes PickCutscene(interface_mode uMsg)
{
	switch (uMsg) {
	case WM_DIABLOADGAME:
	case WM_DIABNEWGAME:
		return CutStart;
	case WM_DIABRETOWN:
		return CutTown;
	case WM_DIABNEXTLVL:
	case WM_DIABPREVLVL:
	case WM_DIABTOWNWARP:
	case WM_DIABTWARPUP: {
		int lvl = Players[MyPlayerId].plrlevel;
		if (lvl == 1 && uMsg == WM_DIABNEXTLVL)
			return CutTown;
		if (lvl == 16 && uMsg == WM_DIABNEXTLVL)
			return CutGate;

		switch (gnLevelTypeTbl[lvl]) {
		case DTYPE_TOWN:
			return CutTown;
		case DTYPE_CATHEDRAL:
			if (lvl > 16)
				return CutLevel5;
			return CutLevel1;
		case DTYPE_CATACOMBS:
			return CutLevel2;
		case DTYPE_CAVES:
			if (lvl > 16)
				return CutLevel6;
			return CutLevel3;
		case DTYPE_HELL:
			return CutLevel4;
		default:
			return CutLevel1;
		}
	}
	case WM_DIABWARPLVL:
		return CutPortal;
	case WM_DIABSETLVL:
	case WM_DIABRTNLVL:
		if (setlvlnum == SL_BONECHAMB)
			return CutLevel2;
		if (setlvlnum == SL_VILEBETRAYER)
			return CutPortalRed;
		return CutLevel1;
	default:
		app_fatal("Unknown progress mode");
	}
}

void InitCutscene(interface_mode uMsg)
{
	const char *celPath;
	const char *palPath;

	int cutId = PickCutscene(uMsg);
	celPath = cutsceneTable->GetValue("image", cutId);
	palPath = cutsceneTable->GetValue("palette", cutId);

	sgpBackCel = StormImage::LoadImageSequence(celPath, false, false);
	LoadPalette(palPath);

	sgdwProgress = 0;
}

void DrawCutscene()
{
	const Surface &out = GlobalBackBuffer();
	int height = sgpBackCel->GetFrame(1).height;
	sgpBackCel->ClipRenderNoLighting(out, PANEL_X, 480 - 1 + UI_OFFSET_Y, 1);

	RenderPresent();
}

} // namespace

void interface_msg_pump()
{
	tagMSG msg;

	while (FetchMessage(&msg)) {
		if (msg.message != DVL_WM_QUIT) {
			TranslateMessage(&msg);
			PushMessage(&msg);
		}
	}
}

bool IncProgress()
{
	interface_msg_pump();
	sgdwProgress += 23;
	if (sgdwProgress > 534)
		sgdwProgress = 534;
	if (sgpBackCel)
		DrawCutscene();
	return sgdwProgress >= 534;
}

void ShowProgress(interface_mode uMsg)
{
	WNDPROC saveProc;

	gbSomebodyWonGameKludge = false;
	plrmsg_delay(true);

	assert(ghMainWnd);
	saveProc = SetWindowProc(DisableInputWndProc);

	interface_msg_pump();
	ClearScreenBuffer();
	scrollrt_draw_game_screen();
	InitCutscene(uMsg);
	BlackPalette();
	DrawCutscene();

	if (IsHardwareCursor())
		SetHardwareCursorVisible(false);

	PaletteFadeIn(8);
	IncProgress();
	sound_init();
	IncProgress();

	auto &myPlayer = Players[MyPlayerId];

	switch (uMsg) {
	case WM_DIABLOADGAME:
		IncProgress();
		IncProgress();
		LoadGame(true);
		IncProgress();
		IncProgress();
		break;
	case WM_DIABNEWGAME:
		myPlayer.pOriginalCathedral = !gbIsHellfire;
		IncProgress();
		FreeGameMem();
		IncProgress();
		pfile_remove_temp_files();
		IncProgress();
		LoadGameLevel(true, ENTRY_MAIN);
		IncProgress();
		break;
	case WM_DIABNEXTLVL:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		setlevel = false;
		currlevel = myPlayer.plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		IncProgress();
		LoadGameLevel(false, ENTRY_MAIN);
		IncProgress();
		break;
	case WM_DIABPREVLVL:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		currlevel--;
		leveltype = gnLevelTypeTbl[currlevel];
		assert(myPlayer.plrlevel == currlevel);
		IncProgress();
		LoadGameLevel(false, ENTRY_PREV);
		IncProgress();
		break;
	case WM_DIABSETLVL:
		SetReturnLvlPos();
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		setlevel = true;
		leveltype = setlvltype;
		FreeGameMem();
		IncProgress();
		LoadGameLevel(false, ENTRY_SETLVL);
		IncProgress();
		break;
	case WM_DIABRTNLVL:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		setlevel = false;
		FreeGameMem();
		IncProgress();
		GetReturnLvlPos();
		LoadGameLevel(false, ENTRY_RTNLVL);
		IncProgress();
		break;
	case WM_DIABWARPLVL:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		GetPortalLevel();
		IncProgress();
		LoadGameLevel(false, ENTRY_WARPLVL);
		IncProgress();
		break;
	case WM_DIABTOWNWARP:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		setlevel = false;
		currlevel = myPlayer.plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		IncProgress();
		LoadGameLevel(false, ENTRY_TWARPDN);
		IncProgress();
		break;
	case WM_DIABTWARPUP:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		currlevel = myPlayer.plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		IncProgress();
		LoadGameLevel(false, ENTRY_TWARPUP);
		IncProgress();
		break;
	case WM_DIABRETOWN:
		IncProgress();
		if (!gbIsMultiplayer) {
			SaveLevel();
		} else {
			DeltaSaveLevel();
		}
		IncProgress();
		FreeGameMem();
		currlevel = myPlayer.plrlevel;
		leveltype = gnLevelTypeTbl[currlevel];
		IncProgress();
		LoadGameLevel(false, ENTRY_MAIN);
		IncProgress();
		break;
	}

	assert(ghMainWnd);

	PaletteFadeOut(8);
	FreeInterface();

	saveProc = SetWindowProc(saveProc);
	assert(saveProc == DisableInputWndProc);

	NetSendCmdLocParam1(true, CMD_PLAYER_JOINLEVEL, myPlayer.position.tile, myPlayer.plrlevel);
	plrmsg_delay(false);

	if (gbSomebodyWonGameKludge && myPlayer.plrlevel == 16) {
		PrepDoEnding();
	}

	gbSomebodyWonGameKludge = false;
}

} // namespace devilution
