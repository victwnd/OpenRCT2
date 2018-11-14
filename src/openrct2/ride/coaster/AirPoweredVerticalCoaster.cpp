/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../drawing/Drawing.h"
#include "../../interface/Viewport.h"
#include "../../paint/Paint.h"
#include "../../paint/Supports.h"
#include "../../paint/tile_element/Paint.TileElement.h"
#include "../../sprites.h"
#include "../../world/Map.h"
#include "../../world/Sprite.h"
#include "../RideData.h"
#include "../TrackData.h"
#include "../TrackPaint.h"

enum
{
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_SW_NE = 22226,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_NW_SE = 22227,
    SPR_AIR_POWERED_VERTICAL_RC_STATION_SW_NE = 22228,
    SPR_AIR_POWERED_VERTICAL_RC_STATION_NW_SE = 22229,
    SPR_AIR_POWERED_VERTICAL_RC_BRAKES_NW_SE = 22230,
    SPR_AIR_POWERED_VERTICAL_RC_BRAKES_SW_NE = 22231,

    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SW_NE_SEQ_0 = 22236,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NW_SE_SEQ_0 = 22237,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NE_SW_SEQ_0 = 22238,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SE_NW_SEQ_0 = 22239,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SW_NE_SEQ_1 = 22232,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NW_SE_SEQ_1 = 22233,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NE_SW_SEQ_1 = 22234,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SE_NW_SEQ_1 = 22235,

    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_SW_NE = 22334,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_NW_SE = 22335,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_NE_SW = 22336,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_SE_NW = 22337,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_SW_NE = 22338,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_NW_SE = 22339,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_NE_SW = 22340,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_SE_NW = 22341,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SW_NE = 22342,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NW_SE = 22343,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NE_SW = 22344,
    SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SE_NW = 22345,
    SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_SW_NE = 22346,
    SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_NW_SE = 22347,
    SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_NE_SW = 22348,
    SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_SE_NW = 22349,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_1 = 22350,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_0 = 22351,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_3 = 22352,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_2 = 22353,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_1 = 22354,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_0 = 22355,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_3 = 22356,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_2 = 22357,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_SW_NE = 22358,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_NW_SE = 22359,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_NE_SW = 22360,
    SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_SE_NW = 22361,

    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_0 = 22240,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_1 = 22241,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_2 = 22242,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_3 = 22243,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_4 = 22244,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_6 = 22245,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_0 = 22246,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_1 = 22247,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_2 = 22248,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_3 = 22249,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_4 = 22250,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_6 = 22251,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_0 = 22252,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_1 = 22253,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_2 = 22254,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_3 = 22255,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_4 = 22256,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_6 = 22257,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_0 = 22258,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_1 = 22259,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_2 = 22260,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_3 = 22261,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_4 = 22262,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_6 = 22263,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_0 = 22264,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_1 = 22265,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_2 = 22266,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_3 = 22267,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_4 = 22268,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_6 = 22269,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_5 = 22270,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_0 = 22271,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_1 = 22272,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_2 = 22273,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_3 = 22274,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_4 = 22275,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_6 = 22276,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_5 = 22277,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_0 = 22278,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_1 = 22279,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_2 = 22280,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_3 = 22281,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_4 = 22282,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_6 = 22283,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_5 = 22284,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_0 = 22285,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_1 = 22286,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_2 = 22287,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_3 = 22288,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_4 = 22289,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_6 = 22290,
    SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_5 = 22291,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_0 = 22292,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_1 = 22293,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_2 = 22294,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_3 = 22295,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_4 = 22296,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_0 = 22297,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_1 = 22298,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_2 = 22299,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_3 = 22300,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_4 = 22301,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_0 = 22302,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_1 = 22303,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_2 = 22304,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_3 = 22305,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_4 = 22306,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_0 = 22307,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_1 = 22308,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_2 = 22309,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_3 = 22310,
    SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_4 = 22311,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_0 = 22312,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_1 = 22313,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_2 = 22314,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_3 = 22315,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_4 = 22316,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_0 = 22317,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_1 = 22318,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_2 = 22319,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_3 = 22320,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_4 = 22321,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_0 = 22322,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_1 = 22323,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_2 = 22324,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_3 = 22325,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_4 = 22326,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_0 = 22327,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_1 = 22328,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_2 = 22329,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_3 = 22330,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_4 = 22331,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_FRONT_NW_SW_PART_4 = 22332,
    SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_FRONT_SE_NE_PART_0 = 22333,
};

static uint32_t air_powered_vertical_rc_get_support_colour(paint_session* session)
{
    uint32_t colourFlags = session->TrackColours[SCHEME_SUPPORTS];
    uint32_t trackColour = session->TrackColours[SCHEME_TRACK];
    if (trackColour & IMAGE_TYPE_REMAP_2_PLUS)
    {
        colourFlags |= (trackColour & 0x9F000000);
    }
    return colourFlags;
}

/** rct2: 0x008AFAD4 */
static void air_powered_vertical_rc_track_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4] = {
        SPR_AIR_POWERED_VERTICAL_RC_FLAT_SW_NE,
        SPR_AIR_POWERED_VERTICAL_RC_FLAT_NW_SE,
        SPR_AIR_POWERED_VERTICAL_RC_FLAT_SW_NE,
        SPR_AIR_POWERED_VERTICAL_RC_FLAT_NW_SE,
    };

    uint32_t imageId = imageIds[direction] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 20, 1, height, 0, 6, height);

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_station(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4][2] = {
        { SPR_AIR_POWERED_VERTICAL_RC_STATION_SW_NE, SPR_STATION_BASE_B_SW_NE },
        { SPR_AIR_POWERED_VERTICAL_RC_STATION_NW_SE, SPR_STATION_BASE_B_NW_SE },
        { SPR_AIR_POWERED_VERTICAL_RC_STATION_SW_NE, SPR_STATION_BASE_B_SW_NE },
        { SPR_AIR_POWERED_VERTICAL_RC_STATION_NW_SE, SPR_STATION_BASE_B_NW_SE },
    };

    sub_98197C_rotated(
        session, direction, imageIds[direction][1] | session->TrackColours[SCHEME_MISC], 0, 0, 32, 28, 1, height - 2, 0, 2,
        height);
    sub_98199C_rotated(
        session, direction, imageIds[direction][0] | session->TrackColours[SCHEME_TRACK], 0, 0, 32, 20, 1, height, 0, 6,
        height);

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    Ride* ride = get_ride(rideIndex);
    track_paint_util_draw_station_platform(session, ride, direction, height, 5, tileElement);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                    int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 8), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 9), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 10), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 11), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 8, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_8);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 56, 0x20);
}

