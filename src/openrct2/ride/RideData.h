/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#define TRACK_COLOUR_PRESETS(...)                                                                                              \
    {                                                                                                                          \
        static_cast<uint8_t>(std::size<TrackColour>({ __VA_ARGS__ })),                                                         \
        {                                                                                                                      \
            __VA_ARGS__                                                                                                        \
        }                                                                                                                      \
    }
#define DEFAULT_FLAT_RIDE_COLOUR_PRESET TRACK_COLOUR_PRESETS({ COLOUR_BRIGHT_RED, COLOUR_LIGHT_BLUE, COLOUR_YELLOW })
#define DEFAULT_STALL_COLOUR_PRESET TRACK_COLOUR_PRESETS({ COLOUR_BRIGHT_RED, COLOUR_BRIGHT_RED, COLOUR_BRIGHT_RED })

#include "../audio/audio.h"
#include "../common.h"
#include "../core/BitSet.hpp"
#include "../entity/Guest.h"
#include "../localisation/StringIds.h"
#include "../sprites.h"
#include "../util/Util.h"
#include "Ride.h"
#include "RideAudio.h"
#include "RideConstruction.h"
#include "RideEntry.h"
#include "RideRatings.h"
#include "ShopItem.h"
#include "Track.h"
#include "TrackPaint.h"
#include "Vehicle.h"

enum class ResearchCategory : uint8_t;

constexpr const uint8_t DefaultFoodStallHeight = 8 * COORDS_Z_STEP;
constexpr const uint8_t DefaultDrinksStallHeight = 8 * COORDS_Z_STEP;
constexpr const uint8_t DefaultShopHeight = 8 * COORDS_Z_STEP;
constexpr const uint8_t DefaultToiletHeight = 4 * COORDS_Z_STEP;
constexpr const uint8_t DefaultInformationKioskHeight = 6 * COORDS_Z_STEP;
constexpr const uint8_t DefaultFirstAidHeight = 6 * COORDS_Z_STEP;
constexpr const uint8_t DefaultCashMachineHeight = 8 * COORDS_Z_STEP;

struct RideComponentName
{
    StringId singular;
    StringId plural;
    StringId capitalised;
    StringId capitalised_plural;
    StringId count;
    StringId count_plural;
    StringId number;
};

enum class RideComponentType
{
    Train,
    Boat,
    Track,
    DockingPlatform,
    Station,
    Car,
    Building,
    Structure,
    Ship,
    Cabin,
    Wheel,
    Ring,
    Player,
    Course,
    Count
};

enum class RideColourKey : uint8_t
{
    Ride,
    Food,
    Drink,
    Shop,
    InfoKiosk,
    FirstAid,
    CashMachine,
    Toilets
};

enum class TrackDesignCreateMode : uint_fast8_t
{
    Default,
    Maze
};

enum class RatingsCalculationType : uint8_t
{
    Normal,
    FlatRide,
    Stall,
};

enum class RatingsModifierType : uint8_t
{
    NoModifier,
    // General Rating Bonuses
    BonusLength,
    BonusSynchronisation,
    BonusTrainLength,
    BonusMaxSpeed,
    BonusAverageSpeed,
    BonusDuration,
    BonusGForces,
    BonusTurns,
    BonusDrops,
    BonusSheltered,
    BonusProximity,
    BonusScenery,
    BonusRotations,
    BonusOperationOption,
    BonusReversedTrains,
    // Ride-specific Rating Bonuses
    BonusGoKartRace,
    BonusTowerRide,
    BonusRotoDrop,
    BonusMazeSize,
    BonusBoatHireNoCircuit,
    BonusSlideUnlimitedRides,
    BonusMotionSimulatorMode,
    Bonus3DCinemaMode,
    BonusTopSpinMode,
    // Number of reversals BONUS for reverser coaster
    BonusReversals,
    // Number of hole BONUS for mini golf
    BonusHoles,
    // Number of cars bonus for dodgems/flying saucers
    BonusNumTrains,
    // Bonus for launched freefall in downward launch mode
    BonusDownwardLaunch,
    // Bonus with further mode-dependent logic for LF
    BonusLaunchedFreefallSpecial,
    // General Rating Requirements
    RequirementLength,
    RequirementDropHeight,
    RequirementNumDrops,
    RequirementMaxSpeed,
    RequirementNegativeGs,
    RequirementLateralGs,
    RequirementInversions,
    RequirementUnsheltered,
    // Number of reversals REQUIREMENT for reverser coaster
    RequirementReversals,
    // Number of hole REQUIREMENT for mini golf
    RequirementHoles,
    // 2 Station requirement for Chairlift
    RequirementStations,
    // Water section requirement for Water Coaster
    RequirementSplashdown,
    PenaltyLateralGs,
};

