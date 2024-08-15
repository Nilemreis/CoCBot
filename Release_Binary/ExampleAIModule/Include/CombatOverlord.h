#pragma once
#include "Includes.h"
#include "Squads.h"
#include "UnitState.h"

namespace CoCBot
{
	class CombatOverlord
	{
		Squads          m_squads;
		BWAPI::Unitset  m_combatUnits;
		BWAPI::Unitset  m_commandUnits;
		bool            m_initialized = false;
		std::list<BWAPI::Unitset> m_unitTypesList;


		void updateDefenseSquads();
		void updateAttackSquads();
		void updateIdleSquad();
		void initializeSquads();
		//void updateDefenseSquadUnits(SquadCommander& defenseSquad, const size_t& flyingDefendersNeeded, const size_t& groundDefendersNeeded);

		int  defendWithWorkers();
		//int  numZerglingsInOurBase();
		//bool beingBuildingRushed();
		bool isSquadUpdateFrame();

		//BWAPI::Unit     findClosestDefender(const SquadCommander& defenseSquad, BWAPI::Position pos, bool flyingDefender);
		BWAPI::Unit     findClosestWorkerToTarget(BWAPI::Unitset& unitsToAssign, BWAPI::Unit target);
		BWAPI::Position getDefendLocation();
		BWAPI::Position getMainAttackLocation();

	public:

		CombatOverlord();

		void update(const std::list<BWAPI::Unitset>& combatUnits, const BWAPI::Unitset& commandUnits);

		void drawSquadInformation(int x, int y);
	};

}