static void air_powered_vertical_rc_track_60_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                    int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 12), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 13), 0, 0, 32, 1, 98, height, 0, 27,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 14), 0, 0, 32, 1, 98, height, 0, 27,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 15), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 32, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 56, TUNNEL_8);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 104, 0x20);
}

static void air_powered_vertical_rc_track_flat_to_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                            int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 0), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 1), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 2), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 3), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 3, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_8);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 48, 0x20);
}

static void air_powered_vertical_rc_track_25_deg_up_to_60_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 0), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 1), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 2), 0, 0, 32, 1, 66, height, 0, 27,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 3), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 4), 0, 0, 32, 1, 66, height, 0, 27,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 5), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 12, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_8);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 72, 0x20);
}

static void air_powered_vertical_rc_track_60_deg_up_to_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 6), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 7), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 8), 0, 0, 32, 1, 66, height, 0, 27,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 9), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 10), 0, 0, 32, 1, 66, height, 0, 27,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP + 11), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 20, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_7);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 24, TUNNEL_8);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 72, 0x20);
}

static void air_powered_vertical_rc_track_25_deg_up_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                            int32_t height, const TileElement * tileElement)
{
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 4), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 5), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 6), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE + 7), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        if (track_paint_util_should_paint_supports(session->MapPosition))
        {
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 6, height, session->TrackColours[SCHEME_SUPPORTS]);
        }
    if (direction == 0 || direction == 3)
    {
        paint_util_push_tunnel_rotated(session, direction, height - 8, TUNNEL_6);
    }
    else
    {
        paint_util_push_tunnel_rotated(session, direction, height + 8, TUNNEL_14);
    }
    paint_util_set_segment_support_height(session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction),
                                          0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 40, 0x20);
}

static void air_powered_vertical_rc_track_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                      int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_60_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                      int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_60_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_flat_to_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                              int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_25_deg_up_to_flat(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_25_deg_down_to_60_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                     uint8_t direction, int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_60_deg_up_to_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_60_deg_down_to_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                     uint8_t direction, int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_25_deg_up_to_60_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_25_deg_down_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                              int32_t height, const TileElement * tileElement)
{
    air_powered_vertical_rc_track_flat_to_25_deg_up(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}


static void air_powered_vertical_rc_track_left_quarter_turn_3(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                              int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+0), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+3), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+6), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+9), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+1), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+4), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+7), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+10), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+2), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+5), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+8), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE+11), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        switch (direction)
        {
        case 2:
            paint_util_push_tunnel_right(session, height, TUNNEL_6);
            break;
        case 3:
            paint_util_push_tunnel_left(session, height, TUNNEL_6);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_right_quarter_turn_3(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                               int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    air_powered_vertical_rc_track_left_quarter_turn_3(session, rideIndex, trackSequence, (direction - 1) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_right_quarter_turn_5(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const sprite_bb imageIds[4][5] = {
        {
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_0, { 0, 2, 0 }, { 0, 0, 0 }, { 32, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_1, { 0, 16, 0 }, { 0, 0, 0 }, { 32, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_2, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_3, { 16, 0, 0 }, { 0, 0, 0 }, { 16, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SW_SE_PART_4, { 2, 0, 0 }, { 0, 0, 0 }, { 32, 32, 2 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_0, { 2, 0, 0 }, { 0, 0, 0 }, { 32, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_1, { 16, 0, 0 }, { 0, 0, 0 }, { 16, 34, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_2, { 0, 16, 0 }, { 0, 0, 0 }, { 16, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_3, { 0, 0, 0 }, { 0, 0, 0 }, { 32, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NW_SW_PART_4, { 0, 2, 0 }, { 0, 0, 0 }, { 32, 27, 2 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_0, { 0, 2, 0 }, { 0, 0, 0 }, { 32, 27, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_1, { 0, 0, 0 }, { 0, 0, 0 }, { 32, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_2, { 16, 16, 0 }, { 0, 0, 0 }, { 16, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_3, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_NE_NW_PART_4, { 2, 0, 0 }, { 0, 0, 0 }, { 27, 32, 2 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_0, { 2, 0, 0 }, { 0, 0, 0 }, { 27, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_1, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 32, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_2, { 16, 0, 0 }, { 0, 0, 0 }, { 16, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_3, { 0, 16, 0 }, { 0, 0, 0 }, { 32, 16, 2 } },
            { SPR_AIR_POWERED_VERTICAL_RC_QUARTER_TURN_5_SE_NE_PART_4, { 0, 2, 0 }, { 0, 0, 0 }, { 32, 32, 2 } },
        }
    };

    track_paint_util_right_quarter_turn_5_tiles_paint_3(
        session, height, direction, trackSequence, session->TrackColours[SCHEME_TRACK], imageIds);
    track_paint_util_right_quarter_turn_5_tiles_wooden_supports(session, height, direction, trackSequence);
    track_paint_util_right_quarter_turn_5_tiles_tunnel(session, height, direction, trackSequence, TUNNEL_6);

    switch (trackSequence)
    {
        case 0:
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            break;
        case 1:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            break;
        case 2:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_D0 | SEGMENT_C4 | SEGMENT_D4 | SEGMENT_BC | SEGMENT_C0 | SEGMENT_CC, direction),
                0xFFFF, 0);
            break;
        case 3:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_C4 | SEGMENT_B8 | SEGMENT_BC | SEGMENT_D0 | SEGMENT_D4,
                    direction),
                0xFFFF, 0);
            break;
        case 4:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            break;
        case 5:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_D4 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C8, direction),
                0xFFFF, 0);
            break;
        case 6:
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            break;
    }

    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_left_quarter_turn_5(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    air_powered_vertical_rc_track_right_quarter_turn_5(
        session, rideIndex, trackSequence, (direction + 1) % 4, height, tileElement);
}

static void air_powered_vertical_rc_track_left_eighth_to_diag(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                              int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+0), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+4), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+8), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+12), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+1), 0, 0, 32, 16, 3, height, 0, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+5), 0, 0, 34, 16, 3, height, 0, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+9), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+13), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+2), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+6), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+10), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+14), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 4:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+3), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+7), 0, 0, 16, 18, 3, height, 0, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+11), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+15), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_right_eighth_to_diag(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                               int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+16), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+20), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+24), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+28), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+17), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+21), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+25), 0, 0, 34, 16, 3, height, 0, 0,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+29), 0, 0, 32, 16, 3, height, 0, 0,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+18), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+22), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+26), 0, 0, 28, 28, 3, height, 4, 4,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+30), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 4:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+19), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+23), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+27), 0, 0, 16, 18, 3, height, 0, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE+31), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_left_eighth_to_orthogonal(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                    uint8_t direction, int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    air_powered_vertical_rc_track_right_eighth_to_diag(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_right_eighth_to_orthogonal(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                     uint8_t direction, int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    air_powered_vertical_rc_track_left_eighth_to_diag(session, rideIndex, trackSequence, (direction + 3) & 3, height, tileElement);
}


static void air_powered_vertical_rc_track_diag_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                    int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_FLAT_DIAGONAL+3), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_FLAT_DIAGONAL+0), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_FLAT_DIAGONAL+2), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_FLAT_DIAGONAL+1), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                         int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+11), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+8), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+10), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+1), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_25_deg_up_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+7), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+4), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+6), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+5), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_flat_to_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+3), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+0), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+2), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+1), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                           int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+9), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+10), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+8), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+11), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 8, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 56, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_flat_to_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                   uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+5), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+6), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+4), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+7), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 4, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        break;
    }

    paint_util_set_general_support_height(session, height + 56, 0x20);
}

