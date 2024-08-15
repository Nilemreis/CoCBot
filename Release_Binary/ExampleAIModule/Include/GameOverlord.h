#pragma once
#include "Includes.h"
#include "CombatOverlord.h"

namespace CoCBot
{
	class GameOverlord
	{
		CombatOverlord m_combatOverlord;
		//TimerManager    m_timerManager;

		BWAPI::Unitset  m_validUnits;
		BWAPI::Unitset  m_commandUnits;
		BWAPI::Unitset  m_combatUnits;
		BWAPI::Unitset  m_scoutUnits;

		bool            m_initialScoutSet = false;

		void assignUnit(BWAPI::Unit unit, BWAPI::Unitset& set);
		bool isAssigned(BWAPI::Unit unit) const;
	public:
		GameOverlord();
		void update();

		BWAPI::Unitset  m_marines;
		BWAPI::Unitset  m_vultures;
		BWAPI::Unitset  m_goliaths;
		std::list<BWAPI::Unitset> m_unitTypesList;

		void handleUnitAssignments();
		void setValidUnits();
		//void setScoutUnits();
		void setCommandUnits();
		void setCombatUnits();
		void setMarines();
		void setVultures();
		void setGoliaths();


		void drawDebugInterface();
		void drawGameInformation(int x, int y);

		BWAPI::Unit getFirstSupplyProvider();
		BWAPI::Unit getClosestUnitToTarget(BWAPI::UnitType type, BWAPI::Position target);
		BWAPI::Unit getClosestWorkerToTarget(BWAPI::Position target);

		void onUnitShow(BWAPI::Unit unit);
		void onUnitHide(BWAPI::Unit unit);
		void onUnitCreate(BWAPI::Unit unit);
		void onUnitComplete(BWAPI::Unit unit);
		void onUnitRenegade(BWAPI::Unit unit);
		void onUnitDestroy(BWAPI::Unit unit);
		void onUnitMorph(BWAPI::Unit unit);
	};

}

