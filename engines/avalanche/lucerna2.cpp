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

/*
 * This code is based on the original source code of Lord Avalot d'Argent version 1.3.
 * Copyright (c) 1994-1995 Mike, Mark and Thomas Thurman.
 */

/* LUCERNA		The screen, [keyboard] and mouse handler.*/

#include "avalanche/avalanche.h"

#include "avalanche/lucerna2.h"
#include "avalanche/gyro2.h"
#include "avalanche/scrolls2.h"
#include "avalanche/logger2.h"
#include "avalanche/enhanced2.h"
#include "avalanche/visa2.h"
#include "avalanche/timeout2.h"
#include "avalanche/trip6.h"
#include "avalanche/enid2.h"
#include "avalanche/celer2.h"
#include "avalanche/pingo2.h"
#include "avalanche/sequence2.h"
#include "avalanche/acci2.h"
#include "avalanche/roomnums.h"

#include "common/system.h"

#include "graphics/palette.h"

//#include "dropdown.h"
//#include "basher.h"


namespace Avalanche {

Lucerna::Lucerna() : fxhidden(false) {}

void Lucerna::setParent(AvalancheEngine *vm) {
	_vm = vm;
}

void Lucerna::init() {
	_vm->_gyro.oh = 17717;
	_vm->_gyro.om = 17717;
	if (_vm->_enhanced.atbios)  _vm->_gyro.atkey = "f1";
	else _vm->_gyro.atkey = "alt-";
}
	
void Lucerna::callverb(char n) {
	if (n == _vm->_acci.pardon) {
		_vm->_scrolls.display(Common::String("The f5 key lets you do a particular action in certain ") +
			"situations. However, at the moment there is nothing " +
			"assigned to it. You may press alt-A to see what the " +
			"current setting of this key is.");
	} else {
		_vm->_gyro.weirdword = false;
		_vm->_acci.polite = true;
		_vm->_acci.verb = n;
		_vm->_acci.do_that();
	}
}

void Lucerna::draw_also_lines() {
	byte ff;
	byte squeaky_code;

	warning("STUB: Lucerna::draw_also_lines()");
}



// nextstring, scram1 and unscrable are only used in load_also 

Common::String Lucerna::nextstring() {
	Common::String str;
	byte length = f.readByte();
	for (int i = 0; i < length; i++)
		str += f.readByte();
	return str;
}

void Lucerna::scram1(Common::String &x) {
	for (int fz = 0; fz < x.size(); fz ++)
		x.setChar(x[fz] ^ 177, fz);
}

void Lucerna::unscramble() {
	for (byte fv = 0; fv < 31; fv ++)
		for (byte ff = 0; ff < 2; ff ++)
			if (_vm->_gyro.also[fv][ff] != 0)
				scram1(*_vm->_gyro.also[fv][ff]);
	scram1(_vm->_gyro.listen);
	scram1(_vm->_gyro.flags);
	/*     for fz:=1 to length(also[fv,ff]^) do
	      also[fv,ff]^[fz]:=chr(ord(also[fv,ff]^[fz]) xor 177);*/
}

void Lucerna::load_also(Common::String n) {
	byte ff, fv;
	
	for (fv = 0; fv < 31; fv++)
		for (ff = 0; ff < 2; ff++)
			if (_vm->_gyro.also[fv][ff] != 0)  {
				delete _vm->_gyro.also[fv][ff];
				_vm->_gyro.also[fv][ff] = 0;
			}

	Common::String filename;
	filename = filename.format("also%s.avd", n.c_str());
	if (!f.open(filename)) {
		warning("AVALANCHE: Lucerna: File not found: %s", filename.c_str());
		return;
	}

	f.seek(128);

	byte minnames = f.readByte();
	for (fv = 0; fv <= minnames; fv++) {
		for (ff = 0; ff < 2; ff++) {
			_vm->_gyro.also[fv][ff] = new Common::String;
			*_vm->_gyro.also[fv][ff] = nextstring();
		}
		*_vm->_gyro.also[fv][0] = Common::String(157) + *_vm->_gyro.also[fv][0] + 157;
	}
	
	memset(_vm->_gyro.lines, 0xFF, sizeof(_vm->_gyro.lines));
	
	//fv = getpixel(0, 0);
	fv = f.readByte();
	for (byte i = 0; i < fv; i++) {
		_vm->_gyro.lines[i].x1 = f.readSint16LE();
		_vm->_gyro.lines[i].y1 = f.readSint16LE();
		_vm->_gyro.lines[i].x2 = f.readSint16LE();
		_vm->_gyro.lines[i].y2 = f.readSint16LE();
		_vm->_gyro.lines[i].col = f.readByte();
	}

	memset(_vm->_gyro.peds, 177, sizeof(_vm->_gyro.peds));
	fv = f.readByte();
	for (byte i = 0; i < fv; i++) {
		_vm->_gyro.peds[i].x = f.readSint16LE();
		_vm->_gyro.peds[i].y = f.readSint16LE();
		_vm->_gyro.peds[i].dir = f.readByte();
	}
	
	_vm->_gyro.numfields = f.readByte();
	for (byte i = 0; i < _vm->_gyro.numfields; i++) {
		_vm->_gyro.fields[i].x1 = f.readSint16LE();
		_vm->_gyro.fields[i].y1 = f.readSint16LE();
		_vm->_gyro.fields[i].x2 = f.readSint16LE();
		_vm->_gyro.fields[i].y2 = f.readSint16LE();
	}

	for (byte i = 0; i < 15; i++) {
		_vm->_gyro.magics[i].op = f.readByte();
		_vm->_gyro.magics[i].data = f.readUint16LE();
	}

	for (byte i = 0; i < 7; i++) {
		_vm->_gyro.portals[i].op = f.readByte();
		_vm->_gyro.portals[i].data = f.readUint16LE();
	}

	_vm->_gyro.flags.clear();
	for (byte i = 0;  i < 26; i++)
		_vm->_gyro.flags += f.readByte();

	int16 listen_length = f.readByte();
	_vm->_gyro.listen.clear();
	for (byte i = 0; i < listen_length; i++)
		_vm->_gyro.listen += f.readByte();
	
	draw_also_lines();

	//setactivepage(1);
	warning("STUB: Lucerna::load_also()");

	f.close();
	unscramble();
	for (fv = 0; fv <= minnames; fv++)
		*_vm->_gyro.also[fv][0] = Common::String(',') + *_vm->_gyro.also[fv][0] + ',';
}

void Lucerna::load(byte n) {     /* Load2, actually */
	byte a0;  /*absolute $A000:800;*/
	byte a1;  /*absolute $A000:17184;*/
	byte bit;
	Common::String xx;
	bool was_virtual;

	was_virtual = _vm->_gyro.visible == _vm->_gyro.m_virtual;
	if (was_virtual)
		_vm->_gyro.off_virtual();
	else
		_vm->_gyro.off();

	_vm->_gyro.clear_vmc();

	xx = _vm->_gyro.strf(n);
	flesh_colours();

	Common::String filename;
	filename = filename.format("place%s.avd", xx.c_str());
	if (!f.open(filename)) {
		warning("AVALANCHE: Lucerna: File not found: %s", filename.c_str());
		return;
	}

	f.seek(146);
	for (byte i = 0; i < 30; i++)
		_vm->_gyro.roomname += f.readByte();
	/* Compression method byte follows this... */

	f.seek(177);

	/*for (bit = 0; bit <= 3; bit++) {
	port[0x3c4] = 2;
	port[0x3ce] = 4;
	port[0x3c5] = 1 << bit;
	port[0x3cf] = bit;
	blockread(f, a0, 12080);
	move(a0, a1, 12080);
	}*/

	Graphics::Surface background;
	
	uint16 backgroundWidht = _vm->_graph._screenWidth;
	byte backgroundHeight = 8 * 12080 / _vm->_graph._screenWidth; // With 640 width it's 151
	// The 8 = number of bits in a byte, and 12080 comes from the original code (see above)

	background.create(backgroundWidht, backgroundHeight, Graphics::PixelFormat::createFormatCLUT8());

	for (byte plane = 0; plane < 4; plane++)
		for (uint16 y = 0; y < backgroundHeight; y++)
			for (uint16 x = 0; x < backgroundWidht; x += 8) {
				byte pixel = f.readByte();
				for (byte i = 0; i < 8; i++) {
					byte pixelBit = (pixel >> i) & 1;
					*(byte *)background.getBasePtr(x + 7 - i, y) += (pixelBit << plane);
				}	
			}

	for (uint16 y = 0; y < backgroundHeight; y++)
		for (uint16 x = 0; x < backgroundWidht; x++)
			*_vm->_graph.getPixel(x + 0, y + 10) = *(byte *)background.getBasePtr(x, y);	

	background.free();

	f.close();



	load_also(xx);
	_vm->_celer.load_chunks(xx);

	_vm->_graph.refreshScreen(); // _vm->_pingo.copy03();  -  See Avalot::setup()

	bit = *_vm->_graph.getPixel(0,0);

	_vm->_logger.log_newroom(_vm->_gyro.roomname);

	if (was_virtual)
		_vm->_gyro.on_virtual();
	else 
		_vm->_gyro.on();
}



void Lucerna::zoomout(int16 x, int16 y) {
	int16 x1, y1, x2, y2;
	byte fv;

	warning("STUB: Lucerna::zoomout()");
}

void Lucerna::find_people(byte room) {
	for (byte fv = 151; fv <= 178; fv++)
		if (_vm->_gyro.whereis[fv] == room) {
			if (fv < 175)
				_vm->_gyro.him = fv;
			else
				_vm->_gyro.her = fv;
		}
}

void Lucerna::exitroom(byte x) {
	//nosound();
	_vm->_celer.forget_chunks();
	_vm->_gyro.seescroll = true;  /* This stops the trippancy system working over the length of this procedure. */

	switch (x) {
	case r__spludwicks:
		_vm->_timeout.lose_timer(_vm->_timeout.reason_avariciustalks);
		 _vm->_gyro.dna.avaricius_talk = 0;
		/* He doesn't HAVE to be talking for this to work. It just deletes it IF it exists. */       
		break;
	case r__bridge:
		if (_vm->_gyro.dna.drawbridge_open > 0) {
			_vm->_gyro.dna.drawbridge_open = 4; /* Fully open. */
			_vm->_timeout.lose_timer(_vm->_timeout.reason_drawbridgefalls);
		}
		break;
	case r__outsidecardiffcastle:
		_vm->_timeout.lose_timer(_vm->_timeout.reason_cardiffsurvey);
		break;
	case r__robins:
		_vm->_timeout.lose_timer(_vm->_timeout.reason_getting_tied_up);
		break;
	}

	_vm->_gyro.interrogation = 0; /* Leaving the room cancels all the questions automatically. */

	_vm->_gyro.seescroll = false; /* Now it can work again! */

	_vm->_gyro.dna.last_room = _vm->_gyro.dna.room;
	if (_vm->_gyro.dna.room != r__map)
		_vm->_gyro.dna.last_room_not_map = _vm->_gyro.dna.room;
}

void Lucerna::new_town() {   /* You've just entered a town from the map. */
	_vm->_dropdown.standard_bar();

	switch (_vm->_gyro.dna.room) {
	case r__outsidenottspub: /* Entry into Nottingham. */
		if ((_vm->_gyro.dna.rooms[r__robins] > 0) && (_vm->_gyro.dna.been_tied_up) && (! _vm->_gyro.dna.taken_mushroom))
			_vm->_gyro.dna.mushroom_growing = true;
		break;
	case r__wisewomans: { /* Entry into Argent. */
		if (_vm->_gyro.dna.talked_to_crapulus && (! _vm->_gyro.dna.lustie_is_asleep)) {
			_vm->_gyro.dna.spludwicks_here = !((_vm->_gyro.dna.rooms[r__wisewomans] % 3) == 1);
			_vm->_gyro.dna.crapulus_will_tell = ! _vm->_gyro.dna.spludwicks_here;
		} else {
			_vm->_gyro.dna.spludwicks_here = true;
			_vm->_gyro.dna.crapulus_will_tell = false;
		}
		if (_vm->_gyro.dna.box_contents == _vm->_gyro.wine)  _vm->_gyro.dna.winestate = 3; /* Vinegar */
	}
	break;
	}

	if (_vm->_gyro.dna.room != r__outsideducks) {
		if ((_vm->_gyro.dna.obj[_vm->_gyro.onion]) && !(_vm->_gyro.dna.onion_in_vinegar))
			_vm->_gyro.dna.rotten_onion = true; /* You're holding the onion */
	}
}



void Lucerna::put_geida_at(byte whichped, byte &ped) {
	if (ped == 0)
		return;
	_vm->_trip.tr[2].init(5, false, &_vm->_trip); /* load Geida */
	_vm->_trip.apped(2, whichped);
	_vm->_trip.tr[2].call_eachstep = true;
	_vm->_trip.tr[2].eachstep = _vm->_trip.procgeida_procs;
}

void Lucerna::enterroom(byte x, byte ped) {
	_vm->_gyro.seescroll = true;  /* This stops the trippancy system working over the length of this procedure. */

	find_people(x);
	_vm->_gyro.dna.room = x;
	if (ped != 0)
		_vm->_gyro.dna.rooms[x]++;

	load(x);

	if ((_vm->_gyro.dna.rooms[x] == 0) && (! _vm->_gyro.flagset('S')))
		points(1);

	_vm->_gyro.whereis[_vm->_gyro.pavalot] = _vm->_gyro.dna.room;

	if (_vm->_gyro.dna.geida_follows)
		_vm->_gyro.whereis[_vm->_gyro.pgeida] = x;

	_vm->_gyro.roomtime = 0;

	
	if ((_vm->_gyro.dna.last_room == r__map) && (_vm->_gyro.dna.last_room_not_map != _vm->_gyro.dna.room))
		new_town();
	

	switch (x) {
	case r__yours:
		if (_vm->_gyro.dna.avvy_in_bed) {
			_vm->_celer.show_one(3);
			_vm->_timeout.set_up_timer(100, _vm->_timeout.procarkata_shouts, _vm->_timeout.reason_arkata_shouts);
		}
		break;

	case r__outsideyours:
		if (ped > 0) {
			if (! _vm->_gyro.dna.talked_to_crapulus) {

				_vm->_gyro.whereis[_vm->_gyro.pcrapulus] = r__outsideyours;
				_vm->_trip.tr[2].init(8, false, &_vm->_trip); /* load Crapulus */

				if (_vm->_gyro.dna.rooms[r__outsideyours] == 1) {
					_vm->_trip.apped(2, 4); /* Start on the right-hand side of the screen. */
					_vm->_trip.tr[2].walkto(5); /* Walks up to greet you. */
				} else {
					_vm->_trip.apped(2, 5); /* Starts where he was before. */
					_vm->_trip.tr[2].face = 3;
				}

				_vm->_trip.tr[2].call_eachstep = true;
				_vm->_trip.tr[2].eachstep = _vm->_trip.procface_avvy; /* He always faces Avvy. */

			} else _vm->_gyro.whereis[_vm->_gyro.pcrapulus] = r__nowhere;

			if (_vm->_gyro.dna.crapulus_will_tell) {
				_vm->_trip.tr[2].init(8, false, &_vm->_trip);
				_vm->_trip.apped(2, 2);
				_vm->_trip.tr[2].walkto(4);
				_vm->_timeout.set_up_timer(20, _vm->_timeout.proccrapulus_splud_out, _vm->_timeout.reason_crapulus_says_spludwick_out);
				_vm->_gyro.dna.crapulus_will_tell = false;
			}
		}
		break;

	/* Need to find out which room is this!
	 
	 case r__outsidespludwicks:
		if ((_vm->_gyro.dna.rooms[r__outsidespludwicks] == 1) && (ped == 1)) {
			_vm->_timeout.set_up_timer(20, _vm->_timeout.procbang, _vm->_timeout.reason_explosion);
			_vm->_gyro.dna.spludwicks_here = true;
		}
		break;*/

	case r__spludwicks:
		if (_vm->_gyro.dna.spludwicks_here) {
			if (ped > 0) {
				_vm->_trip.tr[2].init(2, false, &_vm->_trip); /* load Spludwick */
				_vm->_trip.apped(2, 2);
				_vm->_gyro.whereis['\227'] = r__spludwicks;
			}

			_vm->_gyro.dna.dogfoodpos = 0;  /* _vm->_gyro.also Spludwick pos. */

			_vm->_trip.tr[2].call_eachstep = true;
			_vm->_trip.tr[2].eachstep = _vm->_trip.procgeida_procs;
		} else _vm->_gyro.whereis['\227'] = r__nowhere;
		break;

	case r__brummieroad: {
		if (_vm->_gyro.dna.geida_follows)  put_geida_at(5, ped);
		if (_vm->_gyro.dna.cwytalot_gone) {
			_vm->_gyro.magics[lightred].op = _vm->_gyro.nix;
			_vm->_gyro.whereis[_vm->_gyro.pcwytalot] = r__nowhere;
		} else {
			if (ped > 0) {
				_vm->_trip.tr[2].init(4, false, &_vm->_trip); /* 4=Cwytalot*/
				_vm->_trip.tr[2].call_eachstep = true;
				_vm->_trip.tr[2].eachstep = _vm->_trip.procfollow_avvy_y;
				_vm->_gyro.whereis[_vm->_gyro.pcwytalot] = r__brummieroad;

				if (_vm->_gyro.dna.rooms[r__brummieroad] == 1) { /* First time here... */
					_vm->_trip.apped(2, 2); /* He appears on the right of the screen... */
					_vm->_trip.tr[2].walkto(4); /* ...and he walks up... */
				} else {
					/* You've been here before. */
					_vm->_trip.apped(2, 4); /* He's standing in your way straight away... */
					_vm->_trip.tr[2].face = _vm->_trip.left;
				}
			}
		}
	}
	break;

	case r__argentroad: {
		dnatype &with = _vm->_gyro.dna;
		if ((with.cwytalot_gone) && (! with.cwytalot_in_herts) && (ped == 2) &&
		        (_vm->_gyro.dna.rooms[r__argentroad] > 3)) {
			_vm->_trip.tr[2].init(4, false, &_vm->_trip); /* 4=Cwytalot again*/
			_vm->_trip.apped(2, 1);
			_vm->_trip.tr[2].walkto(2);
			_vm->_trip.tr[2].vanishifstill = true;
			with.cwytalot_in_herts = true;
			/*_vm->_gyro.whereis[#157]:=r__Nowhere;*/ /* can we fit this in? */
			_vm->_timeout.set_up_timer(20, _vm->_timeout.proc_cwytalot_in_herts, _vm->_timeout.reason_cwytalot_in_herts);
		}
	}
	break;

	case r__bridge: {
		if (_vm->_gyro.dna.drawbridge_open == 4) { /*open*/
			_vm->_celer.show_one(3); /* Position of drawbridge */
			_vm->_gyro.magics[green].op = _vm->_gyro.nix; /* You may enter the drawbridge. */
		}
		if (_vm->_gyro.dna.geida_follows)  put_geida_at(ped + 3, ped); /* load Geida */
	}
	break;

	case r__robins: {
		if (ped > 0) {
			if (! _vm->_gyro.dna.been_tied_up) {
				/* A welcome party... or maybe not... */
				_vm->_trip.tr[2].init(6, false, &_vm->_trip);
				_vm->_trip.apped(2, 2);
				_vm->_trip.tr[2].walkto(3);
				_vm->_timeout.set_up_timer(36, _vm->_timeout.procget_tied_up, _vm->_timeout.reason_getting_tied_up);
			}
		}

		if (_vm->_gyro.dna.been_tied_up) {
			_vm->_gyro.whereis[_vm->_gyro.probinhood] = 0;
			_vm->_gyro.whereis[_vm->_gyro.pfriartuck] = 0;
		}

		if (_vm->_gyro.dna.tied_up)
			_vm->_celer.show_one(2);

		if (! _vm->_gyro.dna.mushroom_growing) 
			_vm->_celer.show_one(3);
	}
	break;

	case r__outsidecardiffcastle: {
		if (ped > 0)
			switch (_vm->_gyro.dna.cardiff_things) {
			case 0 : { /* You've answered NONE of his questions. */
				_vm->_trip.tr[2].init(9, false, &_vm->_trip);
				_vm->_trip.apped(2, 2);
				_vm->_trip.tr[2].walkto(3);
				_vm->_timeout.set_up_timer(47, _vm->_timeout.proccardiffsurvey, _vm->_timeout.reason_cardiffsurvey);
			}
			break;
			case 5 :
				_vm->_gyro.magics[2].op = _vm->_gyro.nix;
				break; /* You've answered ALL his questions. => nothing happens. */
			default: { /* You've answered SOME of his questions. */
				_vm->_trip.tr[2].init(9, false, &_vm->_trip);
				_vm->_trip.apped(2, 3);
				_vm->_trip.tr[2].face = _vm->_trip.right;
				_vm->_timeout.set_up_timer(3, _vm->_timeout.proccardiff_return, _vm->_timeout.reason_cardiffsurvey);
			}
			}
		if (_vm->_gyro.dna.cardiff_things < 5)
			_vm->_gyro.interrogation = _vm->_gyro.dna.cardiff_things;
		else _vm->_gyro.interrogation = 0;
	}
	break;

	case r__map: {
		warning("STUB: Lucerna::enterroom() - case: r__map");
	}
	break;

	case r__catacombs: {
		warning("STUB: Lucerna::enterroom() - case: r__catacombs");
	}
	break;

	case r__argentpub: {
		if (_vm->_gyro.dna.wonnim)  _vm->_celer.show_one(1);   /* No lute by the settle. */
		_vm->_gyro.dna.malagauche = 0; /* Ready to boot Malagauche */
		if (_vm->_gyro.dna.givenbadgetoiby) {
			_vm->_celer.show_one(8);
			_vm->_celer.show_one(9);
		}
	}
	break;

	case r__lustiesroom: {
		_vm->_gyro.dna.dogfoodpos = 1; /* Actually, du Lustie pos. */
		if (_vm->_trip.tr[1].whichsprite == 0) /* Avvy in his normal clothes */
			_vm->_timeout.set_up_timer(3, _vm->_timeout.proccallsguards, _vm->_timeout.reason_du_lustie_talks);
		else if (! _vm->_gyro.dna.entered_lusties_room_as_monk) /*already*/
			/* Presumably, Avvy dressed as a monk. */
			_vm->_timeout.set_up_timer(3, _vm->_timeout.procgreetsmonk, _vm->_timeout.reason_du_lustie_talks);

		if (_vm->_gyro.dna.geida_follows) {
			put_geida_at(5, ped);
			if (_vm->_gyro.dna.lustie_is_asleep)  _vm->_celer.show_one(5);
		}
	}
	break;

	case r__musicroom: {
		if (_vm->_gyro.dna.jacques_awake > 0) {
			_vm->_gyro.dna.jacques_awake = 5;
			_vm->_celer.show_one(2);
			_vm->_celer.show_one(4);
			_vm->_gyro.magics[brown].op = _vm->_gyro.nix;
			_vm->_gyro.whereis[_vm->_gyro.pjacques] = 0;
		}
		if (ped != 0) {
			_vm->_celer.show_one(6);
			_vm->_sequence.first_show(5);
			_vm->_sequence.then_show(7);
			_vm->_sequence.start_to_close();
		}
	}
	break;

	case r__outsidenottspub:
		if (ped == 2) {
			_vm->_celer.show_one(3);
			_vm->_sequence.first_show(2);
			_vm->_sequence.then_show(1);
			_vm->_sequence.then_show(4);
			_vm->_sequence.start_to_close();
		}
		break;

	case r__outsideargentpub:
		if (ped == 2)  {
			_vm->_celer.show_one(6);
			_vm->_sequence.first_show(5);
			_vm->_sequence.then_show(7);
			_vm->_sequence.start_to_close();
		}
		break;

	case r__wisewomans: {
		_vm->_trip.tr[2].init(11, false, &_vm->_trip);
		if ((_vm->_gyro.dna.rooms[r__wisewomans] == 1) && (ped > 0)) {
			_vm->_trip.apped(2, 2); /* Start on the right-hand side of the screen. */
			_vm->_trip.tr[2].walkto(4); /* Walks up to greet you. */
		} else {
			_vm->_trip.apped(2, 4); /* Starts where she was before. */
			_vm->_trip.tr[2].face = 3;
		}

		_vm->_trip.tr[2].call_eachstep = true;
		_vm->_trip.tr[2].eachstep = _vm->_trip.procface_avvy; /* She always faces Avvy. */
	}
	break;

	case r__insidecardiffcastle:
		if (ped > 0) {
			_vm->_trip.tr[2].init(10, false, &_vm->_trip); /* Define the dart. */
			_vm->_sequence.first_show(1);
			if (_vm->_gyro.dna.arrow_in_the_door)
				_vm->_sequence.then_show(3);
			else
				_vm->_sequence.then_show(2);

			if (_vm->_gyro.dna.taken_pen)
				_vm->_celer.show_one(4);

			_vm->_sequence.start_to_close();
		} else {
			_vm->_celer.show_one(1);
			if (_vm->_gyro.dna.arrow_in_the_door)  _vm->_celer.show_one(3);
			else _vm->_celer.show_one(2);
		}
		break;

	case r__avvysgarden:
		if (ped == 1)  {
			_vm->_celer.show_one(2);
			_vm->_sequence.first_show(1);
			_vm->_sequence.then_show(3);
			_vm->_sequence.start_to_close();
		}
		break;

	case r__entrancehall:
	case r__insideabbey:
		if (ped == 2)  {
			_vm->_celer.show_one(2);
			_vm->_sequence.first_show(1);
			_vm->_sequence.then_show(3);
			_vm->_sequence.start_to_close();
		}
		break;

	case r__aylesoffice:
		if (_vm->_gyro.dna.ayles_is_awake)
			_vm->_celer.show_one(2);
		break; /* Ayles awake. */

	case r__geidas:
		put_geida_at(2, ped);
		break; /* load Geida */

	case r__easthall:
	case r__westhall:
		if (_vm->_gyro.dna.geida_follows)  put_geida_at(ped + 2, ped);
		break;

	case r__lusties:
		if (_vm->_gyro.dna.geida_follows)  put_geida_at(ped + 6, ped);
		break;

	case r__nottspub: {
		if (_vm->_gyro.dna.sitting_in_pub)  _vm->_celer.show_one(3);
		_vm->_gyro.dna.dogfoodpos = 1; /* Actually, du Lustie pos. */
	}
	break;

	case r__outsideducks:
		if (ped == 2) {
			/* Shut the door */
			_vm->_celer.show_one(3);
			_vm->_sequence.first_show(2);
			_vm->_sequence.then_show(1);
			_vm->_sequence.then_show(4);
			_vm->_sequence.start_to_close();
		}
		break;
	case r__ducks:
		_vm->_gyro.dna.dogfoodpos = 1;
		break; /* Actually, Duck pos. */

	}

	_vm->_gyro.seescroll = false; /* Now it can work again! */

}

void Lucerna::thinkabout(byte z, bool th) {     /* Hey!!! Get it and put it!!! */
	const int16 picsize = 966;
	const bytefield thinkspace = {25, 170, 32, 200};

	_vm->_gyro.thinks = z;
	z--;

	_vm->_gyro.wait();

	if (th) {
		if (!f.open("thinks.avd")) {
			warning("AVALANCHE: Lucerna: File not found: thinks.avd");
			return;
		}
	} else {
		if (!f.open("folk.avd")) {
			warning("AVALANCHE: Lucerna: File not found: folk.avd");
			return;
		}

		z = z - 149;
		if (z >= 25)
			z -= 8;
		if (z == 20) 
			z--; /* Last time... */

	}

	f.seek(z * picsize + 65);

	byte *buffer = new byte[picsize];

	f.read(buffer, picsize);

	_vm->_graph.drawPicture(buffer, 205, 170);

	delete[] buffer;

	f.close();

	_vm->_gyro.off();

	/*setactivepage(3);
	putimage(x, y, p, 0);
	setactivepage(1 - cp);*/

	for (byte fv = 0; fv <= 1; fv ++)
		_vm->_trip.getset[fv].remember(thinkspace);
	
	_vm->_gyro.on();
	_vm->_gyro.thinkthing = th;
}

void Lucerna::load_digits() {   /* Load the scoring digits & rwlites */
	const byte digitsize = 134;
	const byte rwlitesize = 126;

	if (!f.open("digit.avd")) {
		warning("AVALANCHE: Lucerna: File not found: digit.avd");
		return;
	}

	for (byte fv = 0; fv < 10; fv ++) {
		_vm->_gyro.digit[fv] = new byte[digitsize];
		f.read(_vm->_gyro.digit[fv], digitsize);
	}

	for (byte ff = 0; ff < 9; ff ++) {
		_vm->_gyro.rwlite[ff] = new byte[rwlitesize];
		f.read(_vm->_gyro.rwlite[ff], rwlitesize);
	}

	f.close();
}

void Lucerna::toolbar() {
	if (!f.open("useful.avd")) {
		warning("AVALANCHE: Lucerna: File not found: useful.avd");
		return;
	}

	f.seek(40);
	
	/* off;*/

	uint32 bufferSize = f.size()-40;

	byte *buffer = new byte[bufferSize];

	f.read(buffer, bufferSize);

	_vm->_graph.drawPicture(buffer, 5, 169);

	delete[] buffer;

	f.close();

	/* on;*/

	_vm->_gyro.oldrw = 177;
	showrw();
}

void Lucerna::showscore() {

	const bytefield scorespace = {33, 177, 39, 200};

	if (_vm->_gyro.demo)
		return;

	uint16 score = _vm->_gyro.dna.score;
	int8 numbers[3] = {0, 0, 0};
	for (byte i = 0; i < 2; i++) {
		byte divisor = 1;
		for (byte j = 0; j < (2 - i); j++)
			divisor *= 10;
		numbers[i] = score / divisor;
		score -= numbers[i] * divisor;
	}
	numbers[2] = score;

	_vm->_gyro.off();

	//setactivepage(3);

	for (byte fv = 0; fv < 3; fv ++)
		if (_vm->_gyro.lastscore[fv] != numbers[fv]) 
			_vm->_graph.drawPicture(_vm->_gyro.digit[numbers[fv]], 250 + (fv + 1) * 15, 177);

	for (byte fv = 0; fv < 2; fv ++)
		_vm->_trip.getset[fv].remember(scorespace);

	//setactivepage(1 - cp);

	_vm->_gyro.on();
	for (byte i = 0; i < 3; i++)
		_vm->_gyro.lastscore[i] = numbers[i];
}

void Lucerna::points(byte num) {     /* Add on no. of points */
	for (byte q = 1; q <= num; q ++) {
		_vm->_gyro.dna.score++;
		/*if (soundfx)
			for (byte fv = 1; fv <= 97; fv ++)
				sound(177 + dna.score * 3);
		nosound;*/
	}
	warning("STUB: Lucerna::points()");

	_vm->_logger.log_score(num, _vm->_gyro.dna.score);
	showscore();
}

void Lucerna::topcheck() {
	/* Menuset */
	_vm->_dropdown.ddm_m.getmenu(_vm->_gyro.mpx);
	/* Do this one */
}

void Lucerna::mouseway() {
	warning("STUB: Lucerna::mouseway()");
}

void Lucerna::inkey() {
	char r;


	if (_vm->_gyro.demo)
		return; /* Demo handles this itself. */

	if (_vm->_gyro.mousetext == "") {
		/* read keyboard */
		_vm->_enhanced.readkeye();
		if ((_vm->_enhanced.inchar == ' ') && ((_vm->_enhanced.shiftstate & 8) > 0)) {
			_vm->_enhanced.inchar = '\0';
			_vm->_enhanced.extd = '#'; /* alt-spacebar = alt-H */
		}
	} else {
		if (_vm->_gyro.mousetext[0] == '`')
			_vm->_gyro.mousetext.setChar(13,0); /* Backquote = return in a macro */
		_vm->_enhanced.inchar = _vm->_gyro.mousetext[0];
		_vm->_gyro.mousetext = Common::String(_vm->_gyro.mousetext.c_str() + 2, 253);
	}
}

void Lucerna::posxy() {
	warning("STUB: Lucerna::posxy()");
}

void Lucerna::fxtoggle() {
	warning("STUB: Lucerna::fxtoggle()");
}

void Lucerna::objectlist() {
	_vm->_gyro.dna.carrying = 0;
	if (_vm->_gyro.thinkthing && ! _vm->_gyro.dna.obj[_vm->_gyro.thinks])
		thinkabout(_vm->_gyro.money, _vm->_gyro.a_thing); /* you always have money */
	for (byte fv = 0; fv < numobjs; fv ++)
		if (_vm->_gyro.dna.obj[fv]) {
			_vm->_gyro.dna.carrying ++;
			_vm->_gyro.objlist[_vm->_gyro.dna.carrying] = fv + 1;
		}
}

void Lucerna::verte() {
	byte what;

	if (! _vm->_gyro.dna.user_moves_avvy) 
		return;


	/* _vm->_trip.tr[0] : that's the only one we're interested in here */
	if (_vm->_gyro.mx < _vm->_trip.tr[0].x) 
		what = 1;
	else if (_vm->_gyro.mx > (unsigned char)(_vm->_trip.tr[0].x + _vm->_trip.tr[0]._info.xl)) 
		what = 2;
	else
		what = 0; /* On top */

	if (_vm->_gyro.my < _vm->_trip.tr[0].y)
		what += 3;
	else if (_vm->_gyro.my > (unsigned char)(_vm->_trip.tr[0].y + _vm->_trip.tr[0]._info.yl))
		what += 6;

	switch (what) {
	case 0:
		_vm->_trip.stopwalking();
		break; /* Clicked on Avvy- no movement */
	case 1:
		_vm->_trip.rwsp(1, _vm->_trip.left);
		break;
	case 2:
		_vm->_trip.rwsp(1, _vm->_trip.right);
		break;
	case 3:
		_vm->_trip.rwsp(1, _vm->_trip.up);
		break;
	case 4:
		_vm->_trip.rwsp(1, _vm->_trip.ul);
		break;
	case 5:
		_vm->_trip.rwsp(1, _vm->_trip.ur);
		break;
	case 6:
		_vm->_trip.rwsp(1, _vm->_trip.down);
		break;
	case 7:
		_vm->_trip.rwsp(1, _vm->_trip.dl);
		break;
	case 8:
		_vm->_trip.rwsp(1, _vm->_trip.dr);
		break;
	}    /* no other values are possible... */

	showrw();
}

void Lucerna::checkclick() {
	warning("STUB: Lucerna::checkclick()");
}

void Lucerna::mouse_init() {
	_vm->_gyro.wait();
}

void Lucerna::mousepage(uint16 page_) {
	warning("STUB: Lucerna::mousepage()");
}

void Lucerna::errorled() {
	warning("STUB: Lucerna::errorled()");
}

int8 Lucerna::fades(int8 x) {
	byte r = x / 16;
	x = x % 16;
	byte g = x / 4;
	byte b = x % 4;
	if (r > 0)  r --;
	if (g > 0)  g --;
	if (b > 0)  b --;
	return (16 * r + 4 * g + b);
	/* fades:=x-1;*/
}




void Lucerna::fadeout(byte n) {
	warning("STUB: Lucerna::fadeout()"); // I'll bother with colors later.
}

void Lucerna::dusk() {
	warning("STUB: Lucerna::dusk()"); // I'll bother with colors later.
}




void Lucerna::fadein(byte n) {
	warning("STUB: Lucerna::fadein()");
}

void Lucerna::dawn() {
	warning("STUB: Lucerna::dawn()");
}



void Lucerna::showrw() { // It's data is loaded in load_digits().
	if (_vm->_gyro.oldrw == _vm->_gyro.dna.rw) 
		return;
	_vm->_gyro.oldrw = _vm->_gyro.dna.rw;
	_vm->_gyro.off();
	
	/*	for (byte page_ = 0; page_ <= 1; page_ ++) {
	setactivepage(page_);
	putimage(0, 161, rwlite[with.rw], 0);
	}*/

	_vm->_graph.drawPicture(_vm->_gyro.rwlite[_vm->_gyro.dna.rw], 0, 161);

	_vm->_gyro.on();
	//setactivepage(1 - cp);
	
	warning("STUB: Lucerna::showrw()");
}

void Lucerna::mblit(byte x1, byte y1, byte x2, byte y2, byte f, byte t) /* assembler; */
/* The Minstrel Blitter */
/* asm
{  ofsfr:=f*$4000+x1+y1*80;
   ofsto:=t*$4000+x1+y1*80;}

  mov bx,80; { We're multiplying by 80. }
  mov al,y1;
  mul bl;    { AX now contains y1*80. }
  xor cx,cx; { Zero CX. }
  mov cl,x1; { CX now equals x1 }
  add ax,cx; { AX now contains x1+y1*80. }
  mov si,ax;
  mov di,ax;

  mov ax,$4000;
  mov bl,f;
  mul bx; { Note that this is a *uint16*! }
  add si,ax;

  mov ax,$4000;
  mov bl,t;
  mul bx; { Note that this is a *uint16*! }
  add di,ax;

  push ds; { *** <<<< *** WE MUST PRESERVE THIS! }
  cld;  { Clear Direction flag - we're going forwards! }

  mov ax,$A000; { The screen memory. }
  mov ds,ax;
  mov es,ax; { The same. }

  { AH stores the number of bytes to copy. }
  { len:=(x2-x1)+1; }

  mov ah,x2;
  sub ah,x1;
  inc ah;

  { Firstly, let's decide how many times we're going round. }

  mov cl,y2; { How many numbers between y1 and y2? }
  sub cl,y1;
  inc cl; { Inclusive reckoning (for example, from 3 to 5 is 5-3+1=3 turns. }

  { We'll use SI and DI to be Ofsfr and Ofsto. }

  @Y_axis_loop:
   push cx;


   { OK... We've changed this loop from a for-next loop. "Bit" is
     represented by CX. }

{     port[$3c4]:=2; port[$3ce]:=4; }
   mov dx,$3c4;
   mov al,2;
   out dx,al;
   mov dx,$3ce;
   mov al,4;
   out dx,al;

   mov cx,4; { We have to copy planes 3, 2, 1 and Zero. We'll add 1 to the
    number, because at zero it stops. }

   mov bx,3; { This has a similar function to that of CX. }

   @start_of_loop:

    push cx;

{     port[$3C5]:=1 shl bit; }
    mov dx,$3C5;
    mov al,1;
    mov cl,bl; { BL = bit. }
    shl al,cl;
    out dx,al;
{     port[$3CF]:=bit; }
    mov dx,$3CF;
    mov al,bl; { BL = bit. }
    out dx,al;

{   move(mem[$A000:ofsfr],mem[$A000:ofsto],len); }

    xor ch,ch; { Clear CH. }
    mov cl,ah;

    repz movsb; { That's all we need to say! }

    mov cl,ah;
    sub si,cx; { This is MUCH, MUCH faster than pushing and popping them! }
    sub di,cx;

    pop cx; { Get the loop count back again. }
    dec bx; { One less... }
   loop @start_of_loop; { Until cx=0. }

   add si,80; { Do the next line... }
   add di,80;

   pop cx;
  loop @Y_axis_loop;

  pop ds; { Get it back again (or we'll be in trouble with TP!) }
*/
{
}

void Lucerna::blitfix() {
	warning("STUB: Lucerna::blitfix()");
}






void Lucerna::calchand(uint16 ang, uint16 length, /*arccoordstype &a,*/ byte c) {
	warning("STUB: Lucerna::calchand()");
}

void Lucerna::hand(/*arccoordstype a,*/ byte c) {
	warning("STUB: Lucerna::hand()");
}

void Lucerna::chime() {
	warning("STUB: Lucerna::chime()");
}

void Lucerna::plothands() {
	warning("STUB: Lucerna::plothands()");
}

void Lucerna::clock_lucerna() {
	warning("STUB: Lucerna::clock_lucerna()");
}




void Lucerna::flip_page() {
	warning("STUB: Lucerna::flip_page()");
}

void Lucerna::delavvy() {
	byte page_;

	_vm->_gyro.off();
	
	triptype &with = _vm->_trip.tr[0];
	for (page_ = 0; page_ <= 1; page_ ++)
		mblit(with.x / 8, with.y, (with.x + with._info.xl) / 8 + 1, with.y + with._info.yl, 3, page_);
	
	blitfix();
	_vm->_gyro.on();
}

void Lucerna::gameover() {
	byte fv;
	int16 sx, sy;

	_vm->_gyro.dna.user_moves_avvy = false;

	sx = _vm->_trip.tr[1].x;
	sy = _vm->_trip.tr[1].y;
	
	_vm->_trip.tr[1].done();
	_vm->_trip.tr[1].init(12, true, &_vm->_trip);       /* 12 = Avalot falls */
	_vm->_trip.tr[1].step = 0;
	_vm->_trip.tr[1].appear(sx, sy, 0);
	
	_vm->_timeout.set_up_timer(3, _vm->_timeout.procavalot_falls, _vm->_timeout.reason_falling_over);
	/* _vm->_scrolls.display(^m^m^m^m^m^m^i^i^i^i^i^i^s'Z'^v);*/
	_vm->_gyro.alive = false;
}

/* OK. There are two kinds of redraw: Major and Minor. Minor is what happens
  when you load a game, etc. Major redraws EVERYTHING. */

void Lucerna::minor_redraw() {
	byte fv;

	dusk();
	enterroom(_vm->_gyro.dna.room, 0); /* Ped unknown or non-existant. */

	for (fv = 0; fv <= 1; fv ++) {
		_vm->_gyro.cp = 1 - _vm->_gyro.cp;
		_vm->_trip.getback();
	}
	
	for (byte i = 0; i < 3; i++)
		_vm->_gyro.lastscore[i] = -1; /* impossible digits */
	showscore();
	
	dawn();
}

void Lucerna::major_redraw() {
	warning("STUB: Lucerna::major_redraw()");
}

uint16 Lucerna::bearing(byte whichped) {
/* Returns the bearing from ped Whichped to Avvy, in degrees. */
	const double rad2deg = 180 / 3.14/*Pi*/;

	uint16 bearing_result; 
	{
		pedtype &with = _vm->_gyro.peds[whichped];
		if (_vm->_trip.tr[1].x == with.x)
			bearing_result = 0; /* This would cause a division by zero if we let it through. */
		else {
			/*
			 bearing:=trunc(((arctan((_vm->_trip.tr[1].y-y)/(_vm->_trip.tr[1].x-x)))*rad2deg)+90) mod 360*/
		
			if (_vm->_trip.tr[1].x < with.x)
				bearing_result = (atan(double((_vm->_trip.tr[1].y - with.y)) / (_vm->_trip.tr[1].x - with.x)) * rad2deg) + 90;
			else
				bearing_result = (atan(double((_vm->_trip.tr[1].y - with.y)) / (_vm->_trip.tr[1].x - with.x)) * rad2deg) + 270;
		}
	}
	return bearing_result;
}

void Lucerna::flesh_colours()     /* assembler;
asm
  mov ax,$1012;
  mov bx,21;                 { 21 = light pink (why?) }
  mov cx,1;
  mov dx,seg    @flesh;
  mov es,dx;
  mov dx,offset @flesh;
  int $10;

  mov dx,seg    @darkflesh;
  mov es,dx;
  mov dx,offset @darkflesh;
  mov bx,5;                 { 5 = dark pink. }
  int $10;

  jmp @TheEnd;

 @flesh:
  db 56,35,35;

 @darkflesh:
  db 43,22,22;

 @TheEnd: */
{
}

void Lucerna::sprite_run() {
/* A sprite run is performed before displaying a scroll, if not all the
  sprites are still. It performs two fast cycles, only using a few of
  the links usually used, and without any extra animation. This should
  make the sprites the same on both pages. */
	byte fv;

	_vm->_gyro.doing_sprite_run = true;

	for (fv = 0; fv <= 1; fv ++) {
		_vm->_trip.get_back_loretta();
		_vm->_trip.trippancy_link();

		flip_page();
	}

	_vm->_gyro.doing_sprite_run = false;

}

void Lucerna::fix_flashers() {
	_vm->_gyro.ledstatus = 177;
	_vm->_gyro.oldrw = 177;
	_vm->_scrolls.state(2);
	showrw();
}

} // End of namespace Avalanche
