/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "PoolPlaceAction.h"

#include "../Cheats.h"
#include "../OpenRCT2.h"
#include "../core/MemoryStream.h"
#include "../interface/Window.h"
#include "../localisation/StringIds.h"
#include "../management/Finance.h"
#include "../ride/RideConstruction.h"
#include "../world/ConstructionClearance.h"
#include "../world/Pool.h"
#include "../world/Location.hpp"
#include "../world/Park.h"
#include "../world/Scenery.h"
#include "../world/Surface.h"
#include "../world/TileElementsView.h"
#include "../world/Wall.h"
#include "../world/Pool.h"

using namespace OpenRCT2;

PoolPlaceAction::PoolPlaceAction(
    const CoordsXYZ& loc, ObjectEntryIndex type, bool isWater,uint8_t edgeStyle)
    : _loc(loc)
    , _type(type)
    , _isWater(isWater)
    , _edgeStyle(edgeStyle)
{
}

void PoolPlaceAction::AcceptParameters(GameActionParameterVisitor& visitor)
{
    visitor.Visit(_loc);
    visitor.Visit("object", _type);
}

uint16_t PoolPlaceAction::GetActionFlags() const
{
    return GameAction::GetActionFlags();
}

void PoolPlaceAction::Serialise(DataSerialiser& stream)
{
    GameAction::Serialise(stream);

    stream << DS_TAG(_loc)  << DS_TAG(_type) ;
}


GameActions::Result PoolPlaceAction::Query() const
{
    auto res = GameActions::Result();
    res.Cost = 0;
    res.Expenditure = ExpenditureType::Landscaping;
    res.Position = _loc.ToTileCentre();

    if (!LocationValid(_loc) || MapIsEdge(_loc))
    {
        return GameActions::Result(GameActions::Status::InvalidParameters, STR_CANT_BUILD_FOOTPATH_HERE, STR_OFF_EDGE_OF_MAP);
    }

    if (!((gScreenFlags & SCREEN_FLAGS_SCENARIO_EDITOR) || gCheatsSandboxMode) && !MapIsLocationOwned(_loc))
    {
        return GameActions::Result(GameActions::Status::Disallowed, STR_CANT_BUILD_FOOTPATH_HERE, STR_LAND_NOT_OWNED_BY_PARK);
    }

    if (_loc.z < PoolMinHeight)
    {
        return GameActions::Result(GameActions::Status::Disallowed, STR_CANT_BUILD_FOOTPATH_HERE, STR_TOO_LOW);
    }

    if (_loc.z > PoolMaxHeight)
    {
        return GameActions::Result(GameActions::Status::Disallowed, STR_CANT_BUILD_FOOTPATH_HERE, STR_TOO_HIGH);
    }

    pool_provisional_remove();
    auto tileElement = map_get_pool_element(_loc);
    if (tileElement == nullptr)
    {
        return ElementInsertQuery(std::move(res));
    }
    return ElementUpdateQuery(tileElement, std::move(res));
}

GameActions::Result PoolPlaceAction::Execute() const
{
    auto res = GameActions::Result();
    res.Cost = 0;
    res.Expenditure = ExpenditureType::Landscaping;
    res.Position = _loc.ToTileCentre();

    auto tileElement = map_get_pool_element(_loc);
    if (tileElement == nullptr)
    {
        return ElementInsertExecute(std::move(res));
    }
    return ElementUpdateExecute(tileElement, std::move(res));
}

GameActions::Result PoolPlaceAction::ElementUpdateQuery(PoolElement* poolElement, GameActions::Result res) const
{
    if (GetFlags() & GAME_COMMAND_FLAG_GHOST && !poolElement->IsGhost())
    {
        return GameActions::Result(GameActions::Status::Unknown, STR_CANT_BUILD_POOL_HERE, STR_NONE);
    }
return res;
}
GameActions::Result PoolPlaceAction::ElementUpdateExecute(PoolElement* poolElement, GameActions::Result res) const
{
poolElement->SetPoolEntryIndex(_type);

	if(poolElement->IsWater()!=_isWater||poolElement->GetEdgeStyle()!=_edgeStyle)
	{
	poolElement->SetIsWater(_isWater);
	poolElement->SetEdgeStyle(_edgeStyle);
        pool_connect_edges(_loc,reinterpret_cast<TileElement*>(poolElement));
	}
return res;
}

