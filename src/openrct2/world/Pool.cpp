/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../Cheats.h"
#include "../Context.h"
#include "../Game.h"
#include "../Identifiers.h"
#include "../OpenRCT2.h"
#include "../actions/PoolPlaceAction.h"
#include "../actions/PoolRemoveAction.h"
#include "../core/Guard.hpp"
#include "../entity/EntityList.h"
#include "../entity/EntityRegistry.h"
#include "../interface/Window_internal.h"
#include "../localisation/Localisation.h"
#include "../management/Finance.h"
#include "../network/network.h"
#include "../object/PoolObject.h"
#include "../object/ObjectList.h"
#include "../object/ObjectManager.h"
#include "../paint/VirtualFloor.h"
#include "../ride/RideData.h"
#include "../ride/Station.h"
#include "../ride/Track.h"
#include "../ride/TrackData.h"
#include "../util/Util.h"
#include "Map.h"
#include "MapAnimation.h"
#include "Park.h"
#include "Scenery.h"
#include "Surface.h"
#include "TileElement.h"

#include <algorithm>
#include <iterator>

PoolSelection gPoolSelection;
ProvisionalPool gProvisionalPool;

const PoolObject* GetPoolEntry(ObjectEntryIndex entryIndex)
{
    auto& objMgr = OpenRCT2::GetContext()->GetObjectManager();
    auto obj = objMgr.GetLoadedObject(ObjectType::Pool, entryIndex);
    if (obj == nullptr)
        return nullptr;

    return static_cast<PoolObject*>(obj);
}

ObjectEntryIndex PoolElement::GetPoolEntryIndex() const
{
    return PoolIndex;
}

const PoolObject* PoolElement::GetPoolEntry() const
{
    auto& objMgr = OpenRCT2::GetContext()->GetObjectManager();
    return static_cast<PoolObject*>(objMgr.GetLoadedObject(ObjectType::Pool, GetPoolEntryIndex()));
}

void PoolElement::SetPoolEntryIndex(ObjectEntryIndex newIndex)
{
    PoolIndex = newIndex;
}


money32 pool_remove(const CoordsXYZ& footpathLoc, int32_t flags)
{
    auto action = PoolRemoveAction(footpathLoc);
    action.SetFlags(flags);

    if (flags & GAME_COMMAND_FLAG_APPLY)
    {
        auto res = GameActions::Execute(&action);
        return res.Cost;
    }
    auto res = GameActions::Query(&action);
    return res.Cost;
}


void pool_provisional_remove()
{

    if (gProvisionalPool.Flags & PROVISIONAL_POOL_FLAG_1)
    {
        gProvisionalPool.Flags &= ~PROVISIONAL_POOL_FLAG_1;

        pool_remove(
            gProvisionalPool.Position,
            GAME_COMMAND_FLAG_APPLY | GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED | GAME_COMMAND_FLAG_NO_SPEND
                | GAME_COMMAND_FLAG_GHOST);
    }
}

void pool_provisional_update()
{
    pool_provisional_remove();
}


money32 pool_provisional_set(ObjectEntryIndex type, const CoordsXYZ& poolLoc)
{
    money32 cost;

    pool_provisional_remove();

    auto poolPlaceAction = PoolPlaceAction(poolLoc, type);
    poolPlaceAction.SetFlags(GAME_COMMAND_FLAG_GHOST | GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED);
    auto res = GameActions::Execute(&poolPlaceAction);
    cost = res.Error == GameActions::Status::Ok ? res.Cost : MONEY32_UNDEFINED;
    if (res.Error == GameActions::Status::Ok)
    {
        gProvisionalPool.Type = type;
        gProvisionalPool.Position = poolLoc;
        gProvisionalPool.Flags |= PROVISIONAL_POOL_FLAG_1;
    }
    return cost;
}

bool PoolElement::IsInGround() const
{
    return (Flags & POOL_ELEMENT_FLAGS_IN_GROUND) != 0;
}

void PoolElement::SetInGround(bool isInGround)
{
    Flags &= ~POOL_ELEMENT_FLAGS_IN_GROUND;
    if (isInGround)
        Flags |= POOL_ELEMENT_FLAGS_IN_GROUND;
}

