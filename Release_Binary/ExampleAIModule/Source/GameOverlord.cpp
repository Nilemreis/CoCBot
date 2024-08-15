#include "Include/GameOverlord.h"
#include "Include/UnitState.h"

using namespace CoCBot;

GameOverlord::GameOverlord()
	: m_initialScoutSet(false)
{

	handleUnitAssignments();
}

void GameOverlord::update()
{
	//PROFILE_FUNCTION();
	//
	//m_timerManager.startTimer(TimerManager::All);
	//
	//// populate the unit vectors we will pass into various managers

	//
	//// utility managers
	//m_timerManager.startTimer(TimerManager::InformationManager);
	//Global::Info().update();
	//m_timerManager.stopTimer(TimerManager::InformationManager);
	//
	//m_timerManager.startTimer(TimerManager::MapTools);
	//Global::Map().onFrame();
	//m_timerManager.stopTimer(TimerManager::MapTools);
	//
	//// economy and base managers
	//m_timerManager.startTimer(TimerManager::Worker);
	//Global::Workers().onFrame();
	//m_timerManager.stopTimer(TimerManager::Worker);
	//
	//m_timerManager.startTimer(TimerManager::Production);
	//Global::Production().update();
	//m_timerManager.stopTimer(TimerManager::Production);

	// combat and scouting managers
	//m_timerManager.startTimer(TimerManager::Combat);
	m_combatOverlord.update(m_unitTypesList, m_commandUnits);
	//m_timerManager.stopTimer(TimerManager::Combat);

	//m_timerManager.startTimer(TimerManager::Scout);
	//Global::Scout().update();
	//m_timerManager.stopTimer(TimerManager::Scout);
	//
	//m_timerManager.stopTimer(TimerManager::All);
	//
	//Global::Bases().onFrame();

	drawDebugInterface();
}

void GameOverlord::drawDebugInterface()
{
	//Global::Info().drawExtendedInterface();
	//Global::Info().drawUnitInformation(425, 30);
	//Global::Info().drawMapInformation();
	//
	//Global::Production().drawProductionInformation(30, 50);

	m_combatOverlord.drawSquadInformation(200, 30);
	//m_timerManager.displayTimers(490, 225);
	drawGameInformation(4, 1);

	// draw position of mouse cursor
	//if (Config::Debug::DrawMouseCursorInfo)
	//{
	//	int mouseX = BWAPI::Broodwar->getMousePosition().x + BWAPI::Broodwar->getScreenPosition().x;
	//	int mouseY = BWAPI::Broodwar->getMousePosition().y + BWAPI::Broodwar->getScreenPosition().y;
	//	BWAPI::Broodwar->drawTextMap(mouseX + 20, mouseY, " %d %d", mouseX, mouseY);
	//}
}

void GameOverlord::drawGameInformation(int x, int y)
{
	BWAPI::Broodwar->drawTextScreen(x, y, "\x04Players:");
	BWAPI::Broodwar->drawTextScreen(x + 50, y, "%c%s \x04vs. %c%s", BWAPI::Broodwar->self()->getTextColor(), BWAPI::Broodwar->self()->getName().c_str(),
		BWAPI::Broodwar->enemy()->getTextColor(), BWAPI::Broodwar->enemy()->getName().c_str());
	y += 12;

	BWAPI::Broodwar->drawTextScreen(x, y, "\x04Strategy:");
	//BWAPI::Broodwar->drawTextScreen(x + 50, y, "\x03%s %s", Config::Strategy::StrategyName.c_str(), Config::Strategy::FoundEnemySpecificStrategy ? "(enemy specific)" : "");
	BWAPI::Broodwar->setTextSize();
	y += 12;

	BWAPI::Broodwar->drawTextScreen(x, y, "\x04Map:");
	BWAPI::Broodwar->drawTextScreen(x + 50, y, "\x03%s", BWAPI::Broodwar->mapFileName().c_str());
	BWAPI::Broodwar->setTextSize();
	y += 12;

	BWAPI::Broodwar->drawTextScreen(x, y, "\x04Time:");
	BWAPI::Broodwar->drawTextScreen(x + 50, y, "\x04%d %4dm %3ds", BWAPI::Broodwar->getFrameCount(), (int)(BWAPI::Broodwar->getFrameCount() / (23.8 * 60)), (int)((int)(BWAPI::Broodwar->getFrameCount() / 23.8) % 60));
}

// assigns units to various managers
void GameOverlord::handleUnitAssignments()
{
	BWAPI::Broodwar->drawTextScreen(400, 200, "initunit assingment");
	m_validUnits.clear();
	m_combatUnits.clear();

	// filter our units for those which are valid and usable
	setValidUnits();

	// set each type of unit
	setCommandUnits();
	//setScoutUnits();
	setCombatUnits();

}

bool GameOverlord::isAssigned(BWAPI::Unit unit) const
{
	return m_combatUnits.contains(unit) || m_scoutUnits.contains(unit) || m_commandUnits.contains(unit);
}

// validates units as usable for distribution to various managers
void GameOverlord::setValidUnits()
{
	// make sure the unit is completed and alive and usable
	for (auto& unit : BWAPI::Broodwar->self()->getUnits())
	{
		if (UnitState::IsValidUnit(unit))
		{
			m_validUnits.insert(unit);
		}
	}
}

