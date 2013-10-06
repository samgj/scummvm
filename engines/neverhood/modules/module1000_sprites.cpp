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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "neverhood/modules/module1000_sprites.h"

namespace Neverhood {

AsScene1001Door::AsScene1001Door(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1100) {

	createSurface(800, 137, 242);
	_x = 726;
	_y = 440;
	stShowIdleDoor();
	loadSound(1, 0xED403E03);
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1001Door::handleMessage);
}

uint32 AsScene1001Door::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x2000:
		hammerHitsDoor();
		break;
	case 0x3002:
		gotoNextState();
		break;
	}
	return 0;
}

void AsScene1001Door::hammerHitsDoor() {
	switch (getGlobalVar(V_DOOR_STATUS)) {
	case 0:
	case 1:
		playSound(0, 0x65482F03);
		startAnimation(0x624C0498, 1, 3);
		NextState(&AsScene1001Door::stShowIdleDoor);
		break;
	case 2:
		playSound(1);
		startAnimation(0x624C0498, 6, 6);
		NextState(&AsScene1001Door::stBustedDoorMove);
		break;
	default:
		// Nothing
		break;
	}
	incGlobalVar(V_DOOR_STATUS, 1);
}

void AsScene1001Door::stShowIdleDoor() {
	switch (getGlobalVar(V_DOOR_STATUS)) {
	case 1:
		startAnimation(0x624C0498, 4, -1);
		_newStickFrameIndex = 4;
		break;
	case 2:
		startAnimation(0x624C0498, 1, -1);
		_newStickFrameIndex = 1;
		break;
	case 3:
		stopAnimation();
		setVisible(false);
		break;
	default:
		startAnimation(0x624C0498, 0, -1);
		_newStickFrameIndex = 0;
		break;
	}
}

void AsScene1001Door::stBustedDoorMove() {
	setGlobalVar(V_DOOR_BUSTED, 1);
	startAnimation(0x624C0498, 6, 6);
	NextState(&AsScene1001Door::stBustedDoorGone);
	_x = 30;
}

void AsScene1001Door::stBustedDoorGone() {
	playSound(0);
	stopAnimation();
	setVisible(false);
}

AsScene1001Hammer::AsScene1001Hammer(NeverhoodEngine *vm, Sprite *asDoor)
	: AnimatedSprite(vm, 1100), _asDoor(asDoor) {

	_x = 547;
	_y = 206;
	createSurface(900, 177, 192);
	startAnimation(0x022C90D4, -1, -1);
	_newStickFrameIndex = STICK_LAST_FRAME;
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1001Hammer::handleMessage);
}

uint32 AsScene1001Hammer::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x00352100)
			sendMessage(_asDoor, 0x2000, 0);
		else if (param.asInteger() == 0x0A1A0109)
			playSound(0, 0x66410886);
		break;
	case 0x2000:
		startAnimation(0x022C90D4, 1, -1);
		playSound(0, 0xE741020A);
		_newStickFrameIndex = STICK_LAST_FRAME;
		break;
	}
	return 0;
}

AsScene1001Window::AsScene1001Window(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1200) {

	_x = 320;
	_y = 240;
	createSurface(100, 66, 129);
	startAnimation(0xC68C2299, 0, -1);
	_newStickFrameIndex = 0;
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1001Window::handleMessage);
}

uint32 AsScene1001Window::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x0E0A1410)
			playSound(0, 0x60803F10);
		break;
	case 0x2001:
		startAnimation(0xC68C2299, 0, -1);
		break;
	case 0x3002:
		SetMessageHandler(NULL);
		setGlobalVar(V_WINDOW_OPEN, 1);
		setVisible(false);
		break;
	}
	return 0;
}

AsScene1001Lever::AsScene1001Lever(NeverhoodEngine *vm, Scene *parentScene, int16 x, int16 y, int deltaXType)
	: AnimatedSprite(vm, 1100), _parentScene(parentScene) {

	createSurface(1010, 71, 73);
	setDoDeltaX(deltaXType);
	startAnimation(0x04A98C36, 0, -1);
	_newStickFrameIndex = 0;
	_x = x;
	_y = y;
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1001Lever::handleMessage);
}

uint32 AsScene1001Lever::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x00C0C444)
			sendMessage(_parentScene, 0x480F, 0);
		else if (param.asInteger() == 0xC41A02C0)
			playSound(0, 0x40581882);
		break;
	case 0x1011:
		sendMessage(_parentScene, 0x4826, 0);
		messageResult = 1;
		break;
	case 0x3002:
		startAnimation(0x04A98C36, 0, -1);
		_newStickFrameIndex = 0;
		break;
	case 0x480F:
		startAnimation(0x04A98C36, 0, -1);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

