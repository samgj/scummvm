/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/staticres.h"
#include "tsage/ringworld2/ringworld2_scenes3.h"

namespace TsAGE {

namespace Ringworld2 {

/*--------------------------------------------------------------------------
 * Scene 3100 - ARM Base Hanager
 *
 *--------------------------------------------------------------------------*/

Scene3100::Scene3100() {
	_field412 = 0;
}

void Scene3100::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field412);
}

bool Scene3100::Guard::startAction(CursorType action, Event &event) {
	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	Scene3100 *scene = (Scene3100 *)R2_GLOBALS._sceneManager._scene;

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 10;
	R2_GLOBALS._events.setCursor(CURSOR_CROSSHAIRS);
	scene->_stripManager.start(606, scene);
	return true;
}

void Scene3100::postInit(SceneObjectList *OwnerList) {
	if (R2_GLOBALS._sceneManager._previousScene == 1000) {
		if (R2_GLOBALS._player._oldCharacterScene[R2_QUINN] == 3100) {
			loadScene(3101);
			R2_GLOBALS._uiElements._active = false;
		} else {
			loadScene(3100);
			_sceneBounds = Rect(160, 0, 480, SCREEN_HEIGHT);
		}
	} else {
		loadScene(3100);
	}
	// Original was doing it twice in a row. Skipped.

	if (R2_GLOBALS._sceneManager._previousScene == 3255)
		R2_GLOBALS._uiElements._active = false;

	SceneExt::postInit();
	_stripManager.addSpeaker(&_guardSpeaker);

	if (R2_GLOBALS._sceneManager._previousScene == -1)
		R2_GLOBALS._sceneManager._previousScene = 1000;

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player._characterIndex = R2_QUINN;
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_item2.setDetails(Rect(212, 97, 320, 114), 3100, 3, -1, -1, 1, NULL);
	_item1.setDetails(Rect(0, 0, 480, 200), 3100, 0, -1, -1, 1, NULL);
	_field412 = 0;

	if (R2_GLOBALS._sceneManager._previousScene == 1000) {
		if (R2_GLOBALS._player._oldCharacterScene[R2_QUINN] == 3100) {
			_sceneMode = 3102;
			_actor3.postInit();
			_actor4.postInit();
			_actor5.postInit();
			R2_GLOBALS._sound1.play(274);
			_sound1.fadeSound(130);
			setAction(&_sequenceManager, this, 3102, &_actor1, &R2_GLOBALS._player, &_actor3, &_actor4, &_actor5, NULL);
		} else {
			_guard.postInit();
			_guard.setup(3110, 5, 1);
			_guard.changeZoom(50);
			_guard.setPosition(Common::Point(10, 149));
			_guard.setDetails(3100, 6, -1, -1, 2, (SceneItem *)NULL);

			_actor4.postInit();
			_actor4.setup(3103, 1, 1);
			_actor4.setPosition(Common::Point(278, 113));
			_actor4.setDetails(3100, 9, -1, -1, 2, (SceneItem *)NULL);
			_actor4.animate(ANIM_MODE_2, NULL);

			_field412 = 1;
			_actor1.setDetails(3100, 3, -1, -1, 2, (SceneItem *)NULL);
			R2_GLOBALS._sound1.play(243);
			R2_GLOBALS._sound2.play(130);
			_sceneMode = 3100;

			setAction(&_sequenceManager, this, 3100, &R2_GLOBALS._player, &_actor1, NULL);
		}
	} else if (R2_GLOBALS._sceneManager._previousScene == 3255) {
		_sceneMode = 3101;
		_actor2.postInit();
		_actor3.postInit();
		_field412 = 1;

		setAction(&_sequenceManager, this, 3101, &R2_GLOBALS._player, &_actor1, &_actor2, &_actor3, NULL);
	} else {
		_guard.postInit();
		_guard.setup(3110, 5, 1);
		_guard.changeZoom(50);
		_guard.setPosition(Common::Point(10, 149));
		_guard.setDetails(3100, 6, -1, -1, 2, (SceneItem *)NULL);

		_actor4.postInit();
		_actor4.setup(3103, 1, 1);
		_actor4.setPosition(Common::Point(278, 113));
		_actor4.setDetails(3100, 9, -1, -1, 2, (SceneItem *)NULL);
		_actor4.animate(ANIM_MODE_2, NULL);

		_actor1.postInit();
		_actor1.setup(3104, 4, 1);
		_actor1.setPosition(Common::Point(143, 104));
		_actor1.setDetails(3100, 3, -1, -1, 2, (SceneItem *)NULL);

		R2_GLOBALS._player.setup(3110, 3, 1);
		R2_GLOBALS._player.changeZoom(50);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(160, 150));
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		R2_GLOBALS._player.enableControl(CURSOR_WALK);

		R2_GLOBALS._sound1.play(243);
	}

	R2_GLOBALS._player._oldCharacterScene[R2_QUINN] = 3100;
}

void Scene3100::remove() {
	R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
	R2_GLOBALS._sound1.fadeOut2(NULL);
	R2_GLOBALS._sound2.fadeOut2(NULL);
	_sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3100::signal() {
	switch (_sceneMode) {
	case 10:
		R2_GLOBALS._player.enableControl(CURSOR_TALK);
		break;
	case 3100:
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	case 3101:
		R2_GLOBALS._sceneManager.changeScene(1000);
		break;
	case 3102:
		R2_GLOBALS._player._oldCharacterScene[R2_QUINN] = 1000;
		R2_GLOBALS._sceneManager.changeScene(1000);
		break;
	default:
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	}
}

void Scene3100::dispatch() {
	if ((_sceneMode == 3100) && (_field412 != 0) && (R2_GLOBALS._player._position.y == 104)) {
		_field412 = 0;
		R2_GLOBALS._sound2.fadeOut2(NULL);
	}

	if ((_sceneMode == 3101) && (_field412 != 0) && (R2_GLOBALS._player._position.y < 104)) {
		_field412 = 0;
		_sound1.fadeSound(130);
	}

	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3125 - Ghouls dormitory
 *
 *--------------------------------------------------------------------------*/

Scene3125::Scene3125() {
	_field412 = 0;
}

void Scene3125::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field412);
}