GameActions::Result PoolPlaceAction::ElementInsertQuery(GameActions::Result res) const
{
return ElementInsertQueryExecute(res,false);
}

GameActions::Result PoolPlaceAction::ElementInsertExecute(GameActions::Result res) const
{
return ElementInsertQueryExecute(res,true);
}

GameActions::Result PoolPlaceAction::ElementInsertQueryExecute(GameActions::Result res,bool isExecuting) const
{
    if (!isExecuting&&!MapCheckCapacityAndReorganise(_loc))
    {
        return GameActions::Result(GameActions::Status::NoFreeElements, STR_CANT_BUILD_FOOTPATH_HERE, STR_NONE);
    }

res.Cost = 12.00_GBP;

    QuarterTile quarterTile{ 0b1111, 0 };
    auto zLow = _loc.z;
    auto zHigh = zLow + POOL_CLEARANCE;


    auto surfaceElement = MapGetSurfaceElementAt(_loc);
    if (surfaceElement == nullptr)
    {
        return GameActions::Result(GameActions::Status::InvalidParameters, STR_CANT_BUILD_POOL_HERE, STR_NONE);
    }

    bool inGround=false;
    if(surfaceElement->AsSurface()->GetSlope()==0&&surfaceElement->AsSurface()->GetBaseZ()==zLow+POOL_DEPTH)
    {
    inGround=true;
    }

    auto canBuild = MapCanConstructWithClearAt({ _loc, inGround?zLow+POOL_DEPTH:zLow, zHigh }, &MapPlaceNonSceneryClearFunc, quarterTile, GAME_COMMAND_FLAG_APPLY | GetFlags(),
        0);
    if (canBuild.Error != GameActions::Status::Ok)
    {
	
        canBuild.ErrorTitle = STR_CANT_BUILD_POOL_HERE;
        return canBuild;
    }
    res.Cost += canBuild.Cost;

    const auto clearanceData = canBuild.GetData<ConstructClearResult>();
    if (!isExecuting&&!gCheatsDisableClearanceChecks && (clearanceData.GroundFlags & ELEMENT_IS_UNDERWATER))
    {
        return GameActions::Result(
            GameActions::Status::Disallowed, STR_CANT_BUILD_FOOTPATH_HERE, STR_CANT_BUILD_THIS_UNDERWATER);
    }


//    int32_t supportHeight = zLow - surfaceElement->GetBaseZ();
    //res.Cost += supportHeight < 0 ? 20.00_GBP : (supportHeight / POOL_HEIGHT_STEP) * 5.00_GBP;

        if(isExecuting)
	{
        auto* poolElement = TileElementInsert<PoolElement>(_loc, 0b1111);
        Guard::Assert(poolElement != nullptr);

        poolElement->SetClearanceZ(zHigh);
        poolElement->SetPoolEntryIndex(_type);
        poolElement->SetGhost(GetFlags() & GAME_COMMAND_FLAG_GHOST);
	poolElement->SetInGround(inGround);
	poolElement->SetIsWater(_isWater);
	poolElement->SetEdgeStyle(_edgeStyle);

        pool_connect_edges(_loc,reinterpret_cast<TileElement*>(poolElement));

	}
    return res;
}

PoolElement* PoolPlaceAction::map_get_pool_element(const CoordsXYZ& poolPos) const
{
    for (auto* poolElement : TileElementsView<PoolElement>(poolPos))
    {
        if (poolElement->GetBaseZ() != poolPos.z)
            continue;
        return poolElement;
    }

    return nullptr;
}
