/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2003 The ScummVM project
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include "midiparser.h"
#include "mididrv.h"
#include "common/util.h"

#include <stdio.h>
#include <memory.h>

//////////////////////////////////////////////////
//
// The Standard MIDI File version of MidiParser
//
//////////////////////////////////////////////////

class MidiParser_SMF : public MidiParser {
protected:
	byte *_data;
	byte *_buffer;
	bool _malformedPitchBends;

protected:
	void compressToType0();
	void parseNextEvent (EventInfo &info);

public:
	MidiParser_SMF() : _data(0), _buffer(0), _malformedPitchBends(false) {}
	~MidiParser_SMF();

	bool loadMusic (byte *data, uint32 size);
	void unloadMusic();
	void property (int property, int value);
};



//////////////////////////////////////////////////
//
// MidiParser_SMF implementation
//
// Much of this code is adapted from the XMIDI
// implementation from the exult project.
//
//////////////////////////////////////////////////

static byte command_lengths[8] = { 3, 3, 3, 3, 2, 2, 3, 0 };
static byte special_lengths[16] = { 0, 1, 1, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };

MidiParser_SMF::~MidiParser_SMF() {
	if (_buffer)
		free (_buffer);
}

void MidiParser_SMF::property (int prop, int value) {
	switch (prop) {
	case mpMalformedPitchBends:
		_malformedPitchBends = (value > 0);
	default:
		MidiParser::property (prop, value);
	}
}

void MidiParser_SMF::parseNextEvent (EventInfo &info) {
	info.start = _position._play_pos;
	info.delta = readVLQ (_position._play_pos);

	// Process the next info. If mpMalformedPitchBends
	// was set, we must skip over any pitch bend events
	// because they are from Simon games and are not
	// real pitch bend events, they're just two-byte
	// prefixes before the real info.
	do {
		if ((_position._play_pos[0] & 0xF0) >= 0x80)
			info.event = *(_position._play_pos++);
		else
			info.event = _position._running_status;
	} while (_malformedPitchBends && (info.event & 0xF0) == 0xE0 && _position._play_pos++);
	if (info.event < 0x80)
		return;

	_position._running_status = info.event;
	switch (info.command()) {
	case 0xC: case 0xD:
		info.basic.param1 = *(_position._play_pos++);
		info.basic.param2 = 0;
		break;

	case 0x8: case 0x9: case 0xA: case 0xB: case 0xE:
		info.basic.param1 = *(_position._play_pos++);
		info.basic.param2 = *(_position._play_pos++);
		if (info.command() == 0x9 && info.basic.param2 == 0)
			info.event = info.channel() | 0x80;
		info.length = 0;
		break;

	case 0xF: // System Common, Meta or SysEx event
		switch (info.event & 0x0F) {
		case 0x2: // Song Position Pointer
			info.basic.param1 = *(_position._play_pos++);
			info.basic.param2 = *(_position._play_pos++);
			break;

		case 0x3: // Song Select
			info.basic.param1 = *(_position._play_pos++);
			info.basic.param2 = 0;
			break;

		case 0x6: case 0x8: case 0xA: case 0xB: case 0xC: case 0xE:
			info.basic.param1 = info.basic.param2 = 0;
			break;

		case 0x0: // SysEx
			info.length = readVLQ (_position._play_pos);
			info.ext.data = _position._play_pos;
			_position._play_pos += info.length;
			break;

		case 0xF: // META event
			info.ext.type = *(_position._play_pos++);
			info.length = readVLQ (_position._play_pos);
			info.ext.data = _position._play_pos;
			_position._play_pos += info.length;
			break;
		}
	}
}

bool MidiParser_SMF::loadMusic (byte *data, uint32 size) {
	uint32 len;
	byte midi_type;
	uint32 total_size;
	bool isGMF;

	unloadMusic();
	byte *pos = data;
	isGMF = false;

	if (!memcmp (pos, "RIFF", 4)) {
		// Skip the outer RIFF header.
		pos += 8;
	}

	if (!memcmp (pos, "MThd", 4)) {
		// SMF with MTHd information.
		pos += 4;
		len = read4high (pos);
		if (len != 6) {
			printf ("Warning: MThd length 6 expected but found %d\n", (int) len);
			return false;
		}

		// Verify that this MIDI either is a Type 2
		// or has only 1 track. We do not support
		// multitrack Type 1 files.
		_num_tracks = pos[2] << 8 | pos[3];
		midi_type = pos[1];
		if (midi_type > 2 /*|| (midi_type < 2 && _num_tracks > 1)*/) {
			printf ("Warning: No support for a Type %d MIDI with %d tracks\n", (int) midi_type, (int) _num_tracks);
			return false;
		}
		_ppqn = pos[4] << 8 | pos[5];
		pos += len;
	} else if (!memcmp (pos, "GMF\x1", 4)) {
		// Older GMD/MUS file with no header info.
		// Assume 1 track, 192 PPQN, and no MTrk headers.
		isGMF = true;
		midi_type = 0;
		_num_tracks = 1;
		_ppqn = 192;
		pos += 7; // 'GMD\x1' + 3 bytes of useless (translate: unknown) information
	} else {
		printf ("Expected MThd or GMD header but found '%c%c%c%c' instead.\n", pos[0], pos[1], pos[2], pos[3]);
		return false;
	}

	// Now we identify and store the location for each track.
	if (_num_tracks > ARRAYSIZE(_tracks)) {
		printf ("Can only handle %d tracks but was handed %d\n", (int) ARRAYSIZE(_tracks), (int) _num_tracks);
		return false;
	}

	total_size = 0;
	int tracks_read = 0;
	while (tracks_read < _num_tracks) {
		if (memcmp (pos, "MTrk", 4) && !isGMF) {
			printf ("Position: %p ('%c')\n", pos, *pos);
			printf ("Hit invalid block '%c%c%c%c' while scanning for track locations\n", pos[0], pos[1], pos[2], pos[3]);
			return false;
		}

		// If needed, skip the MTrk and length bytes
		_tracks[tracks_read] = pos + (isGMF ? 0 : 8);
		if (!isGMF) {
			pos += 4;
			len = read4high (pos);
			total_size += len;
			pos += len;
		} else {
			// An SMF End of Track meta event must be placed
			// at the end of the stream.
			data[size++] = 0xFF;
			data[size++] = 0x2F;
			data[size++] = 0x00;
			data[size++] = 0x00;
		}
		++tracks_read;
	}

	// If this is a Type 1 MIDI, we need to now compress
	// our tracks down into a single Type 0 track.
	if (_buffer) {
		free (_buffer);
		_buffer = 0;
	}

	if (midi_type == 1) {
		_buffer = (byte *) calloc (size, 1);
		compressToType0();
		_data = _buffer;
		_num_tracks = 1;
		_tracks[0] = _buffer;
	} else {
		_data = data;
	}

	// Note that we assume the original data passed in
	// will persist beyond this call, i.e. we do NOT
	// copy the data to our own buffer. Take warning....
	resetTracking();
	setTrack (0);
	return true;
}

