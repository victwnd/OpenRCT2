/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../Identifiers.h"
#include "../common.h"
#include "../interface/Viewport.h"
#include "../object/Object.h"


class PoolObject;


enum
{
    PROVISIONAL_POOL_FLAG_SHOW_ARROW = (1 << 0),
    PROVISIONAL_POOL_FLAG_1 = (1 << 1),
    PROVISIONAL_POOL_FLAG_2 = (1 << 2),
};

struct PoolSelection
{
ObjectEntryIndex Pool = OBJECT_ENTRY_INDEX_NULL;
};

struct ProvisionalPool
{
    ObjectEntryIndex Type;
    CoordsXYZ Position;
    uint8_t Flags;
};


enum
{
    POOL_ELEMENT_FLAGS_WATER = 1 << 0,
    POOL_ELEMENT_FLAGS_IN_GROUND = 2 << 0,
};


constexpr auto PoolMaxHeight = 248 * COORDS_Z_STEP;
constexpr auto PoolMinHeight = 2 * COORDS_Z_STEP;
constexpr auto POOL_HEIGHT_STEP = 2 * COORDS_Z_STEP;
constexpr auto POOL_DEPTH = 2 * COORDS_Z_STEP;
constexpr auto POOL_CLEARANCE = 6 * COORDS_Z_STEP;


extern PoolSelection gPoolSelection;
extern ProvisionalPool gProvisionalPool;

const PoolObject* GetPoolEntry(ObjectEntryIndex entryIndex);

money32 pool_remove(const CoordsXYZ& footpathLoc, int32_t flags);
void pool_provisional_remove();
void pool_provisional_update();
money32 pool_provisional_set(ObjectEntryIndex type, const CoordsXYZ& poolLoc,bool isWater);
TileElement* map_get_pool_element(const CoordsXYZ& coords);

void pool_connect_edges(const CoordsXY& poolPos, TileElement* tileElement);
void pool_remove_edges(const CoordsXY& poolPos, TileElement* tileElement);
