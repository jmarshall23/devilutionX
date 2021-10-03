/**
 * @file init.cpp
 *
 * Implementation of routines for initializing the environment, disable screen saver, load MPQ.
 */
#include <SDL.h>
#include <config.h>
#include <string>
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
#include <find_steam_game.h>
#endif

#include "DiabloUI/diabloui.h"
#include "dx.h"
#include "pfile.h"
#include "storm/storm.h"
#include "utils/language.h"
#include "utils/log.hpp"
#include "utils/paths.h"
#include "utils/ui_fwd.h"

#ifdef __vita__
// increase default allowed heap size on Vita
int _newlib_heap_size_user = 100 * 1024 * 1024;
#endif

namespace devilution {

/** True if the game is the current active window */
bool gbActive;
/** The current input handler function */
WNDPROC CurrentProc;
/** Indicate if we only have access to demo data */
bool gbIsSpawn;
/** Indicate if we have loaded the Hellfire expansion data */
bool gbIsHellfire;
/** Indicate if we want vanilla savefiles */
bool gbVanilla;

namespace {

HANDLE LoadMPQ(const std::vector<std::string> &paths, const char *mpqName)
{
	HANDLE archive;
	std::string mpqAbsPath;
	for (const auto &path : paths) {
		mpqAbsPath = path + mpqName;
		if (SFileOpenArchive(mpqAbsPath.c_str(), 0, MPQ_OPEN_READ_ONLY, &archive)) {
			LogVerbose("  Found: {} in {}", mpqName, path);
			SFileSetBasePath(path);
			return archive;
		}
		if (SErrGetLastError() != STORM_ERROR_FILE_NOT_FOUND) {
			LogError("Open error {}: {}", SErrGetLastError(), mpqAbsPath);
		}
	}
	if (SErrGetLastError() == STORM_ERROR_FILE_NOT_FOUND) {
		LogVerbose("Missing: {}", mpqName);
	}

	return nullptr;
}

} // namespace

void init_cleanup()
{
	if (gbIsMultiplayer && gbRunGame) {
		pfile_write_hero(/*writeGameData=*/false, /*clearTables=*/true);
	}
	NetClose();
}

void init_archives()
{
	std::vector<std::string> paths;
	paths.push_back(paths::BasePath());
	paths.push_back(paths::PrefPath());
	if (paths[0] == paths[1])
		paths.pop_back();

#if defined(__linux__) && !defined(__ANDROID__)
	paths.emplace_back("/usr/share/diasurgical/devilutionx/");
	paths.emplace_back("/usr/local/share/diasurgical/devilutionx/");
#elif defined(__3DS__)
	paths.emplace_back("romfs:/");
#elif defined(_WIN64) || defined(_WIN32)
	char gogpath[_FSG_PATH_MAX];
	fsg_get_gog_game_path(gogpath, "1412601690");
	if (strlen(gogpath) > 0) {
		paths.emplace_back(std::string(gogpath) + "/");
		paths.emplace_back(std::string(gogpath) + "/hellfire/");
	}
#endif

	paths.emplace_back(""); // PWD

	if (SDL_LOG_PRIORITY_VERBOSE >= SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION)) {
		std::string message;
		for (std::size_t i = 0; i < paths.size(); ++i) {
			char prefix[32];
			std::snprintf(prefix, sizeof(prefix), "\n%6u. '", static_cast<unsigned>(i + 1));
			message.append(prefix);
			message.append(paths[i]);
			message += '\'';
		}
		LogVerbose("MPQ search paths:{}", message);
	}

	HANDLE fh = nullptr;
	if (!SFileOpenFile("ui_art\\title.png", &fh))
		InsertCDDlg();
	SFileCloseFileThreadSafe(fh);
}

void init_create_window()
{
	if (!SpawnWindow("Hellfire"))
		app_fatal("%s", _("Unable to create main window"));
	dx_init();
	gbActive = true;
#ifndef USE_SDL1
	SDL_DisableScreenSaver();
#endif
}

void MainWndProc(uint32_t msg)
{
	switch (msg) {
	case DVL_WM_PAINT:
		force_redraw = 255;
		break;
	case DVL_WM_QUERYENDSESSION:
		diablo_quit(0);
	}
}

WNDPROC SetWindowProc(WNDPROC newProc)
{
	WNDPROC oldProc;

	oldProc = CurrentProc;
	CurrentProc = newProc;
	return oldProc;
}

} // namespace devilution