bool PoolElement::IsWater() const
{
    return (Flags & POOL_ELEMENT_FLAGS_WATER) != 0;
}

void PoolElement::SetIsWater(bool isWater)
{
    Flags &= ~POOL_ELEMENT_FLAGS_WATER;
    if (isWater)
        Flags |= POOL_ELEMENT_FLAGS_WATER;
}

TileElement* map_get_pool_element(const CoordsXYZ& coords)
{
    TileElement* tileElement = map_get_first_element_at(coords);
    do
    {
        if (tileElement == nullptr)
            break;
        if (tileElement->GetType() == TileElementType::Pool && tileElement->GetBaseZ() == coords.z)
            return tileElement;
    } while (!(tileElement++)->IsLastForTile());

    return nullptr;
}

/*
// fix up the corners around the given pool element that gets removed
static void pool_fix_corners_around(const TileCoordsXY& poolPos, TileElement* poolElement)
{
    // A mask for the pools' corners of each possible neighbour
    static constexpr uint8_t cornersTouchingTile[3][3] = {
        { 0b0010, 0b0011, 0b0001 },
        { 0b0110, 0b0000, 0b1001 },
        { 0b0100, 0b1100, 0b1000 },
    };

    for (int32_t xOffset = -1; xOffset <= 1; xOffset++)
    {
        for (int32_t yOffset = -1; yOffset <= 1; yOffset++)
        {
            // Skip self
            if (xOffset == 0 && yOffset == 0)
                continue;

            TileElement* tileElement = map_get_first_element_at(
                TileCoordsXY{ poolPos.x + xOffset, poolPos.y + yOffset }.ToCoordsXY());
            if (tileElement == nullptr)
                continue;
            do
            {
                if (tileElement->GetType() != TileElementType::Pool)
                    continue;
                if (tileElement->base_height != poolElement->base_height)
                    continue;

                const int32_t ix = xOffset + 1;
                const int32_t iy = yOffset + 1;
                tileElement->AsPool()->SetCorners(tileElement->AsPool()->GetCorners() & ~(cornersTouchingTile[iy][ix]));
            } while (!(tileElement++)->IsLastForTile());
        }
    }
}
*/

