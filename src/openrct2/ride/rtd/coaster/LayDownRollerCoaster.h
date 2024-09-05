/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../../../sprites.h"
#include "../../RideData.h"
#include "../../ShopItem.h"
#include "../../Track.h"

// clang-format off
constexpr RideTypeDescriptor LayDownRollerCoasterRTD =
{
    .Category = RIDE_CATEGORY_ROLLERCOASTER,
    .StartTrackPiece = OpenRCT2::TrackElemType::EndStation,
    .TrackPaintFunctions = TrackDrawerDescriptor(
        {
            .Drawer = GetTrackPaintFunctionCorkscrewRC,
            .supportType = MetalSupportType::TubesInverted,
            .EnabledTrackPieces = { TrackGroup::straight, TrackGroup::stationEnd, TrackGroup::liftHill, TrackGroup::flatRollBanking, TrackGroup::verticalLoop, TrackGroup::slope, TrackGroup::slopeSteepUp, TrackGroup::slopeSteepDown, TrackGroup::slopeCurve, TrackGroup::slopeCurveSteep, TrackGroup::sBend, TrackGroup::curveSmall, TrackGroup::curve, TrackGroup::curveLarge, TrackGroup::helixDownBankedHalf, TrackGroup::helixUpBankedHalf, TrackGroup::brakes, TrackGroup::onridePhoto, TrackGroup::blockBrakes, TrackGroup::inlineTwistUninverted, TrackGroup::flyingHalfLoopUninvertedUp, TrackGroup::corkscrewUninverted, TrackGroup::slopeSteepLong, TrackGroup::diagBrakes, TrackGroup::diagBlockBrakes },
            .ExtraTrackPieces = { TrackGroup::flyingHalfLoopUninvertedDown, TrackGroup::booster },
        }
    ),
    .InvertedTrackPaintFunctions = TrackDrawerDescriptor(
        {
            .Drawer = GetTrackPaintFunctionLayDownRCInverted,
            .supportType = MetalSupportType::TubesInverted,
            .EnabledTrackPieces = { TrackGroup::straight, TrackGroup::flatRollBanking, TrackGroup::slope, TrackGroup::slopeSteepUp, TrackGroup::slopeSteepDown, TrackGroup::slopeCurve, TrackGroup::slopeCurveSteep, TrackGroup::sBend, TrackGroup::curveSmall, TrackGroup::curve, TrackGroup::curveLarge, TrackGroup::helixDownBankedQuarter, TrackGroup::helixUpBankedQuarter, TrackGroup::brakes, TrackGroup::onridePhoto, TrackGroup::blockBrakes, TrackGroup::inlineTwistInverted, TrackGroup::flyingHalfLoopInvertedDown, TrackGroup::corkscrewInverted },
            .ExtraTrackPieces = { TrackGroup::flyingHalfLoopInvertedUp, TrackGroup::stationEnd, TrackGroup::liftHill },
        }
    ),
    .Flags = kRtdFlagsHasThreeColours | kRtdFlagsCommonCoaster | kRtdFlagsCommonCoasterNonAlt | 
        EnumsToFlags(RtdFlag::hasLeaveWhenAnotherVehicleArrivesAtStation, RtdFlag::hasInvertedVariant,
                     RtdFlag::checkGForces, RtdFlag::allowMultipleCircuits),
    .RideModes = EnumsToFlags(RideMode::ContinuousCircuit, RideMode::ContinuousCircuitBlockSectioned),
    .DefaultMode = RideMode::ContinuousCircuit,
    .OperatingSettings = { 10, 27 },
    .BoosterSettings = { 25, 25 },
    .LegacyBoosterSettings = { 25, 25 },
    .Naming = { STR_RIDE_NAME_LAY_DOWN_ROLLER_COASTER, STR_RIDE_DESCRIPTION_LAY_DOWN_ROLLER_COASTER },
    .NameConvention = { RideComponentType::Train, RideComponentType::Track, RideComponentType::Station },
    .EnumName = "RIDE_TYPE_LAY_DOWN_ROLLER_COASTER",
    .AvailableBreakdowns = (1 << BREAKDOWN_SAFETY_CUT_OUT) | (1 << BREAKDOWN_RESTRAINTS_STUCK_CLOSED) | (1 << BREAKDOWN_RESTRAINTS_STUCK_OPEN) | (1 << BREAKDOWN_VEHICLE_MALFUNCTION) | (1 << BREAKDOWN_BRAKES_FAILURE),
    .Heights = { 26, 24, 8, 11, },
    .MaxMass = 25,
    .LiftData = { OpenRCT2::Audio::SoundId::LiftClassic, 4, 6 },
    .RatingsMultipliers = { 50, 30, 10 },
    .UpkeepCosts = { 49, 20, 90, 11, 3, 15 },
    .BuildCosts = { 53.50_GBP, 2.50_GBP, 50, },
    .DefaultPrices = { 20, 20 },
    .DefaultMusic = MUSIC_OBJECT_ROCK_1,
    .PhotoItem = ShopItem::Photo2,
    .BonusValue = 100,
    .ColourPresets = TRACK_COLOUR_PRESETS(
        { COLOUR_BRIGHT_RED, COLOUR_BRIGHT_RED, COLOUR_YELLOW },
        { COLOUR_DARK_PURPLE, COLOUR_DARK_PURPLE, COLOUR_YELLOW },
        { COLOUR_YELLOW, COLOUR_YELLOW, COLOUR_DARK_GREEN },
    ),
    .ColourPreview = { SPR_RIDE_DESIGN_PREVIEW_LAY_DOWN_ROLLER_COASTER_TRACK, SPR_RIDE_DESIGN_PREVIEW_LAY_DOWN_ROLLER_COASTER_SUPPORTS },
    .ColourKey = RideColourKey::Ride,
    .Name = "lay_down_rc",
    .RatingsData = 
    {
        RatingsCalculationType::Normal,
        { RIDE_RATING(3, 85), RIDE_RATING(1, 15), RIDE_RATING(2, 75) },
        18,
        -1,
        true,
        {
            { RatingsModifierType::BonusLength,           6000,             764, 0, 0 },
            { RatingsModifierType::BonusSynchronisation,  0,                RIDE_RATING(0, 40), RIDE_RATING(0, 05), 0 },
            { RatingsModifierType::BonusTrainLength,      0,                187245, 0, 0 },
            { RatingsModifierType::BonusMaxSpeed,         0,                44281, 88562, 35424 },
            { RatingsModifierType::BonusAverageSpeed,     0,                291271, 436906, 0 },
            { RatingsModifierType::BonusDuration,         150,              26214, 0, 0 },
            { RatingsModifierType::BonusGForces,          0,                24576, 38130, 49648 },
            { RatingsModifierType::BonusTurns,            0,                26749, 34767, 45749 },
            { RatingsModifierType::BonusDrops,            0,                29127, 46811, 49152 },
            { RatingsModifierType::BonusSheltered,        0,                15420, 32768, 35108 },
            { RatingsModifierType::BonusProximity,        0,                20130, 0, 0 },
            { RatingsModifierType::BonusScenery,          0,                6693, 0, 0 },
            { RatingsModifierType::RequirementInversions, 1,                4, 2, 2 },
            { RatingsModifierType::RequirementMaxSpeed,   0xA0000,          2, 2, 2 },
            { RatingsModifierType::RequirementNegativeGs, FIXED_2DP(0, 40), 2, 2, 2 },
            { RatingsModifierType::RequirementNumDrops,   2,                2, 2, 2 },
            { RatingsModifierType::PenaltyLateralGs,      0,                24576, 38130, 49648 },
        },
    },
};