SsCommonButtonSprite::SsCommonButtonSprite(NeverhoodEngine *vm, Scene *parentScene, uint32 fileHash, int surfacePriority, uint32 soundFileHash)
	: StaticSprite(vm, fileHash, surfacePriority), _parentScene(parentScene), _countdown(0) {

	_priority = 1100;
	_soundFileHash = soundFileHash ? soundFileHash : 0x44141000;
	setVisible(false);
	SetUpdateHandler(&SsCommonButtonSprite::update);
	SetMessageHandler(&SsCommonButtonSprite::handleMessage);
}

void SsCommonButtonSprite::update() {
	if (_countdown != 0 && (--_countdown) == 0)
		setVisible(false);
}

uint32 SsCommonButtonSprite::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x480B:
		sendMessage(_parentScene, 0x480B, 0);
		setVisible(true);
		_countdown = 8;
		playSound(0, _soundFileHash);
		break;
	}
	return messageResult;
}

AsScene1002Ring::AsScene1002Ring(NeverhoodEngine *vm, Scene *parentScene, bool isSpecial, int16 x, int16 y, int16 clipY1, bool isRingLow)
	: AnimatedSprite(vm, 1100), _parentScene(parentScene), _isSpecial(isSpecial) {

	SetUpdateHandler(&AsScene1002Ring::update);

	if (_isSpecial) {
		createSurface(990, 68, 314);
		if (isRingLow) {
			startAnimation(0x04103090, 0, -1);
			SetMessageHandler(&AsScene1002Ring::hmRingHangingLow);
		} else {
			startAnimation(0xA85C4011, _vm->_rnd->getRandomNumber(15), -1);
			SetMessageHandler(&AsScene1002Ring::hmRingIdle);
		}
	} else {
		createSurface(990, 68, 138);
		startAnimation(0xA85C4011, _vm->_rnd->getRandomNumber(15), -1);
		SetMessageHandler(&AsScene1002Ring::hmRingIdle);
	}

	setClipRect(0, clipY1, 640, 480);

	_x = x;
	_y = y;

	setDoDeltaX(_vm->_rnd->getRandomNumber(1));

}

void AsScene1002Ring::update() {
	updateAnim();
	updatePosition();
}

uint32 AsScene1002Ring::hmRingIdle(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x4806:
		setDoDeltaX(((Sprite*)sender)->isDoDeltaX() ? 1 : 0);
		sendMessage(_parentScene, 0x4806, 0);
		SetMessageHandler(&AsScene1002Ring::hmRingPulled1);
		startAnimation(_isSpecial ? 0x87502558 : 0x80DD4010, 0, -1);
		break;
	case 0x480F:
		setDoDeltaX(((Sprite*)sender)->isDoDeltaX() ? 1 : 0);
		sendMessage(_parentScene, 0x480F, 0);
		SetMessageHandler(&AsScene1002Ring::hmRingPulled2);
		startAnimation(0x861A2020, 0, -1);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

uint32 AsScene1002Ring::hmRingPulled1(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x3002:
		startAnimation(_isSpecial ? 0x78D0A812 : 0xB85D2A10, 0, -1);
		SetMessageHandler(&AsScene1002Ring::hmRingHangingLow);
		break;
	case 0x4807:
		sendMessage(_parentScene, 0x4807, 0);
		setDoDeltaX(_vm->_rnd->getRandomNumber(1));
		startAnimation(0x8258A030, 0, -1);
		SetMessageHandler(&AsScene1002Ring::hmRingReleased);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

uint32 AsScene1002Ring::hmRingPulled2(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x3002:
		startAnimation(0x04103090, 0, -1);
		SetMessageHandler(&AsScene1002Ring::hmRingHangingLow);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

uint32 AsScene1002Ring::hmRingHangingLow(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x4807:
		sendMessage(_parentScene, 0x4807, 0);
		setDoDeltaX(_vm->_rnd->getRandomNumber(1));
		startAnimation(0x8258A030, 0, -1);
		SetMessageHandler(&AsScene1002Ring::hmRingReleased);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

uint32 AsScene1002Ring::hmRingReleased(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = hmRingIdle(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x05410F72)
			playSound(0, 0x21EE40A9);
		break;
	case 0x3002:
		startAnimation(0xA85C4011, 0, -1);
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 990);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1010);
		break;
	}
	return messageResult;
}

AsScene1002Door::AsScene1002Door(NeverhoodEngine *vm, NRect &clipRect)
	: StaticSprite(vm, 1200) {

	loadSprite(0x1052370F, kSLFDefDrawOffset | kSLFSetPosition, 800, 526, getGlobalVar(V_FLYTRAP_RING_DOOR) ? 49 : 239);
	setClipRect(clipRect);
	SetUpdateHandler(&AsScene1002Door::update);
	SetMessageHandler(&AsScene1002Door::handleMessage);
	SetSpriteUpdate(NULL);
}

void AsScene1002Door::update() {
	handleSpriteUpdate();
	updatePosition();
}

uint32 AsScene1002Door::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x4808:
		setGlobalVar(V_FLYTRAP_RING_DOOR, 1);
		SetSpriteUpdate(&AsScene1002Door::suOpenDoor);
		break;
	case 0x4809:
		setGlobalVar(V_FLYTRAP_RING_DOOR, 0);
		SetSpriteUpdate(&AsScene1002Door::suCloseDoor);
		break;
	}
	return messageResult;
}