void GameOverlord::setCommandUnits()
{
	for (auto& unit : m_validUnits)
	{
		if (!isAssigned(unit) && unit->getType().isWorker())
		{
			assignUnit(unit, m_commandUnits);
		}
	}
}

//void GameOverlord::setScoutUnits()
//{
//	// if we haven't set a scout unit, do it
//	if (m_scoutUnits.empty() && !m_initialScoutSet)
//	{
//		BWAPI::Unit supplyProvider = getFirstSupplyProvider();
//
//		// if it exists
//		if (supplyProvider)
//		{
//			// grab the closest worker to the supply provider to send to scout
//			BWAPI::Unit workerScout = getClosestWorkerToTarget(supplyProvider->getPosition());
//
//			// if we find a worker (which we should) add it to the scout units
//			if (workerScout)
//			{
//				Global::Scout().setWorkerScout(workerScout);
//				assignUnit(workerScout, m_scoutUnits);
//				m_initialScoutSet = true;
//			}
//		}
//	}
//}

// sets combat units to be passed to CombatCommander
void GameOverlord::setCombatUnits()
{
	for (auto& unit : m_validUnits)
	{
		if (!isAssigned(unit) /*&& UnitState::IsCombatUnit(unit)*/)
		{
			for (auto& unitSet : m_unitTypesList)
			{
				//for (auto& tmp : unitSet)
				BWAPI::Unit tmp = *unitSet.end();
				if (tmp->getType() == unit->getType())
				{
					//unitSet.insert(unit);
					assignUnit(unit, unitSet);
				}
				else
				{
					BWAPI::Unitset uSet;
					assignUnit(unit, m_combatUnits);
					m_unitTypesList.push_back(uSet);
					//*m_unitTypesList.insert(unit, unitSet);
				}
			}
		}
	}
}

//BWAPI::Unit GameOverlord::getFirstSupplyProvider()
//{
//	BWAPI::Unit supplyProvider = nullptr;
//
//	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
//	{
//		for (auto& unit : BWAPI::Broodwar->self()->getUnits())
//		{
//			if (unit->getType() == BWAPI::UnitTypes::Zerg_Spawning_Pool)
//			{
//				supplyProvider = unit;
//			}
//		}
//	}
//	else
//	{
//
//		for (auto& unit : BWAPI::Broodwar->self()->getUnits())
//		{
//			if (unit->getType() == BWAPI::Broodwar->self()->getRace().getSupplyProvider())
//			{
//				supplyProvider = unit;
//			}
//		}
//	}
//
//	return supplyProvider;
//}

//void GameOverlord::onUnitShow(BWAPI::Unit unit)
//{
//	Global::Info().onUnitShow(unit);
//	Global::Workers().onUnitShow(unit);
//}
//
//void GameOverlord::onUnitHide(BWAPI::Unit unit)
//{
//	Global::Info().onUnitHide(unit);
//}
//
//void GameOverlord::onUnitCreate(BWAPI::Unit unit)
//{
//	Global::Info().onUnitCreate(unit);
//}
//
//void GameOverlord::onUnitComplete(BWAPI::Unit unit)
//{
//	Global::Info().onUnitComplete(unit);
//}
//
//void GameOverlord::onUnitRenegade(BWAPI::Unit unit)
//{
//	Global::Info().onUnitRenegade(unit);
//}
//
void GameOverlord::onUnitDestroy(BWAPI::Unit unit)
{
	for (auto& unitSet : m_unitTypesList)
	{
		for (auto& tmp : unitSet)
		{
			if (tmp == unit)
			{
				unitSet.erase(unit);
			}
		}
	}

	if (unit->getType().isWorker())
	{
		m_commandUnits.erase(unit);
	}
	//	Global::Production().onUnitDestroy(unit);
	//	Global::Workers().onUnitDestroy(unit);
	//	Global::Info().onUnitDestroy(unit);
}
//
//void GameOverlord::onUnitMorph(BWAPI::Unit unit)
//{
//	Global::Info().onUnitMorph(unit);
//	Global::Workers().onUnitMorph(unit);
//}

BWAPI::Unit GameOverlord::getClosestUnitToTarget(BWAPI::UnitType type, BWAPI::Position target)
{
	BWAPI::Unit closestUnit = nullptr;
	double closestDist = 100000;

	for (auto& unit : m_validUnits)
	{
		if (unit->getType() == type)
		{
			double dist = unit->getDistance(target);
			if (!closestUnit || dist < closestDist)
			{
				closestUnit = unit;
				closestDist = dist;
			}
		}
	}

	return closestUnit;
}

BWAPI::Unit GameOverlord::getClosestWorkerToTarget(BWAPI::Position target)
{
	BWAPI::Unit closestUnit = nullptr;
	double closestDist = 100000;

	for (auto& unit : m_validUnits)
	{
		if (!isAssigned(unit) && unit->getType().isWorker() /*&& Global::Workers().isFree(unit)*/)
		{
			double dist = unit->getDistance(target);
			if (!closestUnit || dist < closestDist)
			{
				closestUnit = unit;
				closestDist = dist;
			}
		}
	}

	return closestUnit;
}

void GameOverlord::assignUnit(BWAPI::Unit unit, BWAPI::Unitset& set)
{
	if (m_scoutUnits.contains(unit)) { m_scoutUnits.erase(unit); }
	else if (m_combatUnits.contains(unit)) { m_combatUnits.erase(unit); }

	set.insert(unit);
}