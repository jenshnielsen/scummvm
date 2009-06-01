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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/trunk/engines/touche/touche.cpp $
 * $Id: touche.cpp 39003 2009-03-01 04:42:46Z fingolfin $
 *
 */

#include "common/config-manager.h"
#include "common/events.h"
#include "common/system.h"
#include "common/file.h"

#include "asylum/asylum.h"

namespace Asylum {

AsylumEngine::AsylumEngine(OSystem *system, Common::Language language)
	: Engine(system) {

	Common::File::addDefaultDirectory(_gameDataDir.getChild("Data"));
	Common::File::addDefaultDirectory(_gameDataDir.getChild("Vids"));	

	_eventMan->registerRandomSource(_rnd, "asylum");
}

AsylumEngine::~AsylumEngine() {
	//Common::clearAllDebugChannels();	
}

Common::Error AsylumEngine::run() {
	initGraphics(640, 480, true);
	
	return Common::kNoError;
}

} // namespace Asylum