void AsScene1002Door::suOpenDoor() {
	if (_y > 49) {
		_y -= 8;
		if (_y < 49) {
			SetSpriteUpdate(NULL);
			_y = 49;
		}
		_needRefresh = true;
	}
}

void AsScene1002Door::suCloseDoor() {
	if (_y < 239) {
		_y += 8;
		if (_y > 239) {
			SetSpriteUpdate(NULL);
			_y = 239;
		}
		_needRefresh = true;
	}
}

AsScene1002BoxingGloveHitEffect::AsScene1002BoxingGloveHitEffect(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1400) {

	createSurface(1025, 88, 165);
	setVisible(false);
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1002BoxingGloveHitEffect::handleMessage);
}

uint32 AsScene1002BoxingGloveHitEffect::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x2004:
		_x = ((Sprite*)sender)->getX() - 98;
		_y = ((Sprite*)sender)->getY() - 111;
		startAnimation(0x0422255A, 0, -1);
		setVisible(true);
		break;
	case 0x3002:
		stopAnimation();
		setVisible(false);
		break;
	}
	return messageResult;
}

AsScene1002DoorSpy::AsScene1002DoorSpy(NeverhoodEngine *vm, NRect &clipRect, Scene *parentScene, Sprite *asDoor, Sprite *asScene1002BoxingGloveHitEffect)
	: AnimatedSprite(vm, 1300), _clipRect(clipRect), _parentScene(parentScene), _asDoor(asDoor), _asBoxingGloveHitEffect(asScene1002BoxingGloveHitEffect) {

	createSurface(800, 136, 147);
	setClipRect(clipRect);
	suDoorSpy();
	loadSound(0, 0xC0C40298);
	startAnimation(0x586C1D48, 0, 0);
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1002DoorSpy::handleMessage);
	SetSpriteUpdate(&AsScene1002DoorSpy::suDoorSpy);
}

uint32 AsScene1002DoorSpy::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0xA61CA1C2)
			sendMessage(_asBoxingGloveHitEffect, 0x2004, 0);
		else if (param.asInteger() == 0x14CE0620)
			playSound(0);
		break;
	case 0x2003:
		stDoorSpyBoxingGlove();
		break;
	}
	return messageResult;
}

uint32 AsScene1002DoorSpy::hmDoorSpyAnimation(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x3002:
		gotoNextState();
		break;
	}
	return messageResult;
}

void AsScene1002DoorSpy::suDoorSpy() {
	_x = _asDoor->getX() + 34;
	_y = _asDoor->getY() + 175;
}

void AsScene1002DoorSpy::stDoorSpyIdle() {
	setClipRect(_clipRect);
	_parentScene->setSurfacePriority(getSurface(), 800);
	startAnimation(0x586C1D48, 0, 0);
	SetMessageHandler(&AsScene1002DoorSpy::handleMessage);
}

void AsScene1002DoorSpy::stDoorSpyBoxingGlove() {
	setClipRect(0, 0, 640, 480);
	_parentScene->setSurfacePriority(getSurface(), 1200);
	startAnimation(0x586C1D48, 1, -1);
	SetMessageHandler(&AsScene1002DoorSpy::hmDoorSpyAnimation);
	NextState(&AsScene1002DoorSpy::stDoorSpyIdle);
}

SsCommonPressButton::SsCommonPressButton(NeverhoodEngine *vm, Scene *parentScene, uint32 fileHash1, uint32 fileHash2, int surfacePriority, uint32 soundFileHash)
	: StaticSprite(vm, 1100), _parentScene(parentScene), _status(0), _countdown(0) {

	_soundFileHash = soundFileHash != 0 ? soundFileHash : 0x44141000;
	_fileHashes[0] = fileHash1;
	_fileHashes[1] = fileHash2;
	createSurface(surfacePriority, 40, 40);
	loadSprite(fileHash1, kSLFDefDrawOffset | kSLFDefPosition);
	setVisible(false);
	SetUpdateHandler(&SsCommonPressButton::update);
	SetMessageHandler(&SsCommonPressButton::handleMessage);
}

