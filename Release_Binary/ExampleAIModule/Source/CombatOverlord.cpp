#include "Include/CombatOverlord.h"

using namespace CoCBot;

const size_t IdlePriority = 0;
const size_t AttackPriority = 1;
const size_t BaseDefensePriority = 2;
const size_t ScoutDefensePriority = 3;
const size_t DropPriority = 4;

CombatOverlord::CombatOverlord()
{

}

//void CombatOverlord::initializeSquads()
//{
//    SquadOrder idleOrder(SquadOrderTypes::Idle, BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()), 100, "Chill Out");
//    m_squadData.addSquad("Idle", Squad("Idle", idleOrder, IdlePriority));
//
//    // the main attack squad that will pressure the enemy's closest base location
//    SquadOrder mainAttackOrder(SquadOrderTypes::Attack, getMainAttackLocation(), 800, "Attack Enemy Base");
//    m_squadData.addSquad("MainAttack", Squad("MainAttack", mainAttackOrder, AttackPriority));
//
//    BWAPI::Position ourBasePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
//
//    // the scout defense squad will handle chasing the enemy worker scout
//    SquadOrder enemyScoutDefense(SquadOrderTypes::Defend, ourBasePosition, 900, "Get the scout");
//    m_squadData.addSquad("ScoutDefense", Squad("ScoutDefense", enemyScoutDefense, ScoutDefensePriority));
//
//    // add a drop squad if we are using a drop strategy
//    if (Config::Strategy::StrategyName == "Protoss_Drop")
//    {
//        SquadOrder zealotDrop(SquadOrderTypes::Drop, ourBasePosition, 900, "Wait for transport");
//        m_squadData.addSquad("Drop", Squad("Drop", zealotDrop, DropPriority));
//    }
//
//    m_initialized = true;
//}

bool CombatOverlord::isSquadUpdateFrame()
{
    return BWAPI::Broodwar->getFrameCount() % 10 == 0;
}

void CombatOverlord::update(const BWAPI::Unitset& combatUnits)
{
    //PROFILE_FUNCTION();
    //
    //if (!Config::Modules::UsingCombatOverlord)
    //{
    //    return;
    //}
    //
    //if (!m_initialized)
    //{
    //    initializeSquads();
    //}

    m_combatUnits = combatUnits;


    if (isSquadUpdateFrame())
    {
        updateIdleSquad();
        updateDefenseSquads();
        updateAttackSquads();
    }

    //m_squadData.update();
}

void CombatOverlord::updateIdleSquad()
{
    //Squad& idleSquad = m_squadData.getSquad("Idle");
    //for (auto& unit : m_combatUnits)
    //{
    //    // if it hasn't been assigned to a squad yet, put it in the low priority idle squad
    //    if (m_squadData.canAssignUnitToSquad(unit, idleSquad))
    //    {
    //        idleSquad.addUnit(unit);
    //    }
    //}
}

void CombatOverlord::updateAttackSquads()
{
    for (auto& unit : BWAPI::Broodwar->self()->getUnits())
    {
        unit->attack(BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation()));
    }
    //Squad& mainAttackSquad = m_squadData.getSquad("MainAttack");

    //for (auto& unit : m_combatUnits)
    //{
    //    if (unit->getType() == BWAPI::UnitTypes::Zerg_Scourge && UnitUtil::GetAllUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk) < 30)
    //    {
    //        continue;
    //    }

    //    // get every unit of a lower priority and put it into the attack squad
    //    if (!unit->getType().isWorker() && (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord) && m_squadData.canAssignUnitToSquad(unit, mainAttackSquad))
    //    {
    //        m_squadData.assignUnitToSquad(unit, mainAttackSquad);
    //    }
    //}

    //SquadOrder mainAttackOrder(SquadOrderTypes::Attack, getMainAttackLocation(), 800, "Attack Enemy Base");
    //mainAttackSquad.setSquadOrder(mainAttackOrder);
}

