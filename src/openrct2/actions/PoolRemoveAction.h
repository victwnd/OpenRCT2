/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#include "../world/TileElement.h"
#include "GameAction.h"

class PoolRemoveAction final : public GameActionBase<GameCommand::RemovePool>
{
private:
    CoordsXYZ _loc;

public:
    PoolRemoveAction() = default;
    PoolRemoveAction(const CoordsXYZ& location);

    void AcceptParameters(GameActionParameterVisitor& visitor) override;

    uint16_t GetActionFlags() const override;

    void Serialise(DataSerialiser& stream) override;
    GameActions::Result Query() const override;
    GameActions::Result Execute() const override;
    GameActions::Result QueryExecute(bool isExecuting) const;

private:
    TileElement* FindPoolElement() const;
};