void SsCommonPressButton::setFileHashes(uint32 fileHash1, uint32 fileHash2) {
	_fileHashes[0] = fileHash1;
	_fileHashes[1] = fileHash2;
	loadSprite(_status == 2 ? fileHash2 : fileHash1, kSLFDefDrawOffset | kSLFDefPosition);
}

void SsCommonPressButton::update() {
	if (_countdown != 0 && (--_countdown) == 0) {
		if (_status == 1) {
			_status = 2;
			loadSprite(_fileHashes[1], kSLFDefDrawOffset | kSLFDefPosition);
			_countdown = 4;
		} else if (_status == 2) {
			_status = 3;
			loadSprite(_fileHashes[0], kSLFDefDrawOffset | kSLFDefPosition);
			_countdown = 4;
		} else if (_status == 3) {
			_status = 0;
			setVisible(false);
		}
	}
}

uint32 SsCommonPressButton::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x480B:
		sendMessage(_parentScene, 0x480B, 0);
		_status = 1;
		_countdown = 4;
		setVisible(true);
		playSound(0, _soundFileHash);
		break;
	}
	return messageResult;
}

AsScene1002VenusFlyTrap::AsScene1002VenusFlyTrap(NeverhoodEngine *vm, Scene *parentScene, Sprite *klaymen, bool isSecond)
	: AnimatedSprite(vm, 1100), _parentScene(parentScene), _klaymen(klaymen), _isSecond(isSecond), _countdown(0) {

	createSurface(995, 175, 195);
	if (!_isSecond) {
		if (getGlobalVar(V_FLYTRAP_RING_DOOR)) {
			setDoDeltaX(1);
			_x = 366;
			_y = 435;
			stRingGrabbed();
		} else {
			_x = 174 + getGlobalVar(V_FLYTRAP_POSITION_1) * 32;
			_y = 435;
			stIdle();
		}
	} else {
		_x = 186 + getGlobalVar(V_FLYTRAP_POSITION_2) * 32;
		_y = 364;
		if (getGlobalVar(V_FLYTRAP_RING_BRIDGE) || getGlobalVar(V_FLYTRAP_RING_FENCE)) {
			stRingGrabbed();
		} else {
			stIdle();
		}
	}
	_flags = 4;
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::handleMessage);
	SetSpriteUpdate(&AnimatedSprite::updateDeltaXY);
}

void AsScene1002VenusFlyTrap::update() {
	if (_countdown != 0 && (--_countdown == 0))
		gotoNextState();
	AnimatedSprite::update();
}

void AsScene1002VenusFlyTrap::upIdle() {
	if (_countdown == 0 && _klaymen->getX() - 20 > _x)
		setDoDeltaX(1);
	else if (_klaymen->getX() + 20 < _x)
		setDoDeltaX(0);
	update();
}

uint32 AsScene1002VenusFlyTrap::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x000890C4)
			playSound(0, 0xC21190D8);
		else if (param.asInteger() == 0x522200A0)
			playSound(0, 0x931080C8);
		break;
	case 0x1011:
		if (_isSecond) {
			if (_x >= 154 && _x <= 346) {
				sendMessage(_parentScene, 0x2000, 0);
				messageResult = 1;
			}
		} else {
			if (_x >= 174 && _x <= 430) {
				sendMessage(_parentScene, 0x2000, 0);
				messageResult = 1;
			}
		}
		break;
	case 0x480B:
		setDoDeltaX(param.asInteger() != 0 ? 1 : 0);
		if (!_isSecond) {
			if (getGlobalVar(V_FLYTRAP_RING_DOOR))
				stRelease();
			else
				stWalk();
		} else {
			if (getGlobalVar(V_FLYTRAP_RING_BRIDGE) || getGlobalVar(V_FLYTRAP_RING_FENCE))
				stRelease();
			else
				stWalk();
		}
		break;
	case 0x480C:
		if (_isSecond) {
			if (_x >= 154 && _x <= 346)
				messageResult = 1;
			else
				messageResult = 0;
		} else {
			if (_x >= 174 && _x <= 430)
				messageResult = 1;
			else
				messageResult = 0;
		}
		break;
	case 0x480E:
		if (param.asInteger() == 1)
			stGrabRing();
		break;
	case 0x4810:
		swallowKlaymen();
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 995);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1015);
		break;
	}
	return messageResult;
}

uint32 AsScene1002VenusFlyTrap::hmAnimationSimple(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x3002:
		gotoNextState();
		break;
	}
	return messageResult;
}

