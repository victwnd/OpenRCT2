/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <openrct2-ui/interface/Dropdown.h>
#include <openrct2-ui/interface/Viewport.h>
#include <openrct2-ui/interface/Widget.h>
#include <openrct2-ui/windows/Window.h>
#include <openrct2/Cheats.h>
#include <openrct2/Context.h>
#include <openrct2/Game.h>
#include <openrct2/Input.h>
#include <openrct2/OpenRCT2.h>
#include <openrct2/actions/PoolPlaceAction.h>
#include <openrct2/audio/audio.h>
#include <openrct2/localisation/Formatter.h>
#include <openrct2/localisation/Localisation.h>
#include <openrct2/object/PoolObject.h>
#include <openrct2/object/ObjectLimits.h>
#include <openrct2/object/ObjectManager.h>
#include <openrct2/platform/Platform.h>
#include <openrct2/sprites.h>
#include <openrct2/world/Park.h>
#include <openrct2/world/Surface.h>
#include <openrct2/world/Pool.h>

static constexpr const rct_string_id WINDOW_TITLE = STR_POOLS;
static constexpr const int32_t WH = 180;
static constexpr const int32_t WW = 150;

static uint8_t _footpathConstructionMode;

static std::vector<std::pair<ObjectType, ObjectEntryIndex>> _dropdownEntries;

static bool _poolPlaceCtrlState;
static int32_t _poolPlaceCtrlZ;
static bool _poolPlaceShiftState;
static ScreenCoordsXY _poolPlaceShiftStart;
static int32_t _poolPlaceShiftZ;
static int32_t _poolPlaceZ;

// clang-format off
enum
{
    PATH_CONSTRUCTION_MODE_LAND,
    PATH_CONSTRUCTION_MODE_BRIDGE_OR_TUNNEL_TOOL,
    PATH_CONSTRUCTION_MODE_BRIDGE_OR_TUNNEL
};

enum WindowPoolWidgetIdx
{
    WIDX_BACKGROUND,
    WIDX_TITLE,
    WIDX_CLOSE,

    WIDX_TYPE_GROUP,
    WIDX_FOOTPATH_TYPE,
    WIDX_QUEUELINE_TYPE,

WIDX_CORNER_GROUP,
WIDX_CORNER_SQUARE,
WIDX_CORNER_ANGLED,
WIDX_CORNER_ROUNDED,

WIDX_DEPTH_GROUP,
WIDX_DEPTH_SPINNER,
WIDX_DEPTH_SPINNER_UP,
WIDX_DEPTH_SPINNER_DOWN,

WIDX_MODE_GROUP,
WIDX_CONSTRUCT_PATH_TILE,
WIDX_CONSTRUCT_WATER_TILE,
WIDX_CONSTRUCT_DRAW_RECTANGLE,
};

static rct_widget window_pool_widgets[] = {
    WINDOW_SHIM(WINDOW_TITLE, WW, WH),

    // Type group
    MakeWidget({ 3,  17}, {100, 55}, WindowWidgetType::Groupbox, WindowColour::Primary  , STR_TYPE                                                                          ),
    MakeWidget({ 6,  30}, { 47, 36}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, 0xFFFFFFFF,                        STR_POOL_TIP                               ),
    MakeWidget({53,  30}, { 47, 36}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, 0xFFFFFFFF,                        STR_FOOTPATH_TIP                        ),

    //Corner group
    MakeWidget({ 3, 75}, {80, 41}, WindowWidgetType::Groupbox, WindowColour::Primary  , STR_CORNER_STYLE                                                                         ),
    MakeWidget({7, 87}, { 24, 24}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_SQUARE_CORNERS,  STR_POOL_SQUARE_TIP                             ),
    MakeWidget({31, 87}, { 24, 24}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_ANGLED_CORNERS, STR_POOL_ANGLED_TIP                                  ),
    MakeWidget({55, 87}, { 24, 24}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_ROUNDED_CORNERS,    STR_POOL_ROUNDED_TIP                               ),

    //Depth group
    MakeWidget        ({ 88, 75}, { 50,  41}, WindowWidgetType::Groupbox, WindowColour::Primary  , STR_POOL_DEPTH),
    MakeSpinnerWidgets({101, 93}, { 30,  12}, WindowWidgetType::Spinner,  WindowColour::Secondary, 0,STR_POOL_DEPTH),


    // Mode group
    MakeWidget({ 3, 119}, {100, 54}, WindowWidgetType::Groupbox, WindowColour::Primary                                                                                      ),
    MakeWidget({5, 130}, { 36, 36}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_POOL_PATH,    STR_POOL_PATH_TIP),
    MakeWidget({43, 130}, { 36, 36}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_POOL_WATER,    STR_POOL_WATER_TIP),
    MakeWidget({73, 130}, { 36, 36}, WindowWidgetType::FlatBtn,  WindowColour::Secondary, SPR_G2_BUTTON_POOL_RECTANGLE,    STR_POOL_RECTANGLE_TIP),
    WIDGETS_END,
};

