/* ScummVM - Scumm Interpreter
 * Copyright (C) 2006 The ScummVM project
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

#include "parallaction/graphics.h"

namespace Parallaction {

byte Gfx::_mouseArrow[256] = {
	0x12, 0x11, 0x11, 0x11, 0x13, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x11, 0x13, 0x12, 0x12, 0x00,
	0x13, 0x12, 0x12, 0x11, 0x13, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x12,
	0x13, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x13, 0x13, 0x13, 0x12, 0x00,
	0x13, 0x00, 0x12, 0x00, 0x13, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x11, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00,
	0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x11, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
	0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00,
	0x13, 0x13, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00,
	0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x11, 0x12, 0x12, 0x12, 0x12,
	0x12, 0x12, 0x12, 0x13, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x13, 0x12, 0x12, 0x00,
	0x00, 0x12, 0x13, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x11, 0x11, 0x11, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x11, 0x00,
	0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x11,
	0x12, 0x12, 0x12, 0x00, 0x12, 0x12, 0x12, 0x00, 0x13, 0x12, 0x12, 0x00, 0x00, 0x12, 0x13, 0x00,
	0x00, 0x12, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



const char *_zoneFlagNames[] = {
	"closed",
	"active",
	"remove",
	"acting",
	"locked",
	"fixed",
	"noname",
	"nomasked",
	"looping",
	"added",
	"character",
	"nowalk",
	0
};

const char *_zoneTypeNames[] = {
	"examine",
	"door",
	"get",
	"merge",
	"taste",
	"hear",
	"feel",
	"speak",
	"none",
	"trap",
	"yourself",
	"Command",
	0
};

const char _gameNames[10][20] = {
	"GAME1",
	"GAME2",
	"GAME3",
	"GAME4",
	"GAME5",
	"GAME6",
	"GAME7",
	"GAME8",
	"GAME9",
	"GAME10"
};

const char *commands_names[] = {
	"set",
	"clear",
	"start",
	"speak",
	"get",
	"location",
	"open",
	"close",
	"on",
	"off",
	"call",
	"toggle",
	"drop",
	"quit",
	"move",
	"stop",
	0
};

const char *_instructionNames[] = {
	"on",
	"off",
	"x",
	"y",
	"z",
	"f",
	"loop",
	"endloop",
	"show",
	"inc",
	"dec",
	"set",
	"put",
	"call",
	"wait",
	"start",
	"sound",
	"move",
	0
};

const char *_callableNames[] = {
	"HBOff",
	"Projector",
	"StartIntro",
	"EndIntro",
	"MoveSheet",
	"Sketch",
	"Shade",
	"Score",
	"OffSound",
	"StartMusic",
	"CloseMusic",
	"Fade",
	"HBOn",
	"MoveSarc",
	"ContaFoglie",
	"ZeroFoglie",
	"Trasformata",
	"OffMouse",
	"OnMouse",
	"SetMask",
	"EndComment",
	"Frankenstain",
	"Finito",
	"Ridux",
	"TestResult",
	0
};


void _c_play_boogie(void*);
void _c_play_boogie(void*);
void _c_startIntro(void*);
void _c_endIntro(void*);
void _c_moveSheet(void*);
void _c_sketch(void*);
void _c_shade(void*);
void _c_score(void*);
void _c_fade(void*);
void _c_play_boogie(void*);
void _c_moveSarc(void*);
void _c_contaFoglie(void*);
void _c_zeroFoglie(void*);
void _c_trasformata(void*);
void _c_offMouse(void*);
void _c_onMouse(void*);
void _c_setMask(void*);
void _c_endComment(void*);
void _c_frankenstein(void*);
void _c_finito(void*);
void _c_ridux(void*);
void _c_testResult(void*);
void _c_null(void*);

typedef void (*callable)(void*);

callable _callables[] = {
	_c_play_boogie,
	_c_play_boogie,
	_c_startIntro,
	_c_endIntro,
	_c_moveSheet,
	_c_sketch,
	_c_shade,
	_c_score,
	_c_null,
	_c_null,
	_c_null,
	_c_fade,
	_c_play_boogie,
	_c_moveSarc,
	_c_contaFoglie,
	_c_zeroFoglie,
	_c_trasformata,
	_c_offMouse,
	_c_onMouse,
	_c_setMask,
	_c_endComment,
	_c_frankenstein,
	_c_finito,
	_c_ridux,
	_c_testResult
};


Credit _credits[] = {
	{"Music and Sound Effects", "MARCO CAPRELLI"},
	{"PC Version", "RICCARDO BALLARINO"},
	{"Project Manager", "LOVRANO CANEPA"},
	{"Production", "BRUNO BOZ"},
	{"Special Thanks to", "LUIGI BENEDICENTI - GILDA and DANILO"},
	{"Copyright 1992 Euclidea s.r.l ITALY", "All rights reserved"},
	{"CLICK MOUSE BUTTON TO START", 0}
};

const char *_dinoName = "dino";
const char *_donnaName = "donna";
const char *_doughName = "dough";
const char *_drkiName = "drki";

const char *_minidinoName = "minidino";
const char *_minidonnaName = "minidonna";
const char *_minidoughName = "minidough";
const char *_minidrkiName = "minidrki";


} // namespace Parallaction