uint32 AsScene1002VenusFlyTrap::hmAnimationExt(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x000890C4)
			playSound(0, 0xC21190D8);
		else if (param.asInteger() == 0x41881801) {
			if (_isSecond) {
				if (_x > 330)
					sendMessage(_klaymen, 0x4811, 2);
				else
					sendMessage(_klaymen, 0x4811, 0);
			} else {
				sendMessage(_klaymen, 0x4811, 0);
			}
		} else if (param.asInteger() == 0x522200A0)
			playSound(0, 0x931080C8);
		break;
	case 0x3002:
		gotoNextState();
		break;
	case 0x482A:
		sendMessage(_parentScene, 0x1022, 995);
		break;
	case 0x482B:
		sendMessage(_parentScene, 0x1022, 1015);
		break;
	}
	return messageResult;
}

void AsScene1002VenusFlyTrap::stWalkBack() {
	setDoDeltaX(2);
	startAnimation(0xC4080034, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationExt);
	NextState(&AsScene1002VenusFlyTrap::stIdle);
}

void AsScene1002VenusFlyTrap::stWalk() {
	startAnimation(0xC4080034, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationSimple);
	NextState(&AsScene1002VenusFlyTrap::stIdle);
}

void AsScene1002VenusFlyTrap::stRelease() {
	sendMessage(_parentScene, 0x4807, 0);
	startAnimation(0x82292851, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationSimple);
	NextState(&AsScene1002VenusFlyTrap::stIdle);
}

void AsScene1002VenusFlyTrap::stGrabRing() {
	setDoDeltaX(1);
	startAnimation(0x86A82A11, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationSimple);
	NextState(&AsScene1002VenusFlyTrap::stRingGrabbed);
}

void AsScene1002VenusFlyTrap::stRingGrabbed() {
	startAnimation(0xB5A86034, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::handleMessage);
}

void AsScene1002VenusFlyTrap::stKlaymenInside() {
	startAnimation(0x31303094, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(NULL);
	NextState(&AsScene1002VenusFlyTrap::stKlaymenInsideMoving);
	_countdown = 24;
}

void AsScene1002VenusFlyTrap::stIdle() {
	startAnimation(0xC8204250, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::upIdle);
	SetMessageHandler(&AsScene1002VenusFlyTrap::handleMessage);
	if (_isSecond) {
		if (_x >= 154 && _x <= 346)
			setGlobalVar(V_FLYTRAP_POSITION_2, (_x - 186) / 32);
		else {
			NextState(&AsScene1002VenusFlyTrap::stWalkBack);
			_countdown = 12;
		}
	} else {
		if (_x >= 174 && _x <= 430)
			setGlobalVar(V_FLYTRAP_POSITION_1, (_x - 174) / 32);
		else {
			NextState(&AsScene1002VenusFlyTrap::stWalkBack);
			_countdown = 12;
		}
	}
}

void AsScene1002VenusFlyTrap::stKlaymenInsideMoving() {
	startAnimation(0x152920C4, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationExt);
	NextState(&AsScene1002VenusFlyTrap::stSpitOutKlaymen);
}

void AsScene1002VenusFlyTrap::stSpitOutKlaymen() {
	startAnimation(0x84001117, 0, -1);
	SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
	SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationExt);
	NextState(&AsScene1002VenusFlyTrap::stIdle);
}

void AsScene1002VenusFlyTrap::swallowKlaymen() {
	if (_x - 15 < _klaymen->getX() && _x + 15 > _klaymen->getX()) {
		if (_isSecond)
			setDoDeltaX(_x > 265 && _x < 330 ? 1 : 0);
		else
			setDoDeltaX(_x > 320 ? 1 : 0);
		sendMessage(_klaymen, 0x2001, 0);
		startAnimation(0x8C2C80D4, 0, -1);
		SetUpdateHandler(&AsScene1002VenusFlyTrap::update);
		SetMessageHandler(&AsScene1002VenusFlyTrap::hmAnimationExt);
		NextState(&AsScene1002VenusFlyTrap::stKlaymenInside);
	}
}

AsScene1002OutsideDoorBackground::AsScene1002OutsideDoorBackground(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1200), _countdown(0), _isDoorClosed(true) {

	createSurface(850, 186, 212);
	_x = 320;
	_y = 240;
	if (getGlobalVar(V_FLYTRAP_RING_DOOR)) {
		startAnimation(0x004A4495, -1, -1);
		_newStickFrameIndex = STICK_LAST_FRAME;
	} else
		setVisible(false);
	SetUpdateHandler(&AsScene1002OutsideDoorBackground::update);
	SetMessageHandler(&AsScene1002OutsideDoorBackground::handleMessage);
}