void CombatOverlord::updateDefenseSquads()
{
    //if (m_combatUnits.empty())
    //{
    //    return;
    //}
    //
    //// for each of our occupied regions
    //const BaseLocation* enemyBaseLocation = Global::Bases().getPlayerStartingBaseLocation(BWAPI::Broodwar->enemy());
    //for (const BaseLocation* myBaseLocation : Global::Bases().getOccupiedBaseLocations(BWAPI::Broodwar->self()))
    //{
    //    // don't defend inside the enemy region, this will end badly when we are stealing gas or cannon rushing
    //    if (myBaseLocation == enemyBaseLocation)
    //    {
    //        continue;
    //    }
    //
    //    BWAPI::Position basePosition = myBaseLocation->getPosition();
    //
    //    // start off assuming all enemy units in region are just workers
    //    int numDefendersPerEnemyUnit = 2;
    //
    //    // all of the enemy units in this region
    //    std::vector<BWAPI::Unit> enemyUnitsInRegion;
    //    for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
    //    {
    //        // if it's an overlord, don't worry about it for defense, we don't care what they see
    //        if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
    //        {
    //            continue;
    //        }
    //
    //        if (myBaseLocation->containsPosition(unit->getPosition()))
    //        {
    //            enemyUnitsInRegion.push_back(unit);
    //        }
    //    }
    //
    //    // we can ignore the first enemy worker in our region since we assume it is a scout
    //    for (auto unit : enemyUnitsInRegion)
    //    {
    //        if (unit->getType().isWorker())
    //        {
    //            enemyUnitsInRegion.erase(std::remove(enemyUnitsInRegion.begin(), enemyUnitsInRegion.end(), unit), enemyUnitsInRegion.end());
    //            break;
    //        }
    //    }
    //
    //    // calculate how many units are flying / ground units
    //    int numEnemyFlyingInRegion = 0;
    //    int numEnemyGroundInRegion = 0;
    //    for (auto& unit : enemyUnitsInRegion)
    //    {
    //        if (unit->isFlying())
    //        {
    //            numEnemyFlyingInRegion++;
    //        }
    //        else
    //        {
    //            numEnemyGroundInRegion++;
    //        }
    //    }
    //
    //
    //    std::stringstream squadName;
    //    squadName << "Base Defense " << basePosition.x << " " << basePosition.y;
    //
    //    // if there's nothing in this region to worry about
    //    if (enemyUnitsInRegion.empty())
    //    {
    //        // if a defense squad for this region exists, remove it
    //        if (m_squadData.squadExists(squadName.str()))
    //        {
    //            m_squadData.getSquad(squadName.str()).clear();
    //        }
    //
    //        // and return, nothing to defend here
    //        continue;
    //    }
    //    else
    //    {
    //        // if we don't have a squad assigned to this region already, create one
    //        if (!m_squadData.squadExists(squadName.str()))
    //        {
    //            SquadOrder defendRegion(SquadOrderTypes::Defend, basePosition, 32 * 25, "Defend Region!");
    //            m_squadData.addSquad(squadName.str(), Squad(squadName.str(), defendRegion, BaseDefensePriority));
    //        }
    //    }
    //
    //    // assign units to the squad
    //    if (m_squadData.squadExists(squadName.str()))
    //    {
    //        Squad& defenseSquad = m_squadData.getSquad(squadName.str());
    //
    //        // figure out how many units we need on defense
    //        int flyingDefendersNeeded = numDefendersPerEnemyUnit * numEnemyFlyingInRegion;
    //        int groundDefensersNeeded = numDefendersPerEnemyUnit * numEnemyGroundInRegion;
    //
    //        updateDefenseSquadUnits(defenseSquad, flyingDefendersNeeded, groundDefensersNeeded);
    //    }
    //    else
    //    {
    //        UAB_ASSERT(false, "Squad should have existed: %s", squadName.str().c_str());
    //    }
    //}
    //
    //// for each of our defense squads, if there aren't any enemy units near the position, remove the squad
    //std::set<std::string> uselessDefenseSquads;
    //for (const auto& kv : m_squadData.getSquads())
    //{
    //    const Squad& squad = kv.second;
    //    const SquadOrder& order = squad.getSquadOrder();
    //
    //    if (order.getType() != SquadOrderTypes::Defend)
    //    {
    //        continue;
    //    }
    //
    //    bool enemyUnitInRange = false;
    //    for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
    //    {
    //        if (unit->getDistance(order.getPosition()) < order.getRadius())
    //        {
    //            enemyUnitInRange = true;
    //            break;
    //        }
    //    }
    //
    //    if (!enemyUnitInRange)
    //    {
    //        m_squadData.getSquad(squad.getName()).clear();
    //    }
    //}
}

