/* ScummVM - Scumm Interpreter
 * Copyright (C) 2006 The ScummVM project
 *
 * cinE Engine is (C) 2004-2005 by CinE Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "cine/cine.h"

mouseStatusStruct mouseData;

u16 mouseRight = 0;
u16 mouseLeft = 0;

u16 mouseUpdateStatus;
u16 dummyU16;

void manageEvents(void) {
	OSystem::Event event;

	while (g_system->pollEvent(event)) {
		switch (event.type) {
		case OSystem::EVENT_LBUTTONDOWN:
			mouseLeft = 1;
			break;
		case OSystem::EVENT_RBUTTONDOWN:
			mouseRight = 1;
			break;
		case OSystem::EVENT_MOUSEMOVE:
			mouseData.X = event.mouse.x;
			mouseData.Y = event.mouse.y;
			break;
		case OSystem::EVENT_QUIT:
			g_system->quit();
			break;
		default:
			break;
		}
	}

	mouseData.left = mouseLeft;
	mouseData.right = mouseRight;

	mouseLeft = 0;
	mouseRight = 0;
}

void getMouseData(u16 param, u16 *pButton, u16 *pX, u16 *pY) {
	*pX = mouseData.X;
	*pY = mouseData.Y;

	*pButton = 0;

	if (mouseData.right) {
		(*pButton) |= 2;
	}

	if (mouseData.left) {
		(*pButton) |= 1;
	}
}

void mainLoop(int bootScriptIdx) {
	u16 var_6;
	u16 var_2;
	u16 i;
	char *di;
	u16 mouseButton;

	closeEngine3();
	resetMessageHead();
	resetUnkList();
	resetglobalScriptsHead();
	resetObjectScriptHead();
	mainLoopSub1();

	mainLoopSub2(0, 0, 20, 200);

	errorVar = 0;

	addScriptToList0(bootScriptIdx);

	menuVar = 0;

	gfxFuncGen1(page0c, page0, page0c, page0, -1);

	ptrGfxFunc13();

	gfxFuncGen2();

	var_2 = 0;
	allowPlayerInput = 0;
	checkForPendingDataLoadSwitch = 0;

	fadeRequired = 0;
	isDrawCommandEnabled = 0;
	waitForPlayerClick = 0;
	var16 = 0;

	playerCommand = -1;
	strcpy(commandBuffer, "");

	globalVars[0x1F2] = 0;
	globalVars[0x1F4] = 0;

	for (i = 0; i < 16; i++) {
		c_palette[i] = 0;
	}

	var17 = 1;

	strcpy(newPrcName, "");
	strcpy(newRelName, "");
	strcpy(newObjectName, "");
	strcpy(newMsgName, "");
	strcpy(currentBgName[0], "");
	strcpy(currentCtName, "");
	strcpy(currentPartName, "");

	stopSample();

	do {
		mainLoopSub3();
		di = (char *)executePlayerInput();

		if (var18 != 0) {
			if (var18 >= 100 || var19) {
				stopSample();
			}
		}

		processUnkList();
		executeList1();
		executeList0();

		purgeList1();
		purgeList0();

		if (playerCommand == -1) {
			processPendingUpdates(0);
		} else {
			processPendingUpdates(2);
		}

		drawOverlays();
		flip();

		if (waitForPlayerClick) {
			var_6 = 0;

			var20 <<= 3;

			if (var20 < 0x800)
				var20 = 0x800;

			do {
				manageEvents();
				getMouseData(mouseUpdateStatus, &mouseButton,
				    &dummyU16, &dummyU16);
			} while (mouseButton != 0);

			menuVar = 0;

			do {
				manageEvents();
				getMouseData(mouseUpdateStatus, &mouseButton,
				    &dummyU16, &dummyU16);

				if (mouseButton == 0) {
					if (processKeyboard(menuVar)) {
						var_6 = 1;
					}
				} else {
					var_6 = 1;
				}

				mainLoopSub6();
			} while (!var_6);

			menuVar = 0;

			do {
				manageEvents();
				getMouseData(mouseUpdateStatus, &mouseButton,
				    &dummyU16, &dummyU16);
			} while (mouseButton != 0);

			waitForPlayerClick = 0;
		}

		if (checkForPendingDataLoadSwitch) {
			checkForPendingDataLoad();

			checkForPendingDataLoadSwitch = 0;
		}

		if (di) {
			if (!strcmp(di, "quit")) {
				var_2 = 1;
			}
		}

		manageEvents();

	} while (!exitEngine && !var_2 && var21 != 7);

	hideMouse();
	stopSample();
	closeEngine3();
	unloadAllMasks();
	freePrcLinkedList();
	releaseObjectScripts();
	closeEngine7();
	closePart();
}
