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

static constexpr const uint8_t spriteMap[] = {
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 12, 13, 14, 15, 0, 1, 2, 16, 4, 5, 6, 18, 8, 9, 10, 22, 12, 13, 14, 32,
    0, 1, 2, 3, 4, 5, 17, 19, 8, 9,  10, 11, 12, 13, 29, 33, 0, 1, 2, 3,  4, 5, 6, 20, 8, 9, 10, 11, 12, 13, 14, 34,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 25, 26, 30, 35, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 36,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 12, 13, 31, 37, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 38,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 21, 10, 23, 12, 27, 14, 39, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 24, 12, 13, 14, 40,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 12, 13, 14, 41, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 42,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 12, 28, 14, 43, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 44,
    0, 1, 2, 3, 4, 5, 6,  7,  8, 9,  10, 11, 12, 13, 14, 45, 0, 1, 2, 3,  4, 5, 6, 7,  8, 9, 10, 11, 12, 13, 14, 46,
};
void PaintPool(paint_session& session, uint8_t direction, uint16_t height, const PoolElement& tileElement)
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
	imageId = imageId.WithIndex(obj->BaseImageId+spriteMap[rotatedEdgesAndCorners]);
    	}

     PaintAddImageAsParent(session, imageId, {0, 0, height }, { 32, 32,16});

	if(tileElement.IsInGround())paint_util_hide_surface(session);
//TODO set segment support heights

}
