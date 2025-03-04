#include "Include/CombatOverlord.h"

using namespace CoCBot;

const size_t IdlePriority = 0;
const size_t AttackPriority = 1;
const size_t BaseDefensePriority = 2;
const size_t ScoutDefensePriority = 3;
const size_t DropPriority = 4;

CombatOverlord::CombatOverlord()
{

	//m_ordersList.push_back(SquadOrder(SquadOrderTypes::Attack, BWAPI::Position(0, 0), 800, "inactive", "Attack Enemy Base"));
	//m_ordersList.push_back(SquadOrder(SquadOrderTypes::Attack, BWAPI::Position(0, 0), 800, "inactive", "Attack Enemy Starting Position"));
	//m_ordersList.push_back(SquadOrder(SquadOrderTypes::Idle, BWAPI::Position(0, 0), 800, "inactive", "Patrol Map"));
	//m_ordersList.push_back(SquadOrder(SquadOrderTypes::Regroup, getDefendLocation(), 800, "inactive", "Regroup"));
}

void CombatOverlord::initializeSquads()
{
	BWAPI::Unitset::iterator mCmmd = m_commandUnits.begin();
	m_mainCommander = *mCmmd++;
	//SquadOrder idleOrder(SquadOrderTypes::Idle, BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()), 100, "Chill Out");
	//m_squads.addSquad("Idle", SquadCommander("Idle", idleOrder, IdlePriority));
	m_ordersList.push_back(SquadOrder(SquadOrderTypes::Regroup, getDefendLocation(), 800, "inactive", "Regroup"));
	//m_ordersList.push_back(SquadOrder(SquadOrderTypes::Regroup, getDefendLocation(), 800, "inactive", "Regroup"));
	m_ordersList.push_back(SquadOrder(SquadOrderTypes::Attack, getThirdAttackLocation(), 800, "inactive", "Patrol Map"));
	m_ordersList.push_back(SquadOrder(SquadOrderTypes::Attack, getMainAttackLocation(), 800, "inactive", "Attack Enemy Base"));
	m_ordersList.push_back(SquadOrder(SquadOrderTypes::Attack, getSecondaryAttackLocation(), 800, "inactive", "Attack Enemy Starting Position"));
	// the main attack squad that will pressure the enemy's closest base location
	SquadOrder mainAttackOrder = m_ordersList[3];
	mainAttackOrder.Assigned();
	m_ordersList[3].Assigned();
	m_squads.addSquad("MainAttack", SquadCommander("MainAttack", mainAttackOrder, AttackPriority, *mCmmd++));

	SquadOrder secondaryAttackOrder = m_ordersList[2];
	secondaryAttackOrder.Assigned();
	m_ordersList[2].Assigned();
	m_squads.addSquad("SecondAttack", SquadCommander("SecondAttack", secondaryAttackOrder, AttackPriority, *mCmmd++));

	SquadOrder thirdAttackOrder = m_ordersList[1];
	thirdAttackOrder.Assigned();
	m_ordersList[1].Assigned();
	m_squads.addSquad("ThirdAttack", SquadCommander("ThirdAttack", thirdAttackOrder, AttackPriority, *mCmmd));

	BWAPI::Position ourBasePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	// the scout defense squad will handle chasing the enemy worker scout
	//SquadOrder enemyScoutDefense(SquadOrderTypes::Defend, ourBasePosition, 900, "Get the scout");
	//m_squads.addSquad("ScoutDefense", SquadCommander("ScoutDefense", enemyScoutDefense, ScoutDefensePriority));

	//// add a drop squad if we are using a drop strategy
	//if (Config::Strategy::StrategyName == "Protoss_Drop")
	//{
	//    SquadOrder zealotDrop(SquadOrderTypes::Drop, ourBasePosition, 900, "Wait for transport");
	//    m_squadData.addSquad("Drop", Squad("Drop", zealotDrop, DropPriority));
	//}

	m_initialized = true;
}

bool CombatOverlord::isSquadUpdateFrame()
{
	return BWAPI::Broodwar->getFrameCount() % 10 == 0;
}