void AsScene1002OutsideDoorBackground::update() {
	if (_countdown != 0 && (--_countdown == 0)) {
		if (_isDoorClosed)
			stCloseDoor();
		else
			stOpenDoor();
	}
	AnimatedSprite::update();
}

uint32 AsScene1002OutsideDoorBackground::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageResult) {
	case 0x4808:
		_isDoorClosed = false;
		_countdown = 2;
		break;
	case 0x4809:
		_isDoorClosed = true;
		_countdown = 2;
		break;
	}
	return messageResult;
}

uint32 AsScene1002OutsideDoorBackground::hmAnimation(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = handleMessage(messageNum, param, sender);
	switch (messageResult) {
	case 0x3002:
		gotoNextState();
		break;
	}
	return messageResult;
}

void AsScene1002OutsideDoorBackground::stOpenDoor() {
	startAnimation(0x004A4495, 0, -1);
	_newStickFrameIndex = STICK_LAST_FRAME;
	setVisible(true);
	SetMessageHandler(&AsScene1002OutsideDoorBackground::handleMessage);
}

void AsScene1002OutsideDoorBackground::stCloseDoor() {
	startAnimation(0x004A4495, -1, -1);
	_playBackwards = true;
	setVisible(true);
	SetMessageHandler(&AsScene1002OutsideDoorBackground::hmAnimation);
	NextState(&AsScene1002OutsideDoorBackground::stDoorClosed);
}

void AsScene1002OutsideDoorBackground::stDoorClosed() {
	setVisible(false);
	stopAnimation();
}

AsScene1002KlaymenLadderHands::AsScene1002KlaymenLadderHands(NeverhoodEngine *vm, Klaymen *klaymen)
	: AnimatedSprite(vm, 1200), _klaymen(klaymen) {

	createSurface(1200, 40, 163);
	setVisible(false);
	SetUpdateHandler(&AsScene1002KlaymenLadderHands::update);
	SetMessageHandler(&Sprite::handleMessage);
}

void AsScene1002KlaymenLadderHands::update() {
	if (_klaymen->getCurrAnimFileHash() == 0x3A292504) {
		startAnimation(0xBA280522, _klaymen->getFrameIndex(), -1);
		_newStickFrameIndex = _klaymen->getFrameIndex();
		setVisible(true);
		_x = _klaymen->getX();
		_y = _klaymen->getY();
		setDoDeltaX(_klaymen->isDoDeltaX() ? 1 : 0);
	} else if (_klaymen->getCurrAnimFileHash() == 0x122D1505) {
		startAnimation(0x1319150C, _klaymen->getFrameIndex(), -1);
		_newStickFrameIndex = _klaymen->getFrameIndex();
		setVisible(true);
		_x = _klaymen->getX();
		_y = _klaymen->getY();
		setDoDeltaX(_klaymen->isDoDeltaX() ? 1 : 0);
	} else
		setVisible(false);
	AnimatedSprite::update();
}

AsScene1002KlaymenPeekHand::AsScene1002KlaymenPeekHand(NeverhoodEngine *vm, Scene *parentScene, Klaymen *klaymen)
	: AnimatedSprite(vm, 1200), _parentScene(parentScene), _klaymen(klaymen),
	_isClipRectSaved(false) {

	createSurface(1000, 33, 41);
	setVisible(false);
	SetUpdateHandler(&AsScene1002KlaymenPeekHand::update);
	SetMessageHandler(&AsScene1002KlaymenPeekHand::handleMessage);
}

void AsScene1002KlaymenPeekHand::update() {
	if (_klaymen->getCurrAnimFileHash() == 0xAC20C012 && _klaymen->getFrameIndex() < 50) {
		startAnimation(0x9820C913, _klaymen->getFrameIndex(), -1);
		_newStickFrameIndex = _klaymen->getFrameIndex();
		setVisible(true);
		_x = _klaymen->getX();
		_y = _klaymen->getY();
		setDoDeltaX(_klaymen->isDoDeltaX() ? 1 : 0);
	} else
		setVisible(false);
	AnimatedSprite::update();
}

uint32 AsScene1002KlaymenPeekHand::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	uint32 messageResult = Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x4AB28209) {
			sendMessage(_parentScene, 0x1022, 1200);
			_isClipRectSaved = true;
			_savedClipRect = _surface->getClipRect();
			setClipRect(0, 0, 640, 480);
		} else if (param.asInteger() == 0x88001184) {
			sendMessage(_parentScene, 0x1022, 1000);
			if (_isClipRectSaved)
				setClipRect(_savedClipRect);
		}
		break;
	}
	return messageResult;
}

