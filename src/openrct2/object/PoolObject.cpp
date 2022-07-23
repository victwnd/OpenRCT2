/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma warning(disable : 4706) // assignment within conditional expression

#include "PoolObject.h"

#include "../core/IStream.hpp"
#include "../core/Json.hpp"
#include "../core/Memory.hpp"
#include "../core/String.hpp"
#include "../drawing/Drawing.h"
#include "../drawing/Image.h"
#include "../interface/Cursors.h"
#include "../localisation/Language.h"
#include "../world/Pool.h"

#include <algorithm>


void PoolObject::Load()
{
    GetStringTable().Sort();
    NameStringId = language_allocate_object_string(GetName());

    auto numImages = GetImageTable().GetCount();
    if (numImages != 0)
    {
        PreviewImageId = gfx_object_allocate_images(GetImageTable().GetImages(), GetImageTable().GetCount());
        BaseImageId = PreviewImageId + 1;
    }

}

void PoolObject::Unload()
{
    language_free_object_string(NameStringId);
    gfx_object_free_images(PreviewImageId, GetImageTable().GetCount());

    NameStringId = 0;
    PreviewImageId = 0;
    BaseImageId = 0;
}

void PoolObject::DrawPreview(rct_drawpixelinfo* dpi, int32_t width, int32_t height) const
{
auto screenCoords = ScreenCoordsXY{24,32};
gfx_draw_sprite(dpi, ImageId(PreviewImageId + 0), screenCoords);
}


void PoolObject::ReadJson(IReadObjectContext* context, json_t& root)
{
    Guard::Assert(root.is_object(), "PoolObject::ReadJson expects parameter root to be object");

    auto properties = root["properties"];


    PopulateTablesFromJson(context, root);
}