/*
void pool_remove_edges_at(const CoordsXY& poolPos, TileElement* tileElement)
{
    bool fixCorners = false;
    for (uint8_t direction = 0; direction < 4; direction++)
    {
        int32_t z1 = tileElement->base_height;

        // When clearance checks were disabled a neighbouring pool can be connected to both the pool-ghost and to something
        // else, so before removing edges from neighbouring pools we have to make sure there is nothing else they are connected
        // to.
        if (!tile_element_wants_pool_connection_towards({ TileCoordsXY{ poolPos }, z1, direction }, tileElement))
        {
            bool isQueue = tileElement->GetType() == TileElementType::Pool ? tileElement->AsPool()->IsQueue() : false;
            int32_t z0 = z1 - 2;
            pool_remove_edges_towards(
                { poolPos + CoordsDirectionDelta[direction], z0 * COORDS_Z_STEP, z1 * COORDS_Z_STEP }, direction, isQueue);
        }
        else
        {
            // A pool may stay connected, but its edges must be fixed later on when another edge does get removed.
            fixCorners = true;
        }
    }

    // Only fix corners when needed, to avoid changing corners that have been set for its looks.
    if (fixCorners && tileElement->IsGhost())
    {
        auto tileFootpoolPos = TileCoordsXY{ poolPos };
        pool_fix_corners_around(tileFootpoolPos, tileElement);
    }

    if (tileElement->GetType() == TileElementType::Pool)
        tileElement->AsPool()->SetEdgesAndCorners(0);
}
*/
/*

static void loc_6A6FD2(const CoordsXYZ& initialTileElementPos, int32_t direction, TileElement* initialTileElement, bool query)
{
    if ((initialTileElement)->GetType() == TileElementType::Path)
    {
        if (!query)
        {
            initialTileElement->AsPath()->SetEdges(initialTileElement->AsPath()->GetEdges() | (1 << direction));
            map_invalidate_element(initialTileElementPos, initialTileElement);
        }
    }
}

static void loc_6A6F1F(
    const CoordsXYZ& initialTileElementPos, int32_t direction, TileElement* tileElement, TileElement* initialTileElement,
    const CoordsXY& targetPos, int32_t flags, bool query, rct_neighbour_list* neighbourList)
{
    if (query)
    {
        if (fence_in_the_way(
                { targetPos, tileElement->GetBaseZ(), tileElement->GetClearanceZ() }, direction_reverse(direction)))
        {
            return;
        }
        if (tileElement->AsPath()->IsQueue())
        {
            if (connected_path_count[tileElement->AsPath()->GetEdges()] < 2)
            {
                neighbour_list_push(
                    neighbourList, 4, direction, tileElement->AsPath()->GetRideIndex(),
                    tileElement->AsPath()->GetStationIndex());
            }
            else
            {
                if ((initialTileElement)->GetType() == TileElementType::Path && initialTileElement->AsPath()->IsQueue())
                {
                    if (footpath_disconnect_queue_from_path(targetPos, tileElement, 0))
                    {
                        neighbour_list_push(
                            neighbourList, 3, direction, tileElement->AsPath()->GetRideIndex(),
                            tileElement->AsPath()->GetStationIndex());
                    }
                }
            }
        }
        else
        {
            neighbour_list_push(neighbourList, 2, direction, RideId::GetNull(), StationIndex::GetNull());
        }
    }
    else
    {
        footpath_disconnect_queue_from_path(targetPos, tileElement, 1 + ((flags >> 6) & 1));
        tileElement->AsPath()->SetEdges(tileElement->AsPath()->GetEdges() | (1 << direction_reverse(direction)));
        if (tileElement->AsPath()->IsQueue())
        {
            footpath_queue_chain_push(tileElement->AsPath()->GetRideIndex());
        }
    }
    if (!(flags & (GAME_COMMAND_FLAG_GHOST | GAME_COMMAND_FLAG_ALLOW_DURING_PAUSED)))
    {
        footpath_interrupt_peeps({ targetPos, tileElement->GetBaseZ() });
    }
    map_invalidate_element(targetPos, tileElement);
    loc_6A6FD2(initialTileElementPos, direction, initialTileElement, query);
}

static void loc_6A6C85(
    const CoordsXYE& tileElementPos, int32_t direction, int32_t flags, bool query, rct_neighbour_list* neighbourList)
{

    auto pos = CoordsXYZ{ tileElementPos, tileElementPos.element->GetBaseZ() };

    auto targetPos = CoordsXY{ initialTileElementPos } + CoordsDirectionDelta[direction];
        TileElement* tileElement = map_get_first_element_at(targetPos);
        if (tileElement == nullptr)
            return;
        do
        {
            switch (tileElement->GetType())
            {
                case TileElementType::Path:
                    if (tileElement->GetBaseZ() == initialTileElementPos.z)
                    {
                        if (!tileElement->AsPath()->IsSloped() || tileElement->AsPath()->GetSlopeDirection() == direction)
                        {
                            loc_6A6F1F(
                                initialTileElementPos, direction, tileElement, initialTileElement, targetPos, flags, query,
                                neighbourList);
                        }
                        return;
                    }
                    break;
                default:
                    break;
            }

        } while (!(tileElement++)->IsLastForTile());
}

void pool_connect_edges(const CoordsXY& poolPos, TileElement* tileElement, int32_t flags)
{
    rct_neighbour_list neighbourList;
    rct_neighbour neighbour;

    neighbour_list_init(&neighbourList);

    for (Direction direction : ALL_DIRECTIONS)
    {
        loc_6A6C85({ poolPos, tileElement }, direction, flags, true, &neighbourList);
    }

    neighbour_list_sort(&neighbourList);

    while (neighbour_list_pop(&neighbourList, &neighbour))
    {
        loc_6A6C85({ poolPos, tileElement }, neighbour.direction, flags, false, nullptr);
    }

/*
    if (tileElement->GetType() == TileElementType::Pool)
    {
        pool_connect_corners(poolPos, tileElement->AsPool());
    }
*/
//}