AsScene1004TrashCan::AsScene1004TrashCan(NeverhoodEngine *vm)
	: AnimatedSprite(vm, 1100) {

	_x = 330;
	_y = 327;
	createSurface(800, 56, 50);
	setVisible(false);
	SetUpdateHandler(&AnimatedSprite::update);
	SetMessageHandler(&AsScene1004TrashCan::handleMessage);
}

uint32 AsScene1004TrashCan::handleMessage(int messageNum, const MessageParam &param, Entity *sender) {
	Sprite::handleMessage(messageNum, param, sender);
	switch (messageNum) {
	case 0x100D:
		if (param.asInteger() == 0x225A8587)
			playSound(0, 0x109AFC4C);
		break;
	case 0x2002:
		startAnimation(0xEB312C11, 0, -1);
		setVisible(true);
		break;
	case 0x3002:
		stopAnimation();
		setVisible(false);
		break;
	}
	return 0;
}

static const KlaymenIdleTableItem klaymenIdleTable1002[] = {
	{1, kIdlePickEar},
	{2, kIdleWonderAbout}
};

KmScene1001::KmScene1001(NeverhoodEngine *vm, Scene *parentScene, int16 x, int16 y)
	: Klaymen(vm, parentScene, x, y) {
}

uint32 KmScene1001::xHandleMessage(int messageNum, const MessageParam &param) {
	switch (messageNum) {
	case 0x4001:
	case 0x4800:
		startWalkToX(param.asPoint().x, false);
		break;
	case 0x4004:
		GotoState(&Klaymen::stTryStandIdle);
		break;
	case 0x4804:
		if (param.asInteger() == 2)
			GotoState(&Klaymen::stSleeping);
		break;
	case 0x480D:
		GotoState(&Klaymen::stPullHammerLever);
		break;
	case 0x4812:
		GotoState(&Klaymen::stPickUpGeneric);
		break;
	case 0x4816:
		if (param.asInteger() == 1)
			GotoState(&Klaymen::stPressButton);
		else if (param.asInteger() == 2)
			GotoState(&Klaymen::stPressFloorButton);
		else
			GotoState(&Klaymen::stPressButtonSide);
		break;
	case 0x4817:
		setDoDeltaX(param.asInteger());
		gotoNextStateExt();
		break;
	case 0x481B:
		if (param.asPoint().y != 0)
			startWalkToXDistance(param.asPoint().y, param.asPoint().x);
		else
			startWalkToAttachedSpriteXDistance(param.asPoint().x);
		break;
	case 0x481F:
		if (param.asInteger() == 0)
			GotoState(&Klaymen::stWonderAboutHalf);
		else if (param.asInteger() == 1)
			GotoState(&Klaymen::stWonderAboutAfter);
		else if (param.asInteger() == 3)
			GotoState(&Klaymen::stTurnToUseHalf);
		else if (param.asInteger() == 4)
			GotoState(&Klaymen::stTurnAwayFromUse);
		else
			GotoState(&Klaymen::stWonderAbout);
		break;
	case 0x482D:
		setDoDeltaX(_x > (int16)param.asInteger() ? 1 : 0);
		gotoNextStateExt();
		break;
	case 0x4836:
		if (param.asInteger() == 1) {
			sendMessage(_parentScene, 0x2002, 0);
			GotoState(&Klaymen::stWakeUp);
		}
		break;
	case 0x483F:
		startSpecialWalkRight(param.asInteger());
		break;
	case 0x4840:
		startSpecialWalkLeft(param.asInteger());
		break;
	}
	return 0;
}

KmScene1002::KmScene1002(NeverhoodEngine *vm, Scene *parentScene, int16 x, int16 y)
	: Klaymen(vm, parentScene, x, y) {

	setKlaymenIdleTable1();
}

void KmScene1002::xUpdate() {
	if (_x >= 250 && _x <= 435 && _y >= 420) {
		if (_idleTableNum == 0) {
			setKlaymenIdleTable(klaymenIdleTable1002, ARRAYSIZE(klaymenIdleTable1002));
			_idleTableNum = 1;
		}
	} else if (_idleTableNum == 1) {
		setKlaymenIdleTable1();
		_idleTableNum = 0;
	}
}