struct RideNameConvention
{
    RideComponentType vehicle;
    RideComponentType structure;
    RideComponentType station;
};

struct RideBuildCost
{
    money64 TrackPrice; // Cost of a single straight piece of track
    money64 SupportPrice;
    uint8_t PriceEstimateMultiplier;
};

struct RideHeights
{
    uint8_t MaxHeight;
    uint8_t ClearanceHeight;
    int8_t VehicleZOffset;
    uint8_t PlatformHeight;
};

struct RideLiftData
{
    OpenRCT2::Audio::SoundId sound_id;
    uint8_t minimum_speed;
    uint8_t maximum_speed;
};

struct RideColourPreview
{
    uint32_t Track;
    uint32_t Supports;
};

struct RideOperatingSettings
{
    uint8_t MinValue;
    uint8_t MaxValue;
    uint8_t MaxBrakesSpeed;
    uint8_t PoweredLiftAcceleration;
    uint8_t BoosterAcceleration;
    int8_t BoosterSpeedFactor; // The factor to shift the raw booster speed with
    uint16_t AccelerationFactor = 12;
    uint8_t OperatingSettingMultiplier = 1; // Used for the Ride window, cosmetic only.
};

struct RideAlternateTrack
{
    ride_type_t type;
    uint32_t icon;
};

struct RideAlternateTrackList
{
    uint8_t count;
    RideAlternateTrack list[4];
};

struct RatingsModifier
{
    RatingsModifierType Type;
    int32_t Threshold;
    int32_t Excitement;
    int32_t Intensity;
    int32_t Nausea;
};

struct RideRatingsDescriptor
{
    RatingsCalculationType Type;
    RatingTuple BaseRatings;
    uint8_t Unreliability;
    // Used for rides with a set sheltered 8ths value (-1 = normal calculation)
    int8_t RideShelter;
    bool RelaxRequirementsIfInversions;
    RatingsModifier Modifiers[32];
};

struct UpkeepCostsDescriptor
{
    /**
     * Data about ride running costs. This is widely adjusted by the upkeep
     * function, so values that don't make much sense here (a roller coaster having
     * cheaper upkeep than a car ride) are fixed later on.
     *
     * Data generation script: https://gist.github.com/kevinburke/6bcf4a8fcc95faad7bac
     */
    money64 BaseCost;
    /** rct2: 0x0097E3AC */
    uint8_t TrackLengthMultiplier;
    money64 CostPerTrackPiece;
    /** rct2: 0x0097E3B4 */
    money64 CostPerTrain;
    /** rct2: 0x0097E3B6 */
    money64 CostPerCar;
    /** rct2: 0x0097E3B8 */
    money64 CostPerStation;
};

using RideTrackGroup = OpenRCT2::BitSet<TRACK_GROUP_COUNT>;
using UpdateRideApproachVehicleWaypointsFunction = void (*)(Guest&, const CoordsXY&, int16_t&);
using RideMusicUpdateFunction = void (*)(Ride&);
using PeepUpdateRideLeaveEntranceFunc = void (*)(Guest*, Ride&, CoordsXYZD&);
using StartRideMusicFunction = void (*)(const OpenRCT2::RideAudio::ViewportRideMusicInstance&);
using LightFXAddLightsMagicVehicleFunction = void (*)(const Vehicle* vehicle);
using RideLocationFunction = CoordsXY (*)(const Vehicle& vehicle, const Ride& ride, const StationIndex& CurrentRideStation);
using RideUpdateFunction = void (*)(Ride& ride);
using RideUpdateMeasurementsSpecialElementsFunc = void (*)(Ride& ride, const track_type_t trackType);
using MusicTrackOffsetLengthFunc = std::pair<size_t, size_t> (*)(const Ride& ride);
using SpecialElementRatingAdjustmentFunc = void (*)(const Ride& ride, int32_t& excitement, int32_t& intensity, int32_t& nausea);

using UpdateRotatingFunction = void (*)(Vehicle& vehicle);
enum class RideConstructionWindowContext : uint8_t
{
    Default,
    Maze,
};

struct TrackDrawerDescriptor
{
    TRACK_PAINT_FUNCTION_GETTER Regular;
    TRACK_PAINT_FUNCTION_GETTER Covered;

    constexpr TrackDrawerDescriptor(TRACK_PAINT_FUNCTION_GETTER function)
        : Regular(function)
        , Covered(nullptr)
    {
    }

    constexpr TrackDrawerDescriptor(TRACK_PAINT_FUNCTION_GETTER functionRegular, TRACK_PAINT_FUNCTION_GETTER functionCovered)
        : Regular(functionRegular)
        , Covered(functionCovered)
    {
    }
};