static void WindowPoolClose(rct_window * w);
static void WindowPoolMouseup(rct_window * w, rct_widgetindex widgetIndex);
static void WindowPoolMousedown(rct_window * w, rct_widgetindex widgetIndex, rct_widget * widget);
static void WindowPoolDropdown(rct_window * w, rct_widgetindex widgetIndex, int32_t dropdownIndex);
static void WindowPoolUpdate(rct_window * w);
static void WindowPoolToolupdate(rct_window * w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords);
static void WindowPoolTooldown(rct_window * w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords);
static void WindowPoolTooldrag(rct_window * w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords);
static void WindowPoolToolup(rct_window * w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords);
static void WindowPoolInvalidate(rct_window * w);
static void WindowPoolPaint(rct_window * w, rct_drawpixelinfo * dpi);

static rct_window_event_list window_pool_events([](auto& events)
{
    events.close = &WindowPoolClose;
    events.mouse_up = &WindowPoolMouseup;
    events.mouse_down = &WindowPoolMousedown;
    events.dropdown = &WindowPoolDropdown;
    events.update = &WindowPoolUpdate;
    events.tool_update = &WindowPoolToolupdate;
    events.tool_down = &WindowPoolTooldown;
    events.tool_drag = &WindowPoolTooldrag;
    events.tool_up = &WindowPoolToolup;
    events.invalidate = &WindowPoolInvalidate;
    events.paint = &WindowPoolPaint;
});
// clang-format on

static money32 _window_pool_cost;
static uint8_t _lastUpdatedCameraRotation = UINT8_MAX;
static bool _poolErrorOccured;


/** rct2: 0x0098D7E0 */
/*
static constexpr const uint8_t ConstructionPreviewImages[][4] = {
    { 5, 10, 5, 10 },   // Flat
    { 16, 17, 18, 19 }, // Upwards
    { 18, 19, 16, 17 }, // Downwards
};
*/
static void WindowPoolShowPoolTypesDialog(rct_window* w, rct_widget* widget);
static void WindowPoolSetEnabledAndPressedWidgets();
static bool PoolSelectDefault();

/**
 *
 *  rct2: 0x006A7C43
 */
rct_window* WindowPoolOpen()
{
    if (!PoolSelectDefault())
    {
        // No path objects to select from, don't open window
        return nullptr;
    }

    // Check if window is already open
    rct_window* window = window_bring_to_front_by_class(WC_POOL);
    if (window != nullptr)
    {
        return window;
    }

    window = WindowCreate(ScreenCoordsXY(0, 29), WW, WH, &window_pool_events, WC_POOL, 0);
    window->widgets = window_pool_widgets;

    WindowInitScrollWidgets(window);
    window_push_others_right(window);
    show_gridlines();

    tool_cancel();
    _footpathConstructionMode = PATH_CONSTRUCTION_MODE_LAND;
    tool_set(window, WIDX_CONSTRUCT_PATH_TILE, Tool::PathDown);
    input_set_flag(INPUT_FLAG_6, true);
    _poolErrorOccured = false;
    WindowPoolSetEnabledAndPressedWidgets();

    return window;
}