uint32 KmScene1002::xHandleMessage(int messageNum, const MessageParam &param) {
	switch (messageNum) {
	case 0x2001:
		GotoState(&Klaymen::stStandIdleSpecial);
		break;
	case 0x2007:
		_otherSprite = (Sprite*)param.asEntity();
		break;
	case 0x4001:
	case 0x4800:
		startWalkToX(param.asPoint().x, false);
		break;
	case 0x4004:
		GotoState(&Klaymen::stTryStandIdle);
		break;
	case 0x4803:
		if (param.asInteger() == 1)
			GotoState(&Klaymen::stJumpAndFall);
		else if (param.asInteger() == 2)
			GotoState(&Klaymen::stDropFromRing);
		break;
	case 0x4804:
		GotoState(&Klaymen::stPeekWall);
		break;
	case 0x4805:
		switch (param.asInteger()) {
		case 1:
			GotoState(&Klaymen::stJumpToRing1);
			break;
		case 2:
			GotoState(&Klaymen::stJumpToRing2);
			break;
		case 3:
			GotoState(&Klaymen::stJumpToRing3);
			break;
		case 4:
			GotoState(&Klaymen::stJumpToRing4);
			break;
		}
		break;
	case 0x480A:
		GotoState(&Klaymen::stMoveVenusFlyTrap);
		break;
	case 0x480D:
		GotoState(&Klaymen::stJumpToRingVenusFlyTrap);
		break;
	case 0x4816:
		if (param.asInteger() == 0)
			GotoState(&Klaymen::stPressDoorButton);
		break;
	case 0x4817:
		setDoDeltaX(param.asInteger());
		gotoNextStateExt();
		break;
	case 0x481B:
		startWalkToAttachedSpriteXDistance(param.asInteger());
		break;
	case 0x4820:
		sendMessage(_parentScene, 0x2005, 0);
		GotoState(&Klaymen::stContinueClimbLadderUp);
		break;
	case 0x4821:
		sendMessage(_parentScene, 0x2005, 0);
		_destY = param.asInteger();
		GotoState(&Klaymen::stStartClimbLadderDown);
		break;
	case 0x4822:
		sendMessage(_parentScene, 0x2005, 0);
		_destY = param.asInteger();
		GotoState(&Klaymen::stStartClimbLadderUp);
		break;
	case 0x4823:
		sendMessage(_parentScene, 0x2006, 0);
		GotoState(&Klaymen::stClimbLadderHalf);
		break;
	case 0x482E:
		if (param.asInteger() == 1)
			GotoState(&Klaymen::stWalkToFrontNoStep);
		else
			GotoState(&Klaymen::stWalkToFront);
		break;
	case 0x482F:
		if (param.asInteger() == 1)
			GotoState(&Klaymen::stTurnToFront);
		else
			GotoState(&Klaymen::stTurnToBack);
		break;
	case 0x483F:
		startSpecialWalkRight(param.asInteger());
		break;
	case 0x4840:
		startSpecialWalkLeft(param.asInteger());
		break;
	}
	return 0;
}

KmScene1004::KmScene1004(NeverhoodEngine *vm, Scene *parentScene, int16 x, int16 y)
	: Klaymen(vm, parentScene, x, y) {

	_dataResource.load(0x01900A04);
}

uint32 KmScene1004::xHandleMessage(int messageNum, const MessageParam &param) {
	switch (messageNum) {
	case 0x4001:
	case 0x4800:
		startWalkToX(param.asPoint().x, false);
		break;
	case 0x4004:
		GotoState(&Klaymen::stTryStandIdle);
		break;
	case 0x4817:
		setDoDeltaX(param.asInteger());
		gotoNextStateExt();
		break;
	case 0x4818:
		startWalkToX(_dataResource.getPoint(param.asInteger()).x, false);
		break;
	case 0x481E:
		GotoState(&Klaymen::stReadNote);
		break;
	case 0x4820:
		sendMessage(_parentScene, 0x2000, 0);
		GotoState(&Klaymen::stContinueClimbLadderUp);
		break;
	case 0x4821:
		sendMessage(_parentScene, 0x2000, 0);
		_destY = param.asInteger();
		GotoState(&Klaymen::stStartClimbLadderDown);
		break;
	case 0x4822:
		sendMessage(_parentScene, 0x2000, 0);
		_destY = param.asInteger();
		GotoState(&Klaymen::stStartClimbLadderUp);
		break;
	case 0x4823:
		sendMessage(_parentScene, 0x2001, 0);
		GotoState(&Klaymen::stClimbLadderHalf);
		break;
	case 0x4824:
		sendMessage(_parentScene, 0x2000, 0);
		_destY = _dataResource.getPoint(param.asInteger()).y;
		GotoState(&Klaymen::stStartClimbLadderDown);
		break;
	case 0x4825:
		sendMessage(_parentScene, 0x2000, 0);
		_destY = _dataResource.getPoint(param.asInteger()).y;
		GotoState(&Klaymen::stStartClimbLadderUp);
		break;
	case 0x4828:
		GotoState(&Klaymen::stTurnToBackToUse);
		break;
	case 0x483F:
		startSpecialWalkRight(param.asInteger());
		break;
	case 0x4840:
		startSpecialWalkLeft(param.asInteger());
		break;
	}
	return 0;
}

} // End of namespace Neverhood