struct RideTypeDescriptor
{
    uint8_t AlternateType;
    uint8_t Category;
    /** rct2: 0x0097C468 (0 - 31) and 0x0097C5D4 (32 - 63) */
    RideTrackGroup EnabledTrackPieces;
    // Pieces that this ride type _can_ draw, but are disabled because their vehicles lack the relevant sprites,
    // or because they are not realistic for the ride type (e.g. LIM boosters in Mini Roller Coasters).
    RideTrackGroup ExtraTrackPieces;
    RideTrackGroup CoveredTrackPieces;
    /** rct2: 0x0097CC68 */
    track_type_t StartTrackPiece;
    TrackDrawerDescriptor TrackPaintFunctions;
    uint64_t Flags;
    /** rct2: 0x0097C8AC */
    uint64_t RideModes;
    RideMode DefaultMode;
    /** rct2: 0x0097CF40 */
    RideOperatingSettings OperatingSettings;
    RideNaming Naming;
    RideNameConvention NameConvention;
    const char* EnumName;
    uint8_t AvailableBreakdowns;
    /** rct2: 0x0097D218 */
    RideHeights Heights;
    uint8_t MaxMass;
    /** rct2: 0x0097D7C8, 0x0097D7C9, 0x0097D7CA */
    RideLiftData LiftData;
    // rct2: 0x0097CD1E
    RatingTuple RatingsMultipliers;
    UpkeepCostsDescriptor UpkeepCosts;
    // rct2: 0x0097DD78
    RideBuildCost BuildCosts;
    money64 DefaultPrices[RCT2::ObjectLimits::MaxShopItemsPerRideEntry];
    std::string_view DefaultMusic;
    /** rct2: 0x0097D7CB */
    ShopItemIndex PhotoItem;
    /** rct2: 0x0097D21E */
    uint8_t BonusValue;
    TrackColourPresetList ColourPresets;
    RideColourPreview ColourPreview;
    RideColourKey ColourKey;

    // json name lookup
    std::string_view Name;

    RideRatingsDescriptor RatingsData;

    RideAlternateTrackList AlternateTrackList = { 0, { { 0, 0 }, { 0, 0 }, { 0, 0 } } };
    UpdateRotatingFunction UpdateRotating = UpdateRotatingDefault;

    LightFXAddLightsMagicVehicleFunction LightFXAddLightsMagicVehicle = nullptr;
    StartRideMusicFunction StartRideMusic = OpenRCT2::RideAudio::DefaultStartRideMusicChannel;

    TrackDesignCreateMode DesignCreateMode = TrackDesignCreateMode::Default;

    RideMusicUpdateFunction MusicUpdateFunction = DefaultMusicUpdate;
    RideClassification Classification = RideClassification::Ride;

    PeepUpdateRideLeaveEntranceFunc UpdateLeaveEntrance = PeepUpdateRideLeaveEntranceDefault;
    SpecialElementRatingAdjustmentFunc SpecialElementRatingAdjustment = SpecialTrackElementRatingsAjustment_Default;

    RideLocationFunction GetGuestWaypointLocation = GetGuestWaypointLocationDefault;

    RideConstructionWindowContext ConstructionWindowContext = RideConstructionWindowContext::Default;
    RideUpdateFunction RideUpdate = nullptr;

    RideUpdateMeasurementsSpecialElementsFunc UpdateMeasurementsSpecialElements = RideUpdateMeasurementsSpecialElements_Default;

    MusicTrackOffsetLengthFunc MusicTrackOffsetLength = OpenRCT2::RideAudio::RideMusicGetTrackOffsetLength_Default;

    UpdateRideApproachVehicleWaypointsFunction UpdateRideApproachVehicleWaypoints = UpdateRideApproachVehicleWaypointsDefault;

    bool HasFlag(uint64_t flag) const;
    void GetAvailableTrackPieces(RideTrackGroup& res) const;
    bool SupportsTrackPiece(const uint64_t trackPiece) const;
    ResearchCategory GetResearchCategory() const;
    bool SupportsRideMode(RideMode rideMode) const;
};

#ifdef _WIN32
#    define SET_FIELD(fieldname, ...) __VA_ARGS__
#else
#    define SET_FIELD(fieldname, ...) .fieldname = __VA_ARGS__
#endif

extern const RideTypeDescriptor RideTypeDescriptors[RIDE_TYPE_COUNT];

enum
{
    RIDE_TYPE_NO_ALTERNATIVES = 0,
    RIDE_TYPE_ALTERNATIVE_TRACK_PIECES = 1, // Dinghy slide and Water Coaster
    RIDE_TYPE_ALTERNATIVE_TRACK_TYPE = 2,   // Flying RC, Lay-down RC, Multi-dimension RC
};

