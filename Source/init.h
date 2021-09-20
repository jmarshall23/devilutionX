/**
 * @file init.h
 *
 * Interface of routines for initializing the environment, disable screen saver, load MPQ.
 */
#pragma once

#include "miniwin/miniwin.h"

namespace devilution {

extern bool gbActive;
extern WNDPROC CurrentProc;
extern bool gbIsSpawn;
extern bool gbIsHellfire;
extern bool gbVanilla;

void init_cleanup();
void init_archives();
void init_create_window();
void MainWndProc(uint32_t Msg);
WNDPROC SetWindowProc(WNDPROC NewProc);

} // namespace devilution
