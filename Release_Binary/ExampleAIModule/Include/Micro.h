#pragma once

#include <BWAPI.h>

namespace CoCBot
{
namespace Micro
{      
    void SmartAttackUnit(BWAPI::Unit attacker, BWAPI::Unit target);
    void SmartAttackMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);
    void SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);
    void SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target);
    void SmartLaySpiderMine(BWAPI::Unit unit, BWAPI::Position pos);
    void SmartRepair(BWAPI::Unit unit, BWAPI::Unit target);
    void SmartKiteTarget(BWAPI::Unit rangedUnit, BWAPI::Unit target);
    void MutaDanceTarget(BWAPI::Unit muta, BWAPI::Unit target);
    
    BWAPI::Position GetKiteVector(BWAPI::Unit unit, BWAPI::Unit target);
};
}