enum ride_type_flags : uint64_t
{
    RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_MAIN = (1uLL << 0),
    RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_ADDITIONAL = (1uLL << 1),
    RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_SUPPORTS = (1uLL << 2),
    RIDE_TYPE_FLAG_HAS_SINGLE_PIECE_STATION = (1uLL << 3), // Set by flat rides, tower rides and shops/stalls.
    RIDE_TYPE_FLAG_HAS_LEAVE_WHEN_ANOTHER_VEHICLE_ARRIVES_AT_STATION = (1uLL << 4),
    RIDE_TYPE_FLAG_CAN_SYNCHRONISE_ADJACENT_STATIONS = (1uLL << 5),
    RIDE_TYPE_FLAG_TRACK_MUST_BE_ON_WATER = (1uLL << 6), // used only by boat Hire and submarine ride
    RIDE_TYPE_FLAG_HAS_G_FORCES = (1uLL << 7),
    RIDE_TYPE_FLAG_CANNOT_HAVE_GAPS = (1uLL << 8), // used by rides that can't have gaps, like those with a vertical tower, such
                                                   // as the observation tower
    RIDE_TYPE_FLAG_HAS_DATA_LOGGING = (1uLL << 9),
    RIDE_TYPE_FLAG_HAS_DROPS = (1uLL << 10),
    RIDE_TYPE_FLAG_NO_TEST_MODE = (1uLL << 11),
    RIDE_TYPE_FLAG_TRACK_ELEMENTS_HAVE_TWO_VARIETIES = (1uLL << 12), // used by rides with two varieties, like the u and o
                                                                     // shapes of the dinghy slide and the dry and submerged
                                                                     // track of the water coaster
    RIDE_TYPE_FLAG_NO_VEHICLES = (1uLL << 13),                       // used only by maze, spiral slide and shops
    RIDE_TYPE_FLAG_HAS_LOAD_OPTIONS = (1uLL << 14),
    RIDE_TYPE_FLAG_VEHICLE_IS_INTEGRAL = (1uLL << 16), // Set by flat rides where the vehicle is integral to the structure, like
    // Merry-go-round and swinging ships. (Contrast with rides like dodgems.)
    RIDE_TYPE_FLAG_IS_SHOP_OR_FACILITY = (1uLL << 17),
    RIDE_TYPE_FLAG_TRACK_NO_WALLS = (1uLL << 18), // if set, wall scenery can not share a tile with the ride's track
    RIDE_TYPE_FLAG_FLAT_RIDE = (1uLL << 19),
    RIDE_TYPE_FLAG_PEEP_WILL_RIDE_AGAIN = (1uLL << 20), // whether or not guests will go on the ride again if they liked it
                                                        // (this is
    // usually applied to everything apart from transport rides)
    RIDE_TYPE_FLAG_PEEP_SHOULD_GO_INSIDE_FACILITY = (1uLL << 21), // used by toilets and first aid to mark that peep should go
                                                                  // inside the building (rather than 'buying' at the counter)
    RIDE_TYPE_FLAG_IN_RIDE = (1uLL << 22),                        // peeps are "IN" (ride) rather than "ON" (ride)
    RIDE_TYPE_FLAG_SELLS_FOOD = (1uLL << 23),
    RIDE_TYPE_FLAG_SELLS_DRINKS = (1uLL << 24),
    RIDE_TYPE_FLAG_IS_TOILET = (1uLL << 25),
    RIDE_TYPE_FLAG_HAS_VEHICLE_COLOURS = (1uLL << 26), // whether or not vehicle colours can be set
    RIDE_TYPE_FLAG_CHECK_FOR_STALLING = (1uLL << 27),
    RIDE_TYPE_FLAG_HAS_TRACK = (1uLL << 28),
    RIDE_TYPE_FLAG_ALLOW_EXTRA_TOWER_BASES = (1uLL << 29), // Only set by lift
    RIDE_TYPE_FLAG_SUPPORTS_MULTIPLE_TRACK_COLOUR = (1uLL << 31),

