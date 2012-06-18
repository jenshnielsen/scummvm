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

#ifndef TONY_GLOBALS
#define TONY_GLOBALS

#include "common/savefile.h"
#include "tony/adv.h"
#include "tony/gfxengine.h"
#include "tony/input.h"
#include "tony/inventory.h"
#include "tony/loc.h"
#include "tony/tonychar.h"
#include "tony/mpal/mpal.h"
#include "tony/mpal/mpaldll.h"

namespace Tony {

#define TAPPETI_GRILLI 1
#define TAPPETI_GRILLIOV 2
#define TAPPETI_GRILLIVENTO 3
#define TAPPETI_GRILLIVENTO1 4
#define TAPPETI_VENTO 5
#define TAPPETI_MARE 6
#define TAPPETI_MAREMETA 7

#define MAX_PATH    255

struct CharacterStruct {
	uint32 _code;
	RMItem *_item;
	byte _r, _g, _b;
	int _talkPattern;
	int _standPattern;
	int _startTalkPattern, _endTalkPattern;
	int _numTexts;

	void save(Common::OutSaveFile *f) {
		f->writeUint32LE(_code);
		f->writeUint32LE(0);
		f->writeByte(_r);
		f->writeByte(_g);
		f->writeByte(_b);
		f->writeUint32LE(_talkPattern);
		f->writeUint32LE(_standPattern);
		f->writeUint32LE(_startTalkPattern);
		f->writeUint32LE(_endTalkPattern);
		f->writeUint32LE(_numTexts);
	}
	void load(Common::InSaveFile *f) {
		_code = f->readUint32LE();
		f->readUint32LE();
		_item = NULL;
		_r = f->readByte();
		_g = f->readByte();
		_b = f->readByte();
		_talkPattern = f->readUint32LE();
		_standPattern = f->readUint32LE();
		_startTalkPattern = f->readUint32LE();
		_endTalkPattern = f->readUint32LE();
		_numTexts = f->readUint32LE();
	}
};

struct MCharacterStruct {
	uint32 _code;
	RMItem *_item;
	byte _r, _g, _b;
	int _x, _y;
	int _numTalks[10];
	int _curGroup;
	int _numTexts;
	bool _bInTexts;
	int _curTalk;
	bool _bAlwaysBack;

	void save(Common::OutSaveFile *f) {
		f->writeUint32LE(_code);
		f->writeUint32LE(0);
		f->writeByte(_r);
		f->writeByte(_g);
		f->writeByte(_b);
		f->writeUint32LE(_x);
		f->writeUint32LE(_y);
		for (int i = 0; i < 10; ++i)
			f->writeUint32LE(_numTalks[i]);
		f->writeUint32LE(_curGroup);
		f->writeUint32LE(_numTexts);
		f->writeByte(_bInTexts);
		f->writeUint32LE(_curTalk);
		f->writeByte(_bAlwaysBack);
	}
	void load(Common::InSaveFile *f) {
		_code = f->readUint32LE();
		f->readUint32LE();
		_item = NULL;
		_r = f->readByte();
		_g = f->readByte();
		_b = f->readByte();
		_x = f->readUint32LE();
		_y = f->readUint32LE();
		for (int i = 0; i < 10; ++i)
			_numTalks[i] = f->readUint32LE();
		_curGroup = f->readUint32LE();
		_numTexts = f->readUint32LE();
		_bInTexts = f->readByte();
		_curTalk = f->readUint32LE();
		_bAlwaysBack = f->readByte();
	}
};

struct ChangedHotspotStruct {
	uint32 _dwCode;
	uint32 _nX, _nY;

	void save(Common::OutSaveFile *f) {
		f->writeUint32LE(_dwCode);
		f->writeUint32LE(_nX);
		f->writeUint32LE(_nY);
	}
	void load(Common::InSaveFile *f) {
		_dwCode = f->readUint32LE();
		_nX = f->readUint32LE();
		_nY = f->readUint32LE();
	}
};


/**
 * Description of a call to a custom function.
 */
typedef struct {
	int _nCf;

	int _arg1, _arg2, _arg3, _arg4;
} CFCALL;
typedef CFCALL         *LPCFCALL;
typedef LPCFCALL       *LPLPCFCALL;


/****************************************************************************\
*       Global variables
\****************************************************************************/

/**
 * Globals class
 */
class Globals {
public:
	Globals();

