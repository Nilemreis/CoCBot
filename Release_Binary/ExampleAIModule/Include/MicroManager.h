#pragma once

#include "Includes.h"
#include "SquadOrders.h"

namespace CoCBot
{
struct AirThreat
{
    BWAPI::Unit	unit;
    double      weight;
};

struct GroundThreat
{
    BWAPI::Unit	unit;
    double      weight;
};

class MicroManager
{
    BWAPI::Unitset  m_units;
    BWAPI::Unit m_commander;

protected:
    CoCBot::SquadOrder m_order;

    virtual void executeMicro(const BWAPI::Unitset & targets) = 0;
    bool checkPositionWalkable(BWAPI::Position pos);
    void drawOrderText();
    bool unitNearEnemy(BWAPI::Unit unit);
    bool unitNearChokepoint(BWAPI::Unit unit) const;
    void trainSubUnits(BWAPI::Unit unit) const;

public:

    MicroManager();
    virtual ~MicroManager() {}

    const BWAPI::Unitset & getUnits() const;
    BWAPI::Position calcCenter() const;

    void setUnits(const BWAPI::Unitset & u, const BWAPI::Unit& cmdr);
    void execute(const CoCBot::SquadOrder & order);
    void regroup(const BWAPI::Position &regroupPosition) const;
    void tighten(const BWAPI::Position &tightenFormation);

};
}