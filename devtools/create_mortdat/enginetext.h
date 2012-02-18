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
 * This is a utility for extracting needed resource data from different language
 * version of the Mortevielle executable files into a new file mort.dat - this
 * is required for the ScummVM Mortevielle module to work properly
 */

#ifndef ENGINEDATA_H
#define ENGINEDATA_H

const char *engineDataEn[] =  {
	"[2][ ][YES][NO]",
	"Go to",
	"Someone enters, looks surised, but says nothing",
	"Cool",
	" Lourde ",
	"Malsaine",
	"Idem",
	"You",
	"are",
	"Alone",

	"Gosh! You hear some noise...",
	" | You should have noticed,   |       ",
	"% of hints...",
	"Do you want to wake up?",
	"OK",
	"", 
	" Save",

	" Load",
	" Restart     ",
	"F3: Repeat",
	"F8: Proceed",
	"Hide self",
	"take",
	" probe    ",
	" raise    ",
	" -SUITE- ",
	" -STOP-  ",
	"[1] [So, use the DEP menu] [Ok]",
	"lift",
	"read",

	"look",
	"search",
	"open",
	"put",
	"turn",
	"tie",
	"close",
	"hit",
	"pose",
	"smash",

	"smell",
	"scratch",
	"probe",
	"[1] [ | Before, use the DEP menu...] [Ok]",
	"& day",
	NULL
};

const char *engineDataFr[] = {
	"[2][ ][OUI][NON]",
	"aller",
	"quelqu'un entre, parait �tonn� mais ne dit rien",
	"Cool",
	" Lourde ",
	"Malsaine",
	"Idem",
	"Vous",
	"�tes",
	"SEUL",

	"Mince! Vous entendez du bruit...",
	" | Vous devriez avoir remarqu�|       ",
	"% des indices...",
	"D�sirez-vous vous r�veiller?",
	"OK",
	"", 
	" Sauvegarde",

	" Chargement",
	" Recommence  ",
	"F3: Encore",
	"F8: Suite",
	"se cacher",

	"prendre",
	" sonder   ",
	"soulever",
	" -SUITE- ",
	" -STOP-  ",
	"[1][Alors, utilisez le menu DEP...][ok]",
	"soulever",
	"lire",

	"regarder",
	"fouiller",
	"ouvrir",
	"mettre",
	"tourner",
	"attacher",
	"fermer",
	"frapper",
	"poser",
	"d�foncer",

	"sentir",
	"gratter",
	"sonder",
	"[1][ | Avant, utilisez le menu DEP...][ok]",
	"& jour",
	NULL
};

#endif