void pool_remove_edges(const CoordsXY& poolPos, TileElement* tileElement)
{
uint8_t masks[8]={0x9B,0x37,0x6E,0xCD,0xBF,0x7F,0xEF,0xDF};

auto z=tileElement->GetBaseZ();
	for(int i=0;i<8;i++)
	{
	TileElement* neighbour=map_get_pool_element({poolPos+CoordsDirectionDelta[i],z});
		if(neighbour)neighbour->AsPool()->SetEdgesAndCorners(neighbour->AsPool()->GetEdgesAndCorners()&masks[i]);
	}



tileElement->AsPool()->SetCorners(0);
}

void pool_connect_edges(const CoordsXY& poolPos, TileElement* tileElement)
{
auto z=tileElement->GetBaseZ();

TileElement* neighbours[8];

//Set edges
uint8_t edges=0;
	for(int direction=0;direction<4;direction++)
	{
	neighbours[direction]=map_get_pool_element({poolPos+CoordsDirectionDelta[direction],z});
		if(neighbours[direction])edges|=1<<direction;
	}
tileElement->AsPool()->SetEdges(edges);


//Set corners
uint8_t cornerMasks[4]={0x3,0x6,0xC,0x9};

uint8_t corners=0;
	for(int corner=0;corner<4;corner++)
	{
		if((edges&cornerMasks[corner])==cornerMasks[corner])
		{
		neighbours[4+corner]=map_get_pool_element({poolPos+CoordsDirectionDelta[corner+4],z});
			if(neighbours[4+corner])corners|=1<<corner;
		}else neighbours[4+corner]=NULL;
	}
tileElement->AsPool()->SetCorners(corners);

//Update orthogonal neighbours
	for(int i=0;i<4;i++)
	{
		if(neighbours[i])
		{
		uint8_t neighbourEdgesAndCorners=neighbours[i]->AsPool()->GetEdgesAndCorners();
		neighbourEdgesAndCorners|=1<<((i+2)&0x3);
			if(corners&(1<<i))neighbourEdgesAndCorners|=0x10<<((i+1)&0x3);
			if(corners&(1<<((i+3)&0x3)))neighbourEdgesAndCorners|=0x10<<((i+2)&0x3);
		neighbours[i]->AsPool()->SetEdgesAndCorners(neighbourEdgesAndCorners);
		}
	}

//Update diagonal neighbours
	for(int i=4;i<8;i++)
	{
		if(neighbours[i])
		{
		uint8_t neighbourCorners=neighbours[i]->AsPool()->GetCorners();
		neighbourCorners|=1<<((i+2)&0x3);
		neighbours[i]->AsPool()->SetCorners(neighbourCorners);
		}
	}

}
//4 /\ 1
//3 \/ 2    
/*
0:

0->1
3->2

2:

1->4
2->3

3:

3->4
2->1

4:

3->2
4->1


5:

1->3

6:

2->4

7:

3->1

8:

4->2
*/
//   4
//3      1
//   2

uint8_t PoolElement::GetEdges() const
{
    return EdgesAndCorners & FOOTPATH_PROPERTIES_EDGES_EDGES_MASK;
}

void PoolElement::SetEdges(uint8_t newEdges)
{
    EdgesAndCorners &= ~FOOTPATH_PROPERTIES_EDGES_EDGES_MASK;
    EdgesAndCorners |= (newEdges & FOOTPATH_PROPERTIES_EDGES_EDGES_MASK);
}

uint8_t PoolElement::GetCorners() const
{
    return EdgesAndCorners >> 4;
}

void PoolElement::SetCorners(uint8_t newCorners)
{
    EdgesAndCorners &= ~FOOTPATH_PROPERTIES_EDGES_CORNERS_MASK;
    EdgesAndCorners |= (newCorners << 4);
}

uint8_t PoolElement::GetEdgesAndCorners() const
{
    return EdgesAndCorners;
}

void PoolElement::SetEdgesAndCorners(uint8_t newEdgesAndCorners)
{
    EdgesAndCorners = newEdgesAndCorners;
}