constexpr RideTypeDescriptor LayDownRollerCoasterAltRTD =
{
    .Category = RIDE_CATEGORY_NONE,
    .StartTrackPiece = OpenRCT2::TrackElemType::EndStation,
    .TrackPaintFunctions = TrackDrawerDescriptor({ 
        .Drawer = nullptr,
        .EnabledTrackPieces = {  },
        .ExtraTrackPieces = {  },
    }),
    .InvertedTrackPaintFunctions = TrackDrawerDescriptor({
        .Drawer = nullptr,
        .EnabledTrackPieces = {  },
        .ExtraTrackPieces = {  },
    }),
    .Flags = kRtdFlagsHasThreeColours | kRtdFlagsCommonCoaster | 
        EnumsToFlags(RtdFlag::hasLeaveWhenAnotherVehicleArrivesAtStation),
    .RideModes = EnumsToFlags(RideMode::ContinuousCircuit, RideMode::ContinuousCircuitBlockSectioned),
    .DefaultMode = RideMode::ContinuousCircuit,
    .OperatingSettings = { 10, 27 },
    .BoosterSettings = { 25, 25 },
    .LegacyBoosterSettings = { 25, 25 },
    .Naming = { STR_RIDE_NAME_40, STR_RIDE_DESCRIPTION_UNKNOWN },
    .NameConvention = { RideComponentType::Train, RideComponentType::Track, RideComponentType::Station },
    .EnumName = "RIDE_TYPE_LAY_DOWN_ROLLER_COASTER_ALT",
    .AvailableBreakdowns = (1 << BREAKDOWN_SAFETY_CUT_OUT) | (1 << BREAKDOWN_RESTRAINTS_STUCK_CLOSED) | (1 << BREAKDOWN_RESTRAINTS_STUCK_OPEN) | (1 << BREAKDOWN_VEHICLE_MALFUNCTION) | (1 << BREAKDOWN_BRAKES_FAILURE),
    .Heights = { 26, 24, 8, 11, },
    .MaxMass = 25,
    .LiftData = { OpenRCT2::Audio::SoundId::LiftClassic, 4, 6 },
    .RatingsMultipliers = { 50, 30, 10 },
    .UpkeepCosts = { 49, 20, 90, 11, 3, 15 },
    .BuildCosts = { 53.50_GBP, 2.50_GBP, 50, },
    .DefaultPrices = { 20, 20 },
    .DefaultMusic = MUSIC_OBJECT_ROCK_1,
    .PhotoItem = ShopItem::Photo2,
    .BonusValue = 100,
    .ColourPresets = TRACK_COLOUR_PRESETS(
        { COLOUR_BRIGHT_RED, COLOUR_BRIGHT_RED, COLOUR_YELLOW },
        { COLOUR_DARK_PURPLE, COLOUR_DARK_PURPLE, COLOUR_YELLOW },
        { COLOUR_YELLOW, COLOUR_YELLOW, COLOUR_DARK_GREEN },
    ),
    .ColourPreview = { SPR_RIDE_DESIGN_PREVIEW_LAY_DOWN_ROLLER_COASTER_TRACK, SPR_RIDE_DESIGN_PREVIEW_LAY_DOWN_ROLLER_COASTER_SUPPORTS },
    .ColourKey = RideColourKey::Ride,
    .Name = "lay_down_rc_alt",
    .RatingsData = 
    {
        RatingsCalculationType::Normal,
        { RIDE_RATING(3, 85), RIDE_RATING(1, 15), RIDE_RATING(2, 75) },
        18,
        -1,
        true,
        {
            { RatingsModifierType::BonusLength,           6000,             764, 0, 0 },
            { RatingsModifierType::BonusSynchronisation,  0,                RIDE_RATING(0, 40), RIDE_RATING(0, 05), 0 },
            { RatingsModifierType::BonusTrainLength,      0,                187245, 0, 0 },
            { RatingsModifierType::BonusMaxSpeed,         0,                44281, 88562, 35424 },
            { RatingsModifierType::BonusAverageSpeed,     0,                291271, 436906, 0 },
            { RatingsModifierType::BonusDuration,         150,              26214, 0, 0 },
            { RatingsModifierType::BonusGForces,          0,                24576, 38130, 49648 },
            { RatingsModifierType::BonusTurns,            0,                26749, 34767, 45749 },
            { RatingsModifierType::BonusDrops,            0,                29127, 46811, 49152 },
            { RatingsModifierType::BonusSheltered,        0,                15420, 32768, 35108 },
            { RatingsModifierType::BonusProximity,        0,                20130, 0, 0 },
            { RatingsModifierType::BonusScenery,          0,                6693, 0, 0 },
            { RatingsModifierType::RequirementInversions, 1,                4, 2, 2 },
            { RatingsModifierType::RequirementMaxSpeed,   0xA0000,          2, 2, 2 },
            { RatingsModifierType::RequirementNegativeGs, FIXED_2DP(0, 40), 2, 2, 2 },
            { RatingsModifierType::RequirementNumDrops,   2,                2, 2, 2 },
            { RatingsModifierType::PenaltyLateralGs,      0,                24576, 38130, 49648 },
        },
    },
};
// clang-format on