bool Scene3125::Background::startAction(CursorType action, Event &event) {
	Scene3125 *scene = (Scene3125 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (_useLineNum != -1)
			SceneItem::display(_resNum, _useLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	case CURSOR_LOOK:
		if (_lookLineNum != -1)
			SceneItem::display(_resNum, _lookLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	case CURSOR_TALK:
		if (_talkLineNum != -1)
			SceneItem::display(_resNum, _talkLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	default:
		return scene->display(action, event);
		break;
	}

	return true;
}

bool Scene3125::Table::startAction(CursorType action, Event &event) {
	Scene3125 *scene = (Scene3125 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3125;
		scene->setAction(&scene->_sequenceManager1, scene, 3125, &R2_GLOBALS._player, NULL);
		break;
	case CURSOR_LOOK:
		SceneItem::display(3125, 15, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	case CURSOR_TALK:
		SceneItem::display(3125, 13, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	default:
		return SceneHotspot::startAction(action, event);
		break;
	}

	return true;
}

bool Scene3125::Computer::startAction(CursorType action, Event &event) {
	Scene3125 *scene = (Scene3125 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		R2_GLOBALS._player.disableControl();
		scene->_ghoul4.postInit();
		scene->_sceneMode = 3126;
		scene->setAction(&scene->_sequenceManager1, scene, 3126, &R2_GLOBALS._player, 
			&scene->_ghoul1, &scene->_ghoul2, &scene->_ghoul3, &scene->_door, 
			&scene->_ghoul4, NULL);
		break;
	case CURSOR_LOOK:
		SceneItem::display(3125, 9, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	case CURSOR_TALK:
		SceneItem::display(3125, 13, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		break;
	default:
		return SceneHotspot::startAction(action, event);
		break;
	}

	return true;
}

bool Scene3125::Door::startAction(CursorType action, Event &event) {
	Scene3125 *scene = (Scene3125 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3176;
	scene->setAction(&scene->_sequenceManager1, scene, 3176, &R2_GLOBALS._player, &scene->_door, NULL);
	return true;
}

void Scene3125::postInit(SceneObjectList *OwnerList) {
	loadScene(3125);
	SceneExt::postInit();
	_field412 = 0;

	_door.postInit();
	_door.setup(3175, 1, 1);
	_door.setPosition(Common::Point(35, 72));
	_door.setDetails(3125, 12, 13, -1, 1, (SceneItem *)NULL);

	_ghoul1.postInit();
	_ghoul1.setup(3126, 4, 1);
	_ghoul1.setPosition(Common::Point(71, 110));
	_ghoul1._numFrames = 20;

	_ghoul2.postInit();
	_ghoul2.setup(3126, 1, 1);
	_ghoul2.setPosition(Common::Point(215, 62));
	_ghoul2.fixPriority(71);

	_ghoul3.postInit();
	_ghoul3.setup(3126, 1, 1);
	_ghoul3.setPosition(Common::Point(171, 160));
	_ghoul3.fixPriority(201);

	_computer.setDetails(12, 3125, 9, 13, -1);
	_table.setDetails(11, 3125, 15, 13, -1);
	_background.setDetails(Rect(0, 0, 320, 200), 3125, 0, 1, 2, 1, NULL);

	R2_GLOBALS._sound1.play(262);
	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] == 3250) {
		_sceneMode = 3175;
		setAction(&_sequenceManager1, this, 3175, &R2_GLOBALS._player, &_door, NULL);
	} else {
		R2_GLOBALS._player.setup(30, 5, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(89, 76));
		R2_GLOBALS._player.enableControl();
	}
	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3125;
}

void Scene3125::signal() {
	switch (_sceneMode) {
	case 3125:
		SceneItem::display(3125, 3, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		_sceneMode = 3127;
		setAction(&_sequenceManager1, this, 3127, &R2_GLOBALS._player, NULL);
		break;
	case 3126:
		R2_GLOBALS.setFlag(79);
	// No break on purpose
	case 3176:
		R2_GLOBALS._sceneManager.changeScene(3250);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

void Scene3125::dispatch() {
	if ((_sceneMode == 3126) && (_ghoul1._frame == 2) && (_field412 == 0)) {
		_field412 = 1;
		R2_GLOBALS._sound1.play(265);
	}
	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3150 - Jail
 *
 *--------------------------------------------------------------------------*/

bool Scene3150::LightFixture::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (R2_INVENTORY.getObjectScene(R2_LIGHT_BULB) != 3150)
			return SceneHotspot::startAction(action, event);

		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3154;
		scene->setAction(&scene->_sequenceManager, scene, 3154, &R2_GLOBALS._player, &scene->_bulbOrWire, NULL);
		return true;
	case R2_SUPERCONDUCTOR_WIRE:
		if ((R2_INVENTORY.getObjectScene(R2_LIGHT_BULB) != 3150) && (R2_GLOBALS.getFlag(75))) {
			R2_GLOBALS._player.disableControl();
			scene->_bulbOrWire.postInit();
			scene->_bulbOrWire._effect = 3;
			scene->_bulbOrWire._shade = 5;
			scene->_sceneMode = 3155;
			scene->setAction(&scene->_sequenceManager, scene, 3155, &R2_GLOBALS._player, &scene->_bulbOrWire, NULL);
		} else {
			SceneItem::display(3150, 42, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
		}
		return true;
	default:
		return SceneHotspot::startAction(action, event);
		break;
	}
}

bool Scene3150::Toilet::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case R2_PILLOW:
		R2_GLOBALS._player.disableControl();
		scene->_water.postInit();
		scene->_water._effect = 6;
		scene->_water._shade = 3;
		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3158;
		scene->setAction(&scene->_sequenceManager, scene, 3158, &R2_GLOBALS._player, &scene->_water, NULL);
		return true;
	case R2_FOOD_TRAY:
		if ((R2_INVENTORY.getObjectScene(R2_LIGHT_BULB) != 3150) && 
				(R2_INVENTORY.getObjectScene(R2_SUPERCONDUCTOR_WIRE) == 3150) 
				&& (R2_GLOBALS.getFlag(75))) {
			scene->_foodTray.postInit();
			scene->_foodTray._effect = 6;
			scene->_foodTray._shade = 3;
			scene->_foodTray.setDetails(3150, 30, -1, -1, 2, (SceneItem *)NULL);

			R2_GLOBALS._player.disableControl();
			scene->_sceneMode = 3159;
			scene->setAction(&scene->_sequenceManager, scene, 3159, &R2_GLOBALS._player, &scene->_foodTray, NULL);
		} else {
			SceneItem::display(3150, 42, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
		}
		return true;
	default:
		return SceneHotspot::startAction(action, event);
		break;
	}
}

bool Scene3150::Water::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (R2_GLOBALS.getFlag(75))
			return SceneActor::startAction(action, event);

		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3151;
		scene->setAction(&scene->_sequenceManager, scene, 3151, &R2_GLOBALS._player, &scene->_water, NULL);
		return true;
	case R2_FOOD_TRAY:
		return false;
	default:
		return SceneActor::startAction(action, event);
		break;
	}
}

bool Scene3150::FoodTray::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	if ((action != CURSOR_USE) || (R2_GLOBALS.getFlag(77)))
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3157;
	scene->setAction(&scene->_sequenceManager, scene, 3157, &R2_GLOBALS._player, &scene->_foodTray, NULL);
	return true;
}

bool Scene3150::Actor6::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	if (action == CURSOR_USE) {
		if (R2_GLOBALS.getFlag(75)) {
			if (R2_GLOBALS.getFlag(77)) {
				R2_GLOBALS._player.disableControl();
				if (R2_GLOBALS.getFlag(76)) {
					scene->_sceneMode = 3152;
					scene->setAction(&scene->_sequenceManager, scene, 3152, &R2_GLOBALS._player, NULL);
				} else {
					scene->_sceneMode = 3153;
					scene->setAction(&scene->_sequenceManager, scene, 3152, &R2_GLOBALS._player, &scene->_water, NULL);
				}
			} else {
				SceneItem::display(3150, 42, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
			}
		} else {
			R2_GLOBALS._player.disableControl();
			scene->_sceneMode = 3152;
			scene->setAction(&scene->_sequenceManager, scene, 3152, &R2_GLOBALS._player, NULL);
		}
		return true;
	} else {
		return SceneActor::startAction(action, event);
	}
}

bool Scene3150::AirVent::startAction(CursorType action, Event &event) {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	if ((action == R2_LASER_HACKSAW) && (!R2_GLOBALS.getFlag(80))) {
		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3160;
		scene->setAction(&scene->_sequenceManager, scene, 3160, &R2_GLOBALS._player, &scene->_airVent, NULL);
		return true;
	}

	return SceneActor::startAction(action, event);
}

void Scene3150::DoorExit::changeScene() {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	g_globals->_events.setCursor(CURSOR_ARROW);
	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 11;

	Common::Point pt(-20, 180);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3150::VentExit::changeScene() {
	Scene3150 *scene = (Scene3150 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	g_globals->_events.setCursor(CURSOR_ARROW);
	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 12;

	scene->setAction(&scene->_sequenceManager, scene, 3163, &R2_GLOBALS._player, NULL);
}

void Scene3150::postInit(SceneObjectList *OwnerList) {
	loadScene(3150);
	if (R2_GLOBALS._sceneManager._previousScene == -1) {
		R2_INVENTORY.setObjectScene(R2_ANCIENT_SCROLLS, 2000);
		R2_GLOBALS._player._oldCharacterScene[R2_QUINN] = 3100;
		R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 0;
		R2_GLOBALS._player._characterIndex = R2_MIRANDA;
	}
	SceneExt::postInit();

	if (R2_GLOBALS.getFlag(78)) {
		_doorExit.setDetails(Rect(0, 135, 60, 168), EXITCURSOR_SW, 3275);
		_doorExit.setDest(Common::Point(70, 125));
	}

	if (R2_GLOBALS.getFlag(80)) {
		_ventExit.setDetails(Rect(249, 36, 279, 60), EXITCURSOR_NE, 3150);
		_ventExit.setDest(Common::Point(241, 106));
	}

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.disableControl();

	_doorBars.postInit();
	_doorBars.setPosition(Common::Point(64, 139));
	if (R2_GLOBALS.getFlag(78)) {
		_doorBars.setup(3151, 1, 5);
		_doorBars.fixPriority(125);
	} else {
		_doorBars.setup(3151, 1, 1);
		_doorBars.setDetails(3150, 8, -1, 9, 1, (SceneItem *)NULL);
	}

	if (R2_GLOBALS.getFlag(78)) {
		_guard.postInit();
		_guard.setup(3154, 1, 16);
		_guard.setPosition(Common::Point(104, 129));
		_guard._effect = 6;
		_guard._shade = 3;
		_guard.setDetails(3150, 24, -1, -1, -1, (SceneItem *)NULL);
	}

	_airVent.postInit();
	_airVent.setup(3154, 5, 1);
	if (R2_GLOBALS.getFlag(80))
		_airVent.setPosition(Common::Point(264, 108));
	else
		_airVent.setPosition(Common::Point(264, 58));
	_airVent.fixPriority(50);
	_airVent.setDetails(3150, 17, -1, 19, 1, (SceneItem *)NULL);

	if (R2_INVENTORY.getObjectScene(R2_PILLOW) == 3150) {
		_water.postInit();
		if (R2_GLOBALS.getFlag(75)) {
			if (R2_GLOBALS.getFlag(76)) {
				R2_GLOBALS._walkRegions.disableRegion(1);
				R2_GLOBALS._walkRegions.disableRegion(4);
				R2_GLOBALS._walkRegions.disableRegion(5);
				R2_GLOBALS._walkRegions.disableRegion(6);
				_water.setup(3152, 4, 10);
				_water.setDetails(3150, 14, -1, -1, 1, (SceneItem *)NULL);
			} else {
				_water.setup(3152, 7, 4);
				_water.setDetails(3150, 13, -1, -1, 1, (SceneItem *)NULL);
			}
			_water.fixPriority(110);
			_water.setPosition(Common::Point(83, 88));
			_water._effect = 6;
			_water._shade = 3;
		} else {
			_water.setup(3152, 7, 3);
			_water.setPosition(Common::Point(143, 70));
			_water.setDetails(3150, 15, -1, -1, 1, (SceneItem *)NULL);
		}
	}

	if (R2_INVENTORY.getObjectScene(R2_LIGHT_BULB) == 3150) {
		_bulbOrWire.postInit();
		_bulbOrWire.setup(3152, 7, 1);
		_bulbOrWire.setPosition(Common::Point(73, 83));
	}

	if (R2_INVENTORY.getObjectScene(R2_SUPERCONDUCTOR_WIRE) == 3150) {
		_bulbOrWire.postInit();
		_bulbOrWire.setup(3152, 7, 3);
		_bulbOrWire.setPosition(Common::Point(70, 55));
		_bulbOrWire.fixPriority(111);
		_bulbOrWire._effect = 6;
		_bulbOrWire._shade = 5;
	}

	if (R2_INVENTORY.getObjectScene(R2_FOOD_TRAY) == 3150) {
		_foodTray.postInit();
		if (R2_GLOBALS.getFlag(77)) {
			_foodTray.setup(3152, 7, 8);
			_foodTray.setPosition(Common::Point(82, 92));
			_foodTray.fixPriority(111);
			_foodTray._effect = 6;
			_foodTray._shade = 3;
		} else {
			_foodTray.setup(3152, 7, 7);
			_foodTray.setPosition(Common::Point(155, 79));
		}
		_foodTray.setDetails(3150, 30, -1, -1, 2, (SceneItem *)NULL);
	}

	_toiletFlush.postInit();
	_toiletFlush.setup(3152, 7, 6);
	_toiletFlush.setPosition(Common::Point(98, 73));
	_toiletFlush.setDetails(3150, 43, -1, -1, 1, (SceneItem *)NULL);

	_bed.setDetails(12, 3150, 10, -1, 12);
	_lightFixture2.setDetails(Rect(186, 17, 210, 36), 3150, 6, -1, -1, 1, NULL);
	_bars.setDetails(Rect(61, 21, 92, 41), 3150, 7, -1, -1, 1, NULL);
	_lightFixture.setDetails(Rect(63, 48, 78, 58), 3150, 6, -1, -1, 1, NULL);
	_toilet.setDetails(Rect(63, 81, 100, 95), 3150, 3, 4, -1, 1, NULL);
	_background.setDetails(Rect(0, 0, 200, 320), 3150, 0, 1, 2, 1, NULL);

	switch (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA]) {
	case 0:
		_sceneMode = 3150;
		_guard.postInit();
		_guard._effect = 6;
		_guard._shade = 5;
		setAction(&_sequenceManager, this, 3150, &R2_GLOBALS._player, &_guard, &_doorBars, NULL);
		break;
	case 1200:
		_sceneMode = 3162;
		setAction(&_sequenceManager, this, 3162, &R2_GLOBALS._player, NULL);
		break;
	case 3275: {
		_sceneMode = 10;
		R2_GLOBALS._player.setup(30, 3, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(-20, 180));
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);

		Common::Point pt(80, 125);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
		break;
		}
	default:
		if ((R2_GLOBALS._v56AA0 == 1) && (R2_INVENTORY.getObjectScene(R2_ANCIENT_SCROLLS) == 2000) && (R2_GLOBALS._player._oldCharacterScene[R2_QUINN] == 3100)) {
			++R2_GLOBALS._v56AA0;
			_sceneMode = 3156;
			_guard.postInit();
			_guard._effect = 6;
			_guard._shade = 3;

			_doorBars.postInit();
			_foodTray.postInit();
			_foodTray._effect = 6;
			_foodTray._shade = 3;

			setAction(&_sequenceManager, this, 3156, &R2_GLOBALS._player, &_guard, &_doorBars, &_foodTray, NULL);
		} else {
			if ((R2_GLOBALS._v56AA0 != 1) && (R2_GLOBALS._v56AA0 != 2))
				++R2_GLOBALS._v56AA0;

			R2_GLOBALS._player.setup(30, 3, 1);
			R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
			R2_GLOBALS._player.setPosition(Common::Point(155, 120));
			R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
			R2_GLOBALS._player.enableControl();
		}
	}

	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3150;
}

void Scene3150::signal() {
	switch (_sceneMode) {
	case 11:
		R2_GLOBALS._sceneManager.changeScene(3275);
		break;
	case 12:
		R2_GLOBALS._sceneManager.changeScene(1200);
		break;
	case 3151:
		_guard.remove();
		R2_INVENTORY.setObjectScene(R2_PILLOW, 3);
		R2_GLOBALS._player.enableControl();
		break;
	case 3153:
		R2_GLOBALS.setFlag(76);
		_water.setDetails(3150, 14, -1, -1, 3, (SceneItem *)NULL);
		_guard.postInit();
		_guard.setDetails(3150, 24, -1, -1, 2, (SceneItem *)NULL);
		_sceneMode = 3161;
		setAction(&_sequenceManager, this, 3161, &_guard, &_doorBars, NULL);
		break;
	case 3154:
		_bulbOrWire.remove();
		R2_INVENTORY.setObjectScene(R2_LIGHT_BULB, 3);
		R2_GLOBALS._player.enableControl();
		break;
	case 3155:
		R2_INVENTORY.setObjectScene(R2_SUPERCONDUCTOR_WIRE, 3150);
		R2_GLOBALS._player.enableControl();
		break;
	case 3156:
		_foodTray.setDetails(3150, 30, -1, -1, 2, (SceneItem *)NULL);
		R2_INVENTORY.setObjectScene(R2_FOOD_TRAY, 3150);
		R2_GLOBALS._player.enableControl();
		break;
	case 3157:
		_foodTray.remove();
		R2_INVENTORY.setObjectScene(R2_FOOD_TRAY, 3);
		R2_GLOBALS._player.enableControl();
		break;
	case 3158:
		R2_GLOBALS.setFlag(75);
		R2_INVENTORY.setObjectScene(R2_PILLOW, 3150);
		_water.fixPriority(110);
		_water.setDetails(3150, 13, -1, -1, 2, (SceneItem *)NULL);
		R2_GLOBALS._player.enableControl();
		break;
	case 3159:
		R2_GLOBALS.setFlag(77);
		R2_INVENTORY.setObjectScene(R2_FOOD_TRAY, 3150);
		R2_GLOBALS._player.enableControl();
		break;
	case 3160:
		R2_INVENTORY.setObjectScene(R2_TOOLBOX, 3150);
		R2_GLOBALS.setFlag(80);
		R2_GLOBALS._sceneManager.changeScene(1200);
		break;
	case 3161:
		R2_GLOBALS._sceneItems.remove(&_doorBars);
		_doorExit.setDetails(Rect(0, 135, 60, 168), EXITCURSOR_SW, 3275);
		_doorExit.setDest(Common::Point(70, 125));
		R2_GLOBALS._walkRegions.disableRegion(1);
		R2_GLOBALS._walkRegions.disableRegion(4);
		R2_GLOBALS._walkRegions.disableRegion(5);
		R2_GLOBALS._walkRegions.disableRegion(6);
		R2_GLOBALS.setFlag(78);
		R2_GLOBALS._player.enableControl();
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

void Scene3150::dispatch() {
	if (_foodTray._position.x == 155) {
		_foodTray._effect = 0;
		_foodTray._shade = 0;
	}

	if (_guard._visage == 3154) {
		_guard._effect = 0;
		_guard._shade = 0;
	}

	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3175 - Autopsy room
 *
 *--------------------------------------------------------------------------*/

bool Scene3175::Item1::startAction(CursorType action, Event &event) {
	Scene3175 *scene = (Scene3175 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (_useLineNum != -1) {
			SceneItem::display(_resNum, _useLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_LOOK:
		if (_lookLineNum != -1) {
			SceneItem::display(_resNum, _lookLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_TALK:
		if (_talkLineNum != -1) {
			SceneItem::display(_resNum, _talkLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	default:
		break;
	}

	return scene->display(action, event);
}

bool Scene3175::Corpse::startAction(CursorType action, Event &event) {
	Scene3175 *scene = (Scene3175 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (_useLineNum != -1) {
			SceneItem::display(_resNum, _useLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_LOOK:
		if (_lookLineNum != -1) {
			SceneItem::display(_resNum, _lookLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_TALK:
		if (_talkLineNum != -1) {
			SceneItem::display(_resNum, _talkLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	default:
		break;
	}

	return scene->display(action, event);
}

bool Scene3175::Door::startAction(CursorType action, Event &event) {
	Scene3175 *scene = (Scene3175 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		R2_GLOBALS._player.disableControl();
		scene->_sceneMode = 3176;
		scene->setAction(&scene->_sequenceManager, scene, 3176, &R2_GLOBALS._player, &scene->_door, NULL);
		return true;
		break;
	case CURSOR_LOOK:
		SceneItem::display(3175, 9, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		return true;
		break;
	case CURSOR_TALK:
		SceneItem::display(3175, 10, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		return true;
		break;
	default:
		return SceneActor::startAction(action, event);
		break;
	}
}

void Scene3175::postInit(SceneObjectList *OwnerList) {
	loadScene(3175);
	SceneExt::postInit();

	_door.postInit();
	_door.setup(3175, 1, 1);
	_door.setPosition(Common::Point(35, 72));
	_door.setDetails(3175, 9, 10, -1, 1, (SceneItem *)NULL);

	_actor2.postInit();
	_actor2.setup(3175, 2, 1);
	_actor2.setPosition(Common::Point(87, 148));

	_corpse.postInit();
	_corpse.setup(3175, 3, 1);
	_corpse.setPosition(Common::Point(199, 117));
	_corpse.setDetails(3175, 15, 16, 17, 1, (SceneItem *)NULL);

	_item2.setDetails(12, 3175, 3, 1, 5);
	_item3.setDetails(11, 3175, 6, 7, 8);
	_background.setDetails(Rect(0, 0, 320, 200), 3175, 0, 1, 2, 1, NULL);

	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] == 3250) {
		R2_GLOBALS._player.setup(30, 5, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(126, 77));
		R2_GLOBALS._player.enableControl();
	} else {
		_sceneMode = 3175;
		setAction(&_sequenceManager, this, 3175, &R2_GLOBALS._player, &_door, NULL);
	}

	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3175;
}

void Scene3175::signal() {
	if (_sceneMode == 3176)
		R2_GLOBALS._sceneManager.changeScene(3250);
	else
		R2_GLOBALS._player.enableControl();
}

/*--------------------------------------------------------------------------
 * Scene 3200 - Cutscene : Guards - Discussion
 *
 *--------------------------------------------------------------------------*/

void Scene3200::postInit(SceneObjectList *OwnerList) {
	loadScene(3200);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_rockoSpeaker);
	_stripManager.addSpeaker(&_jockoSpeaker);
	_stripManager.addSpeaker(&_sockoSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor3.postInit();
	_actor2.postInit();

	setAction(&_sequenceManager, this, 3200 + R2_GLOBALS._randomSource.getRandomNumber(1), &_actor1, &_actor2, &_actor3, NULL);
}

void Scene3200::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3210 - Cutscene : Captain and Private - Discussion
 *
 *--------------------------------------------------------------------------*/

void Scene3210::postInit(SceneObjectList *OwnerList) {
	loadScene(3210);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_privateSpeaker);
	_stripManager.addSpeaker(&_captainSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor2.postInit();

	setAction(&_sequenceManager, this, 3210 + R2_GLOBALS._randomSource.getRandomNumber(1), &_actor1, &_actor2, NULL);
}

void Scene3210::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3220 - Cutscene : Guards in cargo zone
 *
 *--------------------------------------------------------------------------*/

void Scene3220::postInit(SceneObjectList *OwnerList) {
	loadScene(3220);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_rockoSpeaker);
	_stripManager.addSpeaker(&_jockoSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor2.postInit();

	setAction(&_sequenceManager, this, 3220 + R2_GLOBALS._randomSource.getRandomNumber(1), &_actor1, &_actor2, NULL);
}

void Scene3220::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3230 - Cutscene : Guards on duty
 *
 *--------------------------------------------------------------------------*/

void Scene3230::postInit(SceneObjectList *OwnerList) {
	loadScene(3230);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_rockoSpeaker);
	_stripManager.addSpeaker(&_jockoSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor2.postInit();
	_actor3.postInit();

	setAction(&_sequenceManager, this, 3230 + R2_GLOBALS._randomSource.getRandomNumber(1), &_actor1, &_actor2, &_actor3, NULL);
}

void Scene3230::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3240 - Cutscene : Teal monolog
 *
 *--------------------------------------------------------------------------*/

void Scene3240::postInit(SceneObjectList *OwnerList) {
	loadScene(3240);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_tealSpeaker);
	_stripManager.addSpeaker(&_webbsterSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor2.postInit();

	setAction(&_sequenceManager, this, 3240 + R2_GLOBALS._randomSource.getRandomNumber(1), &_actor1, &_actor2, NULL);
}

void Scene3240::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3245 - Cutscene : Discussions with Dr. Tomko
 *
 *--------------------------------------------------------------------------*/

void Scene3245::postInit(SceneObjectList *OwnerList) {
	loadScene(3245);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();

	_stripManager.addSpeaker(&_ralfSpeaker);
	_stripManager.addSpeaker(&_tomkoSpeaker);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor2.postInit();

	if (R2_GLOBALS._scientistConvIndex < 4)
		++R2_GLOBALS._scientistConvIndex;

	if (R2_GLOBALS._scientistConvIndex >= 4) {
		SceneItem::display(1200, 7, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
		signal();
	} else {
		setAction(&_sequenceManager, this, 3244 + R2_GLOBALS._scientistConvIndex, &_actor1, &_actor2, NULL);
	}
}

void Scene3245::signal() {
	R2_GLOBALS._sceneManager.changeScene(1200);
}

/*--------------------------------------------------------------------------
 * Scene 3250 - Room with large stasis field negator
 *
 *--------------------------------------------------------------------------*/

bool Scene3250::Item::startAction(CursorType action, Event &event) {
	Scene3250 *scene = (Scene3250 *)R2_GLOBALS._sceneManager._scene;

	switch (action) {
	case CURSOR_USE:
		if (_useLineNum != -1) {
			SceneItem::display(_resNum, _useLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_LOOK:
		if (_lookLineNum != -1) {
			SceneItem::display(_resNum, _lookLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	case CURSOR_TALK:
		if (_talkLineNum != -1) {
			SceneItem::display(_resNum, _talkLineNum, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
			return true;
		}
		break;
	default:
		break;
	}

	return scene->display(action, event);
}

bool Scene3250::Door::startAction(CursorType action, Event &event) {
	Scene3250 *scene = (Scene3250 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();

	switch(_position.x) {
	case 25:
		scene->_sceneMode = 3262;
		scene->setAction(&scene->_sequenceManager, scene, 3262, &R2_GLOBALS._player, &scene->_leftDoor, NULL);
		break;
	case 259:
		scene->_sceneMode = 3260;
		scene->setAction(&scene->_sequenceManager, scene, 3260, &R2_GLOBALS._player, &scene->_topDoor, NULL);
		break;
	case 302:
		scene->_sceneMode = 3261;
		scene->setAction(&scene->_sequenceManager, scene, 3261, &R2_GLOBALS._player, &scene->_rightDoor, NULL);
		break;
	default:
		break;
	}
	return true;
}

void Scene3250::postInit(SceneObjectList *OwnerList) {
	loadScene(3250);

	if (R2_GLOBALS._sceneManager._previousScene == -1) {
		R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 1200;
		R2_GLOBALS._player._characterIndex = R2_MIRANDA;
	}

	SceneExt::postInit();
	_leftDoor.postInit();
	_leftDoor.setup(3250, 6, 1);
	_leftDoor.setPosition(Common::Point(25, 148));
	_leftDoor.fixPriority(10);
	_leftDoor.setDetails(3250, 9, 10, -1, 1, (SceneItem *)NULL);

	_topDoor.postInit();
	_topDoor.setup(3250, 4, 1);
	_topDoor.setPosition(Common::Point(259, 126));
	_topDoor.fixPriority(10);
	_topDoor.setDetails(3250, 9, 10, -1, 1, (SceneItem *)NULL);

	_rightDoor.postInit();
	_rightDoor.setup(3250, 5, 1);
	_rightDoor.setPosition(Common::Point(302, 138));
	_rightDoor.fixPriority(10);
	_rightDoor.setDetails(3250, 9, 10, -1, 1, (SceneItem *)NULL);

	_floodLights.setDetails(Rect(119, 111, 149, 168), 3250, 6, 7, 2, 1, NULL);
	_tnuctipunShip.setDetails(Rect(58, 85, 231, 138), 3250, 12, 7, 2, 1, NULL);
	_negator.setDetails(12, 3250, 3, 1, 2);
	_background.setDetails(Rect(0, 0, 320, 200), 3250, 0, 1, 2, 1, NULL);

	R2_GLOBALS._player.postInit();

	switch (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA]) {
	case 1200:
		_sceneMode = 3250;
		_actor4.postInit();
		R2_GLOBALS._player._effect = 0;
		setAction(&_sequenceManager, this, 3250, &R2_GLOBALS._player, &_actor4, NULL);
		break;
	case 3125:
		if (R2_GLOBALS.getFlag(79)) {
			_sceneMode = 3254;
			_ghoul1.postInit();
			_ghoul1._effect = 1;
			_ghoul2.postInit();
			_ghoul2._effect = 1;
			_ghoul3.postInit();
			_ghoul3._effect = 1;
			setAction(&_sequenceManager, this, 3254, &R2_GLOBALS._player, &_rightDoor, 
				&_ghoul1, &_ghoul2, &_ghoul3, &_leftDoor, NULL);
		} else {
			_sceneMode = 3252;
			setAction(&_sequenceManager, this, 3252, &R2_GLOBALS._player, &_rightDoor, NULL);
		}
		break;
	case 3175:
		_sceneMode = 3251;
		setAction(&_sequenceManager, this, 3251, &R2_GLOBALS._player, &_topDoor, NULL);
		break;
	case 3255:
		_sceneMode = 3253;
		setAction(&_sequenceManager, this, 3253, &R2_GLOBALS._player, &_leftDoor, NULL);
		break;
	default:
		R2_GLOBALS._player.setup(31, 3, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(185, 150));
		R2_GLOBALS._player.enableControl();
		break;
	}

	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3250;
}

void Scene3250::signal() {
	switch(_sceneMode) {
	case 3250:
		R2_GLOBALS._player._effect = 1;
		R2_GLOBALS._player.enableControl();
		break;
	case 3254:
	//No break on purpose
	case 3262:
		R2_GLOBALS._sceneManager.changeScene(3255);
		break;
	case 3260:
		R2_GLOBALS._sceneManager.changeScene(3175);
		break;
	case 3261:
		R2_GLOBALS._sceneManager.changeScene(3125);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

void Scene3250::dispatch() {
	if ((R2_GLOBALS._player._visage == 3250) && (R2_GLOBALS._player._strip == 3) && (R2_GLOBALS._player._effect == 0)) {
		R2_GLOBALS._player._effect = 6;
		R2_GLOBALS._player._shade = 6;
	}

	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3255 - Guard Post
 *
 *--------------------------------------------------------------------------*/

void Scene3255::postInit(SceneObjectList *OwnerList) {
	loadScene(3255);
	SceneExt::postInit();

	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);

	if (R2_GLOBALS._sceneManager._previousScene == -1)
		R2_GLOBALS.setFlag(79);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.disableControl();

	if (R2_GLOBALS.getFlag(79)) {
		R2_GLOBALS._sound1.play(267);
		R2_GLOBALS._sound2.play(268);
		_sceneMode = 3257;
		_actor3.postInit();
		_quinn.postInit();
		_quinn._effect = 1;
		setAction(&_sequenceManager, this, 3257, &R2_GLOBALS._player, &_quinn, &_actor3, NULL);
	} else {
		_teal.postInit();
		_teal.setup(303, 1, 1);
		_teal.setPosition(Common::Point(208, 128));
		_guard.postInit();
		_guard.setup(3107, 3, 1);
		_guard.setPosition(Common::Point(230, 127));
		_sceneMode = 3255;
		setAction(&_sequenceManager, this, 3255, &R2_GLOBALS._player, NULL);
	}
	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3255;
}

void Scene3255::signal() {
	switch (_sceneMode) {
	case 10:
		_sceneMode = 3258;
		_ghoul1.postInit();
		_ghoul2.postInit();
		_ghoul3.postInit();
		setAction(&_sequenceManager, this, 3258, &R2_GLOBALS._player, &_quinn, 
			&_actor3, &_ghoul1, &_ghoul2, &_ghoul3, NULL);
		break;
	case 3256:
		R2_GLOBALS._sceneManager.changeScene(3250);
		break;
	case 3257:
		_sceneMode = 10;
		R2_GLOBALS._events.setCursor(CURSOR_CROSSHAIRS);
		_stripManager.start(607, this);
		break;
	case 3258:
		R2_GLOBALS._sceneManager.changeScene(3100);
		break;
	default:
		SceneItem::display(3255, 0, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
		_sceneMode = 3256;
		setAction(&_sequenceManager, this, 3256, &R2_GLOBALS._player, NULL);
	}
}

void Scene3255::dispatch() {
	if (R2_GLOBALS.getFlag(79)) {
		if (_ghoul1._position.y >= 95) {
			if (_ghoul1._position.y <= 110)
				_ghoul1._shade = 6 - (_ghoul1._position.y - 95) / 3;
			else
				_ghoul1._effect = 1;
		} else {
			_ghoul1._effect = 6;
			_ghoul1._shade = 6;
		}

		if (_ghoul2._position.y >= 95) {
			if (_ghoul2._position.y <= 110)
				_ghoul2._shade = 6 - (_ghoul2._position.y - 95) / 3;
			else
				_ghoul2._effect = 1;
		} else {
			_ghoul2._effect = 6;
			_ghoul2._shade = 6;
		}

		if (_ghoul3._position.y >= 95) {
			if (_ghoul3._position.y <= 110)
				_ghoul3._shade = 6 - (_ghoul3._position.y - 95) / 3;
			else
				_ghoul3._effect = 1;
		} else {
			_ghoul3._effect = 6;
			_ghoul3._shade = 6;
		}
	}

	if ((R2_GLOBALS._player._position.x > 250) && (R2_GLOBALS._player._shade == 1)) {
		R2_GLOBALS._player._effect = 6;
		_quinn._effect = 6;
	}
	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3260 - Computer room
 *
 *--------------------------------------------------------------------------*/

bool Scene3260::Door::startAction(CursorType action, Event &event) {
	Scene3260 *scene = (Scene3260 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3271;
	scene->setAction(&scene->_sequenceManager, scene, 3271, &R2_GLOBALS._player, &scene->_door, NULL);
	return true;
}

bool Scene3260::Toolbox::startAction(CursorType action, Event &event) {
	Scene3260 *scene = (Scene3260 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3272;
	scene->setAction(&scene->_sequenceManager, scene, 3272, &R2_GLOBALS._player, &scene->_toolbox, NULL);
	return true;
}

void Scene3260::Action1::signal() {
	SceneObjectExt *fmtObj = (SceneObjectExt *) _endHandler;

	fmtObj->setFrame(R2_GLOBALS._randomSource.getRandomNumber(6));
	setDelay(120 + R2_GLOBALS._randomSource.getRandomNumber(179));
}

void Scene3260::postInit(SceneObjectList *OwnerList) {
	loadScene(3260);
	R2_GLOBALS._player._characterIndex = R2_MIRANDA;
	SceneExt::postInit();
	R2_GLOBALS._sound1.play(285);

	_door.postInit();
	_door.setup(3260, 6, 1);
	_door.setPosition(Common::Point(40, 106));
	_door.setDetails(3260, 18, 1, -1, 1, (SceneItem *)NULL);

	if (R2_INVENTORY.getObjectScene(R2_TOOLBOX) == 3260) {
		_toolbox.postInit();
		_toolbox.setup(3260, 7, 1);
		_toolbox.setPosition(Common::Point(202, 66));
		_toolbox.setDetails(3260, 12, 1, -1, 1, (SceneItem *)NULL);
	}

	_sceeen1.postInit();
	_sceeen1.setup(3260, 1, 1);
	_sceeen1.setPosition(Common::Point(93, 73));
	_sceeen1.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_sceeen1.setAction(&_action1, &_sceeen1);

	_screen2.postInit();
	_screen2.setup(3260, 2, 1);
	_screen2.setPosition(Common::Point(142, 63));
	_screen2.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_screen2.setAction(&_action2, &_screen2);

	_screen3.postInit();
	_screen3.setup(3260, 2, 1);
	_screen3.setPosition(Common::Point(166, 54));
	_screen3.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_screen3.setAction(&_action3, &_screen3);

	_screen4.postInit();
	_screen4.setup(3260, 2, 1);
	_screen4.setPosition(Common::Point(190, 46));
	_screen4.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_screen4.setAction(&_action4, &_screen4);

	_screen5.postInit();
	_screen5.setup(3260, 2, 1);
	_screen5.setPosition(Common::Point(142, 39));
	_screen5.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_screen5.setAction(&_action5, &_screen5);

	_screen6.postInit();
	_screen6.setup(3260, 2, 1);
	_screen6.setPosition(Common::Point(166, 30));
	_screen6.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_screen6.setAction(&_action6, &_screen6);

	_actor7.postInit();
	_actor7.setup(3260, 2, 1);
	_actor7.setPosition(Common::Point(190, 22));
	_actor7.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_actor7.setAction(&_action7, &_actor7);

	_actor8.postInit();
	_actor8.setup(3260, 2, 1);
	_actor8.setPosition(Common::Point(142, 14));
	_actor8.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_actor8.setAction(&_action8, &_actor8);

	_actor9.postInit();
	_actor9.setup(3260, 2, 1);
	_actor9.setPosition(Common::Point(166, 6));
	_actor9.setDetails(3260, 3, 1, 5, 1, (SceneItem *)NULL);
	_actor9.setAction(&_action9, &_actor9);

	_securityConsole.postInit();
	_securityConsole.setup(3260, 3, 1);
	_securityConsole.setPosition(Common::Point(265, 163));
	_securityConsole.fixPriority(180);
	_securityConsole._numFrames = 10;
	_securityConsole.setDetails(3260, 6, 1, 8, 1, (SceneItem *)NULL);
	_securityConsole.animate(ANIM_MODE_2, NULL);

	_computerConsole.postInit();
	_computerConsole.setup(3260, 4, 1);
	_computerConsole.setPosition(Common::Point(127, 108));
	_computerConsole.fixPriority(120);
	_computerConsole.setAction(&_action11, &_computerConsole);
	_computerConsole._numFrames = 15;
	_computerConsole.setDetails(3260, 6, 1, 8, 1, (SceneItem *)NULL);
	_computerConsole.animate(ANIM_MODE_2, NULL);

	_lightingConsole.postInit();
	_lightingConsole.setup(3260, 5, 1);
	_lightingConsole.setPosition(Common::Point(274, 65));
	_lightingConsole.setAction(&_action12, &_lightingConsole);
	_lightingConsole._numFrames = 5;
	_lightingConsole.setDetails(3260, 9, 1, 11, 1, (SceneItem *)NULL);
	_lightingConsole.animate(ANIM_MODE_2, NULL);

	_item1.setDetails(Rect(0, 0, 320, 200), 3260, 0, 1, 2, 1, NULL);
	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] == 3275) {
		_sceneMode = 3270;
		setAction(&_sequenceManager, this, 3270, &R2_GLOBALS._player, &_door, NULL);
	} else {
		R2_GLOBALS._player.setup(30, 5, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(53, 113));
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		R2_GLOBALS._player.enableControl();
	}
	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3260;
}

void Scene3260::remove() {
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3260::signal() {
	switch (_sceneMode) {
	case 3271:
		R2_GLOBALS._sceneManager.changeScene(3275);
		break;
	case 3272:
		_sceneMode = 3273;
		R2_GLOBALS._events.setCursor(CURSOR_WALK);
		SceneItem::display(3260, 15, 0, 280, 1, 160, 9, 1, 2, 20, 7, 154, -999);
		R2_GLOBALS._player.disableControl();
		R2_INVENTORY.setObjectScene(R2_TOOLBOX, 3);
		R2_INVENTORY.setObjectScene(R2_LASER_HACKSAW, 3);
		setAction(&_sequenceManager, this, 3273, &R2_GLOBALS._player, &_toolbox, NULL);
		break;
	case 3273:
		_screen4.remove();
		R2_GLOBALS._player.enableControl();
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3275 - Hall
 *
 *--------------------------------------------------------------------------*/

bool Scene3275::Door::startAction(CursorType action, Event &event) {
	Scene3275 *scene = (Scene3275 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3275;
	scene->setAction(&scene->_sequenceManager, scene, 3275, &R2_GLOBALS._player, &scene->_door, NULL);
	return true;
}

void Scene3275::CellExit::changeScene() {
	Scene3275 *scene = (Scene3275 *)R2_GLOBALS._sceneManager._scene;

	scene->_sceneMode = 0;
	g_globals->_events.setCursor(CURSOR_ARROW);
	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 10;
	Common::Point pt(418, 118);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3275::postInit(SceneObjectList *OwnerList) {
	loadScene(3275);

	if (R2_GLOBALS._sceneManager._previousScene == -1)
		R2_GLOBALS._sceneManager._previousScene = 3260;

	if (R2_GLOBALS._sceneManager._previousScene == 3150)
		_sceneBounds = Rect(160, 0, 480, 200);

	SceneExt::postInit();
	_cellExit.setDetails(Rect(398, 60, 439, 118), SHADECURSOR_UP, 3150);
	_cellExit.setDest(Common::Point(418, 128));

	_actor1.postInit();
	_actor1.setup(3275, 1, 7);
	_actor1.setPosition(Common::Point(419, 119));

	_door.postInit();
	_door.setup(3275, 2, 1);
	_door.setPosition(Common::Point(56, 118));
	_door.setDetails(3275, 3, 4, -1, 1, (SceneItem *)NULL);

	_emptyCell1.setDetails(Rect(153, 58, 200, 120), 3275, 6, 7, 8, 1, NULL);
	_emptyCell2.setDetails(Rect(275, 58, 331, 120), 3275, 6, 7, 8, 1, NULL);
	_securityBeams1.setDetails(Rect(0, 66, 22, 127), 3275, 9, 10, 11, 1, NULL);
	_securityBeams2.setDetails(Rect(457, 66, 480, 127), 3275, 9, 10, 11, 1, NULL);
	_background.setDetails(Rect(0, 0, 480, 200), 3275, 0, 1, 2, 1, NULL);

	R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.disableControl();
	if (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] == 3150) {
		_sceneMode = 11;
		R2_GLOBALS._player.setup(30, 3, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(418, 118));
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		Common::Point pt(418, 128);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
	} else if (R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] == 3260) {
		_sceneMode = 3276;
		setAction(&_sequenceManager, this, 3276, &R2_GLOBALS._player, &_door, NULL);
	} else {
		R2_GLOBALS._player.setup(30, 3, 1);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(245, 135));
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		R2_GLOBALS._player.enableControl();
	}
	R2_GLOBALS._player._oldCharacterScene[R2_MIRANDA] = 3275;
}

void Scene3275::signal() {
	switch (_sceneMode) {
	case 10:
		R2_GLOBALS._sceneManager.changeScene(3150);
		break;
	case 3275:
		R2_GLOBALS._sceneManager.changeScene(3260);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3350 - Cutscene - Ship landing
 *
 *--------------------------------------------------------------------------*/

void Scene3350::postInit(SceneObjectList *OwnerList) {
	loadScene(3350);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();
	R2_GLOBALS._interfaceY = SCREEN_HEIGHT;
	R2_GLOBALS._sound2.play(310);

	_rotation = R2_GLOBALS._scenePalette.addRotation(176, 203, 1);
	_rotation->setDelay(3);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.disableControl();

	_actor1.postInit();
	_actor1.hide();
	_actor2.postInit();
	_actor2.hide();
	_actor3.postInit();
	_actor3.hide();
	_actor4.postInit();
	_actor4.hide();
	_actor9.postInit();
	_actor9.hide();
	_actor8.postInit();
	_actor8.hide();
	_actor5.postInit();
	_actor5.hide();
	_actor6.postInit();
	_actor6.hide();
	_actor7.postInit();
	_actor7.hide();

	_sceneMode = 3350;
	setAction(&_sequenceManager, this, _sceneMode, &_actor5, &_actor6, &_actor7, NULL);
}

void Scene3350::remove() {
	R2_GLOBALS._sound2.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3350::signal() {
	switch (_sceneMode) {
	case 3350:
		_sceneMode = 3351;
		setAction(&_sequenceManager, this, 3351, &_actor4, &_actor9, &_actor8, NULL);
		break;
	case 3351:
		_sceneMode = 3352;
		setAction(&_sequenceManager, this, 3352, &_actor4, &R2_GLOBALS._player, &_actor1, &_actor2, &_actor3, NULL);
		break;
	case 3352:
		R2_GLOBALS._sceneManager.changeScene(3395);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3375 - Circular Walkway
 *
 *--------------------------------------------------------------------------*/

void Scene3375::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field1488);
	s.syncAsSint16LE(_field1492);
	for (int i = 0; i < 4; ++i)
		s.syncAsSint16LE(_sceneAreas[i]);
}

void Scene3375::enterArea(int sceneMode) {
	switch (sceneMode) {
	case 3379:
		R2_GLOBALS._player.setPosition(Common::Point(0, 155));
		_companion1.setPosition(Common::Point(-20, 163));
		_companion2.setPosition(Common::Point(-5, 150));
		_webbster.setPosition(Common::Point(-20, 152));
		break;
	case 3380:
		++R2_GLOBALS._walkwaySceneNumber;
		if (R2_GLOBALS._walkwaySceneNumber >= 4)
			R2_GLOBALS._walkwaySceneNumber = 0;

		loadScene(_sceneAreas[R2_GLOBALS._walkwaySceneNumber]);

		R2_GLOBALS._uiElements.show();
		R2_GLOBALS._player.setStrip(4);
		R2_GLOBALS._player.setPosition(Common::Point(148, 230));

		_companion1.setPosition(Common::Point(191, 274));
		_companion1._effect = 1;
		_companion2.setPosition(Common::Point(124, 255));
		_companion2._effect = 1;
		_webbster.setPosition(Common::Point(155, 245));
		_webbster._effect = 1;
		break;
	case 3381:
		--R2_GLOBALS._walkwaySceneNumber;
		if (R2_GLOBALS._walkwaySceneNumber < 0)
			R2_GLOBALS._walkwaySceneNumber = 3;

		loadScene(_sceneAreas[R2_GLOBALS._walkwaySceneNumber]);

		R2_GLOBALS._uiElements.show();
		R2_GLOBALS._player.setStrip(6);
		R2_GLOBALS._player.setPosition(Common::Point(201, 131));

		_companion1.setPosition(Common::Point(231, 127));
		_companion1._effect = 1;
		_companion2.setPosition(Common::Point(231, 127));
		_companion2._effect = 1;
		_webbster.setPosition(Common::Point(231, 127));
		_webbster._effect = 1;
		break;
	default:
		R2_GLOBALS._player.setPosition(Common::Point(192, 155));

		_companion1.setPosition(Common::Point(138, 134));
		_companion2.setPosition(Common::Point(110, 139));
		_webbster.setPosition(Common::Point(125, 142));
		break;
	}

	if (R2_GLOBALS._walkwaySceneNumber == 2) {
		R2_GLOBALS._sceneItems.remove(&_door);
		for (int i = 0; i <= 12; i++)
			R2_GLOBALS._sceneItems.remove(&_itemArray[i]);
		R2_GLOBALS._sceneItems.remove(&_background);

		_door.show();
		_door.setDetails(3375, 9, 10, -1, 1, (SceneItem *)NULL);

		for (int i = 0; i <= 12; i++)
			_itemArray[i].setDetails(3375, 3, -1, -1);

		_background.setDetails(Rect(0, 0, 320, 200), 3375, 0, -1, -1, 1, NULL);
	} else {
		_door.hide();
		R2_GLOBALS._sceneItems.remove(&_door);
	}

	if (_sceneMode == 0)
		signal();
	else
		setAction(&_sequenceManager, this, _sceneMode, &R2_GLOBALS._player, &_companion1, &_companion2, &_webbster, NULL);
}

bool Scene3375::Companion2::startAction(CursorType action, Event &event) {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3304, scene);

	return true;
}

bool Scene3375::Companion1::startAction(CursorType action, Event &event) {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3301, scene);

	return true;
}

bool Scene3375::Webbster::startAction(CursorType action, Event &event) {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	scene->_stripManager.start(3303, scene);

	return true;
}

bool Scene3375::Door::startAction(CursorType action, Event &event) {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	if (R2_GLOBALS._walkwaySceneNumber != 0) {
		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(3);
	} else {
		R2_GLOBALS._walkRegions.enableRegion(1);
		R2_GLOBALS._walkRegions.enableRegion(3);
		R2_GLOBALS._walkRegions.enableRegion(4);
	}
	R2_GLOBALS._walkRegions.enableRegion(6);
	R2_GLOBALS._walkRegions.enableRegion(7);
	R2_GLOBALS._walkRegions.enableRegion(8);

	R2_GLOBALS._player.disableControl(CURSOR_ARROW);

	scene->_sceneMode = 3375;
	scene->setAction(&scene->_sequenceManager, scene, 3375, &R2_GLOBALS._player, 
		&scene->_companion1, &scene->_companion2, &scene->_webbster, &scene->_door, NULL);

	return true;
}

void Scene3375::LeftExit::changeScene() {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	_moving = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_sceneMode = 3376;
	if (R2_GLOBALS._walkwaySceneNumber != 0) {
		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(3);
	} else {
		R2_GLOBALS._walkRegions.enableRegion(1);
		R2_GLOBALS._walkRegions.enableRegion(3);
		R2_GLOBALS._walkRegions.enableRegion(4);
	}
	if (scene->_companion1._position.y != 163) {
		R2_GLOBALS._player.setStrip2(-1);
		scene->_companion1.setStrip2(-1);
		scene->_companion2.setStrip2(-1);
		scene->_webbster.setStrip2(-1);
		scene->setAction(&scene->_sequenceManager, scene, scene->_sceneMode, &R2_GLOBALS._player, &scene->_companion1, &scene->_companion2, &scene->_webbster, NULL);
	} else {
		R2_GLOBALS._player.setStrip2(2);
		scene->_companion1.setStrip2(2);
		scene->_companion2.setStrip2(2);
		scene->_webbster.setStrip2(2);
		R2_GLOBALS._sound2.play(314);

		Common::Point pt(50, 150);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, scene);
	}
}

void Scene3375::DownExit::changeScene() {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	_moving = false;
	R2_GLOBALS._player._effect = 6;
	R2_GLOBALS._player._shade = 4;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);

	scene->_sceneMode = 3377;
	scene->_field1488 = 3381;

	if (R2_GLOBALS._walkwaySceneNumber != 0) {
		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(3);
	} else {
		R2_GLOBALS._walkRegions.enableRegion(1);
		R2_GLOBALS._walkRegions.enableRegion(3);
		R2_GLOBALS._walkRegions.enableRegion(4);
	}
	scene->setAction(&scene->_sequenceManager, scene, scene->_sceneMode, &R2_GLOBALS._player, &scene->_companion1, &scene->_companion2, &scene->_webbster, NULL);
}

void Scene3375::RightExit::changeScene() {
	Scene3375 *scene = (Scene3375 *)R2_GLOBALS._sceneManager._scene;

	_moving = false;
	R2_GLOBALS._player._effect = 6;
	R2_GLOBALS._player._shade = 4;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);

	scene->_sceneMode = 3378;
	scene->_field1488 = 3380;

	if (R2_GLOBALS._walkwaySceneNumber != 0) {
		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(3);
	} else {
		R2_GLOBALS._walkRegions.enableRegion(1);
		R2_GLOBALS._walkRegions.enableRegion(3);
		R2_GLOBALS._walkRegions.enableRegion(4);
	}
	scene->setAction(&scene->_sequenceManager, scene, scene->_sceneMode, &R2_GLOBALS._player, &scene->_companion1, &scene->_companion2, &scene->_webbster, NULL);
}

Scene3375::Scene3375() {
	_field1488 = _field1492 = 0;

	_sceneAreas[0] = 3376;
	_sceneAreas[1] = 3377;
	_sceneAreas[2] = 3375;
	_sceneAreas[3] = 3378;
}

void Scene3375::postInit(SceneObjectList *OwnerList) {
	loadScene(_sceneAreas[R2_GLOBALS._walkwaySceneNumber]);
	SceneExt::postInit();

	R2_GLOBALS._sound1.play(313);

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);
	_stripManager.addSpeaker(&_webbsterSpeaker);

	R2_GLOBALS._player._characterScene[R2_QUINN] = 3375;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3375;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3375;

	setZoomPercents(126, 55, 200, 167);
	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
		R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
	} else {
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
	}
	R2_GLOBALS._player.changeZoom(-1);

	switch (R2_GLOBALS._player._characterIndex) {
	case R2_SEEKER:
		if (R2_GLOBALS._sceneManager._previousScene == 3385)
			R2_GLOBALS._player.setup(20, 1, 1);
		else
			R2_GLOBALS._player.setup(20, 3, 1);
		break;
	case R2_MIRANDA:
		if (R2_GLOBALS._sceneManager._previousScene == 3385)
			R2_GLOBALS._player.setup(30, 1, 1);
		else
			R2_GLOBALS._player.setup(30, 3, 1);
		break;
	default:
		if (R2_GLOBALS._sceneManager._previousScene == 3385)
			R2_GLOBALS._player.setup(10, 1, 1);
		else
			R2_GLOBALS._player.setup(10, 3, 1);
		break;
	}

	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.disableControl();

	_companion1.postInit();
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
		_companion1._moveRate = 10;
		_companion1._moveDiff = Common::Point(3, 2);
	} else {
		_companion1._moveRate = 7;
		_companion1._moveDiff = Common::Point(5, 3);
	}
	_companion1.changeZoom(-1);
	_companion1._effect = 1;

	int tmpStrip, tmpVisage;
	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		tmpStrip = 1;
	else
		tmpStrip = 4;

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		tmpVisage = 10;
	else
		tmpVisage = 20;

	_companion1.setup(tmpVisage, tmpStrip, 1);
	_companion1.animate(ANIM_MODE_1, NULL);

	_companion2.postInit();
	_companion2._moveDiff = Common::Point(3, 2);
	_companion2.changeZoom(-1);
	_companion2._effect = 1;
	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		tmpStrip = 1;
	else
		tmpStrip = 8;

	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		tmpVisage = 10;
	else
		tmpVisage = 30;

	_companion2.setup(tmpVisage, tmpStrip, 1);
	_companion2.animate(ANIM_MODE_1, NULL);

	_webbster.postInit();
	_webbster._moveRate = 7;
	_webbster._moveDiff = Common::Point(5, 3);
	_webbster.changeZoom(-1);
	_webbster._effect = 1;
	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		tmpStrip = 1;
	else
		tmpStrip = 4;

	_webbster.setup(40, tmpStrip, 1);
	_webbster.animate(ANIM_MODE_1, NULL);

	_companion2.setDetails(3375, -1, -1, -1, 1, (SceneItem *)NULL);
	_webbster.setDetails(3375, 21, -1, -1, 1, (SceneItem *)NULL);
	_companion1.setDetails(3375, -1, -1, -1, 1, (SceneItem *)NULL);

	_door.postInit();
	_door.setup(3375, 1, 1);
	_door.setPosition(Common::Point(254, 166));
	_door.fixPriority(140);
	_door.hide();

	_leftExit.setDetails(Rect(0, 84, 24, 167), EXITCURSOR_W, 3375);
	_leftExit.setDest(Common::Point(65, 155));
	_downExit.setDetails(Rect(103, 152, 183, 170), SHADECURSOR_DOWN, 3375);
	_downExit.setDest(Common::Point(158, 151));
	_rightExit.setDetails(Rect(180, 75, 213, 132), EXITCURSOR_E, 3375);
	_rightExit.setDest(Common::Point(201, 131));

	for (int i = 0; i <= 12; ++i)
		_itemArray[i].setDetails(i, 3375, 3, -1, -1);

	_background.setDetails(Rect(0, 0, 320, 200), 3375, 0, -1, 1, 1, NULL);

	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		_sceneMode = 3379;
	else
		_sceneMode = 0;

	enterArea(_sceneMode);
}

void Scene3375::remove() {
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3375::signalCase3379() {
	switch (R2_GLOBALS._walkwaySceneNumber) {
	case 0:
		_leftExit._enabled = true;
		if (R2_GLOBALS._sceneManager._previousScene == 3385) {
			// WORKAROUND: The original disables the left entry region here for
			// some reason. But there's also some walk issue even I leave it enabled.
			// Instead, for now, add an extra walk into the properly enabled regions
			_sceneMode = 1;
			ADD_MOVER(R2_GLOBALS._player, 70, R2_GLOBALS._player._position.y);
			R2_GLOBALS._sceneManager._previousScene = 3375;
			R2_GLOBALS._player._effect = 1;
			_companion1._effect = 1;
			_companion2._effect = 1;
			_webbster._effect = 1;

			return;
			//R2_GLOBALS._walkRegions.disableRegion(1);
		} else {
			R2_GLOBALS._walkRegions.disableRegion(3);
			R2_GLOBALS._walkRegions.disableRegion(4);
		}
		R2_GLOBALS._walkRegions.disableRegion(6);
		R2_GLOBALS._walkRegions.disableRegion(7);
		break;
	case 2:
		_leftExit._enabled = false;
		R2_GLOBALS._walkRegions.disableRegion(2);
		R2_GLOBALS._walkRegions.disableRegion(3);
		R2_GLOBALS._walkRegions.disableRegion(5);
		R2_GLOBALS._walkRegions.disableRegion(6);
		R2_GLOBALS._walkRegions.disableRegion(7);
		R2_GLOBALS._walkRegions.disableRegion(8);
		R2_GLOBALS._walkRegions.disableRegion(9);
		break;
	default:
		_leftExit._enabled = false;
		R2_GLOBALS._walkRegions.disableRegion(2);
		R2_GLOBALS._walkRegions.disableRegion(3);
		R2_GLOBALS._walkRegions.disableRegion(5);
		R2_GLOBALS._walkRegions.disableRegion(6);
		break;
	}
	R2_GLOBALS._sceneManager._previousScene = 3375;
	R2_GLOBALS._player._effect = 1;
	_companion1._effect = 1;
	_companion2._effect = 1;
	_webbster._effect = 1;
	R2_GLOBALS._player.enableControl(CURSOR_WALK);
}

void Scene3375::signal() {
	switch (_sceneMode) {
	case 1:
		R2_GLOBALS._player.enableControl();
		break;
	case 3375:
		R2_GLOBALS._sceneManager.changeScene(3400);
		break;
	case 3376:
		R2_GLOBALS._sceneManager.changeScene(3385);
		break;
	case 3377:
		// No break on purpose
	case 3378:
		_sceneMode = _field1488;
		_field1488 = 0;
		_companion1._effect = 6;
		_companion1._shade = 4;
		_companion2._effect = 6;
		_companion2._shade = 4;
		_webbster._effect = 6;
		_webbster._shade = 4;
		enterArea(_sceneMode);
		break;
	case 3379:
		signalCase3379();
		break;
	case 9999:
		if (_companion1._position.y == 163)
			R2_GLOBALS._player.setStrip(1);
		else
			R2_GLOBALS._player.setStrip(3);
		R2_GLOBALS._player.enableControl(CURSOR_TALK);
		break;
	default:
		_companion1.setPriority(130);
		_companion2.setPriority(132);
		_webbster.setPriority(134);
		signalCase3379();
		break;
	}
}

void Scene3375::dispatch() {
	if ((R2_GLOBALS._player._position.y >= 168) && (R2_GLOBALS._player._effect == 1))
		R2_GLOBALS._player._effect = 6;
	else if ((R2_GLOBALS._player._position.y < 168) && (R2_GLOBALS._player._effect == 6))
		R2_GLOBALS._player._effect = 1;

	if ((_companion1._position.y >= 168) && (_companion1._effect == 1))
		_companion1._effect = 6;
	else if ((_companion1._position.y < 168) && (_companion1._effect == 6))
		_companion1._effect = 1;

	if ((_companion2._position.y >= 168) && (_companion2._effect == 1))
		_companion2._effect = 6;
	else if ((_companion2._position.y < 168) && (_companion2._effect == 6))
		_companion2._effect = 1;

	if ((_webbster._position.y >= 168) && (_webbster._effect == 1))
		_webbster._effect = 6;
	else if ((_webbster._position.y < 168) && (_webbster._effect == 6))
		_webbster._effect = 1;

	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3385 - Corridor
 *
 *--------------------------------------------------------------------------*/

Scene3385::Scene3385() {
	_field11B2 = 0;
}

void Scene3385::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field11B2);
}

bool Scene3385::Companion1::startAction(CursorType action, Event &event) {
	Scene3385 *scene = (Scene3385 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3304, scene);

	return true;
}

bool Scene3385::Companion2::startAction(CursorType action, Event &event) {
	Scene3385 *scene = (Scene3385 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3301, scene);

	return true;
}

bool Scene3385::Webbster::startAction(CursorType action, Event &event) {
	Scene3385 *scene = (Scene3385 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	scene->_stripManager.start(3303, scene);

	return true;
}

bool Scene3385::Door::startAction(CursorType action, Event &event) {
	Scene3385 *scene = (Scene3385 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	if (R2_GLOBALS._sceneManager._previousScene == 3375)
		R2_GLOBALS._sound2.play(314);

	scene->_sceneMode = 3386;
	scene->setAction(&scene->_sequenceManager, scene, 3386, &R2_GLOBALS._player, 
		&scene->_companion1, &scene->_companion2, &scene->_webbster, &scene->_door, 
		NULL);

	return true;
}

void Scene3385::Exit1::changeScene() {
	Scene3385 *scene = (Scene3385 *)R2_GLOBALS._sceneManager._scene;

	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_sceneMode = 3387;

	if (R2_GLOBALS._sceneManager._previousScene == 3375)
		scene->setAction(&scene->_sequenceManager, scene, scene->_sceneMode, 
		&R2_GLOBALS._player, &scene->_companion1, &scene->_companion2, 
		&scene->_webbster, NULL);
	else
		scene->signal();
}

void Scene3385::Action1::signal() {
	int v = _actionIndex;
	++_actionIndex;

	if (v == 0)
		setDelay(1);
	else if (v == 1)
		R2_GLOBALS._sound2.play(314);
}

void Scene3385::postInit(SceneObjectList *OwnerList) {
	loadScene(3385);
	SceneExt::postInit();

	R2_GLOBALS._sound1.play(313);

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);
	_stripManager.addSpeaker(&_webbsterSpeaker);

	R2_GLOBALS._player._characterScene[R2_QUINN] = 3385;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3385;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3385;

	if (R2_GLOBALS._sceneManager._previousScene == 3375)
		_field11B2 = 3;
	else
		_field11B2 = 4;

	setZoomPercents(102, 40, 200, 160);
	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
	else
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);

	R2_GLOBALS._player.changeZoom(-1);

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player.setup(20, _field11B2, 1);
	else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		R2_GLOBALS._player.setup(30, _field11B2, 1);
	else
		R2_GLOBALS._player.setup(10, _field11B2, 1);

	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.disableControl();

	_companion1.postInit();
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
		_companion1._moveRate = 10;
		_companion1._moveDiff = Common::Point(3, 2);
	} else {
		_companion1._moveRate = 7;
		_companion1._moveDiff = Common::Point(5, 3);
	}
	_companion1.changeZoom(-1);
	_companion1._effect = 1;
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		_companion1.setup(10, _field11B2, 1);
	else
		_companion1.setup(20, _field11B2, 1);
	_companion1.animate(ANIM_MODE_1, NULL);
	_companion1.setDetails(3385, -1, -1, -1, 1, (SceneItem *) NULL);

	_companion2.postInit();
	_companion2._moveDiff = Common::Point(3, 2);
	_companion2.changeZoom(-1);
	_companion2._effect = 1;
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		_companion2.setup(10, _field11B2, 1);
	else
		_companion2.setup(30, _field11B2, 1);
	_companion2.animate(ANIM_MODE_1, NULL);
	_companion2.setDetails(3385, -1, -1, -1, 1, (SceneItem *) NULL);

	_webbster.postInit();
	_webbster._moveDiff = Common::Point(3, 2);
	_webbster.changeZoom(-1);
	_webbster._effect = 1;
	_webbster.setup(40, _field11B2, 1);
	_webbster.animate(ANIM_MODE_1, NULL);
	_webbster.setDetails(3385, 15, -1, -1, 1, (SceneItem *) NULL);

	_exit1.setDetails(Rect(103, 152, 217, 170), SHADECURSOR_DOWN, 3395);
	_exit1.setDest(Common::Point(158, 151));

	_door.postInit();
	_door.setPosition(Common::Point(160, 100));
	_door.fixPriority(90);
	_door.setDetails(3385, 3, 4, -1, 1, (SceneItem *) NULL);

	if (R2_GLOBALS._sceneManager._previousScene == 3375) {
		R2_GLOBALS._player.setPosition(Common::Point(158, 102));
		_companion1.setPosition(Common::Point(164, 100));
		_companion1.fixPriority(98);
		_companion2.setPosition(Common::Point(150, 100));
		_companion2.fixPriority(97);
		_webbster.setPosition(Common::Point(158, 100));
		_webbster.fixPriority(96);
		_sceneMode = 3384;
		_door.setup(3385, 1, 6);
		_door.animate(ANIM_MODE_6, this);
		setAction(&_action1, &_door);
	} else {
		R2_GLOBALS._player.setPosition(Common::Point(158, 230));
		_companion1.setPosition(Common::Point(191, 270));
		_companion2.setPosition(Common::Point(124, 255));
		_webbster.setPosition(Common::Point(155, 245));
		_door.setup(3385, 1, 1);
		_sceneMode = 3385;
		setAction(&_sequenceManager, this, _sceneMode, &R2_GLOBALS._player, &_companion1, &_companion2, &_webbster, NULL);
	}

	_background.setDetails(Rect(0, 0, 320, 200), 3385, 0, -1, -1, 1, NULL);
	R2_GLOBALS._walkwaySceneNumber = 0;
}

void Scene3385::remove() {
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3385::signal() {
	switch (_sceneMode) {
	case 3386:
		R2_GLOBALS._sceneManager.changeScene(3375);
		break;
	case 3387:
		R2_GLOBALS._sceneManager.changeScene(3395);
		break;
	case 9999:
		if (R2_GLOBALS._sceneManager._previousScene == 3375)
			R2_GLOBALS._player.setStrip(3);
		else
			R2_GLOBALS._player.setStrip(4);
		R2_GLOBALS._player.enableControl(CURSOR_TALK);
		break;
	default:
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3395 - Walkway
 *
 *--------------------------------------------------------------------------*/

Scene3395::Scene3395() {
	_field142E = 0;
}

void Scene3395::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field142E);
}

bool Scene3395::Companion1::startAction(CursorType action, Event &event) {
	Scene3395 *scene = (Scene3395 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3304, scene);

	return true;
}

bool Scene3395::Companion2::startAction(CursorType action, Event &event) {
	Scene3395 *scene = (Scene3395 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		scene->_stripManager.start(3302, scene);
	else
		scene->_stripManager.start(3301, scene);

	return true;
}

bool Scene3395::Webbster::startAction(CursorType action, Event &event) {
	Scene3395 *scene = (Scene3395 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_TALK)
		return SceneActor::startAction(action, event);

	scene->_sceneMode = 9999;
	scene->_stripManager.start(3303, scene);

	return true;
}

bool Scene3395::Door::startAction(CursorType action, Event &event) {
	Scene3395 *scene = (Scene3395 *)R2_GLOBALS._sceneManager._scene;

	if (action != CURSOR_USE)
		return SceneActor::startAction(action, event);

	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		R2_GLOBALS._sound2.play(314);

	scene->_sceneMode = 3396;
	scene->setAction(&scene->_sequenceManager, scene, 3396, &R2_GLOBALS._player, 
		&scene->_companion1, &scene->_companion2, &scene->_webbster, &scene->_door, 
		NULL);

	return true;
}

void Scene3395::Action1::signal() {
	int v = _actionIndex;
	++_actionIndex;

	if (v == 0)
		setDelay(2);
	else if (v == 1)
		R2_GLOBALS._sound2.play(314);
}

void Scene3395::postInit(SceneObjectList *OwnerList) {
	loadScene(3395);
	SceneExt::postInit();

	R2_GLOBALS._sound1.play(313);

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);
	_stripManager.addSpeaker(&_webbsterSpeaker);

	R2_GLOBALS._player._characterScene[R2_QUINN] = 3395;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3395;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3395;

	if (R2_GLOBALS._sceneManager._previousScene == 3385)
		_field142E = 3;
	else
		_field142E = 4;

	setZoomPercents(51, 40, 200, 137);
	R2_GLOBALS._player.postInit();

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
	else
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);

	R2_GLOBALS._player.changeZoom(-1);

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player.setup(20, _field142E, 1);
	else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		R2_GLOBALS._player.setup(30, _field142E, 1);
	else
		R2_GLOBALS._player.setup(10, _field142E, 1);

	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.disableControl();

	_companion1.postInit();
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
		_companion1._moveRate = 10;
		_companion1._moveDiff = Common::Point(3, 2);
	} else {
		_companion1._moveRate = 7;
		_companion1._moveDiff = Common::Point(5, 3);
	}
	_companion1.changeZoom(-1);
	_companion1._effect = 1;
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		_companion1.setup(10, _field142E, 1);
	else
		_companion1.setup(20, _field142E, 1);
	_companion1.animate(ANIM_MODE_1, NULL);
	_companion1.setDetails(3395, -1, -1, -1, 1, (SceneItem *) NULL);

	_companion2.postInit();
	_companion2._moveDiff = Common::Point(3, 2);
	_companion2.changeZoom(-1);
	_companion2._effect = 1;
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		_companion2.setup(10, _field142E, 1);
	else
		_companion2.setup(30, _field142E, 1);
	_companion2.animate(ANIM_MODE_1, NULL);
	_companion2.setDetails(3395, -1, -1, -1, 1, (SceneItem *) NULL);

	_webbster.postInit();
	_webbster._moveDiff = Common::Point(3, 2);
	_webbster.changeZoom(-1);
	_webbster._effect = 1;
	_webbster.setup(40, _field142E, 1);
	_webbster.animate(ANIM_MODE_1, NULL);
	_webbster.setDetails(3395, 18, -1, -1, 1, (SceneItem *) NULL);

	_door.postInit();
	_door.setPosition(Common::Point(159, 50));
	_door.fixPriority(40);
	_door.setDetails(3395, 6, 7, -1, 1, (SceneItem *) NULL);

	if (R2_GLOBALS._sceneManager._previousScene == 3385) {
		R2_GLOBALS._player.setPosition(Common::Point(158, 53));
		_companion1.setPosition(Common::Point(164, 51));
		_companion1.fixPriority(48);
		_companion2.setPosition(Common::Point(150, 51));
		_companion2.fixPriority(47);
		_webbster.setPosition(Common::Point(158, 51));
		_webbster.fixPriority(46);
		_sceneMode = 3394;
		_door.setup(3395, 1, 7);
		_door.animate(ANIM_MODE_6, this);
		setAction(&_action1, &_door);
	} else {
		R2_GLOBALS._player.setPosition(Common::Point(158, 200));
		_companion1.setPosition(Common::Point(191, 255));
		_companion2.setPosition(Common::Point(124, 240));
		_webbster.setPosition(Common::Point(155, 242));
		_door.setup(3395, 1, 1);

		R2_GLOBALS._walkRegions.disableRegion(1);

		_sceneMode = 3395;
		setAction(&_sequenceManager, this, _sceneMode, &R2_GLOBALS._player, &_companion1, &_companion2, &_webbster, NULL);
	}

	for (int i = 0; i <= 12; i++) {
		_itemArray[i].setDetails(i, 3395, 0, -1, -1);
	}

	_background.setDetails(Rect(0, 0, 320, 200), 3395, 3, -1, -1, 1, NULL);
}

void Scene3395::remove() {
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3395::signal() {
	switch (_sceneMode) {
	case 3396:
		R2_GLOBALS._sceneManager.changeScene(3385);
		break;
	case 9999:
		if (R2_GLOBALS._sceneManager._previousScene == 3385)
			R2_GLOBALS._player.setStrip(3);
		else
			R2_GLOBALS._player.setStrip(4);
		R2_GLOBALS._player.enableControl(CURSOR_TALK);
		break;
	default:
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3400 - Confrontation
 *
 *--------------------------------------------------------------------------*/

Scene3400::Scene3400() {
	_field157C = 0;
}

void Scene3400::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field157C);
}

void Scene3400::postInit(SceneObjectList *OwnerList) {
	R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
	_sceneBounds = Rect(160, 0, 480, 200);

	loadScene(3400);
	_field157C = 0;
	R2_GLOBALS._v558B6.set(60, 0, 260, 200);
	SceneExt::postInit();
	R2_GLOBALS._sound1.play(317);

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);
	_stripManager.addSpeaker(&_webbsterSpeaker);
	_stripManager.addSpeaker(&_tealSpeaker);

	setZoomPercents(51, 46, 180, 200);
	R2_GLOBALS._player._characterScene[R2_QUINN] = 3400;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3400;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3400;

	_actor7.postInit();
	_actor7.setup(3403, 1, 1);
	_actor7.setPosition(Common::Point(190, 103));
	_actor7.fixPriority(89);

	R2_GLOBALS._player.postInit();
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
	else
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
	R2_GLOBALS._player.changeZoom(-1);
	R2_GLOBALS._player.setPosition(Common::Point(239, 64));

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player.setup(20, 5, 1);
	else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		R2_GLOBALS._player.setup(30, 5, 1);
	else
		R2_GLOBALS._player.setup(10, 5, 1);

	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.disableControl();

	_companion1.postInit();
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
		_companion1._numFrames = 10;
		_companion1._moveDiff = Common::Point(3, 2);
	} else {
		_companion1._numFrames = 7;
		_companion1._moveDiff = Common::Point(5, 3);
	}
	_companion1.changeZoom(-1);
	_companion1._effect = 1;
	_companion1.setPosition(Common::Point(247, 63));
	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		_companion1.setup(10, 5, 1);
	else
		_companion1.setup(20, 5, 1);
	_companion1.animate(ANIM_MODE_1, NULL);

	_companion2.postInit();
	_companion2._moveDiff = Common::Point(3, 2);
	_companion2.changeZoom(-1);
	_companion2._effect = 1;
	_companion2.setPosition(Common::Point(225, 63));
	if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		_companion2.setup(10, 5, 1);
	else
		_companion2.setup(30, 5, 1);
	_companion2.animate(ANIM_MODE_1, NULL);

	_webbster.postInit();
	_webbster._numFrames = 7;
	_webbster._moveDiff = Common::Point(5, 3);
	_webbster.changeZoom(-1);
	_webbster._effect = 1;
	_webbster.setPosition(Common::Point(235, 61));
	_webbster.setup(40, 3, 1);
	_webbster.animate(ANIM_MODE_1, NULL);

	_actor6.postInit();
	_actor6.setup(3400, 1, 6);
	_actor6.setPosition(Common::Point(236, 51));
	_actor6.fixPriority(51);
	_actor6.animate(ANIM_MODE_6, NULL);

	R2_GLOBALS.clearFlag(71);
	_sceneMode = 3400;
	setAction(&_sequenceManager, this, 3400, &R2_GLOBALS._player, &_companion1, &_companion2, &_webbster, NULL);
}

void Scene3400::remove() {
	R2_GLOBALS._sound2.fadeOut2(NULL);
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3400::signal() {
	switch (_sceneMode) {
	case 3305: {
		warning("STUB: sub_1D227()");
		_tealSpeaker._object1.hide();
		_teal.show();
		_teal.setStrip(1);
		Common::Point pt(158, 190);
		NpcMover *mover = new NpcMover();
		_teal.addMover(mover, &pt, this);
		_sceneMode = 3402;
		setAction(&_sequenceManager, this, 3402, &R2_GLOBALS._player, &_companion1, &_companion2, &_webbster, NULL);
		}
		break;
	case 3306:
		R2_GLOBALS._sound2.play(318);
		_companion1.setStrip(2);
		R2_GLOBALS._player.setStrip(6);
		_companion2.setStrip(6);
		_webbster.setStrip(3);
		_teal.setStrip(1);
		R2_INVENTORY.setObjectScene(R2_SAPPHIRE_BLUE, 0);
		_stripManager.start(3307, this);
		if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
			_sceneMode = 3400;
			R2_GLOBALS._player.setAction(&_sequenceManager, this, 3400, &R2_GLOBALS._player, &_teal, &_actor8, NULL);
		} else {
			_sceneMode = 3408;
			_companion1.setAction(&_sequenceManager, this, 3408, &_companion1, &_teal, &_actor8, NULL);
		}
		break;
	case 3307:
	case 3404:
	case 3408:
		if (_field157C == 0) {
			R2_GLOBALS._sound2.fadeOut2(NULL);
			_field157C = 1;
		} else {
			_sceneMode = 3308;
			_stripManager.start(3308, this);
		}
		break;
	case 3308:
		warning("STUB: sub_1D227()");
		_companion1.setStrip(2);
		R2_GLOBALS._player.setStrip(6);
		_companion2.setStrip(6);
		_webbster.setStrip(3);
		_teal.setStrip(1);
		_sceneMode = 3403;
		if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
			setAction(&_sequenceManager, this, 3403, &R2_GLOBALS._player, &_webbster, &_actor7, NULL);
		else
			setAction(&_sequenceManager, this, 3403, &_companion1, &_webbster, &_actor7, NULL);
		break;
	case 3309:
		warning("STUB: sub_1D227()");
		_teal.setStrip(1);
		_sceneMode = 3405;
		if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
			setAction(&_sequenceManager, this, 3405, &R2_GLOBALS._player, &_actor7, NULL);
		else
			setAction(&_sequenceManager, this, 3405, &_companion2, &_actor7, NULL);
		break;
	case 3310:
		warning("STUB: sub_1D227()");
		_teal.setStrip(1);
		_sceneMode = 3406;
		if (R2_GLOBALS._player._characterIndex == R2_QUINN)
			setAction(&_sequenceManager, this, 3406, &R2_GLOBALS._player, &_actor7, NULL);
		else if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
			setAction(&_sequenceManager, this, 3406, &_companion1, &_actor7, NULL);
		else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
			setAction(&_sequenceManager, this, 3406, &_companion2, &_actor7, NULL);
		break;
	case 3311:
		warning("STUB: sub_1D227()");
		_tealSpeaker._object1.hide();
		_teal.show();
		_teal.setStrip(1);
		_sceneMode = 3407;
		setAction(&_sequenceManager, this, 3407, &_teal, &_actor7, NULL);
		break;
	case 3400: {
		_actor8.postInit();
		_actor8.hide();
		_teal.postInit();
		_teal._numFrames = 7;
		_teal._moveDiff = Common::Point(3, 2);
		_teal.changeZoom(-1);
		_teal._effect = 1;
		_teal.setPosition(Common::Point(-15, 90));
		_teal.setup(3402, 1, 1);
		_teal.animate(ANIM_MODE_1, NULL);
		Common::Point pt1(115, 90);
		NpcMover *mover1 = new NpcMover();
		_teal.addMover(mover1, &pt1, this);
		R2_GLOBALS._scrollFollower = &_teal;
		Common::Point pt2(203, 76);
		NpcMover *mover2 = new NpcMover();
		_webbster.addMover(mover2, &pt2, NULL);
		_sceneMode = 3401;
		}
		break;
	case 3401:
		_sceneMode = 3305;
		_stripManager.start(3305, this);
		break;
	case 3402:
		_sceneMode = 3306;
		_stripManager.start(3306, this);
		break;
	case 3403:
		R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
		_sceneMode = 3309;
		_stripManager.start(3309, this);
		break;
	case 3405:
		_sceneMode = 3310;
		_stripManager.start(3310, this);
		break;
	case 3406:
		_sceneMode = 3311;
		_stripManager.start(3311, this);
		break;
	case 3407:
		R2_GLOBALS._sceneManager.changeScene(3600);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3500 - Cavern Maze
 *
 *--------------------------------------------------------------------------*/

Scene3500::Action1::Action1() {
	_field1E = 0;
	_field20 = 0;
	_field22 = 0;
	_field24 = 0;
}

void Scene3500::Action1::synchronize(Serializer &s) {
	Action::synchronize(s);

	s.syncAsSint16LE(_field1E);
	s.syncAsSint16LE(_field20);
	s.syncAsSint16LE(_field22);
	s.syncAsSint16LE(_field24);
}

void Scene3500::Action1::sub108670(int arg1) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	_field1E = arg1;
	_field20 = 1;
	_field24 = 1;

	scene->_actor9.setStrip(2);
	scene->_actor9.show();

	if (_field1E == 1)
		scene->_actor6.show();
	else
		scene->_actor5.show();

	if (scene->_actor1._frame % 2 == 0) {
		scene->_actor1._frameChange = _field1E;
		scene->_actor1.setFrame(scene->_actor1.changeFrame());
	}

	setActionIndex(0);
}

void Scene3500::Action1::sub108732(int arg1) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	_field20 = arg1;
	_field1E = -_field1E;

	if (_field1E == 1) {
		scene->_actor6.show();
		scene->_actor5.hide();
	} else {
		scene->_actor5.show();
		scene->_actor6.hide();
	}

	switch (_actionIndex) {
	case 4:
		scene->_actor1._frameChange = _field1E;
		scene->_actor1.setFrame(scene->_actor1.changeFrame());
	// No break on purpose
	case 3:
		_actionIndex = 10;
		setDelay(0);
		break;
	case 5: {
		scene->_fieldAF8 = 160;
		Common::Point pt(160, 73);
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, NULL);

		scene->_fieldB9E = 160 - (_field1E * 2 * 160);
		Common::Point pt2(scene->_fieldB9E, 73);
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, this);

		_actionIndex = 11;
		}
		break;
	case 6:
		scene->_actor1._frameChange = _field1E;
		scene->_actor1.setFrame(scene->_actor1.changeFrame());
		setDelay(1);
	// No break on purpose
	case 8:
		scene->_actor9.setStrip(2);
		_actionIndex = 1;
		break;
	default:
		break;
	}
}

void Scene3500::Action1::signal() {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	switch(_actionIndex++) {
	case 0:
		R2_GLOBALS._player.disableControl();
		scene->_field1286 = 0;
		if (scene->_field1270 != 0) {
			scene->_field1270 = 0;
			scene->_mazeChangeAmount = 0;
			scene->_field1272 = 0;
			scene->_rotation->_idxChange = 0;
		}
		break;
	case 1:
		if ((scene->_actor1._frame % 2) == 1) {
			setDelay(1);
			return;
		}
	// No break on purpose
	case 3:
		scene->_actor1._frameChange = _field1E;
		scene->_actor1.setFrame(scene->_actor1.changeFrame());
		setDelay(1);
		break;
	case 4: {
		int si = scene->_mazeUI.getCellFromMapXY(Common::Point(scene->_mazePosition.x + 70, scene->_mazePosition.y + 46));
		int var2 = scene->_mazeUI.cellFromX(scene->_mazePosition.x + 70) - 70;
		int var4 = scene->_mazeUI.cellFromY(scene->_mazePosition.y + 46) - 46;
		int di = abs(var2 - scene->_mazePosition.x);
		int var6 = abs(var4 - scene->_mazePosition.y);

		if ((scene->_actor1._frame % 2) != 0) {
			scene->_actor1._frameChange = _field1E;
			scene->_actor1.setFrame(scene->_actor1.changeFrame());
		}

		// Get the new direction starting on
		int var8 = (scene->_action1._field1E * 2 + scene->_mazeDirection);
		if (var8 > MAZEDIR_NORTHWEST)
			var8 = MAZEDIR_NORTH;
		else if (var8 < MAZEDIR_NORTH)
			var8 = MAZEDIR_WEST;

		// Check whether movement is allowed in that direction. If so, then
		// movement is started again
		switch (var8) {
		case MAZEDIR_NORTH:
			if ( ((si != 2)  && (si != 3)  && (si != 6) && (si != 1) && (si != 23) && (si != 24) && (si != 4) && (si != 11))
				|| (var6 != 0)) {
				if ((si != 25) && (si != 26) && (si != 5) && (si != 14) && (si != 15))
					_field20 = 0;
				else if ((var6 != 0) || (di <= 3)) // useless, skipped: "|| (di == 0)"
					_field20 = 0;
				else
					_field20 = 1;
			} else
				_field20 = 1;
			break;
		case MAZEDIR_EAST:
			if ( ((si != 12)  && (si != 13)  && (si != 11) && (si != 16) && (si != 26) && (si != 24) && (si != 15) && (si != 6) && (si != 31))
				|| (di != 0)) {
				if ((si != 25) && (si != 23) && (si != 14) && (si != 5) && (si != 4))
					_field20 = 0;
				else if ((di != 0) || (var6 <= 3)) // useless, skipped: "|| (var6 == 0)"
					_field20 = 0;
				else
					_field20 = 1;
			} else
				_field20 = 1;
			break;
		case MAZEDIR_SOUTH:
			if ( ((si != 2)  && (si != 3)  && (si != 6) && (si != 1) && (si != 25) && (si != 26) && (si != 5) && (si != 16) && (si != 31))
				|| (var6 != 0)) {
					if ((si != 23) && (si != 24) && (si != 4) && (si != 14) && (si != 15))
						_field20 = 0;
					else if ((var6 != 0) || (di <= 3)) // useless, skipped: "|| (di == 0)"
						_field20 = 0;
					else
						_field20 = 1;
			} else
				_field20 = 1;
			break;
		case MAZEDIR_WEST:
			if ( ((si != 12)  && (si != 13)  && (si != 11) && (si != 16) && (si != 25) && (si != 23) && (si != 14) && (si != 1) && (si != 31))
				|| (var6 != 0)) {
					if ((si != 26) && (si != 24) && (si != 15) && (si != 5) && (si != 4))
						_field20 = 0;
					else if ((var6 <= 0) || (di != 0)) // useless, skipped: "|| (var6 == 0)"
						_field20 = 0;
					else
						_field20 = 1;
			} else
				_field20 = 1;
		default:
			break;
		}
		}
	// No break on purpose
	case 2: {
		scene->_actor8.setPosition(Common::Point(160, 73));
		scene->_actor8._moveDiff.x = 160 - scene->_mazeChangeAmount;
		scene->_fieldAF8 = 160 - ((_field1E * 2) * 160);
		Common::Point pt(scene->_fieldAF8, 73);
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, this);

		scene->_actor9.setPosition(Common::Point(160 + ((_field1E * 2) * 160), 73));
		scene->_actor9._moveDiff.x = 160 - scene->_mazeChangeAmount;
		scene->_fieldB9E = 160;
		Common::Point pt2(scene->_fieldB9E, 73);
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, NULL);
		}
		break;
	case 5:
		scene->_actor1._frameChange = _field1E;
		scene->_mazeDirection = scene->_actor1.changeFrame();
		scene->_actor1.setFrame(scene->_mazeDirection);
		setDelay(1);
		break;
	case 6:
		scene->_actor8.setPosition(Common::Point(160, 73));
		if (_field20 == 0)
			scene->_actor8.setStrip(1);
		else
			scene->_actor8.setStrip(2);
		scene->_actor8.fixPriority(1);

		scene->_actor9.setPosition(Common::Point(-160, 73));
		scene->_actor9.setStrip(9);
		scene->_actor9.fixPriority(11);
		scene->_actor9.hide();
		setDelay(1);
		break;
	case 7:
		if ((scene->_actor1._frame % 2) == 0) {
			scene->_actor1._frameChange = _field1E;
			scene->_mazeDirection = scene->_actor1.changeFrame();
			scene->_actor1.setFrame(scene->_mazeDirection);
		}
		setDelay(1);
		break;
	case 8: {
		R2_GLOBALS._player.enableControl();
		R2_GLOBALS._player._canWalk = false;
		scene->_field1286 = 1;
		if ((scene->_actor1._frame % 2) == 0) {
			scene->_actor1._frameChange = _field1E;
			scene->_actor1.setFrame(scene->_actor1.changeFrame());
		}
		// All the var_8 initialization was missing in the original
		// but it's clearly a cut and paste error from case 4.
		// The following code allows the switch to work properly.
		warning("Checkme: fix for dead code");
		int var_8 = (_field1E * 2 + scene->_mazeDirection);
		if (var_8 > 7)
			var_8 = 1;
		else if (var_8 < 1)
			var_8 = 7;
		//

		switch (var_8 - 1) {
		case 0:
		// No break on purpose
		case 4:
			scene->_mazePosition.x = scene->_mazeUI.cellFromX(scene->_mazePosition.x + 70) - 70;
			break;
		case 2:
		// No break on purpose
		case 6:
			scene->_mazePosition.y = scene->_mazeUI.cellFromY(scene->_mazePosition.y + 46) - 46;
			break;
		default:
			break;
		}
		scene->_actor5.hide();
		scene->_actor6.hide();
		_field24 = 0;
		if (_field20 == 0) {
			scene->_actor7.sub1094ED();
			if (scene->_mazeChangeAmount == scene->_field1270)
				scene->_aSound1.play(276);
		}
		break;
		}
	case 10: {
		scene->_fieldAF8 = 160;
		Common::Point pt(160, 73);
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, NULL);

		scene->_fieldB9E = 160 - (_field1E * 2 * 160);
		Common::Point pt2(scene->_fieldB9E, 73);
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, this);
		_actionIndex = 6;
		}
		break;
	case 11: {
		scene->_actor8.setStrip(2);
		scene->_actor8.setPosition(Common::Point(160, 73));
		scene->_fieldAF8 = 160 - (_field1E * 2 * 160);
		Common::Point pt(scene->_fieldAF8, 73);
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, NULL);
		scene->_actor8.fixPriority(11);
		if (_field20 == 0)
			scene->_actor9.setStrip(1);
		else
			scene->_actor9.setStrip(2);
		scene->_actor9.setPosition(Common::Point(160 - (_field1E * 2 * 160), 73));
		scene->_fieldB9E = 160;
		Common::Point pt2(scene->_fieldB9E, 73);
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, this);
		scene->_actor9.fixPriority(1);
		_actionIndex = 5;
		}
		break;
	default:
		break;
	}
}

void Scene3500::Action1::dispatch() {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	Action::dispatch();
	if ((_actionIndex == 1) && (scene->_mazeChangeAmount <= 4)) {
		scene->_rotation->_idxChange = 0;
		signal();
	}
}

/*--------------------------------------------------------------------------*/

Scene3500::Action2::Action2() {
	_field1E = 0;
}

void Scene3500::Action2::synchronize(Serializer &s) {
	Action::synchronize(s);

	s.syncAsSint16LE(_field1E);
}

/*--------------------------------------------------------------------------*/

Scene3500::Item4::Item4() {
	_field34 = 0;
}

void Scene3500::Item4::synchronize(Serializer &s) {
	NamedHotspot::synchronize(s);

	s.syncAsSint16LE(_field34);
}

/*--------------------------------------------------------------------------*/

Scene3500::Actor7::Actor7() {
	_fieldA4 = 0;
	_fieldA6 = 0;
	_fieldA8 = 0;
	_fieldAA = 0;
	_fieldAC = 0;
	_fieldAE = 0;
}

void Scene3500::Actor7::synchronize(Serializer &s) {
	SceneActor::synchronize(s);

	s.syncAsSint16LE(_fieldA4);
	s.syncAsSint16LE(_fieldA6);
	s.syncAsSint16LE(_fieldA8);
	s.syncAsSint16LE(_fieldAA);
	s.syncAsSint16LE(_fieldAC);
	s.syncAsSint16LE(_fieldAE);
}

void Scene3500::Actor7::sub109466(int arg1, int arg2, int arg3, int arg4, int arg5) {
	_fieldAE = 0;
	_fieldA4 = arg1;
	_fieldA6 = arg2;
	_fieldA8 = arg3;
	_fieldAA = arg4;
	_fieldAC = _fieldAA / _fieldA8;

	postInit();
	setup(1050, 3, 1);
	fixPriority(255);
	sub109663(arg5);
}

void Scene3500::Actor7::sub1094ED() {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	scene->_field1270 = _position.x - _fieldA4;
}

void Scene3500::Actor7::sub109663(int arg1){
	sub109693(Common::Point(_fieldA4 + arg1, _fieldA6 - (_fieldAC * arg1)));
}

void Scene3500::Actor7::sub109693(Common::Point Pt) {
	setPosition(Pt);
}

/*--------------------------------------------------------------------------*/

int Scene3500::MazeUI3500::cellFromX(int x) {
	return (_cellSize.x / 2) + x - (x % _cellSize.x);
}

int Scene3500::MazeUI3500::cellFromY(int y) {
	return (_cellSize.y / 2) + y - (y % _cellSize.y) - 1;
}

int Scene3500::MazeUI3500::getCellFromMapXY(Common::Point pt) {
	int cellX = pt.x / _cellSize.x;
	int cellY = pt.y / _cellSize.y;

	if ((cellX >= 0) && (cellY >= 0) && (cellX < _mapCells.x) && (cellY < _mapCells.y)) {
		return (int16)READ_LE_UINT16(_mapData + (_mapCells.x * cellY + cellX) * 2);
	} else
		return -1;
}

bool Scene3500::MazeUI3500::setMazePosition2(Common::Point &p) {
	bool retVal = setMazePosition(p);
	p = _mapOffset;

	return retVal;
}

Scene3500::Scene3500() {
	_fieldAF8 = 0;
	_fieldB9E = 0;
	_rotation = NULL;
	_mazeChangeAmount = 0;
	_field1270 = 0;
	_field1272 = 0;
	_field1274 = 0;
	_mazeDirection = MAZEDIR_NONE;
	_field1278 = 0;
	_mazePosition.x = 0;
	_mazePosition.y = 0;
	_field127E = 0;
	_field1280 = 0;
	_field1282 = 0;
	_field1284 = 0;
	_field1286 = 0;
}

void Scene3500::synchronize(Serializer &s) {
	SceneExt::synchronize(s);
	SYNC_POINTER(_rotation);

	s.syncAsSint16LE(_fieldAF8);
	s.syncAsSint16LE(_fieldB9E);
	s.syncAsSint16LE(_mazeChangeAmount);
	s.syncAsSint16LE(_field1270);
	s.syncAsSint16LE(_field1272);
	s.syncAsSint16LE(_field1274);
	s.syncAsSint16LE(_mazeDirection);
	s.syncAsSint16LE(_field1278);
	s.syncAsSint16LE(_mazePosition.x);
	s.syncAsSint16LE(_mazePosition.y);
	s.syncAsSint16LE(_field127E);
	s.syncAsSint16LE(_field1280);
	s.syncAsSint16LE(_field1282);
	s.syncAsSint16LE(_field1284);
	s.syncAsSint16LE(_field1286);
}

void Scene3500::sub107F71(int arg1) {
	switch (arg1) {
	case -1:
		_actor7.sub1094ED();
		if (_field1270 != 0) {
			_field1270--;
			_actor7.sub109663(_field1270);
		}
		if (_action1._field24 != 0)
			_field1270 = 0;
		break;
	case 1:
		_actor7.sub1094ED();
		if (_field1270 < 16) {
			++_field1270;
			_actor7.sub109663(_field1270);
		}
		if (_action1._field24 != 0)
			_field1270 = 0;
		break;
	case 88:
		if ((_action == 0) || (_action1._field24 == 0)) {
		// The original makes a second useless check on action, skipped
			_action2.sub10831F(2);
			if ((_action) && ((_action2.getActionIndex() != 0) || (_action2._field1E != 2))) {
				_action2.signal();
			} else {
				_actor9.setAction(&_action2, &_actor9, NULL);
			}
		}
		break;
	case 96:
		if ((_action) && (_action1._field24 != 0) && (_action2._field1E != 1)) {
			_field1278 = 0;
			_action1.sub108732(0);
		} else if ((_action) && (_field1278 == 0) && (_action1._field24 != 0)) {
			_field1278 = arg1;
		} else if ((_action) && (_action1._field24 == 0)) {
			_action1.sub108670(1);
			_action1.signal();
		} else if (_action == 0) {
			_action1.sub108670(1);
			setAction(&_action1, &_actor1, NULL);
		}
		break;
	case 104:
		if ((_action == 0) || (_action1._field24 == 0)) {
			_action2.sub10831F(-1);
			if ((_action) && ((_action2.getActionIndex() != 0) || (_action2._field1E != -1))) {
				_action2.signal();
			} else {
				_actor9.setAction(&_action2, &_actor9, NULL);
			}
		}
		break;
	case 112:
		if ((_action) && (_action1._field24 != 0) && (_action2._field1E != -1)) {
			_field1278 = 0;
			_action1.sub108732(0);
		} else if ((_action) && (_field1278 == 0) && (_action1._field24 != 0)) {
			_field1278 = arg1;
		} else if ((_action) && (_action1._field24 == 0)) {
			_action1.sub108670(-1);
			_action1.signal();
		} else if (_action == 0) {
			_action1.sub108670(-1);
			setAction(&_action1, &_actor1, NULL);
		}
		break;
	default:
		_field1270 = arg1;
		_actor7.sub109663(arg1);
		if (_action1._field24 != 0) {
			_field1270 = 0;
		}
		break;
	}
}

void Scene3500::Action2::sub10831F(int arg1) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	_field1E = arg1;
	if (_field1E == -1)
		scene->_actor3.setFrame2(3);
	else
		scene->_actor3.setFrame2(1);

	setActionIndex(0);
}

void Scene3500::Action2::signal() {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	int si;
	int di;

	switch (_actionIndex++) {
	case 0: {
		if (scene->_actor8._mover) {
			si = scene->_fieldAF8;
			di = scene->_fieldB9E;
		} else {
			scene->_fieldAF8 = scene->_actor8._position.x;
			si = scene->_fieldAF8;
			scene->_fieldB9E = scene->_actor9._position.y;
			di = scene->_fieldB9E;
		}

		scene->_actor8._moveDiff.y = 9 - (scene->_mazeChangeAmount / 2);
		Common::Point pt(si, 73 - (_field1E * 12));
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, NULL);

		scene->_actor9._moveDiff.y = 9 - (scene->_mazeChangeAmount / 2);
		Common::Point pt2(di, 73 - (_field1E * 12));
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, NULL);
		scene->_mazeChangeAmount = (scene->_mazeChangeAmount / 2) + (scene->_mazeChangeAmount % 2);
		setDelay(17 - scene->_mazeChangeAmount);
		}
		break;
	case 1: {
		R2_GLOBALS._sound2.play(339);
		if (scene->_actor8._mover) {
			si = scene->_fieldAF8;
			di = scene->_fieldB9E;
		} else {
			si = scene->_actor8._position.x;
			di = scene->_actor9._position.x;
		}

		scene->_actor7.sub1094ED();

		scene->_actor8._moveDiff.y = 9 - (scene->_mazeChangeAmount / 2);
		Common::Point pt(si, 73);
		NpcMover *mover = new NpcMover();
		scene->_actor8.addMover(mover, &pt, NULL);

		scene->_actor9._moveDiff.y = 9 - (scene->_mazeChangeAmount / 2);
		Common::Point pt2(di, 73);
		NpcMover *mover2 = new NpcMover();
		scene->_actor9.addMover(mover2, &pt2, NULL);

		scene->_actor3.setFrame2(2);
		}
		break;
	default:
		break;
	}
}

bool Scene3500::Item4::startAction(CursorType action, Event &event) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	if (scene->_field1286 == 0)
		return true;

	if (scene->_field1286 != 4)
		return SceneHotspot::startAction(action, event);

	R2_GLOBALS._sound2.play(14);
	scene->sub107F71(_field34);

	return true;
}

void Scene3500::Actor7::process(Event &event) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	if (scene->_field1286 == 0)
		return;

	if ((event.eventType == EVENT_BUTTON_DOWN) && (R2_GLOBALS._events.getCursor() == CURSOR_USE) && (_bounds.contains(event.mousePos))) {
		_fieldAE = 1 + event.mousePos.y - _position.y;
		event.eventType = EVENT_NONE;
	}

	if ((event.eventType == EVENT_BUTTON_UP) && (_fieldAE != 0)) {
		_fieldAE = 0;
		event.handled = true;
		if (scene->_action1._field24 == 0)
			sub1094ED();
	}

	if (_fieldAE == 0)
		return;

	R2_GLOBALS._sound2.play(338);
	event.handled = true;

	int cx = event.mousePos.y - _fieldAE + 1;
	if (_fieldA6 >= cx) {
		if (_fieldA6 - _fieldAA <= cx)
			sub109693(Common::Point(((_fieldA6 - cx) / 2) + _fieldA4 + ((_fieldA6 - cx) % 2), cx));
		else
			sub109693(Common::Point(_fieldA4 + _fieldA8, _fieldA6 - _fieldAA));
	} else {
		sub109693(Common::Point(_fieldA4, _fieldA6));
	}
}

bool Scene3500::Actor7::startAction(CursorType action, Event &event) {
	Scene3500 *scene = (Scene3500 *)R2_GLOBALS._sceneManager._scene;

	if (scene->_field1286 == 0)
		return true;

	if (scene->_field1286 == 4)
		return false;

	return SceneActor::startAction(action, event);
}

void Scene3500::postInit(SceneObjectList *OwnerList) {
	byte tmpPal[768];

	loadScene(1050);
	R2_GLOBALS._uiElements._active = false;
	R2_GLOBALS._interfaceY = SCREEN_HEIGHT;

	R2_GLOBALS._v5589E.set(0, 0, 320, 200);
	R2_GLOBALS._sound1.play(305);
	R2_GLOBALS._player._characterIndex = R2_QUINN;
	R2_GLOBALS._player._characterScene[R2_QUINN] = 3500;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3500;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3500;
	_field1284 = 0;
	_field1282 = 0;
	_field1278 = 0;
	_field1272 = 1;
	_field1270 = 4;
	_mazeChangeAmount = 4;
	_mazePosition = Common::Point(860, 891);
	_rotation = R2_GLOBALS._scenePalette.addRotation(240, 254, -1);
	_rotation->setDelay(0);
	_rotation->_idxChange = 1;

	for (int i = 240; i <= 254; i++) {
		int tmpIndex = _rotation->_currIndex - 240;

		if (tmpIndex > 254)
			tmpIndex--;

		tmpPal[3 * i] = R2_GLOBALS._scenePalette._palette[3 * tmpIndex];
		tmpPal[(3 * i) + 1] = R2_GLOBALS._scenePalette._palette[(3 * tmpIndex) + 1];
		tmpPal[(3 * i) + 2] = R2_GLOBALS._scenePalette._palette[(3 * tmpIndex) + 2];
	}

	for (int i = 240; i <= 254; i++) {
		R2_GLOBALS._scenePalette._palette[3 * i] = tmpPal[3 * i];
		R2_GLOBALS._scenePalette._palette[(3 * i) + 1] = tmpPal[(3 * i) + 1];
		R2_GLOBALS._scenePalette._palette[(3 * i) + 2] = tmpPal[(3 * i) + 2];
	}

	_actor7.sub109466(38, 165, 16, 32, _field1270);
	_actor7.setDetails(3500, 6, 7, -1, 1, (SceneItem *)NULL);
	R2_GLOBALS._sound1.play(276);

	_item4._field34 = 88;
	_item4.setDetails(88, 3500, 18, 10, -1);

	_item5._field34 = 112;
	_item5.setDetails(112, 3500, 9, 10, -1);

	_item6._field34 = 104;
	_item6.setDetails(104, 3500, 15, 10, -1);

	_item7._field34 = 96;
	_item7.setDetails(96, 3500, 12, 10, -1);

	_actor8.postInit();
	_actor8.setup(1050, 1, 1);
	_actor8.setPosition(Common::Point(160, 73));
	_actor8.fixPriority(1);

	_actor9.postInit();
	_actor9.setup(1050, 2, 1);
	_actor9.setPosition(Common::Point(-160, 73));
	_actor9.fixPriority(11);
	_actor9.hide();

	_item2.setDetails(27, 3500, 21, -1, -1);
	_item3.setDetails(Rect(160, 89, 299, 182), 3500, 3, -1, -1, 1, NULL);
	_item1.setDetails(Rect(0, 0, 320, 200), 3500, 0, -1, 2, 1, NULL);

	_actor1.postInit();
	_mazeDirection = MAZEDIR_NORTH;
	_actor1.setup(1004, 1, _mazeDirection);
	_actor1.setPosition(Common::Point(230, 135));
	_actor1.fixPriority(200);
	_actor1._frameChange = 1;

	_actor5.postInit();
	_actor5.setup(1004, 3, 1);
	_actor5.setPosition(Common::Point(117, 163));
	_actor5.fixPriority(200);
	_actor5.hide();

	_actor4.postInit();
	_actor4.setup(1004, 3, 2);
	_actor4.setPosition(Common::Point(126, 163));
	_actor4.fixPriority(200);

	_actor6.postInit();
	_actor6.setup(1004, 3, 3);
	_actor6.setPosition(Common::Point(135, 163));
	_actor6.fixPriority(200);
	_actor6.hide();

	_actor2.postInit();
	_actor2.setup(1004, 4, _mazeChangeAmount + 1);
	_actor2.setPosition(Common::Point(126, 137));
	_actor2.fixPriority(200);

	_actor3.postInit();
	_actor3.setup(1004, 5, 2);
	_actor3.setPosition(Common::Point(126, 108));
	_actor3.fixPriority(200);

	_mazeUI.setDisplayBounds(Rect(160, 89, 299, 182));
	_mazeUI.load(2);
	_mazeUI.setMazePosition(_mazePosition);

	_action1._field24 = 0;
	_mazeUI.draw();
	_field1286 = 1;

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.hide();
	R2_GLOBALS._player.enableControl(CURSOR_USE);
	R2_GLOBALS._player._uiEnabled = false;
	R2_GLOBALS._player._canWalk = false;
}

void Scene3500::remove() {
	R2_GLOBALS._sound2.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3500::signal() {
	R2_GLOBALS._player.enableControl(CURSOR_USE);
	R2_GLOBALS._player._canWalk = false;
	_field1286 = 1;
}

void Scene3500::process(Event &event) {
	if (_field1286 == 0)
		return;

	if (event.eventType == EVENT_KEYPRESS) {
		switch (event.kbd.keycode) {
		case Common::KEYCODE_KP7:
			R2_GLOBALS._sound2.play(338);
			sub107F71(16);
			event.handled = true;
			break;
		case Common::KEYCODE_UP:
		case Common::KEYCODE_KP8:
			R2_GLOBALS._sound2.play(14, NULL, 63);
			sub107F71(88);
			event.handled = true;
			break;
		case Common::KEYCODE_KP9:
			if (_field1270 < 16)
				R2_GLOBALS._sound2.play(338);
			sub107F71(1);
			event.handled = true;
			break;
		case Common::KEYCODE_KP4:
		case Common::KEYCODE_LEFT:
			R2_GLOBALS._sound2.play(14, NULL, 63);
			sub107F71(112);
			event.handled = true;
			break;
		case Common::KEYCODE_KP6:
		case Common::KEYCODE_RIGHT:
			R2_GLOBALS._sound2.play(14, NULL, 63);
			sub107F71(96);
			event.handled = true;
			break;
		case Common::KEYCODE_KP1:
			R2_GLOBALS._sound2.play(338);
			sub107F71(0);
			event.handled = true;
			break;
		case Common::KEYCODE_KP2:
		case Common::KEYCODE_DOWN:
			R2_GLOBALS._sound2.play(14, NULL, 63);
			sub107F71(104);
			event.handled = true;
			break;
		case Common::KEYCODE_KP3:
			if (_field1270 != 0)
				R2_GLOBALS._sound2.play(338);
			sub107F71(-1);
			event.handled = true;
			break;
		case Common::KEYCODE_KP0:
			R2_GLOBALS._sound2.play(338);
			sub107F71(8);
			event.handled = true;
			break;
		case Common::KEYCODE_KP_PERIOD:
			R2_GLOBALS._sound2.play(338);
			sub107F71(4);
			event.handled = true;
			break;
		default:
			break;
		}
	}

	if (!event.handled)
		_actor7.process(event);

	if (!event.handled)
		_item4.process(event);

	if (!event.handled)
		_item5.process(event);

	if (!event.handled)
		_item6.process(event);

	if (!event.handled)
		_item7.process(event);

	Scene::process(event);
}

void Scene3500::dispatch() {
	Rect tmpRect;
	Scene::dispatch();

	if (((_actor1._frame % 2) == 0) && (_action1._field24 == 0)) {
		_actor1.setFrame(_actor1.changeFrame());
		_mazeDirection = _actor1._frame;
	}
	int oldField1278;
	if ((_field1278 != 0) && (_action1._field24 == 0)) {
		oldField1278 = _field1278;
		_field1278 = 0;
		sub107F71(oldField1278);
	}

	if (!_rotation)
		return;

	int newMazeX = 0;
	int newMazeY = 0;
	int var_4 = 0;
	int var_6 = 0;
	int var_8 = 0;
	int var_a = 0;
	int dx = 0;
	int cellId = 0;

	if ((_mazeChangeAmount == 0) && (_field1282 == 0)) {
		if (_field1284 == 2)
			R2_GLOBALS._sceneManager.changeScene(1000);
	} else {
		_field1282 = 0;
		tmpRect.set(160, 89, 299, 182);

		newMazeX = _mazePosition.x;
		newMazeY = _mazePosition.y;
		var_4 = _mazeUI.cellFromX(newMazeX + 70) - 70;
		var_6 = _mazeUI.cellFromY(_mazePosition.y + 46) - 46;
		var_8 = abs(var_4 - newMazeX);
		var_a = abs(var_6 - newMazeY);
		dx = 0;

		switch (_mazeDirection) {
		case MAZEDIR_NORTH:
			cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
			if (((cellId == 2) || (cellId == 3) || (cellId == 6) || (cellId == 1)) ||
					((cellId == 25 || cellId == 26 || cellId == 5 || cellId == 14 || cellId == 15) && var_8 > 3)) {
				R2_GLOBALS._sound2.play(339);
				_rotation->_idxChange = 0;
				_field1270 = 0;
				_mazeChangeAmount = 0;
				_field1272 = 0;
				if (_action1._field24 == 0)
					_actor8.hide();
			} else {
				var_6 = _mazeUI.cellFromY(newMazeY + 46) - 46;
				newMazeY = _mazePosition.y - _mazeChangeAmount;
				dx = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
				if (((cellId == 23) || (cellId == 24) || (cellId == 4)) && (cellId != dx)) {
					newMazeY = var_6;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else if ((cellId == 11) && (cellId != dx)) {
					newMazeY = var_6 + 3;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else {
					var_6 = _mazeUI.cellFromY(newMazeY + 46) - 46;
					var_a = abs(var_6 - newMazeY);
					cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));

					if ( (((cellId == 23) || (cellId == 24) || (cellId == 4)) && (newMazeY <= var_6) && (_mazePosition.y>= var_6))
						|| (((cellId == 25) || (cellId == 26) || (cellId == 5) || (cellId == 14) || (cellId == 15)) && (_mazeChangeAmount >= var_a) && (_mazeChangeAmount > 3) && (_action1._field24 != 0)) ) {
						newMazeY = var_6;
						if ((cellId != 25) && (cellId != 26) && (cellId != 5) && (cellId != 14) && (cellId == 15))
							R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if ((cellId == 11) && (var_6 + 3 >= newMazeY) && (_mazePosition.y >= var_6 + 3)) {
						R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if (((cellId == 25) || (cellId == 26) || (cellId == 5) || (cellId == 14) || (cellId == 15)) && (var_8 != 0) && (var_8 <= 3)) {
						newMazeX = var_4;
						R2_GLOBALS._sound2.play(339);
					} else {
						// Nothing
					}
				}
			}
			break;
		case MAZEDIR_EAST:
			cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
			if (  ((cellId == 12) || (cellId == 13) || (cellId == 11) || (cellId == 16) || (cellId == 31))
			  || (((cellId == 25) || (cellId == 23) || (cellId == 14) || (cellId == 5) || (cellId == 4)) && (var_a > 3)) ) {
				R2_GLOBALS._sound2.play(339);
				_rotation->_idxChange = 0;
				_field1270 = 0;
				_mazeChangeAmount = 0;
				_field1272 = 0;
				if (_action1._field24 == 0)
					_actor8.hide();
			} else {
				var_4 = _mazeUI.cellFromX(newMazeX + 70) - 70;
				newMazeX = _mazePosition.x + _mazeChangeAmount;
				dx = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
				if (((cellId == 26) || (cellId == 24) || (cellId == 15)) && (cellId != dx)) {
					newMazeX = var_4;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else if ((cellId == 6) && (cellId != dx)) {
					newMazeX = var_4 - 5;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else {
					var_4 = _mazeUI.cellFromX(newMazeX + 70) - 70;
					var_8 = abs(newMazeX - var_4);
					cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
					if ( (((cellId == 26) || (cellId == 24) || (cellId == 15)) && (newMazeX >= var_4) && (_mazePosition.x <= var_4))
						|| (((cellId == 25) || (cellId == 23) || (cellId == 14) || (cellId == 5) || (cellId == 4)) && (_mazeChangeAmount >= var_8) && (_mazeChangeAmount <= 3) && (_action1._field24 != 0)) ) {
						newMazeX = var_4;
						if ((cellId == 25) || (cellId == 23) || (cellId == 14) || (cellId == 5) || (cellId == 4))
							R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if ((cellId == 6) && (var_4 - 5 <= newMazeX) && (_mazePosition.x <= var_4 - 5)) {
						newMazeX = var_4 - 5;
						R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if (((cellId == 25) || (cellId == 23) || (cellId == 14) || (cellId == 5) || (cellId == 4)) && (var_a != 0) && (var_a <= 3)) {
						newMazeY = var_6;
						R2_GLOBALS._sound2.play(339);
					} else {
						// Nothing
					}
				}
			}
			break;
		case MAZEDIR_SOUTH:
			cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
			if (  ((cellId == 2) || (cellId == 3) || (cellId == 6) || (cellId == 1))
			  || (((cellId == 23) || (cellId == 24) || (cellId == 4) || (cellId == 14) || (cellId == 15)) && (var_8 > 3)) ) {
				R2_GLOBALS._sound2.play(339);
				_rotation->_idxChange = 0;
				_field1270 = 0;
				_mazeChangeAmount = 0;
				_field1272 = 0;
				if (_action1._field24 == 0)
					_actor8.hide();
			} else {
				var_6 = _mazeUI.cellFromY(newMazeY + 46) - 46;
				newMazeY = _mazePosition.y + _mazeChangeAmount;
				dx = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));

				if (((cellId == 25) || (cellId == 26) || (cellId == 5)) && (cellId != dx)) {
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else if ((cellId == 16) && (cellId != dx)) {
					newMazeY = var_6 - 3;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else if ((cellId == 31) && (cellId != dx)) {
					newMazeY = var_6 + 4;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else {
					var_6 = _mazeUI.cellFromY(newMazeY + 46) - 46;
					var_a = abs(newMazeY - var_6);
					cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
					if ( (((cellId == 25) || (cellId == 26) || (cellId == 5)) && (newMazeY >= var_6) && (_mazePosition.y <= var_6))
					  || (((cellId == 23) || (cellId == 24) || (cellId == 4) || (cellId == 14) || (cellId == 15)) && (_mazeChangeAmount >= var_a) && (_mazeChangeAmount <= 3) && (_action1._field24 != 0)) ){
						newMazeY = var_6;

						if ((cellId != 23) && (cellId != 24) && (cellId != 4) && (cellId != 14) && (cellId != 15))
							R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if ((cellId == 16) && (var_6 - 3 <= newMazeY) && (_mazePosition.y <= var_6 - 3)) {
						newMazeY = var_6 - 3;
						R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if ((cellId == 31) && (var_6 + 4 <= newMazeY) && (_mazePosition.y <= var_6 + 4)) {
						newMazeY = var_6 + 4;
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
						if ((newMazeX == 660) && (_mazeChangeAmount + 306 <= newMazeY) && (newMazeY <= 307))
							 ++_field1284;
						else
							R2_GLOBALS._sound2.play(339);
					} else if (((cellId == 23) || (cellId == 24) || (cellId == 4) || (cellId == 14) || (cellId == 15)) && (var_8 != 0) && (var_8 <= 3)) {
						newMazeX = var_4;
						R2_GLOBALS._sound2.play(339);
					} else {
						// Nothing
					}
				}
			}
			break;
		case MAZEDIR_WEST:
			cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
			if ( ((cellId == 12) || (cellId == 13) || (cellId == 11) || (cellId == 16) || (cellId == 31))
			 || (((cellId == 26) || (cellId == 24) || (cellId == 15) || (cellId == 5) || (cellId == 4)) && (var_a > 3)) ) {
				R2_GLOBALS._sound2.play(339);
				_rotation->_idxChange = 0;
				_field1270 = 0;
				_mazeChangeAmount = 0;
				_field1272 = 0;
				if (_action1._field24 == 0)
					_actor8.hide();
			} else {
				var_4 = _mazeUI.cellFromX(newMazeX + 70) - 70;
				newMazeX = _mazePosition.x - _mazeChangeAmount;
				dx = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
				if (((cellId == 25) || (cellId == 23) || (cellId == 14)) && (cellId != dx)) {
					newMazeX = var_4;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else if ((cellId == 1) && (cellId != dx)) {
					newMazeX = var_4 + 5;
					R2_GLOBALS._sound2.play(339);
					_rotation->_idxChange = 0;
					_field1270 = 0;
					_mazeChangeAmount = 0;
					_field1272 = 0;
					if (_action1._field24 == 0)
						_actor8.hide();
				} else {
					var_4 = _mazeUI.cellFromX(newMazeX + 70) - 70;
					var_8 = abs(var_4 - newMazeX);
					cellId = _mazeUI.getCellFromMapXY(Common::Point(newMazeX + 70, newMazeY + 46));
					if ( (((cellId == 25) || (cellId == 23) || (cellId == 14)) && (newMazeX <= var_4) && (_mazePosition.x >= var_4))
					  || (((cellId == 26) || (cellId == 24) || (cellId == 15) || (cellId == 5) || (cellId == 4)) && (_mazeChangeAmount >= var_8) && (_mazeChangeAmount <= 3) && (_action1._field24 != 0)) ) {
						newMazeX = var_4;
						if ((cellId == 26) || (cellId == 24) || (cellId == 15) || (cellId == 5) || (cellId == 4))
							R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if ((cellId == 1) && (newMazeX >= var_4 + 5) && (_mazePosition.x >= var_4 + 5)) {
						newMazeX = var_4 + 5;
						R2_GLOBALS._sound2.play(339);
						_rotation->_idxChange = 0;
						_field1270 = 0;
						_mazeChangeAmount = 0;
						_field1272 = 0;
						if (_action1._field24 == 0)
							_actor8.hide();
					} else if (((cellId == 26) || (cellId == 24) || (cellId == 15) || (cellId == 5) || (cellId == 4)) && (var_a != 0) && (var_a <= 3)) {
						newMazeY = var_6;
						R2_GLOBALS._sound2.play(339);
					} else {
						// Nothing
					}
				}
			}
			break;
		default:
			break;
		}

		if (_field1284 < 2) {
			_mazePosition.x = newMazeX;
			_mazePosition.y = newMazeY;
			if (_mazeUI.setMazePosition2(_mazePosition) != 0) {
				_field1272 = 0;
				_mazeChangeAmount = 0;
				_field1270 = 0;
				_rotation->setDelay(0);
				_rotation->_idxChange = 0;
			}

			_mazeUI.draw();
			if (_field1284 != 0)
				++_field1284;
		}
	}

	if (_field1272 == 0) {
		if (_mazeChangeAmount != _field1270) {
			if (_mazeChangeAmount >= _field1270) {
				if (_mazeChangeAmount == 1) {
					if (_action1._field24 != 0) {
						if ( ((_mazeDirection == 1) && (var_8 == 0) && (var_a != 0) && (var_a <= 3) && ((cellId == 25) || (cellId == 26) || (cellId == 5) || (cellId == 14) || (cellId == 15)))
						  || ((_mazeDirection == 3) && (var_a == 0) && (var_8 != 0) && (var_8 <= 3) && ((cellId == 25) || (cellId == 23) || (cellId == 14) || (cellId == 5) || (cellId == 4)))
						  || ((_mazeDirection == 5) && (var_8 == 0) && (var_a != 0) && (var_a <= 3) && ((cellId == 23) || (cellId == 24) || (cellId == 4) || (cellId == 14) || (cellId == 15)))
						  || ((_mazeDirection == 7) && (var_a == 0) && (var_8 != 0) && (var_8 <= 3) && ((cellId == 26) || (cellId == 24) || (cellId == 15) || (cellId == 5) || (cellId == 4))) ){
							_mazeChangeAmount = 1;
						} else
							_mazeChangeAmount--;
					} else
						_mazeChangeAmount--;
				} else
					_mazeChangeAmount--;
			} else
				++_mazeChangeAmount;
			_field1272 = 1;
		}
		_actor2.setFrame2(_mazeChangeAmount + 1);
	}

	if (_field1272 == 1) {
		if (_mazeChangeAmount == 0)
			_rotation->_idxChange = 0;
		else if (_mazeChangeAmount > 8)
			_rotation->_idxChange = 2;
		else
			_rotation->_idxChange = 1;
	}

	if (_field1272 != 0)
		_field1272--;

	if (_mazeChangeAmount != 0) {
 		R2_GLOBALS._player._uiEnabled = false;
		if (_mazeChangeAmount != _field1270)
			_aSound1.play(276);
	} else {
		R2_GLOBALS._player._uiEnabled = true;
		_aSound1.fadeOut2(NULL);
	}

	if (_rotation->_currIndex != _field1274)
		_field1274 = _rotation->_currIndex;
}

/*--------------------------------------------------------------------------
 * Scene 3600 - Cutscene - walking at gunpoint
 *
 *--------------------------------------------------------------------------*/

Scene3600::Scene3600() {
	_field2548 = 0;
	_field254A = 0;
	_field254C = 0;
	_field254E = 0;
	_ghoulTeleported = false;
}
void Scene3600::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_field2548);
	s.syncAsSint16LE(_field254A);
	s.syncAsSint16LE(_field254C);
	s.syncAsSint16LE(_field254E);
	s.syncAsSint16LE(_ghoulTeleported);
}

Scene3600::Action3600::Action3600() {
	_field1E = 0;
	_field20 = 0;
}

void Scene3600::Action3600::synchronize(Serializer &s) {
	Action::synchronize(s);

	s.syncAsSint16LE(_field1E);
	s.syncAsSint16LE(_field20);
}

void Scene3600::Action3600::signal() {
	Scene3600 *scene = (Scene3600 *)R2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		setDelay(60);
		break;
	case 1:
		if (_field1E == 0) {
			_field1E = 1;
			scene->_actor2.setAction(NULL);
			R2_GLOBALS._sound2.play(330, NULL, 0);
			R2_GLOBALS._sound2.fade(127, 5, 10, false, NULL);
		}

		setDelay(1);
		R2_GLOBALS._scenePalette.fade((const byte *)&scene->_palette1._palette, true, _field20);
		if (_field20 > 0)
			_field20 -= 2;
		break;
	case 2:
		R2_GLOBALS._sound2.stop();
		++_actionIndex;
		setDelay(3);
		break;
	case 3:
		R2_GLOBALS._sound2.play(330, this, 0);
		R2_GLOBALS._sound2.fade(127, 5, 10, false, NULL);
		_actionIndex = 1;
		break;
	default:
		break;
	}
}

void Scene3600::Action2::signal() {
	Scene3600 *scene = (Scene3600 *)R2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 3621:
		R2_GLOBALS._events.proc1();
		R2_GLOBALS._player.enableControl();
		_actionIndex = 3619;
		scene->_protector._state = 0;
	// No break on purpose
	case 3619: {
		++_actionIndex;
		scene->_protector.setup(3127, 2, 1);
		scene->_protector.animate(ANIM_MODE_1, NULL);
		NpcMover *mover = new NpcMover();
		scene->_protector.addMover(mover, &scene->_protector._field8A, scene);
		}
		break;
	default:
		_actionIndex = 3619;
		setDelay(360);
		break;
	}
}

bool Scene3600::Item5::startAction(CursorType action, Event &event) {
	Scene3600 *scene = (Scene3600 *)R2_GLOBALS._sceneManager._scene;

	if ((action != CURSOR_USE) || (scene->_action1._field1E == 0))
		return SceneItem::startAction(action, event);

	R2_GLOBALS._walkRegions.enableRegion(2);
	R2_GLOBALS._walkRegions.enableRegion(7);

	R2_GLOBALS._player.disableControl();
	scene->_sceneMode = 3624;
	scene->_actor10.setStrip2(-1);
	scene->_actor11.setStrip2(-1);
	scene->_actor12.setStrip2(-1);
	scene->_actor4.setStrip2(-1);

	if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
		R2_GLOBALS._player.setAction(&scene->_sequenceManager3, scene, 3611, &R2_GLOBALS._player, NULL);
	else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
		R2_GLOBALS._player.setAction(&scene->_sequenceManager4, scene, 3612, &R2_GLOBALS._player, NULL);
	else
		R2_GLOBALS._player.setAction(&scene->_sequenceManager2, scene, 3610, &R2_GLOBALS._player, NULL);

	return true;
}

bool Scene3600::Protector::startAction(CursorType action, Event &event) {
	Scene3600 *scene = (Scene3600 *)R2_GLOBALS._sceneManager._scene;

	switch(action) {
	case CURSOR_TALK:
		if (_action)
			return SceneActor::startAction(action, event);

		scene->_protectorSpeaker._displayMode = 1;
		if (!R2_GLOBALS._player._mover)
			R2_GLOBALS._player.addMover(NULL);
		if (!scene->_actor10._mover)
			scene->_actor10.addMover(NULL);
		if (!scene->_actor11._mover)
			scene->_actor11.addMover(NULL);
		if (!scene->_actor12._mover)
			scene->_actor12.addMover(NULL);
		if (!scene->_actor4._mover)
			scene->_actor4.addMover(NULL);

		setup(3127, 2, 1);
		scene->_sceneMode = 3327;
		scene->_stripManager.start(3327, scene);

		return true;
	case R2_SONIC_STUNNER:
	// No break on purpose
	case R2_PHOTON_STUNNER:
		if (action == R2_SONIC_STUNNER)
			R2_GLOBALS._sound3.play(43);
		else
			R2_GLOBALS._sound3.play(99);

		if (_state != 0) {
			_state = 1;
			setup(3128, 1, 1);
			addMover(NULL);
		}
		scene->_action2.setActionIndex(3621);

		if (!_action)
			setAction(&scene->_action2, scene, NULL);

		animate(ANIM_MODE_5, &scene->_action2);
		R2_GLOBALS._player.disableControl();
		return true;
		break;
	default:
		return SceneActor::startAction(action, event);
		break;
	}
}

void Scene3600::postInit(SceneObjectList *OwnerList) {
	if (R2_GLOBALS._sceneManager._previousScene == 3600) {
		R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
		R2_GLOBALS._v558B6.set(60, 0, 260, 200);
	} else {
		R2_GLOBALS._scrollFollower = &_actor2;
		_sceneBounds = Rect(160, 0, 480, 200);
		R2_GLOBALS._v558B6.set(25, 0, 260, 200);
	}

	loadScene(3600);
	SceneExt::postInit();
	_field254C = 0;

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);
	_stripManager.addSpeaker(&_tealSpeaker);
	_stripManager.addSpeaker(&_protectorSpeaker);

	setZoomPercents(142, 80, 167, 105);
	R2_GLOBALS._player._characterScene[R2_QUINN] = 3600;
	R2_GLOBALS._player._characterScene[R2_SEEKER] = 3600;
	R2_GLOBALS._player._characterScene[R2_MIRANDA] = 3600;

	_item2.setDetails(33, 3600, 6, -1, -1);
	_item3.setDetails(Rect(3, 3, 22, 45), 3600, 9, -1, -1, 1, NULL);
	_item4.setDetails(Rect(449, 3, 475, 45), 3600, 9, -1, -1, 1, NULL);

	_actor10.postInit();
	_actor10._moveDiff = Common::Point(3, 2);
	_actor10.changeZoom(-1);
	_actor10._effect = 1;

	if (R2_GLOBALS._player._characterIndex != 1)
		_actor10.setDetails(9001, 0, -1, -1, 1, (SceneItem *) NULL);

	_actor11.postInit();
	_actor11._numFrames = 7;
	_actor11._moveDiff = Common::Point(5, 3);
	_actor11.changeZoom(-1);
	_actor11._effect = 1;

	if (R2_GLOBALS._player._characterIndex != 2)
		_actor11.setDetails(9002, 1, -1, -1, 1, (SceneItem *) NULL);

	_actor12.postInit();
	_actor12._moveDiff = Common::Point(3, 2);
	_actor12.changeZoom(-1);
	_actor12._effect = 1;

	if (R2_GLOBALS._player._characterIndex != 3)
		_actor12.setDetails(9003, 1, -1, -1, 1, (SceneItem *) NULL);

	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.changeZoom(-1);
	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.disableControl();

	_actor4.postInit();
	_actor4._numFrames = 7;
	_actor4._moveDiff = Common::Point(5, 3);
	_actor4.changeZoom(-1);
	_actor4._effect = 1;
	_actor4.setDetails(3600, 27, -1, -1, 1, (SceneItem *) NULL);

	_actor5.postInit();
	_actor5._numFrames = 7;
	_actor5._moveDiff = Common::Point(3, 2);
	_actor5.changeZoom(-1);
	_actor5._effect = 1;
	_actor5.setDetails(3600, 12, -1, -1, 1, (SceneItem *) NULL);

	_palette1.loadPalette(0);
	_palette1.loadPalette(3601);

	if (R2_GLOBALS._sceneManager._previousScene == 3600) {
		_item5._sceneRegionId = 200;
		_item5.setDetails(3600, 30, -1, -1, 5, &_actor4);
		_field254A = 1;
		_field2548 = 1;

		R2_GLOBALS._walkRegions.disableRegion(2);
		R2_GLOBALS._walkRegions.disableRegion(7);
		R2_GLOBALS._walkRegions.disableRegion(14);
		R2_GLOBALS._walkRegions.disableRegion(15);
		R2_GLOBALS._walkRegions.disableRegion(16);

		_actor10.setup(10, 5, 11);
		_actor10.animate(ANIM_MODE_1, NULL);

		_actor11.setup(20, 5, 11);
		_actor11.animate(ANIM_MODE_1, NULL);

		_actor12.setup(30, 5, 11);
		_actor12.animate(ANIM_MODE_1, NULL);

		if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
			_actor10.setPosition(Common::Point(76, 148));
			_actor11.setPosition(Common::Point(134, 148));
			_actor12.setPosition(Common::Point(100, 148));
			R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
			R2_GLOBALS._player.setup(20, _actor11._strip, 1);
			R2_GLOBALS._player.setPosition(_actor11._position);
			_actor11.hide();
		} else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA) {
			_actor10.setPosition(Common::Point(110, 148));
			_actor11.setPosition(Common::Point(76, 148));
			_actor12.setPosition(Common::Point(134, 148));
			R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
			R2_GLOBALS._player.setup(30, _actor12._strip, 1);
			R2_GLOBALS._player.setPosition(_actor12._position);
			_actor12.hide();
		} else {
			_actor10.setPosition(Common::Point(134, 148));
			_actor11.setPosition(Common::Point(76, 148));
			_actor12.setPosition(Common::Point(110, 148));
			R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
			R2_GLOBALS._player.setup(10, _actor10._strip, 1);
			R2_GLOBALS._player.setPosition(_actor10._position);
			_actor10.hide();
		}
		_actor4.setPosition(Common::Point(47, 149));
		_actor4.setup(40, 1, 11);
		_actor4.animate(ANIM_MODE_1, NULL);

		_actor5.setPosition(Common::Point(367, 148));
		_actor5.setup(3601, 7, 5);

		if (!R2_GLOBALS.getFlag(71)) {
			_protector.postInit();
			_protector._state = 0;
			_protector._field8A = Common::Point(226, 152);
			_protector._moveDiff = Common::Point(3, 2);
			_protector.setPosition(Common::Point(284, 152));
			_protector.setup(3127, 2, 1);
			_protector.changeZoom(-1);
			_protector.setDetails(3600, 15, -1, 17, 1, (SceneItem *) NULL);
		}

		R2_GLOBALS._sound2.play(330);
		_actor3.postInit();
		_actor3.setPosition(Common::Point(84, 156));
		_actor3.fixPriority(158);
		_actor3.setup(3601, 5, 1);
		_actor3.animate(ANIM_MODE_2, NULL);

		_action1._field1E = 1;
		_action1._field20 = 0;
		_action1.setActionIndex(1);

		_actor3.setAction(&_action1);
		_sceneMode = 3623;

		g_globals->_events.setCursor(CURSOR_ARROW);
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
	} else {
		_field254A = 0;
		_field2548 = 0;

		R2_GLOBALS._walkRegions.disableRegion(17);
		R2_GLOBALS._walkRegions.disableRegion(18);

		_actor10.setPosition(Common::Point(393, 148));
		_actor11.setPosition(Common::Point(364, 153));
		_actor12.setPosition(Common::Point(413, 164));

		R2_GLOBALS._player.hide();

		_actor4.setPosition(Common::Point(373, 164));

		_actor5.setup(3403, 8, 11);
		_actor5.setPosition(Common::Point(403, 155));

		_protector.setup(3403, 7, 1);
		_protector.setPosition(Common::Point(405, 155));

		_actor2.postInit();
		_actor2.setup(3600, 2, 1);
		_actor2.setPosition(Common::Point(403, 161));
		_actor2.fixPriority(149);
		_actor2.changeZoom(-1);

		_action1._field1E = 0;
		_action1._field20 = 90;

		_sceneMode = 3600;
		setAction(&_sequenceManager1, this, 3600, &_actor11, &_actor10, &_actor12, &_actor4, &_actor5, &_actor2, NULL);
		_field254E = 0;
	}
	_field254E = 0;
	_ghoulTeleported = R2_GLOBALS.getFlag(71);

	R2_GLOBALS._sound1.play(326);
	_item1.setDetails(Rect(0, 0, 480, 200), 3600, 0, -1, -1, 1, NULL);
}

void Scene3600::remove() {
	_actor3.animate(ANIM_MODE_NONE, NULL);
	_actor3.setAction(NULL);
	R2_GLOBALS._sound2.fadeOut2(NULL);
	R2_GLOBALS._sound1.fadeOut2(NULL);
	R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
	SceneExt::remove();
}

void Scene3600::signal() {
	switch (_sceneMode) {
	case 3320:
		// TODO: warning("STUB: sub_1D227()");
		R2_GLOBALS._walkRegions.disableRegion(14);
		R2_GLOBALS._scrollFollower = &_actor11;
		_tealSpeaker._object1.hide();
		_actor5.show();
		_actor5.setStrip(2);
		if (R2_GLOBALS._player._characterIndex == R2_SEEKER)
			_sceneMode = 3602;
		else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA)
			_sceneMode = 3603;
		else
			_sceneMode = 3601;
		setAction(&_sequenceManager1, this, _sceneMode, &_actor11, &_actor10, &_actor12, &_actor4, &_actor5, NULL);
		break;
	case 3321:
		warning("STUB: sub_1D227()");
		R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
		_tealSpeaker.proc16();
		_actor5.show();
		_actor5.setStrip(1);
		_actor3.postInit();
		_sceneMode = 3604;
		setAction(&_sequenceManager1, this, _sceneMode, &_actor5, &_actor3, &_actor10, &_actor11, &_actor12, &_actor4, NULL);
		break;
	case 3322:
		warning("STUB: sub_1D227()");
		_quinnSpeaker.proc16();
		_quinnSpeaker._displayMode = 1;
		_tealSpeaker.proc16();
		_tealSpeaker._displayMode = 7;
		R2_GLOBALS._scrollFollower = &_actor5;
		_sceneMode = 3605;
		setAction(&_sequenceManager1, this, _sceneMode, &_actor5, &_protector, &_actor2, NULL);
		break;
	case 3323:
		if (_field254A == 0)
			_field254A = 1;
		else {
			warning("STUB: sub_1D227()");
			_protectorSpeaker.proc16();
			_protector.show();
			_protector.setup(3258, 6, 1);

			_sceneMode = 3607;
			_protector.setAction(&_sequenceManager1, this, _sceneMode, &_protector, NULL);

			R2_GLOBALS._v558C2 = 1;
			_protectorSpeaker.proc16();
			_protectorSpeaker._displayMode = 1;
			_quinnSpeaker._displayMode = 1;
			_protector.show();

			R2_GLOBALS._scrollFollower = &R2_GLOBALS._player;
			R2_GLOBALS._walkRegions.enableRegion(17);
			R2_GLOBALS._walkRegions.enableRegion(18);
			R2_GLOBALS._walkRegions.disableRegion(2);
			R2_GLOBALS._walkRegions.disableRegion(7);
			R2_GLOBALS._walkRegions.disableRegion(14);
			R2_GLOBALS._walkRegions.disableRegion(15);
			R2_GLOBALS._walkRegions.disableRegion(16);

			_actor3.setAction(&_action1);
		}
		break;
	case 3324:
	// No break on purpose
	case 3607:
		g_globals->_events.setCursor(CURSOR_ARROW);
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		_protector.fixPriority(-1);
		_sceneMode = 3623;
		_field2548 = 1;
		break;
	case 3327:
		g_globals->_events.setCursor(CURSOR_ARROW);
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		_sceneMode = 3623;
		break;
	case 3450:
		R2_GLOBALS._sound1.stop();
		_actor1.hide();
		_actor6.hide();

		_sceneBounds = Rect(40, 0, SCREEN_WIDTH + 40, SCREEN_HEIGHT);
		setZoomPercents(142, 80, 167, 105);
		loadScene(3600);
		R2_GLOBALS._uiElements.show();
		_item5._sceneRegionId = 200;
		_item5.setDetails(3600, 30, -1, -1, 5, &_actor4);

		_actor3.show();
		_actor10.show();
		_actor11.show();
		_actor12.show();
		_actor4.show();
		_actor5.show();

		_actor5.setPosition(Common::Point(298, 151));

		_protector.postInit();
		_protector._state = 0;
		_protector._field8A = Common::Point(226, 152);
		_protector._moveDiff = Common::Point(5, 3);
		_protector.setup(3403, 7, 1);
		_protector.setPosition(Common::Point(405, 155));
		_protector.changeZoom(-1);
		_protector.addMover(NULL);
		_protector.animate(ANIM_MODE_NONE);
		_protector.hide();
		_protector.setDetails(3600, 15, -1, 17, 5, &_item5);

		_actor2.setup(3600, 2, 1);
		_actor2.setPosition(Common::Point(403, 161));
		_actor2.fixPriority(149);
		_actor2.changeZoom(-1);
		_actor2.show();

		_quinnSpeaker._displayMode = 2;
		_tealSpeaker._displayMode = 2;

		if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
			R2_GLOBALS._player._moveDiff = Common::Point(5, 3);
			R2_GLOBALS._player.setup(20, _actor11._strip, 1);
			R2_GLOBALS._player.setPosition(_actor11._position);
			_actor11.hide();
		} else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA) {
			R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
			R2_GLOBALS._player.setup(30, _actor12._strip, 1);
			R2_GLOBALS._player.setPosition(_actor12._position);
			_actor12.hide();
		} else {
			R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
			R2_GLOBALS._player.setup(10, _actor10._strip, 1);
			R2_GLOBALS._player.setPosition(_actor10._position);
			_actor10.hide();
		}
		R2_GLOBALS._player.show();
		R2_GLOBALS._sound1.play(326);
		_sceneMode = 3322;
		_stripManager.start(3322, this);
		R2_GLOBALS._sound2.play(329);
		break;
	case 3600:
		_sceneMode = 3320;
		_stripManager.start(3320, this);
		break;
	case 3601:
	// No break on purpose
	case 3602:
	// No break on purpose
	case 3603:
		R2_GLOBALS._walkRegions.disableRegion(2);
		R2_GLOBALS._walkRegions.disableRegion(7);
		R2_GLOBALS._v558B6.set(60, 0, 260, 200);
		_tealSpeaker._displayMode = 1;
		_sceneMode = 3321;
		_stripManager.start(3321, this);
		break;
	case 3604:
		R2_GLOBALS._sound2.fadeOut2(NULL);
		R2_GLOBALS._sound1.stop();
		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(7);

		_actor2.hide();
		_actor3.hide();
		R2_GLOBALS._player.hide();
		_actor10.hide();
		_actor11.hide();
		_actor12.hide();
		_actor4.hide();
		_actor5.hide();

		_sceneBounds = Rect(60, 0, SCREEN_WIDTH + 60, SCREEN_HEIGHT);
		setZoomPercents(51, 46, 180, 200);

		loadScene(3400);
		R2_GLOBALS._uiElements.show();
		_actor1.postInit();

		_actor2.setup(3403, 1, 1);
		_actor2.setPosition(Common::Point(190, 103));
		_actor2.fixPriority(89);
		_actor2.show();

		_actor6.postInit();
		_actor6.setup(3400, 1, 6);
		_actor6.setPosition(Common::Point(236, 51));
		_actor6.fixPriority(51);
		R2_GLOBALS._scrollFollower = &_actor6;

		R2_GLOBALS._sound1.play(323);
		_sceneMode = 3450;
		setAction(&_sequenceManager1, this, 3450, &_actor1, &_actor6, NULL);
		break;
	case 3605:
		_protector.setup(3258, 4, 1);
		_protector.setAction(&_sequenceManager1, this, 3606, &_actor5, &_protector, &_actor2, NULL);
		_sceneMode = 3323;
		_stripManager.start(3323, this);

		break;
	case 3620:
	// No break on purpose
	case 3623:
		if ((_protector._position.x == 226) && (_protector._position.y == 152) 
				&& (_action1._field1E != 0) && (_protector._visage == 3127) && (!R2_GLOBALS.getFlag(71))) {
			R2_GLOBALS._sound2.stop();
			R2_GLOBALS._sound2.play(331);
			R2_GLOBALS.setFlag(71);
			_sceneMode = 3626;
			setAction(&_sequenceManager1, this, 3626, &_protector, NULL);
		}
		break;
	case 3624:
		R2_GLOBALS._player.disableControl();
		if ((_field254E != 0) && (_actor10._position.x == 229) && (_actor10._position.y == 154) && (_actor11._position.x == 181) && (_actor11._position.y == 154) && (_actor12._position.x == 207) && (_actor12._position.y == 154) && (_actor4._position.x == 155) && (_actor4._position.y == 154)) {
			R2_GLOBALS._sound2.stop();
			R2_GLOBALS._sound2.play(331);
			_sceneMode = 3625;
			setAction(&_sequenceManager1, this, 3625, &_actor10, &_actor11, &_actor12, &_actor4, NULL);
		}
		break;
	case 3625:
		R2_GLOBALS._sound2.stop();
		R2_GLOBALS._sceneManager.changeScene(3700);
		break;
	case 3626:
		_protector.setPosition(Common::Point(0, 0));
		_action1.setActionIndex(2);
		if (R2_GLOBALS._events.getCursor() > R2_LAST_INVENT) {
			R2_GLOBALS._events.setCursor(CURSOR_USE);
			R2_GLOBALS._player.enableControl(CURSOR_USE);
		} else {
			R2_GLOBALS._player.enableControl();
		}
		R2_GLOBALS._sound2.stop();
		_sceneMode = 3623;
		break;
	default:
		break;
	}
}

void Scene3600::process(Event &event) {
	if ((event.eventType == EVENT_BUTTON_DOWN) && (R2_GLOBALS._events.getCursor() == CURSOR_ARROW) && (event.mousePos.x > 237) && (!R2_GLOBALS.getFlag(71))) {
		SceneItem::display(3600, 17, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7 -999);
		event.handled = true;
	}
	Scene::process(event);
}

void Scene3600::dispatch() {
	if ((R2_GLOBALS._player.getRegionIndex() == 200) && (_action1._field1E != 0) 
			&& (_field254E == 0)) {
		R2_GLOBALS._sound2.fadeOut2(NULL);
		if (_protector._mover)
			_protector.addMover(NULL);

		if (R2_GLOBALS._player._action)
			R2_GLOBALS._player.setAction(NULL);
		if (R2_GLOBALS._player._mover)
			R2_GLOBALS._player.addMover(NULL);

		_field254C = 0;
		_field254E = 1;

		R2_GLOBALS._walkRegions.enableRegion(2);
		R2_GLOBALS._walkRegions.enableRegion(7);
		R2_GLOBALS._player.disableControl();

		_sceneMode = 3624;

		_actor10.setStrip(-1);
		_actor11.setStrip(-1);
		_actor12.setStrip(-1);
		_actor4.setStrip(-1);

		R2_GLOBALS._player.hide();

		if (R2_GLOBALS._player._characterIndex == R2_SEEKER) {
			_actor11.setPosition(R2_GLOBALS._player._position);
			_actor11.show();
		} else if (R2_GLOBALS._player._characterIndex == R2_MIRANDA) {
			_actor12.setPosition(R2_GLOBALS._player._position);
			_actor12.show();
		} else {
			_actor10.setPosition(R2_GLOBALS._player._position);
			_actor10.show();
		}
		_actor10.setAction(&_sequenceManager2, this, 3610, &_actor10, NULL);
		_actor11.setAction(&_sequenceManager3, this, 3611, &_actor11, NULL);
		_actor12.setAction(&_sequenceManager4, this, 3612, &_actor12, NULL);
		_actor4.setAction(&_sequenceManager1, this, 3613, &_actor4, NULL);
	}

	if ((_protector.getRegionIndex() == 200) && (_action1._field1E != 0) && !_ghoulTeleported) {
		R2_GLOBALS._sound2.fadeOut2(NULL);
		_sceneMode = 3620;
		_ghoulTeleported = true;
		R2_GLOBALS._player.disableControl();

		if (R2_GLOBALS._player._mover)
			R2_GLOBALS._player.addMover(NULL);
		if (_actor10._mover)
			_actor10.addMover(NULL);
		if (_actor11._mover)
			_actor11.addMover(NULL);
		if (_actor12._mover)
			_actor12.addMover(NULL);
		if (_actor4._mover)
			_actor4.addMover(NULL);
	}

	Scene::dispatch();
}

/*--------------------------------------------------------------------------
 * Scene 3700 - Cutscene - Teleport outside
 *
 *--------------------------------------------------------------------------*/

void Scene3700::postInit(SceneObjectList *OwnerList) {
	loadScene(3700);
	R2_GLOBALS._uiElements._active = false;
	SceneExt::postInit();
	R2_GLOBALS._interfaceY = SCREEN_HEIGHT;

	_stripManager.setColors(60, 255);
	_stripManager.setFontNumber(3);
	_stripManager.addSpeaker(&_quinnSpeaker);
	_stripManager.addSpeaker(&_seekerSpeaker);
	_stripManager.addSpeaker(&_mirandaSpeaker);

	_quinn.postInit();
	_quinn._moveDiff = Common::Point(3, 2);

	_seeker.postInit();
	_seeker._numFrames = 7;
	_seeker._moveDiff = Common::Point(5, 3);
	_seeker.hide();

	_miranda.postInit();
	_miranda._moveDiff = Common::Point(3, 2);
	_miranda.hide();

	_webbster.postInit();
	_webbster._numFrames = 7;
	_webbster._moveDiff = Common::Point(5, 3);
	_webbster.hide();

	_actor5.postInit();

	R2_GLOBALS._player.disableControl();
	R2_GLOBALS._sound1.play(332);

	_sceneMode = 3700;
	setAction(&_sequenceManager, this, 3700, &_quinn, &_seeker, &_miranda, 
		&_webbster, &_actor5, NULL);
}

void Scene3700::remove() {
	R2_GLOBALS._sound1.fadeOut2(NULL);
	SceneExt::remove();
}

void Scene3700::signal() {
	switch (_sceneMode) {
	case 3328:
	// No break on purpose
	case 3329:
		warning("STUB: sub_1D227()");
		_sceneMode = 3701;
		setAction(&_sequenceManager, this, 3701, &_seeker, &_miranda, &_webbster, NULL);
		break;
	case 3700:
		_quinn.setup(10, 6, 1);
		_seeker.setup(20, 5, 1);
		if (R2_GLOBALS.getFlag(71)) {
			_sceneMode = 3329;
			_stripManager.start(3329, this);
		} else {
			_sceneMode = 3328;
			_stripManager.start(3328, this);
		}
		break;
	case 3701:
		R2_GLOBALS._sceneManager.changeScene(1000);
		break;
	default:
		R2_GLOBALS._player.enableControl();
		break;
	}
}

/*--------------------------------------------------------------------------
 * Scene 3800 - Desert
 *
 *--------------------------------------------------------------------------*/

Scene3800::Scene3800() {
	_desertDirection = 0;
}

void Scene3800::synchronize(Serializer &s) {
	SceneExt::synchronize(s);

	s.syncAsSint16LE(_desertDirection);
}

void Scene3800::NorthExit::changeScene() {
	Scene3800 *scene = (Scene3800 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl();
	scene->_desertDirection = 1;

	if (R2_GLOBALS.getFlag(46)) {
		if (scene->_desertDirection == R2_GLOBALS._desertCorrectDirection) {
			R2_GLOBALS._desertPreviousDirection = 3;
			if (R2_GLOBALS._desertWrongDirCtr + 1 == 0) {
				R2_GLOBALS._desertStepsRemaining--;
				R2_GLOBALS._desertCorrectDirection = 0;
			} else {
				R2_GLOBALS._desertCorrectDirection = R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr];
				R2_GLOBALS._desertWrongDirCtr--;
			}
		} else {
			++R2_GLOBALS._desertWrongDirCtr;
			if (R2_GLOBALS._desertWrongDirCtr > 999)
				R2_GLOBALS._desertWrongDirCtr = 999;
			R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr] = R2_GLOBALS._desertCorrectDirection;
			R2_GLOBALS._desertCorrectDirection = 3;
		}
	}

	if (R2_GLOBALS._desertStepsRemaining == 0)
		scene->_sceneMode = 16;
	else
		scene->_sceneMode = 11;

	Common::Point pt(160, 115);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3800::EastExit::changeScene() {
	Scene3800 *scene = (Scene3800 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_desertDirection = 2;

	if (R2_GLOBALS.getFlag(46)) {
		if (scene->_desertDirection == R2_GLOBALS._desertCorrectDirection) {
			R2_GLOBALS._desertPreviousDirection = 4;
			if (R2_GLOBALS._desertWrongDirCtr + 1 == 0) {
				R2_GLOBALS._desertStepsRemaining--;
				R2_GLOBALS._desertCorrectDirection = 0;
			} else {
				R2_GLOBALS._desertCorrectDirection = R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr];
				R2_GLOBALS._desertWrongDirCtr--;
			}
		} else {
			++R2_GLOBALS._desertWrongDirCtr;
			if (R2_GLOBALS._desertWrongDirCtr > 999)
				R2_GLOBALS._desertWrongDirCtr = 999;
			R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr] = R2_GLOBALS._desertCorrectDirection;
			R2_GLOBALS._desertCorrectDirection = 4;
		}
	}

	if (R2_GLOBALS._desertStepsRemaining == 0)
		scene->_sceneMode = 16;
	else
		scene->_sceneMode = 12;

	Common::Point pt(330, 145);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3800::SouthExit::changeScene() {
	Scene3800 *scene = (Scene3800 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_desertDirection = 3;

	if (R2_GLOBALS.getFlag(46)) {
		if (scene->_desertDirection == R2_GLOBALS._desertCorrectDirection) {
			R2_GLOBALS._desertPreviousDirection = 1;
			if (R2_GLOBALS._desertWrongDirCtr + 1 == 0) {
				R2_GLOBALS._desertStepsRemaining--;
				R2_GLOBALS._desertCorrectDirection = 0;
			} else {
				R2_GLOBALS._desertCorrectDirection = R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr];
				R2_GLOBALS._desertWrongDirCtr--;
			}
		} else {
			++R2_GLOBALS._desertWrongDirCtr;
			if (R2_GLOBALS._desertWrongDirCtr > 999)
				R2_GLOBALS._desertWrongDirCtr = 999;
			R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr] = R2_GLOBALS._desertCorrectDirection;
			R2_GLOBALS._desertCorrectDirection = 1;
		}
	}

	if (R2_GLOBALS._desertStepsRemaining == 0)
		scene->_sceneMode = 16;
	else
		scene->_sceneMode = 13;

	Common::Point pt(160, 220);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3800::WestExit::changeScene() {
	Scene3800 *scene = (Scene3800 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_desertDirection = 4;

	if (R2_GLOBALS.getFlag(46)) {
		if (scene->_desertDirection == R2_GLOBALS._desertCorrectDirection) {
			R2_GLOBALS._desertPreviousDirection = 2;
			if (R2_GLOBALS._desertWrongDirCtr + 1 == 0) {
				R2_GLOBALS._desertStepsRemaining--;
				R2_GLOBALS._desertCorrectDirection = 0;
			} else {
				R2_GLOBALS._desertCorrectDirection = R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr];
				R2_GLOBALS._desertWrongDirCtr--;
			}
		} else {
			++R2_GLOBALS._desertWrongDirCtr;
			if (R2_GLOBALS._desertWrongDirCtr > 999)
				R2_GLOBALS._desertWrongDirCtr = 999;
			R2_GLOBALS._desertMovements[R2_GLOBALS._desertWrongDirCtr] = R2_GLOBALS._desertCorrectDirection;
			R2_GLOBALS._desertCorrectDirection = 2;
		}
	}

	if (R2_GLOBALS._desertStepsRemaining == 0)
		scene->_sceneMode = 16;
	else
		scene->_sceneMode = 14;

	Common::Point pt(-10, 145);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3800::initExits() {
	_northExit._enabled = true;
	_eastExit._enabled = true;
	_southExit._enabled = true;
	_westExit._enabled = true;
	_northExit._insideArea = false;
	_eastExit._insideArea = false;
	_southExit._insideArea = false;
	_westExit._insideArea = false;
	_northExit._moving = false;
	_eastExit._moving = false;
	_southExit._moving = false;
	_westExit._moving = false;

	loadScene(R2_GLOBALS._maze3800SceneNumb);

	R2_GLOBALS._uiElements.draw();
}

void Scene3800::enterArea() {
	R2_GLOBALS._player.disableControl();
	switch (_desertDirection) {
	case 0:
		R2_GLOBALS._player.postInit();
		R2_GLOBALS._player.setVisage(10);
		R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
		R2_GLOBALS._player.setPosition(Common::Point(160, 145));
		R2_GLOBALS._player.setStrip(3);
		R2_GLOBALS._player.changeZoom(-1);
		R2_GLOBALS._player._moveDiff = Common::Point(3, 2);
		_actor1.postInit();
		_actor1.fixPriority(10);
		_actor1.changeZoom(-1);
		_actor1.setVisage(1110);
		_actor1._effect = 5;
		_actor1._field9C = this->_field312;
		R2_GLOBALS._player._linkedActor = &_actor1;
		switch (R2_GLOBALS._sceneManager._previousScene) {
		case 2600:
			_object1.postInit();
			_object2.postInit();
			_actor1.hide();
			_sceneMode = 3800;
			setAction(&_sequenceManager1, this, 3800, &R2_GLOBALS._player, &_object1, &_object2, NULL);
			break;
		case 3900:
			_sceneMode = 15;
			switch (R2_GLOBALS._desertPreviousDirection - 1) {
			case 0: {
				R2_GLOBALS._player.setPosition(Common::Point(160, 220));
				Common::Point pt(160, 160);
				NpcMover *mover = new NpcMover();
				R2_GLOBALS._player.addMover(mover, &pt, this);
				}
				break;
			case 1: {
				R2_GLOBALS._player.setPosition(Common::Point(-10, 145));
				Common::Point pt(19, 145);
				NpcMover *mover = new NpcMover();
				R2_GLOBALS._player.addMover(mover, &pt, this);
				}
				break;
			case 2: {
				R2_GLOBALS._player.setPosition(Common::Point(160, 115));
				Common::Point pt(160, 120);
				NpcMover *mover = new NpcMover();
				R2_GLOBALS._player.addMover(mover, &pt, this);
				}
				break;
			case 3: {
				R2_GLOBALS._player.setPosition(Common::Point(330, 145));
				Common::Point pt(300, 145);
				NpcMover *mover = new NpcMover();
				R2_GLOBALS._player.addMover(mover, &pt, this);
				}
				break;
			default:
				break;
			}
			break;
		default:
			R2_GLOBALS._player.enableControl(CURSOR_WALK);
			break;
		}
		break;
	case 1: {
		_sceneMode = 15;
		R2_GLOBALS._player.setPosition(Common::Point(160, 220));
		Common::Point pt(160, 160);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
		}
		break;
	case 2: {
		_sceneMode = 15;
		R2_GLOBALS._player.setPosition(Common::Point(-10, 145));
		Common::Point pt(19, 145);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
		}
		break;
	case 3: {
		_sceneMode = 15;
		R2_GLOBALS._player.setPosition(Common::Point(160, 115));
		Common::Point pt(160, 120);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
		}
		break;
	case 4: {
		_sceneMode = 15;
		R2_GLOBALS._player.setPosition(Common::Point(330, 145));
		Common::Point pt(300, 145);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
		}
		break;
	default:
		break;
	}
}

void Scene3800::postInit(SceneObjectList *OwnerList) {
	_desertDirection = 0;

	initExits();

	SceneExt::postInit();
	R2_GLOBALS._sound1.play(231);

	scalePalette(65, 65, 65);

	setZoomPercents(87, 40, 144, 100);

	_northExit.setDetails(Rect(14, 87, 305, 125), SHADECURSOR_UP, 3800);
	_northExit.setDest(Common::Point(160, 126));
	_eastExit.setDetails(Rect(305, 87, 320, 168), EXITCURSOR_E, 3800);
	_eastExit.setDest(Common::Point(312, 145));
	_southExit.setDetails(Rect(14, 160, 305, 168), SHADECURSOR_DOWN, 3800);
	_southExit.setDest(Common::Point(160, 165));
	_westExit.setDetails(Rect(0, 87, 14, 168), EXITCURSOR_W, 3800);
	_westExit.setDest(Common::Point(7, 145));

	_rect1.set(0, 0, 320, 87);
	_background.setDetails(Rect(0, 0, 320, 200), 3800, 0, 1, 2, 1, (SceneItem *) NULL);

	enterArea();
}

void Scene3800::signal() {
	switch (_sceneMode) {
	case 11:
		R2_GLOBALS._maze3800SceneNumb += 15;
		if (R2_GLOBALS._maze3800SceneNumb > 3815)
			R2_GLOBALS._maze3800SceneNumb -= 20;
		initExits();
		enterArea();
		break;
	case 12:
		R2_GLOBALS._maze3800SceneNumb += 5;
		if (R2_GLOBALS._maze3800SceneNumb > 3815)
			R2_GLOBALS._maze3800SceneNumb = 3800;
		initExits();
		enterArea();
		break;
	case 13:
		R2_GLOBALS._maze3800SceneNumb -= 15;
		if (R2_GLOBALS._maze3800SceneNumb < 3800)
			R2_GLOBALS._maze3800SceneNumb += 20;
		initExits();
		enterArea();
		break;
	case 14:
		R2_GLOBALS._maze3800SceneNumb -= 5;
		if (R2_GLOBALS._maze3800SceneNumb < 3800)
			R2_GLOBALS._maze3800SceneNumb = 3815;
		initExits();
		enterArea();
		break;
	case 15:
		R2_GLOBALS._player.enableControl();
		break;
	case 16:
		g_globals->_sceneManager.changeScene(3900);
		break;
	case 3800:
		_actor1.show();
		_object1.remove();
		_object2.remove();
		R2_GLOBALS._player.enableControl();
		break;
	case 3805:
		_northExit._enabled = false;
		_eastExit._enabled = false;
		_southExit._enabled = false;
		_westExit._enabled = false;
		R2_GLOBALS._player._canWalk = false;
		R2_GLOBALS._events.setCursor(CURSOR_USE);
		break;
	case 3806:
		_northExit._enabled = true;
		_eastExit._enabled = true;
		_southExit._enabled = true;
		_westExit._enabled = true;
		R2_GLOBALS._player.enableControl();
		break;
	default:
		break;
	}
}

void Scene3800::process(Event &event) {
	if ((R2_GLOBALS._player._uiEnabled) && (event.eventType == EVENT_BUTTON_DOWN) && (_rect1.contains(event.mousePos))) {
		event.handled = true;
		switch (R2_GLOBALS._events.getCursor()) {
		case R2_NEGATOR_GUN:
			R2_GLOBALS._player.addMover(NULL);
			R2_GLOBALS._player.updateAngle(event.mousePos);
			break;
		case R2_STEPPING_DISKS:
			SceneItem::display(3800, 5, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
			break;
		case R2_ATTRACTOR_UNIT:
			SceneItem::display(3800, 3, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
			break;
		default:
			event.handled = false;
			break;
		}
	}

	Scene::process(event);
}

/*--------------------------------------------------------------------------
 * Scene 3900 - Forest Entrance
 *
 *--------------------------------------------------------------------------*/

void Scene3900::NorthExit::changeScene() {
	Scene3900 *scene = (Scene3900 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	R2_GLOBALS._desertCorrectDirection = 3;
	R2_GLOBALS._desertPreviousDirection = 1;
	R2_GLOBALS._desertStepsRemaining = 1;
	scene->_sceneMode = 14;

	Common::Point pt(160, 115);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3900::EastExit::changeScene() {
	Scene3900 *scene = (Scene3900 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	R2_GLOBALS._desertCorrectDirection = 4;
	R2_GLOBALS._desertPreviousDirection = 2;
	R2_GLOBALS._desertStepsRemaining = 1;
	scene->_sceneMode = 14;

	Common::Point pt(330, 145);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3900::SouthExit::changeScene() {
	Scene3900 *scene = (Scene3900 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	R2_GLOBALS._desertCorrectDirection = 1;
	R2_GLOBALS._desertPreviousDirection = 3;
	R2_GLOBALS._desertStepsRemaining = 1;
	scene->_sceneMode = 14;

	Common::Point pt(160, 220);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3900::WestExit::changeScene() {
	Scene3900 *scene = (Scene3900 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	R2_GLOBALS._desertCorrectDirection = 2;
	R2_GLOBALS._desertPreviousDirection = 4;
	R2_GLOBALS._desertStepsRemaining = 1;
	scene->_sceneMode = 14;

	Common::Point pt(-10, 145);
	NpcMover *mover = new NpcMover();
	R2_GLOBALS._player.addMover(mover, &pt, scene);
}

void Scene3900::Exit5::changeScene() {
	Scene3900 *scene = (Scene3900 *)R2_GLOBALS._sceneManager._scene;

	_enabled = false;
	R2_GLOBALS._player.disableControl(CURSOR_ARROW);
	scene->_sceneMode = 13;

	if (R2_GLOBALS._desertCorrectDirection == 4) {
		Common::Point pt(-10, 135);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, scene);
	} else {
		Common::Point pt(330, 135);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, scene);
	}

	R2_GLOBALS._desertCorrectDirection = 0;
}

void Scene3900::postInit(SceneObjectList *OwnerList) {
	if ((R2_GLOBALS._desertPreviousDirection == 2) 
			&& (R2_GLOBALS._sceneManager._previousScene != 2700))
		loadScene(3825);
	else
		loadScene(3820);
	SceneExt::postInit();

	R2_GLOBALS._sound1.changeSound(231);
	setZoomPercents(87, 40, 144, 100);

	R2_GLOBALS._player.disableControl();
	R2_GLOBALS._player.postInit();
	R2_GLOBALS._player.setVisage(10);
	R2_GLOBALS._player.animate(ANIM_MODE_1, NULL);
	R2_GLOBALS._player.setStrip(3);
	R2_GLOBALS._player.changeZoom(-1);
	R2_GLOBALS._player._moveDiff = Common::Point(3, 2);

	_actor1.postInit();
	_actor1.fixPriority(10);
	_actor1.changeZoom(-1);
	_actor1.setVisage(1110);
	_actor1._effect = 5;
	_actor1._field9C = _field312;
	R2_GLOBALS._player._linkedActor = &_actor1;

	if ((R2_GLOBALS._desertPreviousDirection == 2) && (R2_GLOBALS._sceneManager._previousScene != 2700)) {
//		loadScene(3825);
		R2_GLOBALS._desertCorrectDirection = 4;
		_northExit.setDetails(Rect(29, 87, 305, 125), SHADECURSOR_UP, 3900);
		_southExit.setDetails(Rect(29, 160, 305, 168), SHADECURSOR_DOWN, 3900);

		_eastExit.setDetails(Rect(305, 87, 320, 168), EXITCURSOR_E, 3900);
		_eastExit.setDest(Common::Point(312, 145));
		_eastExit._enabled = true;
		_eastExit._insideArea = false;
		_eastExit._moving = false;

		_westExit._enabled = false;

		_exit5.setDetails(Rect(0, 87, 29, 168), EXITCURSOR_W, 3900);
		_exit5.setDest(Common::Point(24, 135));
	} else {
//		loadScene(3820);
		R2_GLOBALS._desertCorrectDirection = 2;
		_northExit.setDetails(Rect(14, 87, 290, 125), SHADECURSOR_UP, 3900);
		_southExit.setDetails(Rect(14, 160, 290, 168), SHADECURSOR_DOWN, 3900);

		_eastExit._enabled = false;

		_westExit.setDetails(Rect(0, 87, 14, 168), EXITCURSOR_W, 3900);
		_westExit.setDest(Common::Point(7, 145));
		_westExit._enabled = true;
		_westExit._insideArea = false;
		_westExit._moving = false;

		_exit5.setDetails(Rect(290, 87, 320, 168), EXITCURSOR_E, 3900);
		_exit5.setDest(Common::Point(295, 135));
	}
	_exit5._enabled = true;
	_exit5._insideArea = false;
	_exit5._moving = false;

	scalePalette(65, 65, 65);

	_northExit.setDest(Common::Point(160, 126));
	_northExit._enabled = true;
	_northExit._insideArea = false;
	_northExit._moving = false;

	_southExit.setDest(Common::Point(160, 165));
	_southExit._enabled = true;
	_southExit._insideArea = false;
	_southExit._moving = false;

	R2_GLOBALS._uiElements.draw();

	_rect1.set(0, 0, 320, 87);
	_item1.setDetails(Rect(0, 0, 320, 200), 3800, 0, 1, 2, 1, (SceneItem *)NULL);
	if (R2_GLOBALS._sceneManager._previousScene == 3800) {
		_sceneMode = 11;
		switch (R2_GLOBALS._desertPreviousDirection) {
		case 1: {
			// Entering from the north
			R2_GLOBALS._player.setPosition(Common::Point(160, 115));
			Common::Point pt(160, 120);
			NpcMover *mover = new NpcMover();
			R2_GLOBALS._player.addMover(mover, &pt, this);
			}
			break;
		case 2: {
			// Entering from the east
			R2_GLOBALS._player.setPosition(Common::Point(330, 145));
			Common::Point pt(300, 145);
			NpcMover *mover = new NpcMover();
			R2_GLOBALS._player.addMover(mover, &pt, this);
			}
			break;
		case 3: {
			// Entering from the south
			R2_GLOBALS._player.setPosition(Common::Point(160, 220));
			Common::Point pt(160, 160);
			NpcMover *mover = new NpcMover();
			R2_GLOBALS._player.addMover(mover, &pt, this);
			}
			break;
		case 4: {
			// Entering from the west
			R2_GLOBALS._player.setPosition(Common::Point(-10, 145));
			Common::Point pt(19, 145);
			NpcMover *mover = new NpcMover();
			R2_GLOBALS._player.addMover(mover, &pt, this);
			}
			break;
		default:
			break;
		}
	} else if (R2_GLOBALS._sceneManager._previousScene == 2700) {
		_sceneMode = 12;
		R2_GLOBALS._player.setPosition(Common::Point(330, 135));
		Common::Point pt(265, 135);
		NpcMover *mover = new NpcMover();
		R2_GLOBALS._player.addMover(mover, &pt, this);
	} else {
		R2_GLOBALS._player.setPosition(Common::Point(160, 145));
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
	}
}

void Scene3900::signal() {
	switch (_sceneMode) {
	case 11:
	// No break on purpose
	case 12:
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	case 13:
		R2_GLOBALS._sceneManager.changeScene(2700);
		break;
	case 14:
		R2_GLOBALS._sceneManager.changeScene(3800);
		break;
	case 3805:
		_northExit._enabled = false;
		_eastExit._enabled = false;
		_southExit._enabled = false;
		_westExit._enabled = false;
		R2_GLOBALS._player._canWalk = false;
		R2_GLOBALS._events.setCursor(CURSOR_USE);
		break;
	case 3806:
		_northExit._enabled = true;
		_eastExit._enabled = true;
		_southExit._enabled = true;
		_westExit._enabled = true;
		R2_GLOBALS._player.enableControl(CURSOR_WALK);
		break;
	default:
		break;
	}
}

void Scene3900::process(Event &event) {
	if ((R2_GLOBALS._player._uiEnabled) && (event.eventType == EVENT_BUTTON_DOWN) && (_rect1.contains(event.mousePos))) {
		event.handled = true;
		switch (R2_GLOBALS._events.getCursor()) {
		case R2_NEGATOR_GUN:
			R2_GLOBALS._player.addMover(NULL);
			R2_GLOBALS._player.updateAngle(event.mousePos);
			break;
		case R2_STEPPING_DISKS:
			SceneItem::display(3800, 5, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
			break;
		case R2_ATTRACTOR_UNIT:
			SceneItem::display(3800, 3, 0, 280, 1, 160, 9, 1, 2, 20, 7, 7, -999);
			break;
		default:
			event.handled = false;
			break;
		}
	}
	Scene::process(event);
}

} // End of namespace Ringworld2
} // End of namespace TsAGE