    RIDE_TYPE_FLAG_ALLOW_DOORS_ON_TRACK = (1uLL << 32),
    RIDE_TYPE_FLAG_MUSIC_ON_DEFAULT = (1uLL << 33),
    RIDE_TYPE_FLAG_ALLOW_MUSIC = (1uLL << 34),
    RIDE_TYPE_FLAG_HAS_INVERTED_TRACK = (1uLL << 35), // Used by the Flying RC, Lay-down RC, Multi-dimension RC
    RIDE_TYPE_FLAG_PEEP_CHECK_GFORCES = (1uLL << 36),
    RIDE_TYPE_FLAG_HAS_ENTRANCE_EXIT = (1uLL << 37),
    RIDE_TYPE_FLAG_ALLOW_MORE_VEHICLES_THAN_STATION_FITS = (1uLL << 38),
    RIDE_TYPE_FLAG_HAS_AIR_TIME = (1uLL << 39),
    RIDE_TYPE_FLAG_SINGLE_SESSION = (1uLL << 40),
    RIDE_TYPE_FLAG_ALLOW_MULTIPLE_CIRCUITS = (1uLL << 41),
    RIDE_TYPE_FLAG_ALLOW_CABLE_LIFT_HILL = (1uLL << 42),
    RIDE_TYPE_FLAG_SHOW_IN_TRACK_DESIGNER = (1uLL << 43),
    RIDE_TYPE_FLAG_TRANSPORT_RIDE = (1uLL << 44),
    RIDE_TYPE_FLAG_INTERESTING_TO_LOOK_AT = (1uLL << 45),
    RIDE_TYPE_FLAG_SLIGHTLY_INTERESTING_TO_LOOK_AT = (1uLL << 46),
    RIDE_TYPE_FLAG_START_CONSTRUCTION_INVERTED = (1uLL << 47), // This is only set on the Flying RC and its alternative type.

    RIDE_TYPE_FLAG_LIST_VEHICLES_SEPARATELY = (1uLL << 48),
    RIDE_TYPE_FLAG_SUPPORTS_LEVEL_CROSSINGS = (1uLL << 49),
    RIDE_TYPE_FLAG_IS_SUSPENDED = (1uLL << 50),
    RIDE_TYPE_FLAG_HAS_LANDSCAPE_DOORS = (1uLL << 51),
    RIDE_TYPE_FLAG_UP_INCLINE_REQUIRES_LIFT = (1uLL << 52),
    RIDE_TYPE_FLAG_PEEP_CAN_USE_UMBRELLA = (1uLL << 53),
    RIDE_TYPE_FLAG_IS_CASH_MACHINE = (1uLL << 54),
    RIDE_TYPE_FLAG_HAS_ONE_STATION = (1uLL << 55),
    RIDE_TYPE_FLAG_HAS_SEAT_ROTATION = (1uLL << 56),
    RIDE_TYPE_FLAG_IS_FIRST_AID = (1uLL << 57),
    RIDE_TYPE_FLAG_IS_MAZE = (1uLL << 58),
    RIDE_TYPE_FLAG_IS_SPIRAL_SLIDE = (1uLL << 59),
    RIDE_TYPE_FLAG_ALLOW_REVERSED_TRAINS = (1uLL << 60),
    RIDE_TYPE_FLAG_HAS_ALTERNATE_TRACK = (1uLL << 61),
    RIDE_TYPE_FLAG_ALLOW_CABLE_LAUNCH = (1uLL << 62),
};

// Set on ride types that have a main colour, additional colour and support colour.
constexpr uint64_t RIDE_TYPE_FLAGS_TRACK_HAS_3_COLOURS = RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_MAIN
    | RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_ADDITIONAL | RIDE_TYPE_FLAG_HAS_TRACK_COLOUR_SUPPORTS;
// Set on _all_ roller coaster ride types, including the _ALT types used for constructing upside down.
constexpr uint64_t RIDE_TYPE_FLAGS_COMMON_COASTER = RIDE_TYPE_FLAG_HAS_G_FORCES | RIDE_TYPE_FLAG_HAS_DATA_LOGGING
    | RIDE_TYPE_FLAG_HAS_DROPS | RIDE_TYPE_FLAG_HAS_LOAD_OPTIONS | RIDE_TYPE_FLAG_PEEP_WILL_RIDE_AGAIN
    | RIDE_TYPE_FLAG_HAS_VEHICLE_COLOURS | RIDE_TYPE_FLAG_CHECK_FOR_STALLING | RIDE_TYPE_FLAG_HAS_TRACK
    | RIDE_TYPE_FLAG_SUPPORTS_MULTIPLE_TRACK_COLOUR | RIDE_TYPE_FLAG_ALLOW_MUSIC | RIDE_TYPE_FLAG_INTERESTING_TO_LOOK_AT
    | RIDE_TYPE_FLAG_CAN_SYNCHRONISE_ADJACENT_STATIONS;
// Set on all roller coaster ride types, excluding the _ALT types used for constructing upside down.
constexpr uint64_t RIDE_TYPE_FLAGS_COMMON_COASTER_NON_ALT = RIDE_TYPE_FLAG_SHOW_IN_TRACK_DESIGNER | RIDE_TYPE_FLAG_HAS_AIR_TIME
    | RIDE_TYPE_FLAG_HAS_ENTRANCE_EXIT;