static void air_powered_vertical_rc_track_diag_25_deg_down_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                   uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        {
            switch (direction)
            {
            case 3:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+1), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 1:
        {
            switch (direction)
            {
            case 0:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+2), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 2:
        {
            switch (direction)
            {
            case 2:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+0), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    case 3:
        {
            switch (direction)
            {
            case 0:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 1:
                sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_GENTLE_DIAGONAL+3), -16, -16, 32, 32, 3, height,
                                   -16, -16, height);
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 2:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            case 3:
                metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height,
                                             session->TrackColours[SCHEME_SUPPORTS]);
                break;
            }
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 48, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_60_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                         int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+11), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+8), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+10), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 3:
	{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 32, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+9), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 36, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 32, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 36, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_25_deg_up_to_60_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                      uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+3), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+0), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+2), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 3:
	{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 16, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+1), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 16, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 16, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 16, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_60_deg_up_to_25_deg_up(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                      uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+7), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+4), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+6), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 3:
	{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 21, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+5), -16, -16, 16, 16, 3, height, 0,
				       0, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 21, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 21, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 21, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_60_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                           int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+9), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+10), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+8), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    case 3:
{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 24, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+11), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 28, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 24, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 28, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 104, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_25_deg_down_to_60_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                          uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+5), -16, -16, 16, 16, 3, height, 0,
				       0, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+6), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+4), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 3:
	{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 17, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+7), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 17, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 17, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 17, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_60_deg_down_to_25_deg_down(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                          uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
	{
		switch (direction)
		{
		case 3:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+1), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 1:
	{
		switch (direction)
		{
		case 0:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+2), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 2:
	{
		switch (direction)
		{
		case 2:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+0), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    case 3:
	{
		switch (direction)
		{
		case 0:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 8, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 1:
		    sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_STEEP_DIAGONAL+3), -16, -16, 32, 32, 3, height,
				       -16, -16, height);
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 8, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 2:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 8, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		case 3:
		    metal_b_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 8, height, session->TrackColours[SCHEME_SUPPORTS]);
		    break;
		}
	}
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 72, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_flat_to_left_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+4), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+0), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+1), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+3), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+2), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_flat_to_right_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                  uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+9), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+5), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+7), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+8), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+6), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_left_bank_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                                 int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+6), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+7), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+8), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+5), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+9), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_right_bank_to_flat(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                  uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+2), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+3), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+0), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+1), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+4), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}/** rct2: 0x008AFB74 */