/**
 *
 *  rct2: 0x006A852F
 */
static void WindowPoolClose(rct_window* w)
{
    pool_provisional_update();
    viewport_set_visibility(0);
    map_invalidate_map_selection_tiles();
    gMapSelectFlags &= ~MAP_SELECT_FLAG_ENABLE_CONSTRUCT;
    window_invalidate_by_class(WC_TOP_TOOLBAR);
    hide_gridlines();
}

/**
 *
 *  rct2: 0x006A7E92
 */
static void WindowPoolMouseup(rct_window* w, rct_widgetindex widgetIndex)
{
    switch (widgetIndex)
    {
        case WIDX_CLOSE:
            window_close(w);
            break;
    }
}

/**
 *
 *  rct2: 0x006A7EC5
 */
static void WindowPoolMousedown(rct_window* w, rct_widgetindex widgetIndex, rct_widget* widget)
{
    switch (widgetIndex)
    {
        case WIDX_FOOTPATH_TYPE:
            WindowPoolShowPoolTypesDialog(w, widget);
            break;
        case WIDX_QUEUELINE_TYPE:
            //WindowPoolShowPoolTypesDialog(w, widget, true);
            break;
        case WIDX_CORNER_SQUARE:
            //WindowPoolMousedownSlope(6);
            break;
        case WIDX_CORNER_ANGLED:
            //WindowPoolMousedownSlope(0);
            break;
        case WIDX_CORNER_ROUNDED:
            //WindowPoolMousedownSlope(2);
            break;
    }
}

/**
 *
 *  rct2: 0x006A7F18
 */
static void WindowPoolDropdown(rct_window* w, rct_widgetindex widgetIndex, int32_t dropdownIndex)
{
/*
    if (dropdownIndex < 0 || static_cast<size_t>(dropdownIndex) >= _dropdownEntries.size())
        return;

    auto entryIndex = _dropdownEntries[dropdownIndex];
    if (widgetIndex == WIDX_FOOTPATH_TYPE)
    {
        if (entryIndex.first == ObjectType::Paths)
        {
        }
        else
        {
            gPoolSelection.Pool = entryIndex.second;
        }
    }
    else
    {
        return;
    }

    pool_provisional_update();
    _window_pool_cost = MONEY32_UNDEFINED;
    w->Invalidate();
*/
}