void CombatOverlord::update(const std::list<BWAPI::Unitset>& combatUnits, const BWAPI::Unitset& commandUnits)
{
	//PROFILE_FUNCTION();
	//
	//if (!Config::Modules::UsingCombatOverlord)
	//{
	//    return;
	//}
	BWAPI::Broodwar << "testwid" << std::endl;
	m_commandUnits = commandUnits;
	if (!m_initialized)
	{
		initializeSquads();
	}

	m_unitTypesList = combatUnits;
	emergencyReaction();

	if (isSquadUpdateFrame())
	{
		updateIdleSquad();
		updateDefenseSquads();
		updateAttackSquads();
	}

	m_squads.update();

	//for (auto& unit : BWAPI::Broodwar->self()->getUnits())
	//{
	//	unit->attack(BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation()));
	//}
	//BWAPI::Broodwar << m_squads.getSquad("MainAttack").getUnits().size() << "frst" << std::endl;
	//BWAPI::Broodwar << m_squads.getSquad("SecondAttack").getUnits().size() << "scnd" << std::endl;
	//BWAPI::Broodwar << m_squads.getSquad("ThirdAttack").getUnits().size() << "thrd" << std::endl;
	//for (auto& unit : m_squads.getSquad("MainAttack").getUnits())
	//{
	//	unit->attack(BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation()));
	//}
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
	SquadCommander& mainAttackSquad = m_squads.getSquad("MainAttack");

	for (auto& unitsType : m_unitTypesList)
	{
		//if (unit->getType() == BWAPI::UnitTypes::Zerg_Scourge && UnitState::GetAllUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk) < 30)
		//{
		//    continue;
		//}
		int theThirdPart = unitsType.size() / 3;
		BWAPI::Unitset::iterator iter = unitsType.begin();
		//for (auto& unit : unitsType)
		for (int i = 0; i < theThirdPart; i++)
		{
			// get every unit of a lower priority and put it into the attack squad
			if (/*!unit->getType().isWorker() && (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord) &&*/ m_squads.canAssignUnitToSquad(*iter, mainAttackSquad))
			{
				m_squads.assignUnitToSquad(*iter, mainAttackSquad);
			}
			iter++;

		}
	}
	//BWAPI::Broodwar << m_squads.getSquad("MainAttack").getUnits().size() << "frsdasst" << std::endl;
	//
	//SquadOrder mainAttackOrder(SquadOrderTypes::Attack, getMainAttackLocation(), 800, "Attack Enemy Base");
	//mainAttackSquad.setSquadOrder(mainAttackOrder);

	SquadCommander& secondaryAttackSquad = m_squads.getSquad("SecondAttack");

	for (auto& unitsType : m_unitTypesList)
	{
		//if (unit->getType() == BWAPI::UnitTypes::Zerg_Scourge && UnitState::GetAllUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk) < 30)
		//{
		//    continue;
		//}
		int theThirdPart = unitsType.size() / 3;
		BWAPI::Unitset::iterator iter = unitsType.begin();
		advance(iter, theThirdPart);
		for (int i = theThirdPart; i < 2 * theThirdPart; i++)
			//for (auto& unit : unitsType)
		{
			// get every unit of a lower priority and put it into the attack squad
			if (/*!unit->getType().isWorker() && (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord) &&*/ m_squads.canAssignUnitToSquad(*iter, secondaryAttackSquad))
			{
				m_squads.assignUnitToSquad(*iter, secondaryAttackSquad);
			}
			iter++;
		}
	}

	//SquadOrder secondaryAttackOrder(SquadOrderTypes::Attack, getSecondaryAttackLocation(), 800, "Attack Enemy Base");
	//secondaryAttackSquad.setSquadOrder(secondaryAttackOrder);

	SquadCommander& thirdAttackSquad = m_squads.getSquad("ThirdAttack");

	for (auto& unitsType : m_unitTypesList)
	{
		//if (unit->getType() == BWAPI::UnitTypes::Zerg_Scourge && UnitState::GetAllUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk) < 30)
		//{
		//    continue;
		//}
		int theThirdPart = unitsType.size() / 3;
		BWAPI::Unitset::iterator iter = unitsType.begin();
		advance(iter, theThirdPart * 2);
		for (int i = theThirdPart * 2; i < unitsType.size(); i++)
			//for (auto& unit : unitsType)
		{
			// get every unit of a lower priority and put it into the attack squad
			if (/*!unit->getType().isWorker() && (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord) &&*/ m_squads.canAssignUnitToSquad(*iter, thirdAttackSquad))
			{
				m_squads.assignUnitToSquad(*iter, thirdAttackSquad);
			}
			iter++;
		}
	}

	//SquadOrder thirdAttackOrder(SquadOrderTypes::Attack, getThirdAttackLocation(), 800, "Attack Enemy Base");
	//thirdAttackSquad.setSquadOrder(thirdAttackOrder);

	for (auto& squad : m_squads.getSquads())
	{
		if (squad.second.getSquadOrder().getStatus() == "completed")
		{
			if (squad.second.getSquadOrder().getName() == "Patrol Map")
			{
				squad.second.setSquadOrder(SquadOrder(SquadOrderTypes::Attack, getThirdAttackLocation(), 800, "assigned", "Patrol Map"));
				continue;
			}
			for (auto& order : m_ordersList)
			{
				if (order.getName() == squad.second.getSquadOrder().getName())
				{
					order.Completed();
					//squad.second.
				}
			}
		}
	}
	for (auto& squad : m_squads.getSquads())
	{
		if (squad.second.getSquadOrder().getStatus() == "completed")
		{
			for (auto& order : m_ordersList)
			{
				if (order.getStatus() != "completed")
				{
					SquadCommander& squadm = m_squads.getSquad(squad.second.getName());
					squadm.setSquadOrder(order);
					if (order.getStatus() == "inactive")
					{
						order.Assigned();
					}
				}
			}
		}
	}
}