//void CombatOverlord::updateDefenseSquadUnits(Squad& defenseSquad, const size_t& flyingDefendersNeeded, const size_t& groundDefendersNeeded)
//{
//    const BWAPI::Unitset& squadUnits = defenseSquad.getUnits();
//    size_t flyingDefendersInSquad = std::count_if(squadUnits.begin(), squadUnits.end(), UnitUtil::CanAttackAir);
//    size_t groundDefendersInSquad = std::count_if(squadUnits.begin(), squadUnits.end(), UnitUtil::CanAttackGround);
//
//    // if there's nothing left to defend, clear the squad
//    if (flyingDefendersNeeded == 0 && groundDefendersNeeded == 0)
//    {
//        defenseSquad.clear();
//        return;
//    }
//
//    // add flying defenders if we still need them
//    size_t flyingDefendersAdded = 0;
//    while (flyingDefendersNeeded > flyingDefendersInSquad + flyingDefendersAdded)
//    {
//        BWAPI::Unit defenderToAdd = findClosestDefender(defenseSquad, defenseSquad.getSquadOrder().getPosition(), true);
//
//        // if we find a valid flying defender, add it to the squad
//        if (defenderToAdd)
//        {
//            m_squadData.assignUnitToSquad(defenderToAdd, defenseSquad);
//            ++flyingDefendersAdded;
//        }
//        // otherwise we'll never find another one so break out of this loop
//        else
//        {
//            break;
//        }
//    }
//
//    // add ground defenders if we still need them
//    size_t groundDefendersAdded = 0;
//    while (groundDefendersNeeded > groundDefendersInSquad + groundDefendersAdded)
//    {
//        BWAPI::Unit defenderToAdd = findClosestDefender(defenseSquad, defenseSquad.getSquadOrder().getPosition(), false);
//
//        // if we find a valid ground defender add it
//        if (defenderToAdd)
//        {
//            m_squadData.assignUnitToSquad(defenderToAdd, defenseSquad);
//            ++groundDefendersAdded;
//        }
//        // otherwise we'll never find another one so break out of this loop
//        else
//        {
//            break;
//        }
//    }
//}

//BWAPI::Unit CombatOverlord::findClosestDefender(const Squad& defenseSquad, BWAPI::Position pos, bool flyingDefender)
//{
//    BWAPI::Unit closestDefender = nullptr;
//    double minDistance = std::numeric_limits<double>::max();
//
//    int zerglingsInOurBase = numZerglingsInOurBase();
//    bool zerglingRush = zerglingsInOurBase > 0 && BWAPI::Broodwar->getFrameCount() < 5000;
//
//    for (auto& unit : m_combatUnits)
//    {
//        if ((flyingDefender && !UnitUtil::CanAttackAir(unit)) || (!flyingDefender && !UnitUtil::CanAttackGround(unit)))
//        {
//            continue;
//        }
//
//        if (!m_squadData.canAssignUnitToSquad(unit, defenseSquad))
//        {
//            continue;
//        }
//
//        // add workers to the defense squad if we are being rushed very quickly
//        if (!Config::Micro::WorkersDefendRush || (unit->getType().isWorker() && !zerglingRush && !beingBuildingRushed()))
//        {
//            continue;
//        }
//
//        double dist = unit->getDistance(pos);
//        if (!closestDefender || (dist < minDistance))
//        {
//            closestDefender = unit;
//            minDistance = dist;
//        }
//    }
//
//    return closestDefender;
//}

