/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "PoolRemoveAction.h"

#include "../Cheats.h"
#include "../OpenRCT2.h"
#include "../common.h"
#include "../core/MemoryStream.h"
#include "../interface/Window.h"
#include "../localisation/Localisation.h"
#include "../localisation/StringIds.h"
#include "../management/Finance.h"
#include "../ride/Ride.h"
#include "../world/Park.h"
#include "../world/Pool.h"
#include "../world/TileElementsView.h"
#include "GameAction.h"
#include "PoolPlaceAction.h"

using namespace OpenRCT2;

PoolRemoveAction::PoolRemoveAction(const CoordsXYZ& location)
    : _loc(location)
{
}

void PoolRemoveAction::AcceptParameters(GameActionParameterVisitor& visitor)
{
    visitor.Visit(_loc);
}

uint16_t PoolRemoveAction::GetActionFlags() const
{
    return GameAction::GetActionFlags();
}

void PoolRemoveAction::Serialise(DataSerialiser& stream)
{
    GameAction::Serialise(stream);

    stream << DS_TAG(_loc);
}

GameActions::Result PoolRemoveAction::Query() const
{
return QueryExecute(false);
}

GameActions::Result PoolRemoveAction::Execute() const
{
return QueryExecute(true);
}

GameActions::Result PoolRemoveAction::QueryExecute(bool isExecuting) const
{
    GameActions::Result res = GameActions::Result();

    if (!isExecuting&&!LocationValid(_loc))
    {
        return GameActions::Result(GameActions::Status::InvalidParameters, STR_CANT_REMOVE_THIS, STR_LAND_NOT_OWNED_BY_PARK);
    }


    res.Cost = -6.00_GBP;
    res.Expenditure = ExpenditureType::Landscaping;
    res.Position = _loc;

    if (!isExecuting&&!(gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR) && !(GetFlags() & GAME_COMMAND_FLAG_GHOST) && !gCheatsSandboxMode)
    {
        // Check if the land is owned
        if (!map_is_location_owned(_loc))
        {
            res.Error = GameActions::Status::NoClearance;
            res.ErrorTitle = STR_CANT_REMOVE_THIS;
            res.ErrorMessage = STR_LAND_NOT_OWNED_BY_PARK;
            return res;
        }
    }


    TileElement* tileElement = FindPoolElement();
    if (tileElement == nullptr)
    {
        return GameActions::Result(
            GameActions::Status::InvalidParameters, STR_CANT_REMOVE_THIS, STR_INVALID_SELECTION_OF_OBJECTS);
    }

	if(isExecuting)
	{
	res.Position.z = tile_element_height(res.Position);

	map_invalidate_tile_full(_loc);
        pool_remove_edges(_loc,reinterpret_cast<TileElement*>(tileElement));
	tile_element_remove(tileElement);
	}

    return res;
}


TileElement* PoolRemoveAction::FindPoolElement() const
{
    const bool isGhost = GetFlags() & GAME_COMMAND_FLAG_GHOST;
    for (auto* poolElement : TileElementsView<PoolElement>(_loc))
    {
        // If we are removing ghost elements
        if (isGhost && poolElement->IsGhost() == false)
            continue;

        if (poolElement->GetBaseZ() != _loc.z)
            continue;

        return poolElement->as<TileElement>();
    }
    return nullptr;
}