static std::optional<CoordsXYZ> PoolGetPlacePositionFromScreenPosition(ScreenCoordsXY screenCoords)
{
    CoordsXY mapCoords;

//Handle CTRL
    if (!_poolPlaceCtrlState)
    {
        if (gInputPlaceObjectModifier & PLACE_OBJECT_MODIFIER_COPY_Z)
        {
            auto info = get_map_coordinates_from_pos(screenCoords, 0xFCCA);
            if (info.SpriteType != ViewportInteractionItem::None)
            {
                _poolPlaceCtrlZ = info.Element->GetBaseZ();
                _poolPlaceCtrlState = true;
            }
        }
    }
    else
    {
        if (!(gInputPlaceObjectModifier & PLACE_OBJECT_MODIFIER_COPY_Z))
        {
            _poolPlaceCtrlState = false;
        }
    }

//Handle SHIFT
    if (!_poolPlaceShiftState)
    {
        if (gInputPlaceObjectModifier & PLACE_OBJECT_MODIFIER_SHIFT_Z)
        {
            _poolPlaceShiftState = true;
            _poolPlaceShiftStart = screenCoords;
            _poolPlaceShiftZ = 0;
        }
    }
    else
    {
        if (gInputPlaceObjectModifier & PLACE_OBJECT_MODIFIER_SHIFT_Z)
        {
            uint16_t maxHeight = ZoomLevel::max().ApplyTo(std::numeric_limits<decltype(TileElement::base_height)>::max() - 32);

            _poolPlaceShiftZ = _poolPlaceShiftStart.y - screenCoords.y + 4;
            // Scale delta by zoom to match mouse position.
            auto* mainWnd = window_get_main();
            if (mainWnd != nullptr && mainWnd->viewport != nullptr)
            {
                _poolPlaceShiftZ = mainWnd->viewport->zoom.ApplyTo(_poolPlaceShiftZ);
            }
            _poolPlaceShiftZ = floor2(_poolPlaceShiftZ, 16);

            // Clamp to maximum possible value of base_height can offer.
            _poolPlaceShiftZ = std::min<int16_t>(_poolPlaceShiftZ, maxHeight);

            screenCoords = _poolPlaceShiftStart;
        }
        else
        {
            _poolPlaceShiftState = false;
        }
    }

    if (!_poolPlaceCtrlState)
    {
        mapCoords = ViewportInteractionGetTileStartAtCursor(screenCoords);
        if (mapCoords.IsNull())
            return std::nullopt;

        _poolPlaceZ = 0;
        if (_poolPlaceShiftState)
        {
            auto surfaceElement = map_get_surface_element_at(mapCoords);
            if (surfaceElement == nullptr)
                return std::nullopt;
            auto mapZ = floor2(surfaceElement->GetBaseZ(), 16);
            mapZ += _poolPlaceShiftZ;
            mapZ = std::max<int16_t>(mapZ, 16);
            _poolPlaceZ = mapZ;
        }
    }
    else
    {
        auto mapZ = _poolPlaceCtrlZ;
        auto mapXYCoords = screen_get_map_xy_with_z(screenCoords, mapZ);
        if (mapXYCoords.has_value())
        {
            mapCoords = mapXYCoords.value();
        }
        else
        {
            return std::nullopt;
        }

        if (_poolPlaceShiftState != 0)
        {
            mapZ += _poolPlaceShiftZ;
        }
        _poolPlaceZ = std::max<int32_t>(mapZ, 16);
    }

    if (mapCoords.x == LOCATION_NULL)
        return std::nullopt;

mapCoords=mapCoords.ToTileStart();
	if (_poolPlaceZ == 0)
	{
	auto z=map_get_highest_z(mapCoords);
	//Check if pool can be placed in ground
	auto surfaceElement = map_get_surface_element_at(mapCoords);
	    if(surfaceElement&&surfaceElement->AsSurface()->GetSlope()==0&&surfaceElement->AsSurface()->GetBaseZ()==z)z-=POOL_DEPTH;
	return CoordsXYZ(mapCoords,z);
	}


return CoordsXYZ(mapCoords,_poolPlaceZ);
}


static void WindowPoolToolupdate(rct_window* w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords)
{
    auto coords=PoolGetPlacePositionFromScreenPosition(screenCoords);
	if(!coords)return;
    auto loc=*coords;
    
    map_invalidate_selection_rect();

        // Check for change
        if ((gProvisionalPool.Flags & PROVISIONAL_POOL_FLAG_1) && gProvisionalPool.Position == loc)return;
        // Set map selection
        gMapSelectFlags |= MAP_SELECT_FLAG_ENABLE;
        gMapSelectType = MAP_SELECT_TYPE_FULL;
        gMapSelectPositionA = *coords;
        gMapSelectPositionB = *coords;

        //pool_provisional_update();
       _window_pool_cost = pool_provisional_set(gPoolSelection.Pool, loc);
      window_invalidate_by_class(WC_POOL);
}


static void WindowPoolPlacePoolAt(const ScreenCoordsXY& screenCoords)
{
    if (_poolErrorOccured)
    {
        return;
    }

    pool_provisional_update();

    auto coords=PoolGetPlacePositionFromScreenPosition(screenCoords);
	if(!coords)return;
    auto loc=*coords;

    // Try and place path
    gGameCommandErrorTitle = STR_CANT_BUILD_POOL_HERE;
    auto poolPlaceAction = PoolPlaceAction(loc, gPoolSelection.Pool);
    poolPlaceAction.SetCallback([](const GameAction* ga, const GameActions::Result* result) {
        if (result->Error == GameActions::Status::Ok)
        {
            // Don't play sound if it is no cost to prevent multiple sounds. TODO: make this work in no money scenarios
            if (result->Cost != 0)
            {
                OpenRCT2::Audio::Play3D(OpenRCT2::Audio::SoundId::PlaceItem, result->Position);
            }
        }
        else
        {
            _poolErrorOccured = true;
        }
    });
    GameActions::Execute(&poolPlaceAction);

}

