/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../../../drawing/LightFX.h"
#include "../../../sprites.h"
#include "../../RideData.h"
#include "../../ShopItem.h"
#include "../../Track.h"

// clang-format off
constexpr RideTypeDescriptor DodgemsRTD =
{
    SET_FIELD(AlternateType, RIDE_TYPE_NULL),
    SET_FIELD(Category, RIDE_CATEGORY_GENTLE),
    SET_FIELD(EnabledTrackPieces, {}),
    SET_FIELD(ExtraTrackPieces, {}),
    SET_FIELD(CoveredTrackPieces, {}),
    SET_FIELD(StartTrackPiece, TrackElemType::FlatTrack4x4),
    SET_FIELD(TrackPaintFunctions, TrackDrawerDescriptor(GetTrackPaintFunctionDodgems)),
    SET_FIELD(Flags, RIDE_TYPE_FLAGS_TRACK_HAS_3_COLOURS | RIDE_TYPE_FLAG_HAS_SINGLE_PIECE_STATION |
                     RIDE_TYPE_FLAG_CANNOT_HAVE_GAPS | RIDE_TYPE_FLAG_NO_TEST_MODE | RIDE_TYPE_FLAG_TRACK_NO_WALLS |
                     RIDE_TYPE_FLAG_FLAT_RIDE | RIDE_TYPE_FLAG_HAS_VEHICLE_COLOURS | RIDE_TYPE_FLAG_MUSIC_ON_DEFAULT |
                     RIDE_TYPE_FLAG_ALLOW_MUSIC | RIDE_TYPE_FLAG_HAS_ENTRANCE_EXIT |
                     RIDE_TYPE_FLAG_SLIGHTLY_INTERESTING_TO_LOOK_AT | RIDE_TYPE_FLAG_LIST_VEHICLES_SEPARATELY),
    SET_FIELD(RideModes, EnumsToFlags(RideMode::Dodgems)),
    SET_FIELD(DefaultMode, RideMode::Dodgems),
    SET_FIELD(OperatingSettings, { 20, 180, 0, 0, 0, 0 }),
    SET_FIELD(Naming, { STR_RIDE_NAME_DODGEMS, STR_RIDE_DESCRIPTION_DODGEMS }),
    SET_FIELD(NameConvention, { RideComponentType::Car, RideComponentType::Building, RideComponentType::Station }),
    SET_FIELD(EnumName, nameof(RIDE_TYPE_DODGEMS)),
    SET_FIELD(AvailableBreakdowns, (1 << BREAKDOWN_SAFETY_CUT_OUT)),
    SET_FIELD(Heights, { 9, 48, 2, 2, }),
    SET_FIELD(MaxMass, 255),
    SET_FIELD(LiftData, { OpenRCT2::Audio::SoundId::Null, 5, 5 }),
    SET_FIELD(RatingsMultipliers, { 40, 20, 0 }),
    SET_FIELD(UpkeepCosts, { 50, 1, 0, 5, 0, 0 }),
    SET_FIELD(BuildCosts, { 27.50_GBP, 1.00_GBP, 1, }),
    SET_FIELD(DefaultPrices, { 15, 0 }),
    SET_FIELD(DefaultMusic, MUSIC_OBJECT_DODGEMS),
    SET_FIELD(PhotoItem, ShopItem::Photo),
    SET_FIELD(BonusValue, 35),
    SET_FIELD(ColourPresets, TRACK_COLOUR_PRESETS(
        { COLOUR_BRIGHT_RED, COLOUR_SATURATED_BROWN, COLOUR_SATURATED_BROWN },
        { COLOUR_LIGHT_BLUE, COLOUR_GREY, COLOUR_GREY },
        { COLOUR_DARK_GREEN, COLOUR_BLACK, COLOUR_DARK_GREEN },
        { COLOUR_YELLOW, COLOUR_LIGHT_BROWN, COLOUR_DARK_YELLOW },
        { COLOUR_SATURATED_GREEN, COLOUR_SATURATED_BROWN, COLOUR_SATURATED_BROWN },
    )),
    SET_FIELD(ColourPreview, { SPR_RIDE_DESIGN_PREVIEW_DODGEMS_TRACK, SPR_RIDE_DESIGN_PREVIEW_DODGEMS_SUPPORTS }),
    SET_FIELD(ColourKey, RideColourKey::Ride),
    SET_FIELD(Name, "dodgems"),
    SET_FIELD(RatingsData,
    {
        RatingsCalculationType::FlatRide,
        { RIDE_RATING(1, 30), RIDE_RATING(0, 50), RIDE_RATING(0, 35) },
        16,
        7,
        false,
        {
            // Special case, passing -2 to represent division by 2
            { RatingsModifierType::BonusOperationOption, 0, 1, -2, 0 }, 
            { RatingsModifierType::BonusNumTrains,       4, RIDE_RATING(0, 80), 0, 0 },
            { RatingsModifierType::BonusScenery,         0, 5577, 0, 0 },
        },
    }),
    SET_FIELD(AlternateTrackList, {0,{{0,static_cast<uint32_t>(SPR_NONE)}}}),
    SET_FIELD(UpdateRotating, UpdateRotatingDefault),
    SET_FIELD(LightFXAddLightsMagicVehicle, LightFxAddLightsMagicVehicle_BoatHire),
};
// clang-format on