// clang-format off
constexpr RideComponentName RideComponentNames[] =
{
    { STR_RIDE_COMPONENT_TRAIN,             STR_RIDE_COMPONENT_TRAIN_PLURAL,            STR_RIDE_COMPONENT_TRAIN_CAPITALISED,               STR_RIDE_COMPONENT_TRAIN_CAPITALISED_PLURAL,            STR_RIDE_COMPONENT_TRAIN_COUNT,             STR_RIDE_COMPONENT_TRAIN_COUNT_PLURAL,              STR_RIDE_COMPONENT_TRAIN_NO },
    { STR_RIDE_COMPONENT_BOAT,              STR_RIDE_COMPONENT_BOAT_PLURAL,             STR_RIDE_COMPONENT_BOAT_CAPITALISED,                STR_RIDE_COMPONENT_BOAT_CAPITALISED_PLURAL,             STR_RIDE_COMPONENT_BOAT_COUNT,              STR_RIDE_COMPONENT_BOAT_COUNT_PLURAL,               STR_RIDE_COMPONENT_BOAT_NO },
    { STR_RIDE_COMPONENT_TRACK,             STR_RIDE_COMPONENT_TRACK_PLURAL,            STR_RIDE_COMPONENT_TRACK_CAPITALISED,               STR_RIDE_COMPONENT_TRACK_CAPITALISED_PLURAL,            STR_RIDE_COMPONENT_TRACK_COUNT,             STR_RIDE_COMPONENT_TRACK_COUNT_PLURAL,              STR_RIDE_COMPONENT_TRACK_NO },
    { STR_RIDE_COMPONENT_DOCKING_PLATFORM,  STR_RIDE_COMPONENT_DOCKING_PLATFORM_PLURAL, STR_RIDE_COMPONENT_DOCKING_PLATFORM_CAPITALISED,    STR_RIDE_COMPONENT_DOCKING_PLATFORM_CAPITALISED_PLURAL, STR_RIDE_COMPONENT_DOCKING_PLATFORM_COUNT,  STR_RIDE_COMPONENT_DOCKING_PLATFORM_COUNT_PLURAL,   STR_RIDE_COMPONENT_DOCKING_PLATFORM_NO },
    { STR_RIDE_COMPONENT_STATION,           STR_RIDE_COMPONENT_STATION_PLURAL,          STR_RIDE_COMPONENT_STATION_CAPITALISED,             STR_RIDE_COMPONENT_STATION_CAPITALISED_PLURAL,          STR_RIDE_COMPONENT_STATION_COUNT,           STR_RIDE_COMPONENT_STATION_COUNT_PLURAL,            STR_RIDE_COMPONENT_STATION_NO },
    { STR_RIDE_COMPONENT_CAR,               STR_RIDE_COMPONENT_CAR_PLURAL,              STR_RIDE_COMPONENT_CAR_CAPITALISED,                 STR_RIDE_COMPONENT_CAR_CAPITALISED_PLURAL,              STR_RIDE_COMPONENT_CAR_COUNT,               STR_RIDE_COMPONENT_CAR_COUNT_PLURAL,                STR_RIDE_COMPONENT_CAR_NO },
    { STR_RIDE_COMPONENT_BUILDING,          STR_RIDE_COMPONENT_BUILDING_PLURAL,         STR_RIDE_COMPONENT_BUILDING_CAPITALISED,            STR_RIDE_COMPONENT_BUILDING_CAPITALISED_PLURAL,         STR_RIDE_COMPONENT_BUILDING_COUNT,          STR_RIDE_COMPONENT_BUILDING_COUNT_PLURAL,           STR_RIDE_COMPONENT_BUILDING_NO },
    { STR_RIDE_COMPONENT_STRUCTURE,         STR_RIDE_COMPONENT_STRUCTURE_PLURAL,        STR_RIDE_COMPONENT_STRUCTURE_CAPITALISED,           STR_RIDE_COMPONENT_STRUCTURE_CAPITALISED_PLURAL,        STR_RIDE_COMPONENT_STRUCTURE_COUNT,         STR_RIDE_COMPONENT_STRUCTURE_COUNT_PLURAL,          STR_RIDE_COMPONENT_STRUCTURE_NO },
    { STR_RIDE_COMPONENT_SHIP,              STR_RIDE_COMPONENT_SHIP_PLURAL,             STR_RIDE_COMPONENT_SHIP_CAPITALISED,                STR_RIDE_COMPONENT_SHIP_CAPITALISED_PLURAL,             STR_RIDE_COMPONENT_SHIP_COUNT,              STR_RIDE_COMPONENT_SHIP_COUNT_PLURAL,               STR_RIDE_COMPONENT_SHIP_NO },
    { STR_RIDE_COMPONENT_CABIN,             STR_RIDE_COMPONENT_CABIN_PLURAL,            STR_RIDE_COMPONENT_CABIN_CAPITALISED,               STR_RIDE_COMPONENT_CABIN_CAPITALISED_PLURAL,            STR_RIDE_COMPONENT_CABIN_COUNT,             STR_RIDE_COMPONENT_CABIN_COUNT_PLURAL,              STR_RIDE_COMPONENT_CABIN_NO },
    { STR_RIDE_COMPONENT_WHEEL,             STR_RIDE_COMPONENT_WHEEL_PLURAL,            STR_RIDE_COMPONENT_WHEEL_CAPITALISED,               STR_RIDE_COMPONENT_WHEEL_CAPITALISED_PLURAL,            STR_RIDE_COMPONENT_WHEEL_COUNT,             STR_RIDE_COMPONENT_WHEEL_COUNT_PLURAL,              STR_RIDE_COMPONENT_WHEEL_NO },
    { STR_RIDE_COMPONENT_RING,              STR_RIDE_COMPONENT_RING_PLURAL,             STR_RIDE_COMPONENT_RING_CAPITALISED,                STR_RIDE_COMPONENT_RING_CAPITALISED_PLURAL,             STR_RIDE_COMPONENT_RING_COUNT,              STR_RIDE_COMPONENT_RING_COUNT_PLURAL,               STR_RIDE_COMPONENT_RING_NO },
    { STR_RIDE_COMPONENT_PLAYER,            STR_RIDE_COMPONENT_PLAYER_PLURAL,           STR_RIDE_COMPONENT_PLAYER_CAPITALISED,              STR_RIDE_COMPONENT_PLAYER_CAPITALISED_PLURAL,           STR_RIDE_COMPONENT_PLAYER_COUNT,            STR_RIDE_COMPONENT_PLAYER_COUNT_PLURAL,             STR_RIDE_COMPONENT_PLAYER_NO },
    { STR_RIDE_COMPONENT_COURSE,            STR_RIDE_COMPONENT_COURSE_PLURAL,           STR_RIDE_COMPONENT_COURSE_CAPITALISED,              STR_RIDE_COMPONENT_COURSE_CAPITALISED_PLURAL,           STR_RIDE_COMPONENT_COURSE_COUNT,            STR_RIDE_COMPONENT_COURSE_COUNT_PLURAL,             STR_RIDE_COMPONENT_COURSE_NO },
};
// clang-format on