static void air_powered_vertical_rc_track_diag_left_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                         int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+33), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+30), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+32), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+31), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_diag_right_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction,
                                          int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+31), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+32), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+30), -16, -16, 32, 32, 0, height,
                               -16, -16, height + 27);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_BANK_TRANSITION_DIAGONAL+33), -16, -16, 32, 32, 3, height,
                               -16, -16, height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_left_quarter_turn_3_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                   uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+0), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+1), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+4), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+7), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+11), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+2), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+5), 0, 0, 16, 16, 1, height, 0, 0,
                               height + 27);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+8), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+12), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+3), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+6), 0, 0, 1, 32, 26, height, 27, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+9), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+10), 0, 0, 1, 32, 26, height, 27, 0,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_SMALL_CURVE_BANKED+13), 0, 0, 20, 32, 3, height, 6, 0,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        switch (direction)
        {
        case 2:
            paint_util_push_tunnel_right(session, height, TUNNEL_6);
            break;
        case 3:
            paint_util_push_tunnel_left(session, height, TUNNEL_6);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_right_quarter_turn_3_bank(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                    uint8_t direction, int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
    air_powered_vertical_rc_track_left_quarter_turn_3_bank(session, rideIndex, trackSequence, (direction - 1) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_left_eighth_bank_to_diag(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                   uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+0), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+4), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+8), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+12), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+1), 0, 0, 32, 16, 3, height, 0, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+5), 0, 0, 34, 16, 0, height, 0, 0,
                               height + 27);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+9), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+13), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+2), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+6), 0, 0, 16, 16, 0, height, 16, 16,
                               height + 27);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+10), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+14), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 4:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+3), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+7), 0, 0, 16, 18, 0, height, 0, 16,
                               height + 27);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+11), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+15), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_right_eighth_bank_to_diag(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                    uint8_t direction, int32_t height, const TileElement * tileElement)
{
    switch (trackSequence)
    {
    case 0:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+16), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+20), 0, 0, 32, 20, 3, height, 0, 6,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+24), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+28), 0, 0, 32, 1, 26, height, 0, 27,
                               height);
            break;
        }
        metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 4, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
        if (direction == 0 || direction == 3)
        {
            paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 1:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+17), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+21), 0, 0, 32, 16, 3, height, 0, 16,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+25), 0, 0, 34, 16, 0, height, 0, 0,
                               height + 27);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+29), 0, 0, 32, 16, 3, height, 0, 0,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_BC | SEGMENT_C0 | SEGMENT_C4 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 2:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+18), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+22), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+26), 0, 0, 28, 28, 0, height, 4, 4,
                               height + 27);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+30), 0, 0, 16, 16, 3, height, 0, 16,
                               height);
            break;
        }
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 3:
        paint_util_set_segment_support_height(
            session, paint_util_rotate_segments(SEGMENT_C0 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_D4, direction), 0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    case 4:
        switch (direction)
        {
        case 0:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+19), 0, 0, 16, 16, 3, height, 16, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 1, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 1:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+23), 0, 0, 16, 16, 3, height, 0, 0,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 2:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+27), 0, 0, 16, 18, 0, height, 0, 16,
                               height + 27);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 2, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        case 3:
            sub_98197C_rotated(session, direction, session->TrackColours[SCHEME_TRACK] | (SPR_G2_AIR_POWERED_VERTICAL_TRACK_LARGE_CURVE_BANKED+31), 0, 0, 16, 16, 3, height, 16, 16,
                               height);
            metal_a_supports_paint_setup(session, METAL_SUPPORTS_TUBES, 3, 0, height, session->TrackColours[SCHEME_SUPPORTS]);
            break;
        }
        paint_util_set_segment_support_height(
            session,
            paint_util_rotate_segments(SEGMENT_B8 | SEGMENT_C4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_D0 | SEGMENT_D4, direction),
            0xFFFF, 0);
        paint_util_set_general_support_height(session, height + 32, 0x20);
        break;
    }
}

static void air_powered_vertical_rc_track_left_eighth_bank_to_orthogonal(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                         uint8_t direction, int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    air_powered_vertical_rc_track_right_eighth_bank_to_diag(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_right_eighth_bank_to_orthogonal(paint_session * session, uint8_t rideIndex, uint8_t trackSequence,
                                                          uint8_t direction, int32_t height, const TileElement * tileElement)
{
    trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
    air_powered_vertical_rc_track_left_eighth_bank_to_diag(session, rideIndex, trackSequence, (direction + 3) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_flat_to_left_bank(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4][2] = {
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_SW_NE, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SW_NE },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_NW_SE, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NW_SE },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_NE_SW, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NE_SW },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_SE_NW, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SE_NW },
    };

    uint32_t imageId = imageIds[direction][0] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 20, 3, height, 0, 6, height);

    if (direction == 0 || direction == 1)
    {
        imageId = imageIds[direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 1, 26, height, 0, 27, height);
    }

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

