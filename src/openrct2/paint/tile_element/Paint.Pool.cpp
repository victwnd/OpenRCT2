/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "Paint.Surface.h"

#include "../../Cheats.h"
#include "../../Context.h"
#include "../../OpenRCT2.h"
#include "../../config/Config.h"
#include "../../core/Guard.hpp"
#include "../../core/Numerics.hpp"
#include "../../drawing/Drawing.h"
#include "../../entity/EntityRegistry.h"
#include "../../entity/PatrolArea.h"
#include "../../entity/Peep.h"
#include "../../entity/Staff.h"
#include "../../interface/Colour.h"
#include "../../interface/Viewport.h"
#include "../../object/ObjectManager.h"
#include "../../object/PoolObject.h"
#include "../../paint/Paint.h"
#include "../../paint/Paint.SessionFlags.h"
#include "../../profiling/Profiling.h"
#include "../../sprites.h"
#include "../../world/Pool.h"
#include "../../world/TileInspector.h"
#include "Paint.TileElement.h"

#include <algorithm>
#include <cstring>
#include <iterator>

static const PoolObject* get_pool_object(size_t index)
{
    PROFILED_FUNCTION();

    PoolObject* result{};
    auto& objMgr = OpenRCT2::GetContext()->GetObjectManager();
    auto obj = objMgr.GetLoadedObject(ObjectType::Pool, index);
    if (obj != nullptr)
    {
        return static_cast<PoolObject*>(obj);
    }
    return result;
}


static constexpr const uint8_t squareSpriteMap[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,16,4,5,6,18,8,9,10,22,12,13,14,32,0,1,2,3,4,5,17,19,8,9,10,11,12,13,29,33,0,1,2,16,4,5,17,20,8,9,10,22,12,13,29,34,0,1,2,3,4,5,6,7,8,9,10,11,25,26,30,35,0,1,2,16,4,5,6,18,8,9,10,22,25,26,30,36,0,1,2,3,4,5,17,19,8,9,10,11,25,26,31,37,0,1,2,16,4,5,17,20,8,9,10,22,25,26,31,38,0,1,2,3,4,5,6,7,8,21,10,23,12,27,14,39,0,1,2,16,4,5,6,18,8,21,10,24,12,27,14,40,0,1,2,3,4,5,17,19,8,21,10,23,12,27,29,41,0,1,2,16,4,5,17,20,8,21,10,24,12,27,29,42,0,1,2,3,4,5,6,7,8,21,10,23,25,28,30,43,0,1,2,16,4,5,6,18,8,21,10,24,25,28,30,45,0,1,2,3,4,5,17,19,8,21,10,23,25,28,31,44,0,1,2,16,4,5,17,20,8,21,10,24,25,28,31,46
};

static constexpr const uint8_t waterSpriteMap[] ={0,1,2,16,4,5,17,20,8,21,10,24,25,28,31,46,159,151,156,16,60,59,95,20,81,105,80,24,117,139,144,46,160,63,152,96,153,62,17,20,66,132,65,142,98,147,31,46,97,96,97,96,95,20,95,20,85,142,85,142,144,46,144,46,157,72,69,120,149,71,99,145,154,101,68,138,25,28,31,46,111,112,119,120,115,116,137,145,113,143,121,138,117,139,144,46,100,88,99,145,100,88,99,145,98,147,31,46,98,147,31,46,137,145,137,145,137,145,137,145,144,46,144,46,144,46,144,46,158,155,78,104,75,74,135,141,150,21,77,24,102,28,148,46,106,106,104,104,94,94,141,141,105,105,24,24,139,139,46,46,126,128,127,146,134,136,135,141,130,132,131,142,140,147,148,46,146,146,146,146,141,141,141,141,142,142,142,142,46,46,46,46,103,101,91,138,102,28,148,46,103,101,91,138,102,28,148,46,143,143,138,138,139,139,46,46,143,143,138,138,139,139,46,46,140,147,148,46,140,147,148,46,140,147,148,46,140,147,148,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46};



static constexpr const uint8_t solidSpriteMap[] ={162,193,194,176,191,161,177,161,192,175,161,161,178,161,161,161,162,189,186,176,191,161,169,161,192,174,161,161,178,161,161,161,162,193,190,171,183,161,177,161,192,175,161,161,170,161,161,161,162,189,182,171,183,161,169,161,192,174,161,161,170,161,161,161,162,193,194,176,187,161,172,161,184,167,161,161,178,161,161,161,162,189,186,176,187,161,165,161,184,163,161,161,178,161,161,161,162,193,190,171,179,161,172,161,184,167,161,161,170,161,161,161,162,189,182,171,179,161,165,161,184,163,161,161,170,161,161,161,162,185,194,168,191,161,177,161,188,175,161,161,173,161,161,161,162,181,186,168,191,161,169,161,188,174,161,161,173,161,161,161,162,185,190,164,183,161,177,161,188,175,161,161,166,161,161,161,162,181,182,164,183,161,169,161,188,174,161,161,166,161,161,161,162,185,194,168,187,161,172,161,180,167,161,161,173,161,161,161,162,181,186,168,187,161,165,161,180,163,161,161,173,161,161,161,162,185,190,164,179,161,172,161,180,167,161,161,166,161,161,161,162,181,182,164,179,161,165,161,180,163,161,161,166,161,161,161};

void PaintPool(PaintSession& session, uint8_t direction, uint16_t height, const PoolElement& tileElement)
{
    PROFILED_FUNCTION();

    session.InteractionType = ViewportInteractionItem::Pool;
    session.Flags |= PaintSessionFlags::PassedSurface;
    session.SurfaceElement = reinterpret_cast<const TileElement*>(&tileElement);
    const auto pool_type = tileElement.GetPoolEntryIndex();


     // Rol edges around rotation
    uint8_t edges = ((tileElement.GetEdges() << session.CurrentRotation) & 0xF)
        | (((tileElement.GetEdges()) << session.CurrentRotation) >> 4);

    uint8_t corners = (((tileElement.GetCorners()) << session.CurrentRotation) & 0xF)
        | (((tileElement.GetCorners()) << session.CurrentRotation) >> 4);

    uint16_t rotatedEdgesAndCorners = edges | (corners << 4);

    ImageId imageId;

    if (tileElement.IsGhost())
    {
    session.InteractionType = tileElement.IsInGround()?ViewportInteractionItem::Terrain:ViewportInteractionItem::None;
        imageId = imageId.WithRemap(FilterPaletteID::Palette44);
    }


    auto obj = get_pool_object(pool_type);
        if (obj != nullptr)
	{
		switch(tileElement.GetEdgeStyle())
		{
		case PoolEdgeStyle::Square:
		if(tileElement.IsWater())
		{
		imageId = imageId.WithIndex(obj->BaseImageId+squareSpriteMap[rotatedEdgesAndCorners]);
    		}
		else
		{
		imageId = imageId.WithIndex(obj->BaseImageId+161);
		}
		break;
		case PoolEdgeStyle::Angled:
		case PoolEdgeStyle::Curved:
		if(tileElement.IsWater())
		{
		imageId = imageId.WithIndex(obj->BaseImageId+waterSpriteMap[rotatedEdgesAndCorners]);
    		}
		else
		{
		imageId = imageId.WithIndex(obj->BaseImageId+solidSpriteMap[rotatedEdgesAndCorners]);
		}
		break;
		}
	}

     PaintAddImageAsParent(session, imageId, {0, 0, height }, { 32, 32,16});

	if(tileElement.IsInGround())PaintUtilHideSurface(session);
//TODO set segment support heights

}