	char _nextMusic[MAX_PATH];
	bool _nextLoop;
	int _nextChannel;
	int _nextSync;
	int _curChannel;
	int _flipflop;
	// OSystem::MutexRef csMusic;
	CharacterStruct _character[16];
	MCharacterStruct _mCharacter[10];
	ChangedHotspotStruct _changedHotspot[256];
	bool _isMChar[16];
	bool _bAlwaysDisplay;
	RMPoint _saveTonyPos;
	int _saveTonyLoc;
	RMTextDialog *_curBackText;
	bool _bTonyIsSpeaking;
	int _curChangedHotspot;
	bool _bPatIrqFreeze;
	bool _bCfgInvLocked;
	bool _bCfgInvNoScroll;
	bool _bCfgTimerizedText;
	bool _bCfgInvUp;
	bool _bCfgAnni30;
	bool _bCfgAntiAlias;
	bool _bShowSubtitles;
	bool _bCfgTransparence;
	bool _bCfgInterTips;
	bool _bCfgDubbing;
	bool _bCfgMusic;
	bool _bCfgSFX;
	int  _nCfgTonySpeed;
	int  _nCfgTextSpeed;
	int  _nCfgDubbingVolume;
	int  _nCfgMusicVolume;
	int  _nCfgSFXVolume;
	bool _bSkipSfxNoLoop;
	bool _bIdleExited;
	bool _bNoBullsEye;
	int _curDialog;
	int _curSoundEffect;
	bool _bFadeOutStop;

	RMTony *_tony;
	RMPointer *_pointer;
	RMGameBoxes *_boxes;
	RMLocation *_loc;
	RMInventory *_inventory;
	RMInput *_input;
	RMGfxEngine *_gfxEngine;

	uint32(*LoadLocation)(int, RMPoint, RMPoint start);
	void (*UnloadLocation)(CORO_PARAM, bool bDoOnExit, uint32 *result);
	void (*LinkGraphicTask)(RMGfxTask *task);
	void (*Freeze)(void);
	void (*Unfreeze)(void);
	void (*WaitFrame)(CORO_PARAM);
	void (*PlayMusic)(int nChannel, const char *fileName, int nFX, bool bLoop, int nSync);
	void (*WaitWipeEnd)(CORO_PARAM);
	void (*CloseWipe)(void);
	void (*InitWipe)(int type);
	void (*EnableGUI)(void);
	void (*DisableGUI)(void);
	void (*SetPerorate)(bool bpal);

	uint32 _dwTonyNumTexts;
	bool _bTonyInTexts;
	bool _bStaticTalk;
	RMTony::CharacterTalkType _nTonyNextTalkType;

	RMPoint _startLocPos[256];
	uint32 _mut[10];

	bool _bSkipIdle;
	uint32 _hSkipIdle;

	int _lastMusic, _lastTappeto;

	int _tappeti[200];
	RMPoint _fullScreenMessagePt;
	int _fullScreenMessageLoc;

	/**
	 * @defgroup MPAL variables
	 */
	uint32                  _mpalError;
	LPITEMIRQFUNCTION       _lpiifCustom;
	LPLPCUSTOMFUNCTION      _lplpFunctions;
	Common::String         *_lplpFunctionStrings;
	uint16                  _nObjs;
	uint16                  _nVars;
	HGLOBAL                 _hVars;
	LPMPALVAR               _lpmvVars;
	uint16                  _nMsgs;
	HGLOBAL                 _hMsgs;
	LPMPALMSG               _lpmmMsgs;
	uint16                  _nDialogs;
	HGLOBAL                 _hDialogs;
	LPMPALDIALOG            _lpmdDialogs;
	uint16                  _nItems;
	HGLOBAL                 _hItems;
	LPMPALITEM              _lpmiItems;
	uint16                  _nLocations;
	HGLOBAL                 _hLocations;
	LPMPALLOCATION          _lpmlLocations;
	uint16                  _nScripts;
	HGLOBAL                 _hScripts;
	LPMPALSCRIPT            _lpmsScripts;
	Common::File            _hMpr;
	uint16                  _nResources;
	uint32                 *_lpResources;
	bool                    _bExecutingAction;
	bool                    _bExecutingDialog;
	uint32                  _nPollingLocations[MAXPOLLINGLOCATIONS];
	uint32                  _hEndPollingLocations[MAXPOLLINGLOCATIONS];
	uint32                  _pollingThreads[MAXPOLLINGLOCATIONS];
	uint32                  _hAskChoice;
	uint32                  _hDoneChoice;
	uint32                  _nExecutingAction;
	uint32                  _nExecutingDialog;
	uint32                  _nExecutingChoice;
	uint32                  _nSelectedChoice;
};

} // End of namespace Tony

#endif // TONY_GLOBALS