BWAPI::Position CombatOverlord::getDefendLocation()
{
    return BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
}

void CombatOverlord::drawSquadInformation(int x, int y)
{
    //m_squadData.drawSquadInformation(x, y);
}

BWAPI::Position CombatOverlord::getMainAttackLocation()
{
    //const BaseLocation* enemyBaseLocation = Global::Bases().getPlayerStartingBaseLocation(BWAPI::Broodwar->enemy());
    //
    //// First choice: Attack an enemy region if we can see units inside it
    //if (enemyBaseLocation)
    //{
    //    BWAPI::Position enemyBasePosition = enemyBaseLocation->getPosition();
    //
    //    // get all known enemy units in the area
    //    BWAPI::Unitset enemyUnitsInArea;
    //    Global::Map().getUnits(enemyUnitsInArea, enemyBasePosition, 800, false, true);
    //
    //    bool onlyOverlords = true;
    //    for (auto& unit : enemyUnitsInArea)
    //    {
    //        if (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord)
    //        {
    //            onlyOverlords = false;
    //        }
    //    }
    //
    //    if (!BWAPI::Broodwar->isExplored(BWAPI::TilePosition(enemyBasePosition)) || !enemyUnitsInArea.empty())
    //    {
    //        if (!onlyOverlords)
    //        {
    //            return enemyBaseLocation->getPosition();
    //        }
    //    }
    //}
    //
    //// Second choice: Attack known enemy buildings
    //for (const auto& kv : Global::Info().getUnitInfo(BWAPI::Broodwar->enemy()))
    //{
    //    const UnitInfo& ui = kv.second;
    //
    //    if (ui.type.isBuilding() && ui.lastPosition != BWAPI::Positions::None)
    //    {
    //        return ui.lastPosition;
    //    }
    //}
    //
    //// Third choice: Attack visible enemy units that aren't overlords
    //for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
    //{
    //    if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
    //    {
    //        continue;
    //    }
    //
    //    if (UnitUtil::IsValidUnit(unit) && unit->isVisible())
    //    {
    //        return unit->getPosition();
    //    }
    //}
    //
    //// Fourth choice: We can't see anything so explore the map attacking along the way
    //return BWAPI::Position(Global::Map().getLeastRecentlySeenTile());
}

BWAPI::Unit CombatOverlord::findClosestWorkerToTarget(BWAPI::Unitset& unitsToAssign, BWAPI::Unit target)
{
    //UAB_ASSERT(target != nullptr, "target was null");
    //
    //if (!target)
    //{
    //    return nullptr;
    //}
    //
    //BWAPI::Unit closestMineralWorker = nullptr;
    //double closestDist = 100000;
    //
    //// for each of our workers
    //for (auto& unit : unitsToAssign)
    //{
    //    if (!unit->getType().isWorker())
    //    {
    //        continue;
    //    }
    //
    //    // if it is a move worker
    //    if (Global::Workers().isFree(unit))
    //    {
    //        double dist = unit->getDistance(target);
    //
    //        if (!closestMineralWorker || dist < closestDist)
    //        {
    //            closestMineralWorker = unit;
    //            dist = closestDist;
    //        }
    //    }
    //}
    //
    //return closestMineralWorker;
}

// when do we want to defend with our workers?
// this function can only be called if we have no fighters to defend with
int CombatOverlord::defendWithWorkers()
{
    // our home nexus position
    BWAPI::Position homePosition(BWAPI::Broodwar->self()->getStartLocation());

    // enemy units near our workers
    int enemyUnitsNearWorkers = 0;

    // defense radius of nexus
    int defenseRadius = 300;

    // fill the set with the types of units we're concerned about
    for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
    {
        // if it's a zergling or a worker we want to defend
        if (unit->getType() == BWAPI::UnitTypes::Zerg_Zergling)
        {
            if (unit->getDistance(homePosition) < defenseRadius)
            {
                enemyUnitsNearWorkers++;
            }
        }
    }

    // if there are enemy units near our workers, we want to defend
    return enemyUnitsNearWorkers;
}
