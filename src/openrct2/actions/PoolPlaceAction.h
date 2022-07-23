/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../world/Footpath.h"
#include "GameAction.h"

class PoolPlaceAction final : public GameActionBase<GameCommand::PlacePool>
{
private:
    CoordsXYZ _loc;
    ObjectEntryIndex _type{};

public:
    PoolPlaceAction() = default;
    PoolPlaceAction(
        const CoordsXYZ& loc, ObjectEntryIndex type);
    void AcceptParameters(GameActionParameterVisitor& visitor) override;

    uint16_t GetActionFlags() const override;

    void Serialise(DataSerialiser& stream) override;
    GameActions::Result Query() const override;
    GameActions::Result Execute() const override;

private:
    GameActions::Result ElementUpdateQuery(PoolElement* pathElement, GameActions::Result res) const;
    GameActions::Result ElementUpdateExecute(PoolElement* pathElement, GameActions::Result res) const;
    GameActions::Result ElementInsertQuery(GameActions::Result res) const;
    GameActions::Result ElementInsertExecute(GameActions::Result res) const;
    GameActions::Result ElementInsertQueryExecute(GameActions::Result res,bool isExecuting) const;
    PoolElement* map_get_pool_element(const CoordsXYZ& poolPos) const;
};