constexpr std::string_view MUSIC_OBJECT_DODGEMS = "rct2.music.dodgems";
constexpr std::string_view MUSIC_OBJECT_EGYPTIAN = "rct2.music.egyptian";
constexpr std::string_view MUSIC_OBJECT_FAIRGROUND = "rct2.music.fairground";
constexpr std::string_view MUSIC_OBJECT_GENTLE = "rct2.music.gentle";
constexpr std::string_view MUSIC_OBJECT_HORROR = "rct2.music.horror";
constexpr std::string_view MUSIC_OBJECT_PIRATE = "rct2.music.pirate";
constexpr std::string_view MUSIC_OBJECT_ROCK_1 = "rct2.music.rock1";
constexpr std::string_view MUSIC_OBJECT_ROCK_2 = "rct2.music.rock2";
constexpr std::string_view MUSIC_OBJECT_ROCK_3 = "rct2.music.rock3";
constexpr std::string_view MUSIC_OBJECT_SUMMER = "rct2.music.summer";
constexpr std::string_view MUSIC_OBJECT_TECHNO = "rct2.music.techno";
constexpr std::string_view MUSIC_OBJECT_WATER = "rct2.music.water";
constexpr std::string_view MUSIC_OBJECT_WILD_WEST = "rct2.music.wildwest";

constexpr const RideComponentName& GetRideComponentName(const RideComponentType type)
{
    return RideComponentNames[EnumValue(type)];
}

constexpr uint64_t AllRideModesAvailable = EnumsToFlags(
    RideMode::ContinuousCircuit, RideMode::ContinuousCircuitBlockSectioned, RideMode::ReverseInclineLaunchedShuttle,
    RideMode::PoweredLaunchPasstrough, RideMode::Shuttle, RideMode::Normal, RideMode::BoatHire, RideMode::UpwardLaunch,
    RideMode::RotatingLift, RideMode::StationToStation, RideMode::SingleRidePerAdmission, RideMode::UnlimitedRidesPerAdmission,
    RideMode::Maze, RideMode::Race, RideMode::Dodgems, RideMode::Swing, RideMode::ShopStall, RideMode::Rotation,
    RideMode::ForwardRotation, RideMode::BackwardRotation, RideMode::FilmAvengingAviators, RideMode::MouseTails3DFilm,
    RideMode::SpaceRings, RideMode::Beginners, RideMode::LimPoweredLaunch, RideMode::FilmThrillRiders,
    RideMode::StormChasers3DFilm, RideMode::SpaceRaiders3DFilm, RideMode::Intense, RideMode::Berserk, RideMode::HauntedHouse,
    RideMode::Circus, RideMode::DownwardLaunch, RideMode::CrookedHouse, RideMode::FreefallDrop, RideMode::PoweredLaunch,
    RideMode::PoweredLaunchBlockSectioned);