static void WindowPoolTooldown(rct_window* w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords)
{
WindowPoolPlacePoolAt(screenCoords);
}

static void WindowPoolTooldrag(rct_window* w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords)
{
WindowPoolPlacePoolAt(screenCoords);
}

static void WindowPoolToolup(rct_window* w, rct_widgetindex widgetIndex, const ScreenCoordsXY& screenCoords)
{
    if (widgetIndex == WIDX_CONSTRUCT_PATH_TILE)
    {
        _poolErrorOccured = false;
    }
}

static void WindowPoolUpdate(rct_window* w)
{

    // #2502: The camera might have changed rotation, so we need to update which directional buttons are pressed
    uint8_t currentRotation = get_current_rotation();
    if (_lastUpdatedCameraRotation != currentRotation)
    {
        _lastUpdatedCameraRotation = currentRotation;
        WindowPoolSetEnabledAndPressedWidgets();
    }

    // Check tool
    if (_footpathConstructionMode == PATH_CONSTRUCTION_MODE_LAND)
    {
        if (!(input_test_flag(INPUT_FLAG_TOOL_ACTIVE)))
        {
            window_close(w);
        }
        else if (gCurrentToolWidget.window_classification != WC_POOL)
        {
            window_close(w);
        }
        else if (gCurrentToolWidget.widget_index != WIDX_CONSTRUCT_PATH_TILE)
        {
            window_close(w);
        }
    }
    else if (_footpathConstructionMode == PATH_CONSTRUCTION_MODE_BRIDGE_OR_TUNNEL_TOOL)
    {
        if (!(input_test_flag(INPUT_FLAG_TOOL_ACTIVE)))
        {
            window_close(w);
        }
        else if (gCurrentToolWidget.window_classification != WC_POOL)
        {
            window_close(w);
        }
    }
}

/**
 *
 *  rct2: 0x006A7D1C
 */


static void WindowPoolInvalidate(rct_window* w)
{
    // Press / unpress footpath and queue type buttons
    w->pressed_widgets &= ~(1ULL << WIDX_FOOTPATH_TYPE);
    w->pressed_widgets &= ~(1ULL << WIDX_QUEUELINE_TYPE);
    w->pressed_widgets |= 0;
}

static void WindowPoolDrawDropdownButton(
    rct_window* w, rct_drawpixelinfo* dpi, rct_widgetindex widgetIndex, ImageIndex image)
{
    const auto& widget = w->widgets[widgetIndex];
    gfx_draw_sprite(dpi, ImageId(image), { w->windowPos.x + widget.left, w->windowPos.y + widget.top });
}

static void WindowPoolDrawDropdownButtons(rct_window* w, rct_drawpixelinfo* dpi)
{
auto poolImage = static_cast<uint32_t>(SPR_NONE);
auto poolEntry = GetPoolEntry(gPoolSelection.Pool);
if (poolEntry != nullptr)
{
    poolImage = poolEntry->PreviewImageId;
}


WindowPoolDrawDropdownButton(w, dpi, WIDX_FOOTPATH_TYPE, poolImage);
}

static void WindowPoolPaint(rct_window* w, rct_drawpixelinfo* dpi)
{
    ScreenCoordsXY screenCoords;
    WindowDrawWidgets(w, dpi);
    WindowPoolDrawDropdownButtons(w, dpi);

    // Draw cost
    if (_window_pool_cost != MONEY32_UNDEFINED)
    {
        if (!(gParkFlags & PARK_FLAGS_NO_MONEY))
        {
            auto ft = Formatter();
            ft.Add<money64>(_window_pool_cost);
            DrawTextBasic(dpi, screenCoords, STR_COST_LABEL, ft, { TextAlignment::CENTRE });
        }
    }
}

