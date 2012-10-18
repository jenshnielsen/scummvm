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

#ifndef NEVERHOOD_SCENE_H
#define NEVERHOOD_SCENE_H

#include "common/array.h"
#include "neverhood/neverhood.h"
#include "neverhood/background.h"
#include "neverhood/entity.h"
#include "neverhood/graphics.h"
#include "neverhood/klayman.h"
#include "neverhood/module.h"
#include "neverhood/palette.h"
#include "neverhood/smackerplayer.h"
#include "neverhood/sprite.h"
#include "neverhood/staticdata.h"

namespace Neverhood {

class Scene : public Entity {
public:
	Scene(NeverhoodEngine *vm, Module *parentModule, bool clearHitRects);
	virtual ~Scene();
	virtual void draw();
	void addEntity(Entity *entity);
	bool removeEntity(Entity *entity);
	void addSurface(BaseSurface *surface);
	bool removeSurface(BaseSurface *surface);
	Sprite *addSprite(Sprite *sprite);
	void removeSprite(Sprite *sprite);
	void setSurfacePriority(BaseSurface *surface, int priority);
	void setSpriteSurfacePriority(Sprite *sprite, int priority);
	void deleteSprite(Sprite **sprite);
	Background *addBackground(Background *background);
	void setBackground(uint32 fileHash);
	void changeBackground(uint32 fileHash);
	void setBackgroundY(int16 y) { _background->getSurface()->getDrawRect().y = y; }
	int16 getBackgroundY() { return _background->getSurface()->getDrawRect().y; }
	void setPalette(uint32 fileHash = 0);
	void setHitRects(uint32 id);
	Sprite *insertStaticSprite(uint32 fileHash, int surfacePriority);
	void insertMouse433(uint32 fileHash, NRect *mouseRect = NULL);
	void insertMouse435(uint32 fileHash, int16 x1, int16 x2);
	void insertNavigationMouse(uint32 fileHash, int type);
	void showMouse(bool visible);
	void changeMouseCursor(uint32 fileHash);
	SmackerPlayer *addSmackerPlayer(SmackerPlayer *smackerPlayer);
	void update();
	void leaveScene(uint32 result);
	// Some crazy templated functions to make the logic code smaller/simpler (imo!)
	// insertKlayman
	template<class T> 
	void insertKlayman() {
		_klayman = (T*)addSprite(new T(_vm, this));
	}
	template<class T, class Arg1> 
	void insertKlayman(Arg1 arg1) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1));
	}
	template<class T, class Arg1, class Arg2> 
	void insertKlayman(Arg1 arg1, Arg2 arg2) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1, arg2));
	}
	template<class T, class Arg1, class Arg2, class Arg3> 
	void insertKlayman(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1, arg2, arg3));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4> 
	void insertKlayman(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1, arg2, arg3, arg4));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5> 
	void insertKlayman(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1, arg2, arg3, arg4, arg5));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6> 
	void insertKlayman(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
		_klayman = (T*)addSprite(new T(_vm, this, arg1, arg2, arg3, arg4, arg5, arg6));
	}
	// insertSprite
	template<class T> 
	T* insertSprite() {
		return (T*)addSprite(new T(_vm));
	}
	template<class T, class Arg1> 
	T* insertSprite(Arg1 arg1) {
		return (T*)addSprite(new T(_vm, arg1));
	}
	template<class T, class Arg1, class Arg2> 
	T* insertSprite(Arg1 arg1, Arg2 arg2) {
		return (T*)addSprite(new T(_vm, arg1, arg2));
	}
	template<class T, class Arg1, class Arg2, class Arg3> 
	T* insertSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
		return (T*)addSprite(new T(_vm, arg1, arg2, arg3));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4> 
	T* insertSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
		return (T*)addSprite(new T(_vm, arg1, arg2, arg3, arg4));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5> 
	T* insertSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
		return (T*)addSprite(new T(_vm, arg1, arg2, arg3, arg4, arg5));
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6> 
	T* insertSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
		return (T*)addSprite(new T(_vm, arg1, arg2, arg3, arg4, arg5, arg6));
	}
	// createSprite
	template<class T> 
	T* createSprite() {
		return new T(_vm);
	}
	template<class T, class Arg1> 
	T* createSprite(Arg1 arg1) {
		return new T(_vm, arg1);
	}
	template<class T, class Arg1, class Arg2> 
	T* createSprite(Arg1 arg1, Arg2 arg2) {
		return new T(_vm, arg1, arg2);
	}
	template<class T, class Arg1, class Arg2, class Arg3> 
	T* createSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
		return new T(_vm, arg1, arg2, arg3);
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4> 
	T* createSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
		return new T(_vm, arg1, arg2, arg3, arg4);
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5> 
	T* createSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
		return new T(_vm, arg1, arg2, arg3, arg4, arg5);
	}
	template<class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5, class Arg6> 
	T* createSprite(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
		return new T(_vm, arg1, arg2, arg3, arg4, arg5, arg6);
	}
protected:
	Module *_parentModule;
	Common::Array<Entity*> _entities;
	Common::Array<BaseSurface*> _surfaces;

	bool _surfaceFlag;

	Klayman *_klayman;
	Background *_background;
	Palette *_palette;
	SmackerPlayer *_smackerPlayer;

	MessageList *_messageList;
	MessageList *_messageList2;
	int _messageListStatus;
	uint _messageListCount;
	uint _messageListIndex;
	bool _systemCallbackFlag;

	bool _canAcceptInput;
	bool _isKlaymanBusy;
	bool _isMessageListBusy;

	Mouse *_mouseCursor;
	NPoint _mouseClickPos;
	bool _mouseClicked;
	bool _mouseCursorWasVisible;

	int _rectType;
	RectList *_rectList;
	DataResource _dataResource;

	HitRectList _hitRectList;

	void (Entity::*_savedUpdateHandlerCb)();
	uint32 (Entity::*_savedMessageHandlerCb)(int messageNum, const MessageParam &param, Entity *sender);
	bool _smackerDone;
	int _messageValue;
	uint32 handleMessage(int messageNum, const MessageParam &param, Entity *sender);
	void smackerUpdate();
	uint32 smackerHandleMessage(int messageNum, const MessageParam &param, Entity *sender);
	bool queryPositionSprite(int16 mouseX, int16 mouseY);
	bool queryPositionRectList(int16 mouseX, int16 mouseY);
	void setMessageList(uint32 id, bool canAcceptInput = true, bool systemCallbackFlag = false);
	void setMessageList(MessageList *messageList, bool canAcceptInput = true, bool systemCallbackFlag = false);
	bool setMessageList2(uint32 id, bool canAcceptInput = true, bool systemCallbackFlag = false);
	bool setMessageList2(MessageList *messageList, bool canAcceptInput = true, bool systemCallbackFlag = false);
	void processMessageList();
	void setRectList(uint32 id);
	void setRectList(RectList *rectList);
	void clearRectList();
	void loadHitRectList();
	void cancelMessageList();
	void loadDataResource(uint32 fileHash);
	uint16 convertMessageNum(uint32 messageNum);
};

} // End of namespace Neverhood

#endif /* NEVERHOOD_SCENE_H */
