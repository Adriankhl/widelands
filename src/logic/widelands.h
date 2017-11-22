/*
 * Copyright (C) 2007-2017 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef WL_LOGIC_WIDELANDS_H
#define WL_LOGIC_WIDELANDS_H

#include <cassert>
#include <cstddef>
#include <limits>

#include <stdint.h>

#include "base/macros.h"

namespace Widelands {

//  Type definitions for the game logic.
using MilitaryInfluence = uint16_t;

/// 5 bits used, so 0 .. 31
/// Data type must match kMaxPlayers in graphics/playercolor.h
using PlayerNumber = uint8_t;
inline PlayerNumber neutral() {
	return 0;
}
#define iterate_player_numbers(p, nr_players)                                                      \
	for (Widelands::PlayerNumber p = 1; p < nr_players + 1; ++p)

/**
 * Every player has a team number. Team number 0 is special,
 * indicating that the player is not part of any team.
 *
 * Players having the same positive team number are allied.
 */
using TeamNumber = uint8_t;

/** This is used as index for wares, workers, terrain and other Descriptions.
 *
 * So we can have at most 254 types of these,
 * as some are predefined as invalid by the declarations below.
 *
 * A DescriptionMaintainer can be used to lookup the actual item.
 * See TerrainDescription, WareDescr, WorkerDescr
 * EditorCategory, BuildingDescr, ImmovableDescr, ShipDescr, TribeDescr
 * and others.
 */
using DescriptionIndex = uint8_t;

constexpr DescriptionIndex INVALID_INDEX = std::numeric_limits<uint8_t>::max();
constexpr DescriptionIndex kInvalidWare = INVALID_INDEX - 1;
constexpr DescriptionIndex kNoResource = INVALID_INDEX - 1;

using ResourceAmount = uint8_t;  /// 4 bits used, so 0 .. 15.

using Quantity = uint32_t;  // e.g. the number of a type of ware in a warehouse.

using Vision = uint16_t;

using Time = int32_t;  // TODO(unknown): should be unsigned
inline Time never() {
	return 0xffffffff;
}

using Duration = uint32_t;
inline Duration endless() {
	return 0xffffffff;
}

using Serial = uint32_t;  /// Serial number for MapObject.

using Direction = uint8_t;

}  // namespace Widelands

#endif  // end of include guard: WL_LOGIC_WIDELANDS_H