static void WindowPoolShowPoolTypesDialog(rct_window* w, rct_widget* widget)
{
    auto& objManager = OpenRCT2::GetContext()->GetObjectManager();

    uint32_t numPathTypes = 0;

    _dropdownEntries.clear();
    std::optional<size_t> defaultIndex;
    for (ObjectEntryIndex i = 0; i < MAX_POOL_OBJECTS; i++)
    {
        const auto* pathType = static_cast<PoolObject*>(objManager.GetLoadedObject(ObjectType::Pool, i));
        if (pathType == nullptr)
        {
            continue;
        }

        gDropdownItems[numPathTypes].Format = STR_NONE;
        Dropdown::SetImage(numPathTypes, ImageId(pathType->PreviewImageId));
        _dropdownEntries.push_back({ ObjectType::Pool, i });
        numPathTypes++;
    }
/*
    for (ObjectEntryIndex i = 0; i < MAX_PATH_OBJECTS; i++)
    {
        auto* pathObj = static_cast<PoolObject*>(objManager.GetLoadedObject(ObjectType::Paths, i));
        if (pathObj == nullptr)
        {
            continue;
        }

        auto pathEntry = reinterpret_cast<const rct_footpath_entry*>(pathObj->GetLegacyData());
        if ((pathEntry->flags & FOOTPATH_ENTRY_FLAG_SHOW_ONLY_IN_SCENARIO_EDITOR) && !showEditorPaths)
        {
            continue;
        }

        if (gPoolSelection.LegacyPath != OBJECT_ENTRY_INDEX_NULL && gPoolSelection.LegacyPath == i)
        {
            defaultIndex = numPathTypes;
        }

        gDropdownItems[numPathTypes].Format = STR_NONE;
        Dropdown::SetImage(
            numPathTypes, ImageId(showQueues ? pathEntry->GetQueuePreviewImage() : pathEntry->GetPreviewImage()));
        _dropdownEntries.push_back({ ObjectType::Paths, i });
        numPathTypes++;
    }
*/
    auto itemsPerRow = DropdownGetAppropriateImageDropdownItemsPerRow(numPathTypes);
    WindowDropdownShowImage(
        w->windowPos.x + widget->left, w->windowPos.y + widget->top, widget->height() + 1, w->colours[1], 0, numPathTypes, 47,
        36, itemsPerRow);
    if (defaultIndex)
        gDropdownDefaultIndex = static_cast<int32_t>(*defaultIndex);
}







static void WindowPoolSetEnabledAndPressedWidgets()
{
    rct_window* w = window_find_by_class(WC_POOL);
    if (w == nullptr)
    {
        return;
    }

    uint64_t pressedWidgets = w->pressed_widgets
        & ~((1LL << WIDX_CORNER_SQUARE) | (1LL << WIDX_CORNER_ANGLED) | (1LL << WIDX_CORNER_ROUNDED));
    uint64_t disabledWidgets = 0;

    w->pressed_widgets = pressedWidgets;
    w->disabled_widgets = disabledWidgets;
    w->Invalidate();
}

static ObjectEntryIndex PoolGetDefault()
{
    for (ObjectEntryIndex i = 0; i < MAX_FOOTPATH_SURFACE_OBJECTS; i++)
    {
        auto pathEntry = GetPathSurfaceEntry(i);
        if (pathEntry != nullptr)return i;
    }
    return OBJECT_ENTRY_INDEX_NULL;
}

static bool PoolSelectDefault()
{
    // Select default footpath
    ObjectEntryIndex surfaceIndex = PoolGetDefault();

    if (surfaceIndex == OBJECT_ENTRY_INDEX_NULL)
    {
            // No surfaces or legacy paths available
            return false;
    }

    gPoolSelection.Pool = surfaceIndex;
    return true;
}