extern const CarEntry CableLiftVehicle;
extern const CarEntry CableLaunchVehicles[4];

extern const uint16_t RideFilmLength[3];

extern const StringId RideModeNames[static_cast<uint8_t>(RideMode::Count)];

// clang-format off
constexpr RideTypeDescriptor DummyRTD =
{
    SET_FIELD(AlternateType, RIDE_TYPE_NULL),
    SET_FIELD(Category, RIDE_CATEGORY_NONE),
    SET_FIELD(EnabledTrackPieces, {}),
    SET_FIELD(ExtraTrackPieces, {}),
    SET_FIELD(CoveredTrackPieces, {}),
    SET_FIELD(StartTrackPiece, TrackElemType::EndStation),
    SET_FIELD(TrackPaintFunctions, TrackDrawerDescriptor(nullptr)),
    SET_FIELD(Flags, 0),
    SET_FIELD(RideModes, EnumsToFlags(RideMode::ContinuousCircuit)),
    SET_FIELD(DefaultMode, RideMode::ContinuousCircuit),
    SET_FIELD(OperatingSettings, { 0, 0, 0, 0, 0, 0 }),
    SET_FIELD(Naming, { STR_UNKNOWN_RIDE, STR_RIDE_DESCRIPTION_UNKNOWN }),
    SET_FIELD(NameConvention, { RideComponentType::Train, RideComponentType::Track, RideComponentType::Station }),
    SET_FIELD(EnumName, "(INVALID)"),
    SET_FIELD(AvailableBreakdowns, 0),
    SET_FIELD(Heights, { 12, 64, 0, 0, }),
    SET_FIELD(MaxMass, 255),
    SET_FIELD(LiftData, { OpenRCT2::Audio::SoundId::Null, 5, 5 }),
    SET_FIELD(RatingsMultipliers, { 0, 0, 0 }),
    SET_FIELD(UpkeepCosts, { 50, 1, 0, 0, 0, 0 }),
    SET_FIELD(BuildCosts, { 0.00_GBP, 0.00_GBP, 1 }),
    SET_FIELD(DefaultPrices, { 20, 20 }),
    SET_FIELD(DefaultMusic, MUSIC_OBJECT_GENTLE),
    SET_FIELD(PhotoItem, ShopItem::Photo),
    SET_FIELD(BonusValue, 0),
    SET_FIELD(ColourPresets, DEFAULT_FLAT_RIDE_COLOUR_PRESET),
    SET_FIELD(ColourPreview, { static_cast<uint32_t>(SPR_NONE), static_cast<uint32_t>(SPR_NONE) }),
    SET_FIELD(ColourKey, RideColourKey::Ride),
    SET_FIELD(Name, "invalid"),
	SET_FIELD(RatingsData,
    {
        RatingsCalculationType::FlatRide,
        { RIDE_RATING(1, 00), RIDE_RATING(1, 00), RIDE_RATING(1, 00) },
        1,
        -1,
        false,
        {
            { RatingsModifierType::NoModifier, 0, 0, 0, 0 },
        },
    }),
    SET_FIELD(AlternateTrackList, {0,{{0,static_cast<uint32_t>(SPR_NONE)}}}),
    SET_FIELD(UpdateRotating, UpdateRotatingDefault),
    SET_FIELD(LightFXAddLightsMagicVehicle, nullptr),
    SET_FIELD(StartRideMusic, OpenRCT2::RideAudio::DefaultStartRideMusicChannel),
    SET_FIELD(DesignCreateMode, TrackDesignCreateMode::Default),
    SET_FIELD(MusicUpdateFunction, DefaultMusicUpdate),
    SET_FIELD(Classification, RideClassification::Ride),
    SET_FIELD(UpdateLeaveEntrance, PeepUpdateRideLeaveEntranceDefault),
};
// clang-format on

constexpr const RideTypeDescriptor& GetRideTypeDescriptor(ObjectEntryIndex rideType)
{
    if (rideType >= std::size(RideTypeDescriptors))
        return DummyRTD;

    return RideTypeDescriptors[rideType];
}

constexpr bool RideTypeIsValid(ObjectEntryIndex rideType)
{
    return rideType < std::size(RideTypeDescriptors);
}

bool IsTrackEnabled(int32_t trackFlagIndex);
void UpdateEnabledRidePieces(ride_type_t rideType);
void UpdateDisabledRidePieces(const RideTrackGroup& res);
