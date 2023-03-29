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

using namespace OpenRCT2::Ui;



#pragma region Measurements

static constexpr const StringId WINDOW_TITLE = STR_POOLS;
static constexpr const int32_t WH = 180;
static constexpr const int32_t WW = 150;

#pragma endregion

#pragma region Widgets

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

static Widget window_pool_widgets[] = {
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

#pragma endregion


class PoolWindow final : public Window
{
private:
    std::vector<std::pair<ObjectType, ObjectEntryIndex>> _dropdownEntries;
   
    uint8_t curEdgeStyle=PoolEdgeStyle::Square;
    bool _poolPlaceCtrlState;
    int32_t _poolPlaceCtrlZ;
    bool _poolPlaceShiftState;
    ScreenCoordsXY _poolPlaceShiftStart;
    int32_t _poolPlaceShiftZ;
    int32_t _poolPlaceZ;
    bool _poolIsWater=true;
    money32 _window_pool_cost;
    uint8_t _lastUpdatedCameraRotation = UINT8_MAX;
    bool _poolErrorOccured;

public:
#pragma region Window Override Events

    void OnOpen() override
    {

        widgets = window_pool_widgets;
        WindowInitScrollWidgets(*this);
        WindowPushOthersRight(*this);
        ShowGridlines();
    
        ToolCancel();
        ToolSet(*this, WIDX_CONSTRUCT_WATER_TILE, Tool::PathDown);
        InputSetFlag(INPUT_FLAG_6, true);
        _poolIsWater=true;
        _poolErrorOccured = false;
        WindowPoolSetEnabledAndPressedWidgets();
    }

    void OnClose() override
    {
        pool_provisional_update();
        ViewportSetVisibility(0);
        MapInvalidateMapSelectionTiles();
        gMapSelectFlags &= ~MAP_SELECT_FLAG_ENABLE_CONSTRUCT;
        WindowInvalidateByClass(WindowClass::TopToolbar);
        HideGridlines();
    }

    void OnUpdate() override
    {
    
        // #2502: The camera might have changed rotation, so we need to update which directional buttons are pressed
        uint8_t currentRotation = GetCurrentRotation();
        if (_lastUpdatedCameraRotation != currentRotation)
        {
            _lastUpdatedCameraRotation = currentRotation;
            WindowPoolSetEnabledAndPressedWidgets();
        }
    
    }

    void OnMouseDown(WidgetIndex widgetIndex) override
    {
        switch (widgetIndex)
        {
            case WIDX_FOOTPATH_TYPE:
                WindowPoolShowPoolTypesDialog(&window_pool_widgets[widgetIndex]);
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

    void OnMouseUp(WidgetIndex widgetIndex) override
    {
        switch (widgetIndex)
        {
            case WIDX_CLOSE:
                WindowClose(*this);
                break;
            case WIDX_CONSTRUCT_PATH_TILE:
    		ToolCancel();
    		ToolSet(*this, WIDX_CONSTRUCT_PATH_TILE, Tool::PathDown);
                    InputSetFlag(INPUT_FLAG_6, true);
            	_poolIsWater=false;
    	break;
            case WIDX_CONSTRUCT_WATER_TILE:
    		ToolCancel();
    		ToolSet(*this, WIDX_CONSTRUCT_WATER_TILE, Tool::PathDown);
    	        InputSetFlag(INPUT_FLAG_6, true);
            	_poolIsWater=true;
    		break;
        }
    }

    void OnDropdown(WidgetIndex widgetIndex, int32_t selectedIndex) override
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

    void OnToolUpdate(WidgetIndex widgetIndex, const ScreenCoordsXY& screenCoords) override
{
    auto coords=PoolGetPlacePositionFromScreenPosition(screenCoords);
	if(!coords)return;
    auto loc=*coords;
    
    MapInvalidateSelectionRect();

        // Check for change
        if ((gProvisionalPool.Flags & PROVISIONAL_POOL_FLAG_1) && gProvisionalPool.Position == loc)return;
        // Set map selection
        gMapSelectFlags |= MAP_SELECT_FLAG_ENABLE;
        gMapSelectType = MAP_SELECT_TYPE_FULL;
        gMapSelectPositionA = *coords;
        gMapSelectPositionB = *coords;

        //pool_provisional_update();
       _window_pool_cost = pool_provisional_set(gPoolSelection.Pool, loc, _poolIsWater, PoolEdgeStyle::Curved);
      WindowInvalidateByClass(WindowClass::Pool);
}

    void OnToolUp(WidgetIndex widgetIndex, const ScreenCoordsXY&) override
{
    if (widgetIndex == WIDX_CONSTRUCT_PATH_TILE)
    {
        _poolErrorOccured = false;
    }
}

    void OnToolDown(WidgetIndex widgetIndex, const ScreenCoordsXY& screenCoords) override
    {
    WindowPoolPlacePoolAt(screenCoords);
    }

    void OnToolDrag(WidgetIndex widgetIndex, const ScreenCoordsXY& screenCoords) override
    {
    WindowPoolPlacePoolAt(screenCoords);
    }

    void OnPrepareDraw() override
    {
        // Press / unpress footpath and queue type buttons
        pressed_widgets &= ~(1ULL << WIDX_FOOTPATH_TYPE);
        pressed_widgets &= ~(1ULL << WIDX_QUEUELINE_TYPE);
        pressed_widgets |= 0;
    }

    void OnDraw(DrawPixelInfo& dpi) override
    {
        ScreenCoordsXY screenCoords;
        WindowDrawWidgets(*this, &dpi);
        WindowPoolDrawDropdownButtons(dpi);
    
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

#pragma endregion

private:
  
std::optional<CoordsXYZ> PoolGetPlacePositionFromScreenPosition(ScreenCoordsXY screenCoords)
{
    CoordsXY mapCoords;

//Handle CTRL
    if (!_poolPlaceCtrlState)
    {
        if (gInputPlaceObjectModifier & PLACE_OBJECT_MODIFIER_COPY_Z)
        {
            auto info = GetMapCoordinatesFromPos(screenCoords, 0xFCCA);
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
            uint16_t maxHeight = ZoomLevel::max().ApplyTo(std::numeric_limits<decltype(TileElement::BaseHeight)>::max() - 32);

            _poolPlaceShiftZ = _poolPlaceShiftStart.y - screenCoords.y + 4;
            // Scale delta by zoom to match mouse position.
            auto* mainWnd = WindowGetMain();
            if (mainWnd != nullptr && mainWnd->viewport != nullptr)
            {
                _poolPlaceShiftZ = mainWnd->viewport->zoom.ApplyTo(_poolPlaceShiftZ);
            }
            _poolPlaceShiftZ = Floor2(_poolPlaceShiftZ, 16);

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
            auto surfaceElement = MapGetSurfaceElementAt(mapCoords);
            if (surfaceElement == nullptr)
                return std::nullopt;
            auto mapZ = Floor2(surfaceElement->GetBaseZ(), 16);
            mapZ += _poolPlaceShiftZ;
            mapZ = std::max<int16_t>(mapZ, 16);
            _poolPlaceZ = mapZ;
        }
    }
    else
    {
        auto mapZ = _poolPlaceCtrlZ;
        auto mapXYCoords = ScreenGetMapXYWithZ(screenCoords, mapZ);
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
	auto z=MapGetHighestZ(mapCoords);
	//Check if pool can be placed in ground
	auto surfaceElement = MapGetSurfaceElementAt(mapCoords);
	    if(surfaceElement&&surfaceElement->AsSurface()->GetSlope()==0&&surfaceElement->AsSurface()->GetBaseZ()==z)z-=POOL_DEPTH;
	return CoordsXYZ(mapCoords,z);
	}


return CoordsXYZ(mapCoords,_poolPlaceZ);
}

void WindowPoolPlacePoolAt(const ScreenCoordsXY& screenCoords)
{
    if (_poolErrorOccured)
    {
        return;
    }

    pool_provisional_update();

    auto coords=PoolGetPlacePositionFromScreenPosition(screenCoords);
	if(!coords)return;
    auto loc=*coords;

    // Try and place pool
    auto poolPlaceAction = PoolPlaceAction(loc, gPoolSelection.Pool,_poolIsWater,curEdgeStyle);
    poolPlaceAction.SetCallback([this](const GameAction* ga, const GameActions::Result* result) {
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

void WindowPoolDrawDropdownButton(
     DrawPixelInfo& dpi, WidgetIndex widgetIndex, ImageIndex image)
{
    const auto& widget = widgets[widgetIndex];
    GfxDrawSprite(&dpi, ImageId(image), { windowPos.x + widget.left, windowPos.y + widget.top });
}

void WindowPoolDrawDropdownButtons( DrawPixelInfo& dpi)
{
auto poolImage = static_cast<uint32_t>(SPR_NONE);
auto poolEntry = GetPoolEntry(gPoolSelection.Pool);
if (poolEntry != nullptr)
{
    poolImage = poolEntry->PreviewImageId;
}


WindowPoolDrawDropdownButton(dpi, WIDX_FOOTPATH_TYPE, poolImage);
}

void WindowPoolShowPoolTypesDialog( Widget* widget)
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
        windowPos.x + widget->left, windowPos.y + widget->top, widget->height() + 1, colours[1], 0, numPathTypes, 47,
        36, itemsPerRow);
    if (defaultIndex)
        gDropdownDefaultIndex = static_cast<int32_t>(*defaultIndex);
}

void WindowPoolSetEnabledAndPressedWidgets()
{
    WindowBase* w = WindowFindByClass(WindowClass::Pool);
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



};

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

WindowBase* WindowPoolOpen()
{
        if (!PoolSelectDefault())
        {
            // No path objects to select from, don't open window
        }
    
    return WindowFocusOrCreate<PoolWindow>(WindowClass::Pool, WW, WH, 0);
}

