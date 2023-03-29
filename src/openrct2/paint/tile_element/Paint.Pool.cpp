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



void PaintPool(PaintSession& session, uint8_t direction, uint16_t height, const PoolElement& tileElement)
{
    PROFILED_FUNCTION();

    session.InteractionType = ViewportInteractionItem::Pool;
    session.Flags |= PaintSessionFlags::PassedSurface;
    session.SurfaceElement = reinterpret_cast<const TileElement*>(&tileElement);
    const auto pool_type = tileElement.GetPoolEntryIndex();

/*
     // Rol edges around rotation
    uint8_t edges = ((tileElement.GetEdges() << session.CurrentRotation) & 0xF)
        | (((tileElement.GetEdges()) << session.CurrentRotation) >> 4);

    uint8_t corners = (((tileElement.GetCorners()) << session.CurrentRotation) & 0xF)
        | (((tileElement.GetCorners()) << session.CurrentRotation) >> 4);

    uint16_t rotatedEdgesAndCorners = edges | (corners << 4);
*/
    ImageId imageId;

    if (tileElement.IsGhost())
    {
    session.InteractionType = tileElement.IsInGround()?ViewportInteractionItem::Terrain:ViewportInteractionItem::None;
        imageId = imageId.WithRemap(FilterPaletteID::Palette44);
    }


    auto obj = get_pool_object(pool_type);
        if (obj != nullptr)
	{
	imageId = imageId.WithIndex(obj->BaseImageId+tileElement.GetEdgesAndCorners());
		/*switch(tileElement.GetEdgeStyle())
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
		}*/
	}

     PaintAddImageAsParent(session, imageId, {0, 0, height }, { 32, 32,16});

	if(tileElement.IsInGround())PaintUtilHideSurface(session);
//TODO set segment support heights

}