void CombatOverlord::emergencyReaction()
{
	BWAPI::Unitset nearbyEnemies;
	CoCBot::Global::Map().getUnits(nearbyEnemies, m_mainCommander->getPosition(), 300, false, true);
	if (nearbyEnemies.size() > 0)
	{
		for (auto& squad : m_squads.getSquads())
		{
			squad.second.setEmergencyOrder(SquadOrder(SquadOrderTypes::Attack, m_mainCommander->getPosition(), 300, "assigned", "Defend Commander", true));
		}
	}
	else if (nearbyEnemies.size() == 0)
	{
		for (auto& squad : m_squads.getSquads())
		{
			if (squad.second.getSquadOrder().getName() == "Defend Commander")
			{
				squad.second.setEmergencyOrder(squad.second.getOriginalOrder());
			}
		}

	}
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
	const BaseLocation* enemyBaseLocation = Global::Bases().getPlayerStartingBaseLocation(BWAPI::Broodwar->enemy());
	//const BWAPI::TilePosition enemyBaseLocation = BWAPI::Broodwar->enemy()->getStartLocation();

	// First choice: Attack an enemy region if we can see units inside it
	if (enemyBaseLocation)
	{
		BWAPI::Position enemyBasePosition = enemyBaseLocation->getPosition();
		//BWAPI::Position enemyBasePosition = BWAPI::Position(enemyBaseLocation);

		// get all known enemy units in the area
		BWAPI::Unitset enemyUnitsInArea;
		Global::Map().getUnits(enemyUnitsInArea, enemyBasePosition, 800, false, true);

		bool onlyOverlords = true;
		for (auto& unit : enemyUnitsInArea)
		{
			if (unit->getType() != BWAPI::UnitTypes::Zerg_Overlord)
			{
				onlyOverlords = false;
			}
		}

		if (!BWAPI::Broodwar->isExplored(BWAPI::TilePosition(enemyBasePosition)) || !enemyUnitsInArea.empty())
		{
			if (!onlyOverlords)
			{
				return enemyBaseLocation->getPosition();
				//return enemyBasePosition;
			}
		}
	}

	// Second choice: Attack known enemy buildings
	for (const auto& kv : Global::Info().getUnitInfo(BWAPI::Broodwar->enemy()))
	{
		const UnitInfo& ui = kv.second;

		if (ui.type.isBuilding() /*&& UnitState::IsValidUnit(ui.unit)*/ && ui.lastPosition != BWAPI::Positions::None)
		{
			return ui.lastPosition;
		}
	}

	// Third choice: Attack visible enemy units that aren't overlords
	for (auto& unit : BWAPI::Broodwar->enemy()->getUnits())
	{
		if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord)
		{
			continue;
		}

		if (UnitState::IsValidUnit(unit) && unit->isVisible())
		{
			return unit->getPosition();
		}
	}

	// Fourth choice: We can't see anything so explore the map attacking along the way
	return BWAPI::Position(Global::Map().getLeastRecentlySeenTile());
	srand(time(NULL));
	return BWAPI::Position(rand() % 1000, rand() % 1000);
}

BWAPI::Position CombatOverlord::getSecondaryAttackLocation()
{
	return BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation());
}

BWAPI::Position CombatOverlord::getThirdAttackLocation()
{
	srand(time(NULL));
	return BWAPI::Position(rand() % 3500, rand() % 3500);
}

//BWAPI::Unit CombatOverlord::findClosestWorkerToTarget(BWAPI::Unitset& unitsToAssign, BWAPI::Unit target)
//{
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
//}

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