void MidiParser_SMF::compressToType0() {
	// We assume that _buffer has been allocated
	// to sufficient size for this operation.
	byte *track_pos[16];
	byte running_status[16];
	uint32 track_timer[16];
	uint32 delta;
	int i;

	for (i = 0; i < _num_tracks; ++i) {
		running_status[i] = 0;
		track_pos[i] = _tracks[i];
		track_timer[i] = readVLQ (track_pos[i]);
		running_status[i] = 0;
	}

	int best_i;
	uint32 length;
	byte *output = _buffer;
	byte *pos, *pos2;
	byte event;
	uint32 copy_bytes;
	bool write;
	byte active_tracks = (byte) _num_tracks;

	while (active_tracks) {
		write = true;
		best_i = 255;
		for (i = 0; i < _num_tracks; ++i) {
			if (track_pos[i] && (best_i == 255 || track_timer[i] < track_timer[best_i]))
				best_i = i;
		}
		if (best_i == 255) {
			printf ("Premature end of tracks!\n");
			break;
		}

		// Initial VLQ delta computation
		delta = 0;
		length = track_timer[best_i];
		for (i = 0; length; ++i) {
			delta = (delta << 8) | (length & 0x7F) | (i ? 0x80 : 0);
			length >>= 7;
		}

		// Process MIDI event.
		copy_bytes = 0;
		pos = track_pos[best_i];
		do {
			event = *(pos++);
			if (event < 0x80)
				event = running_status[best_i];
		} while (_malformedPitchBends && (event & 0xF0) == 0xE0 && pos++);
		running_status[best_i] = event;

		if (command_lengths [(event >> 4) - 8] > 0) {
			copy_bytes = command_lengths [(event >> 4) - 8];
		} else if (special_lengths [(event & 0x0F)] > 0) {
			copy_bytes = special_lengths [(event & 0x0F)];
		} else if (event == 0xF0) {
			// SysEx
			pos2 = pos;
			length = readVLQ (pos);
			copy_bytes = 1 + (pos - pos2) + length;
		} else if (event == 0xFF) {
			// META
			event = *(pos++);
			if (event == 0x2F && active_tracks > 1) {
				track_pos[best_i] = 0;
				write = false;
			} else {
				pos2 = pos;
				length = readVLQ (pos);
				copy_bytes = 2 + (pos - pos2) + length;
			}
			if (event == 0x2F)
				--active_tracks;
		} else {
			printf ("Bad MIDI command %02X!\n", (int) event);
			track_pos[best_i] = 0;
		}

		// Update all tracks' deltas
		if (write) {
			for (i = 0; i < _num_tracks; ++i) {
				if (track_pos[i] && i != best_i)
					track_timer[i] -= track_timer[best_i];
			}
		}

		if (track_pos[best_i]) {
			if (write) {
				track_timer[best_i] = 0;

				// Write VLQ delta
				while (delta & 0x80) {
					*output++ = (byte) (delta & 0xFF);
					delta >>= 8;
				}
				*output++ = (byte) (delta & 0xFF);

				// Write MIDI data
				memcpy (output, track_pos[best_i], copy_bytes);
				output += copy_bytes;
			}

			// Fetch new VLQ delta for winning track
			track_pos[best_i] += copy_bytes;
			if (active_tracks)
				track_timer[best_i] += readVLQ (track_pos[best_i]);
		}
	}

	*output++ = 0x00;
}

void MidiParser_SMF::unloadMusic() {
	resetTracking();
	allNotesOff();
	_data = 0;
	_num_tracks = 0;
	_active_track = 255;
}

MidiParser *MidiParser::createParser_SMF() { return new MidiParser_SMF; }