/** rct2: 0x008AFB84 */
static void air_powered_vertical_rc_track_flat_to_right_bank(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4][2] = {
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_SW_NE, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SW_NE },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_NW_SE, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NW_SE },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_NE_SW, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_NE_SW },
        { SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_RIGHT_BANK_SE_NW, SPR_AIR_POWERED_VERTICAL_RC_FLAT_TO_LEFT_BANK_FRONT_SE_NW },
    };

    uint32_t imageId = imageIds[direction][0] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 20, 3, height, 0, 6, height);

    if (direction == 2 || direction == 3)
    {
        imageId = imageIds[direction][1] | session->TrackColours[SCHEME_TRACK];
        sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 1, 26, height, 0, 27, height);
    }

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_left_bank_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    air_powered_vertical_rc_track_flat_to_right_bank(
        session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

/** rct2: 0x008AFBA4 */
static void air_powered_vertical_rc_track_right_bank_to_flat(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    air_powered_vertical_rc_track_flat_to_left_bank(
        session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_banked_right_quarter_turn_5(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const sprite_bb imageIds[4][5] = {
        {
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_0, { 0, 0, 0 }, { 0, 6, 0 }, { 32, 20, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_1, { 0, 0, 0 }, { 0, 16, 0 }, { 32, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_2, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_3, { 0, 0, 0 }, { 16, 0, 0 }, { 16, 32, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SW_SE_PART_4, { 0, 0, 0 }, { 6, 0, 0 }, { 20, 32, 3 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_0, { 0, 0, 0 }, { 6, 0, 0 }, { 20, 32, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_1, { 0, 0, 0 }, { 16, 0, 0 }, { 16, 32, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_2, { 0, 0, 0 }, { 0, 16, 0 }, { 16, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_3, { 0, 0, 0 }, { 0, 0, 0 }, { 32, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NW_SW_PART_4, { 0, 0, 0 }, { 0, 6, 0 }, { 32, 20, 3 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_0, { 0, 0, 0 }, { 0, 27, 0 }, { 32, 1, 26 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_1, { 0, 0, 0 }, { 0, 0, 27 }, { 32, 16, 1 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_2, { 0, 0, 0 }, { 16, 16, 27 }, { 16, 16, 1 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_3, { 0, 0, 0 }, { 0, 0, 27 }, { 16, 32, 1 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_NE_NW_PART_4, { 0, 0, 0 }, { 27, 0, 0 }, { 1, 32, 26 } },
        },
        {
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_0, { 0, 0, 0 }, { 6, 0, 0 }, { 20, 32, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_1, { 0, 0, 0 }, { 0, 0, 0 }, { 16, 32, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_2, { 0, 0, 0 }, { 16, 0, 0 }, { 16, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_3, { 0, 0, 0 }, { 0, 16, 0 }, { 32, 16, 3 } },
            { SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_SE_NE_PART_4, { 0, 0, 0 }, { 0, 6, 0 }, { 32, 20, 3 } },
        }
    };

    track_paint_util_right_quarter_turn_5_tiles_paint_2(
        session, height, direction, trackSequence, session->TrackColours[SCHEME_TRACK], imageIds);

    if (direction == 1 && trackSequence == 6)
    {
        uint32_t imageId = SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_FRONT_NW_SW_PART_4
            | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 32, 1, 26, height, 0, 27, height);
    }
    else if (direction == 3 && trackSequence == 0)
    {
        uint32_t imageId = SPR_AIR_POWERED_VERTICAL_RC_BANKED_QUARTER_TURN_5_FRONT_SE_NE_PART_0
            | session->TrackColours[SCHEME_TRACK];
        sub_98197C(session, imageId, 0, 0, 1, 32, 26, height, 27, 0, height);
    }

    track_paint_util_right_quarter_turn_5_tiles_wooden_supports(session, height, direction, trackSequence);
    track_paint_util_right_quarter_turn_5_tiles_tunnel(session, height, direction, trackSequence, TUNNEL_6);

    switch (trackSequence)
    {
        case 0:
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            break;
        case 1:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            break;
        case 2:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_D0 | SEGMENT_C4 | SEGMENT_D4 | SEGMENT_BC | SEGMENT_C0 | SEGMENT_CC, direction),
                0xFFFF, 0);
            break;
        case 3:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC | SEGMENT_C4 | SEGMENT_B8 | SEGMENT_BC | SEGMENT_D0 | SEGMENT_D4,
                    direction),
                0xFFFF, 0);
            break;
        case 4:
            paint_util_set_segment_support_height(
                session, paint_util_rotate_segments(SEGMENT_B4 | SEGMENT_C8 | SEGMENT_CC, direction), 0xFFFF, 0);
            break;
        case 5:
            paint_util_set_segment_support_height(
                session,
                paint_util_rotate_segments(
                    SEGMENT_D4 | SEGMENT_C4 | SEGMENT_D0 | SEGMENT_B8 | SEGMENT_C0 | SEGMENT_C8, direction),
                0xFFFF, 0);
            break;
        case 6:
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            break;
    }

    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_banked_left_quarter_turn_5(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    trackSequence = mapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
    air_powered_vertical_rc_track_banked_right_quarter_turn_5(
        session, rideIndex, trackSequence, (direction + 1) % 4, height, tileElement);
}

/** rct2: 0x008AFBD4 */
static void air_powered_vertical_rc_track_left_bank(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4] = {
        SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_SW_NE,
        SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_NW_SE,
        SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_NE_SW,
        SPR_AIR_POWERED_VERTICAL_RC_LEFT_BANK_SE_NW,
    };

    uint32_t imageId = imageIds[direction] | session->TrackColours[SCHEME_TRACK];
    if (direction == 0 || direction == 1)
    {
        sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 1, 26, height, 0, 27, height);
    }
    else
    {
        sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 20, 3, height, 0, 6, height);
    }

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_right_bank(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    air_powered_vertical_rc_track_left_bank(session, rideIndex, trackSequence, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_brakes(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4] = {
        SPR_AIR_POWERED_VERTICAL_RC_BRAKES_NW_SE,
        SPR_AIR_POWERED_VERTICAL_RC_BRAKES_SW_NE,
        SPR_AIR_POWERED_VERTICAL_RC_BRAKES_NW_SE,
        SPR_AIR_POWERED_VERTICAL_RC_BRAKES_SW_NE,
    };

    uint32_t imageId = imageIds[direction] | session->TrackColours[SCHEME_TRACK];
    sub_98197C_rotated(session, direction, imageId, 0, 0, 32, 20, 1, height, 0, 6, height);

    wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

    paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 32, 0x20);
}

static void air_powered_vertical_rc_track_vertical_slope_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t trackImageIds[7][4] = {
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_0,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_0,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_0,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_0,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_1,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_2,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_3,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_4,
        },
        { 0, 0, 0, 0 },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SW_NE_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NW_SE_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_NE_SW_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SE_NW_6,
        },
    };

    static constexpr const uint32_t supportImageIds[7][4] = {
        {
            22264,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_0,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_0,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_0,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_1,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_1,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_2,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_2,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_3,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_3,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_4,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_4,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_5,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_5,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_5,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_5,
        },
        {
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SW_NE_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NW_SE_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_NE_SW_6,
            SPR_AIR_POWERED_VERTICAL_RC_SLOPE_SUPPORTS_SE_NW_6,
        },
    };

    static constexpr const int8_t bbHeights03[] = { 1, 6, 14, 37, 76 };
    static constexpr const int8_t bbHeights12[] = { 1, 6, 14, 27, 59 };
    static constexpr const int32_t supportHeights[] = { 48, 64, 128, 176, 208, 240, 240 };

    uint32_t supportsImageId = supportImageIds[trackSequence][direction] | air_powered_vertical_rc_get_support_colour(session);
    uint32_t trackImageId = trackImageIds[trackSequence][direction] | session->TrackColours[SCHEME_TRACK];
    int8_t bbHeight;
    bool isDirection03 = (direction == 0 || direction == 3);
    switch (trackSequence)
    {
        case 0:
            // HACK this might be a mistake in original code
            if (direction & 1)
            {
                bbHeight = bbHeights12[trackSequence];
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 20, 32, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 20, 32, bbHeight, height, 0, 6, height);

                wooden_a_supports_paint_setup(session, 0, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);

                paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
                paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
                break;
            }
        case 1:
        case 2:
        case 3:
            if (isDirection03)
            {
                bbHeight = bbHeights03[trackSequence];

                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
            }
            else
            {
                bbHeight = bbHeights12[trackSequence];
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
            }

            wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

            if (trackSequence == 0)
            {
                paint_util_push_tunnel_rotated(session, direction, height, TUNNEL_6);
            }

            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
        case 4:
            if (isDirection03)
            {
                bbHeight = bbHeights03[trackSequence];
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
            }
            else
            {
                bbHeight = bbHeights12[trackSequence];
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
                sub_98199C_rotated(session, direction, supportsImageId, 0, 0, 32, 20, bbHeight, height, 0, 6, height);
            }

            wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
        case 5:
            if (wooden_a_supports_paint_setup(
                    session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr))
            {
                uint32_t floorImageId;
                if (direction & 1)
                {
                    floorImageId = SPR_FLOOR_PLANKS_90_DEG | session->TrackColours[SCHEME_SUPPORTS];
                }
                else
                {
                    floorImageId = SPR_FLOOR_PLANKS | session->TrackColours[SCHEME_SUPPORTS];
                }
                sub_98197C(session, floorImageId, 0, 0, 26, 26, 126, height, 3, 3, height);
                sub_98199C_rotated(session, direction, supportsImageId, 0, 0, 26, 26, 126, height, 3, 3, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 26, 26, 126, height, 3, 3, height);
            }
            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
        case 6:
            if (isDirection03)
            {
                sub_98197C_rotated(session, direction, supportsImageId, 0, 0, 5, 20, 79, height, 0, 6, height + 128);
                sub_98199C_rotated(session, direction, trackImageId, 0, 0, 5, 20, 79, height, 0, 6, height + 128);
            }
            else
            {
                sub_98197C_rotated(session, direction, trackImageId, 0, 0, 1, 20, 126, height, 27, 6, height);
                sub_98199C_rotated(session, direction, supportsImageId, 0, 0, 1, 20, 126, height, 27, 6, height);
            }
            wooden_a_supports_paint_setup(session, direction & 1, 0, height, session->TrackColours[SCHEME_SUPPORTS], nullptr);

            paint_util_set_vertical_tunnel(session, height + 240);

            paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
            paint_util_set_general_support_height(session, height + supportHeights[trackSequence], 0x20);
            break;
    }
}

static void air_powered_vertical_rc_track_vertical_up(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    static constexpr const uint32_t imageIds[4][2] = {
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SW_NE_SEQ_0, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SW_NE_SEQ_1 },
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NW_SE_SEQ_0, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NW_SE_SEQ_1 },
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NE_SW_SEQ_0, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_NE_SW_SEQ_1 },
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SE_NW_SEQ_0, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_UP_SE_NW_SEQ_1 },
    };

    uint32_t imageId;
    switch (trackSequence)
    {
        case 0:
            imageId = imageIds[direction][0] | air_powered_vertical_rc_get_support_colour(session);
            sub_98197C_rotated(session, direction, imageId, 0, 0, 26, 26, 79, height, 3, 3, height);
            break;
        case 1:
            imageId = imageIds[direction][1] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0 || direction == 3)
            {
                sub_98197C_rotated(session, direction, imageId, 0, 0, 2, 20, 79, height, 0, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, imageId, 0, 0, 2, 20, 79, height, 30, 6, height);
            }

            paint_util_set_vertical_tunnel(session, height + 80);
            break;
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 80, 0x20);
}

static void air_powered_vertical_rc_track_vertical_top(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    if (direction == 2 || direction == 3)
    {
        air_powered_vertical_rc_track_vertical_top(
            session, rideIndex, 3 - trackSequence, (direction + 2) & 3, height, tileElement);
        return;
    }

    static constexpr const uint32_t imageIds[4][6] = {
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_SW_NE, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_0,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_1,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_2, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_NE_SW,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_SW_NE_SEQ_3 },
        { SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_NW_SE, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_0,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_1,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_2, SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_SUPPORT_SE_NW,
          SPR_AIR_POWERED_VERTICAL_RC_VERTICAL_TOP_TRACK_NW_SE_SEQ_3 },
    };

    uint32_t imageIdS, imageIdT;
    switch (trackSequence)
    {
        case 0:
            imageIdS = imageIds[direction][0] | air_powered_vertical_rc_get_support_colour(session);
            imageIdT = imageIds[direction][1] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0)
            {
                sub_98197C_rotated(session, direction, imageIdS, 0, 0, 32, 20, 15, height, 0, 6, height);
                sub_98199C_rotated(session, direction, imageIdT, 0, 0, 31, 20, 15, height, 1, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, imageIdS, 0, 0, 5, 20, 1, height, 24, 6, height);
                sub_98199C_rotated(session, direction, imageIdT, 0, 0, 5, 20, 1, height, 24, 6, height);
            }
            break;
        case 1:
            imageIdT = imageIds[direction][2] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0)
            {
                sub_98197C_rotated(session, direction, imageIdT, 0, 0, 2, 20, 15, height, 0, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, imageIdT, 0, 0, 2, 20, 1, height, 33, 6, height);
            }
            paint_util_set_vertical_tunnel(session, height + 80);
            break;
        case 2:
            imageIdT = imageIds[direction][3] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0)
            {
                sub_98197C_rotated(session, direction, imageIdT, 0, 0, 2, 20, 1, height, 33, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, imageIdT, 0, 0, 2, 20, 15, height, 0, 6, height);
            }
            paint_util_set_vertical_tunnel(session, height + 80);
            break;
        case 3:
            imageIdS = imageIds[direction][4] | air_powered_vertical_rc_get_support_colour(session);
            imageIdT = imageIds[direction][5] | session->TrackColours[SCHEME_TRACK];
            if (direction == 0)
            {
                sub_98197C_rotated(session, direction, imageIdS, 0, 0, 5, 20, 1, height, 24, 6, height);
                sub_98199C_rotated(session, direction, imageIdT, 0, 0, 5, 20, 1, height, 24, 6, height);
            }
            else
            {
                sub_98197C_rotated(session, direction, imageIdS, 0, 0, 32, 20, 15, height, 0, 6, height);
                sub_98199C_rotated(session, direction, imageIdT, 0, 0, 32, 20, 15, height, 0, 6, height);
            }
            break;
    }

    paint_util_set_segment_support_height(session, SEGMENTS_ALL, 0xFFFF, 0);
    paint_util_set_general_support_height(session, height + 80, 0x20);
}

