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


money32 pool_provisional_set(ObjectEntryIndex type, const CoordsXYZ& poolLoc, bool isWater,uint8_t edgeStyle)
{
    money32 cost;

    pool_provisional_remove();

    auto poolPlaceAction = PoolPlaceAction(poolLoc, type, isWater, edgeStyle);
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

uint8_t PoolElement::GetEdgeStyle() const
{
return EdgeStyle;
}

void PoolElement::SetEdgeStyle(uint8_t newEdgeStyle)
{
EdgeStyle=newEdgeStyle;
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

//4 /\ 1
//3 \/ 2    

//   4
//3      1
//   2

static bool pool_should_connect_neighbour(TileElement* pool,TileElement* neighbour)
{
return neighbour->AsPool()->IsWater()==pool->AsPool()->IsWater();
}


void pool_remove_edges(const CoordsXY& poolPos, TileElement* tileElement)
{
//uint8_t masks[8]={0x9B,0x37,0x6E,0xCD,0xBF,0x7F,0xEF,0xDF};
uint8_t masks[8]={0xFB,0xF7,0xFE,0xFD,0xBF,0x7F,0xEF,0xDF};

auto z=tileElement->GetBaseZ();
	for(int i=0;i<8;i++)
	{
	TileElement* neighbour=map_get_pool_element({poolPos+CoordsDirectionDelta[i],z});
		if(neighbour&&pool_should_connect_neighbour(tileElement,neighbour))neighbour->AsPool()->SetEdgesAndCorners(neighbour->AsPool()->GetEdgesAndCorners()&masks[i]);
	}



tileElement->AsPool()->SetCorners(0);
}

void pool_connect_edges(const CoordsXY& poolPos, TileElement* tileElement)
{
auto z=tileElement->GetBaseZ();

TileElement* neighbours[8];

//Set edges and corners
uint8_t edges=0;
	for(int direction=0;direction<4;direction++)
	{
	TileElement* neighbour=map_get_pool_element({poolPos+CoordsDirectionDelta[direction],z});
		if(neighbour&&pool_should_connect_neighbour(tileElement,neighbour))
		{
		edges|=1<<direction;
		neighbours[direction]=neighbour;
		}
		else neighbours[direction]=NULL;
	}
tileElement->AsPool()->SetEdges(edges);


//Set corners
uint8_t corners=0;
	for(int direction=0;direction<4;direction++)
	{
	TileElement* neighbour=map_get_pool_element({poolPos+CoordsDirectionDelta[direction+4],z});
		if(neighbour&&pool_should_connect_neighbour(tileElement,neighbour))
		{
		corners|=1<<direction;
		neighbours[direction+4]=neighbour;
		}
		else neighbours[direction+4]=NULL;
	}
tileElement->AsPool()->SetCorners(corners);

//Update orthogonal neighbours
	for(int i=0;i<4;i++)
	{
		if(neighbours[i])
		{
		uint8_t neighbourEdgesAndCorners=neighbours[i]->AsPool()->GetEdgesAndCorners();
		neighbourEdgesAndCorners|=1<<((i+2)&0x3);
		//	if(corners&(1<<i))neighbourEdgesAndCorners|=0x10<<((i+1)&0x3);
		//	if(corners&(1<<((i+3)&0x3)))neighbourEdgesAndCorners|=0x10<<((i+2)&0x3);
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