static void air_powered_vertical_rc_track_vertical_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    air_powered_vertical_rc_track_vertical_up(session, rideIndex, trackSequence ^ 1, (direction + 2) & 3, height, tileElement);
}

static void air_powered_vertical_rc_track_vertical_slope_down(
    paint_session* session, uint8_t rideIndex, uint8_t trackSequence, uint8_t direction, int32_t height,
    const TileElement* tileElement)
{
    air_powered_vertical_rc_track_vertical_slope_up(
        session, rideIndex, 6 - trackSequence, (direction + 2) & 3, height, tileElement);
}

TRACK_PAINT_FUNCTION get_track_paint_function_air_powered_vertical_rc(int32_t trackType, int32_t direction)
{
    switch (trackType)
    {
        case TRACK_ELEM_FLAT:
            return air_powered_vertical_rc_track_flat;
        case TRACK_ELEM_END_STATION:
        case TRACK_ELEM_BEGIN_STATION:
        case TRACK_ELEM_MIDDLE_STATION:
            return air_powered_vertical_rc_track_station;
	case TRACK_ELEM_25_DEG_UP:
            return air_powered_vertical_rc_track_25_deg_up;
        case TRACK_ELEM_60_DEG_UP:
            return air_powered_vertical_rc_track_60_deg_up;
        case TRACK_ELEM_FLAT_TO_25_DEG_UP:
            return air_powered_vertical_rc_track_flat_to_25_deg_up;
        case TRACK_ELEM_25_DEG_UP_TO_60_DEG_UP:
            return air_powered_vertical_rc_track_25_deg_up_to_60_deg_up;
        case TRACK_ELEM_60_DEG_UP_TO_25_DEG_UP:
            return air_powered_vertical_rc_track_60_deg_up_to_25_deg_up;
        case TRACK_ELEM_25_DEG_UP_TO_FLAT:
            return air_powered_vertical_rc_track_25_deg_up_to_flat;
        case TRACK_ELEM_25_DEG_DOWN:
            return air_powered_vertical_rc_track_25_deg_down;
        case TRACK_ELEM_60_DEG_DOWN:
            return air_powered_vertical_rc_track_60_deg_down;
        case TRACK_ELEM_FLAT_TO_25_DEG_DOWN:
            return air_powered_vertical_rc_track_flat_to_25_deg_down;
        case TRACK_ELEM_25_DEG_DOWN_TO_60_DEG_DOWN:
            return air_powered_vertical_rc_track_25_deg_down_to_60_deg_down;
        case TRACK_ELEM_60_DEG_DOWN_TO_25_DEG_DOWN:
            return air_powered_vertical_rc_track_60_deg_down_to_25_deg_down;
        case TRACK_ELEM_25_DEG_DOWN_TO_FLAT:
            return air_powered_vertical_rc_track_25_deg_down_to_flat;
        case TRACK_ELEM_LEFT_QUARTER_TURN_5_TILES:
            return air_powered_vertical_rc_track_left_quarter_turn_5;
        case TRACK_ELEM_RIGHT_QUARTER_TURN_5_TILES:
            return air_powered_vertical_rc_track_right_quarter_turn_5;
        case TRACK_ELEM_FLAT_TO_LEFT_BANK:
            return air_powered_vertical_rc_track_flat_to_left_bank;
        case TRACK_ELEM_FLAT_TO_RIGHT_BANK:
            return air_powered_vertical_rc_track_flat_to_right_bank;
        case TRACK_ELEM_LEFT_BANK_TO_FLAT:
            return air_powered_vertical_rc_track_left_bank_to_flat;
        case TRACK_ELEM_RIGHT_BANK_TO_FLAT:
            return air_powered_vertical_rc_track_right_bank_to_flat;
        case TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES:
            return air_powered_vertical_rc_track_left_quarter_turn_3;
        case TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES:
            return air_powered_vertical_rc_track_right_quarter_turn_3;
        case TRACK_ELEM_BANKED_LEFT_QUARTER_TURN_5_TILES:
            return air_powered_vertical_rc_track_banked_left_quarter_turn_5;
        case TRACK_ELEM_BANKED_RIGHT_QUARTER_TURN_5_TILES:
            return air_powered_vertical_rc_track_banked_right_quarter_turn_5;
        case TRACK_ELEM_LEFT_EIGHTH_TO_DIAG:
            return air_powered_vertical_rc_track_left_eighth_to_diag;
        case TRACK_ELEM_RIGHT_EIGHTH_TO_DIAG:
            return air_powered_vertical_rc_track_right_eighth_to_diag;
        case TRACK_ELEM_LEFT_EIGHTH_TO_ORTHOGONAL:
            return air_powered_vertical_rc_track_left_eighth_to_orthogonal;
        case TRACK_ELEM_RIGHT_EIGHTH_TO_ORTHOGONAL:
            return air_powered_vertical_rc_track_right_eighth_to_orthogonal;
        case TRACK_ELEM_DIAG_FLAT:
            return air_powered_vertical_rc_track_diag_flat;
        case TRACK_ELEM_DIAG_25_DEG_UP:
            return air_powered_vertical_rc_track_diag_25_deg_up;
        case TRACK_ELEM_DIAG_60_DEG_UP:
            return air_powered_vertical_rc_track_diag_60_deg_up;
        case TRACK_ELEM_DIAG_FLAT_TO_25_DEG_UP:
            return air_powered_vertical_rc_track_diag_flat_to_25_deg_up;
        case TRACK_ELEM_DIAG_25_DEG_UP_TO_60_DEG_UP:
            return air_powered_vertical_rc_track_diag_25_deg_up_to_60_deg_up;
        case TRACK_ELEM_DIAG_60_DEG_UP_TO_25_DEG_UP:
            return air_powered_vertical_rc_track_diag_60_deg_up_to_25_deg_up;
        case TRACK_ELEM_DIAG_25_DEG_UP_TO_FLAT:
            return air_powered_vertical_rc_track_diag_25_deg_up_to_flat;
        case TRACK_ELEM_DIAG_25_DEG_DOWN:
            return air_powered_vertical_rc_track_diag_25_deg_down;
        case TRACK_ELEM_DIAG_60_DEG_DOWN:
            return air_powered_vertical_rc_track_diag_60_deg_down;
        case TRACK_ELEM_DIAG_FLAT_TO_25_DEG_DOWN:
            return air_powered_vertical_rc_track_diag_flat_to_25_deg_down;
        case TRACK_ELEM_DIAG_25_DEG_DOWN_TO_60_DEG_DOWN:
            return air_powered_vertical_rc_track_diag_25_deg_down_to_60_deg_down;
        case TRACK_ELEM_DIAG_60_DEG_DOWN_TO_25_DEG_DOWN:
            return air_powered_vertical_rc_track_diag_60_deg_down_to_25_deg_down;
        case TRACK_ELEM_DIAG_25_DEG_DOWN_TO_FLAT:
            return air_powered_vertical_rc_track_diag_25_deg_down_to_flat;
        case TRACK_ELEM_DIAG_FLAT_TO_LEFT_BANK:
            return air_powered_vertical_rc_track_diag_flat_to_left_bank;
        case TRACK_ELEM_DIAG_FLAT_TO_RIGHT_BANK:
            return air_powered_vertical_rc_track_diag_flat_to_right_bank;
        case TRACK_ELEM_DIAG_LEFT_BANK_TO_FLAT:
            return air_powered_vertical_rc_track_diag_left_bank_to_flat;
        case TRACK_ELEM_DIAG_RIGHT_BANK_TO_FLAT:
            return air_powered_vertical_rc_track_diag_right_bank_to_flat;
        case TRACK_ELEM_DIAG_LEFT_BANK:
            return air_powered_vertical_rc_track_diag_left_bank;
        case TRACK_ELEM_DIAG_RIGHT_BANK:
            return air_powered_vertical_rc_track_diag_right_bank;
        case TRACK_ELEM_LEFT_QUARTER_TURN_3_TILES_BANK:
            return air_powered_vertical_rc_track_left_quarter_turn_3_bank;
        case TRACK_ELEM_RIGHT_QUARTER_TURN_3_TILES_BANK:
            return air_powered_vertical_rc_track_right_quarter_turn_3_bank;
        case TRACK_ELEM_LEFT_EIGHTH_BANK_TO_DIAG:
            return air_powered_vertical_rc_track_left_eighth_bank_to_diag;
        case TRACK_ELEM_RIGHT_EIGHTH_BANK_TO_DIAG:
            return air_powered_vertical_rc_track_right_eighth_bank_to_diag;
        case TRACK_ELEM_LEFT_EIGHTH_BANK_TO_ORTHOGONAL:
            return air_powered_vertical_rc_track_left_eighth_bank_to_orthogonal;
        case TRACK_ELEM_RIGHT_EIGHTH_BANK_TO_ORTHOGONAL:
            return air_powered_vertical_rc_track_right_eighth_bank_to_orthogonal;
        case TRACK_ELEM_LEFT_BANK:
            return air_powered_vertical_rc_track_left_bank;
        case TRACK_ELEM_RIGHT_BANK:
            return air_powered_vertical_rc_track_right_bank;
        case TRACK_ELEM_BRAKES:
            return air_powered_vertical_rc_track_brakes;
        case TRACK_ELEM_REVERSE_FREEFALL_SLOPE:
            return air_powered_vertical_rc_track_vertical_slope_up;
        case TRACK_ELEM_REVERSE_FREEFALL_VERTICAL:
            return air_powered_vertical_rc_track_vertical_up;
        case TRACK_ELEM_AIR_THRUST_TOP_CAP:
            return air_powered_vertical_rc_track_vertical_top;
        case TRACK_ELEM_AIR_THRUST_VERTICAL_DOWN:
            return air_powered_vertical_rc_track_vertical_down;
        case TRACK_ELEM_AIR_THRUST_VERTICAL_DOWN_TO_LEVEL:
            return air_powered_vertical_rc_track_vertical_slope_down;
    }
    return nullptr;
}
