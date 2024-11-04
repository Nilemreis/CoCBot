#pragma once
#include "Global.h"
#include "Includes.h"
#include "Squads.h"
#include "MapTools.h"
#include "InformationManager.h"
#include "BaseLocationManager.h"
#include "UnitData.h"
#include "UnitState.h"


namespace CoCBot
{
	class CombatOverlord
	{
		BWAPI::Unit     m_mainCommander;
		Squads          m_squads;
		BWAPI::Unitset  m_combatUnits;
		BWAPI::Unitset  m_commandUnits;
		bool            m_initialized = false;
		std::list<BWAPI::Unitset> m_unitTypesList;
		std::vector<SquadOrder> m_ordersList;

		void updateDefenseSquads();
		void updateAttackSquads();
		void updateIdleSquad();
		void initializeSquads();
		void assignOrders();
		void emergencyReaction();
		//void updateDefenseSquadUnits(SquadCommander& defenseSquad, const size_t& flyingDefendersNeeded, const size_t& groundDefendersNeeded);

		int  defendWithWorkers();
		//int  numZerglingsInOurBase();
		//bool bekingBuildingRushed();
		bool isSquadUpdateFrame();

		//BWAPI::Unit     findClosestDefender(const SquadCommander& defenseSquad, BWAPI::Position pos, bool flyingDefender);
		BWAPI::Unit     findClosestWorkerToTarget(BWAPI::Unitset& unitsToAssign, BWAPI::Unit target);
		BWAPI::Position getDefendLocation();
		BWAPI::Position getMainAttackLocation();
		BWAPI::Position getSecondaryAttackLocation();
		BWAPI::Position getThirdAttackLocation();

	public:

		CombatOverlord();

		void update(const std::list<BWAPI::Unitset>& combatUnits, const BWAPI::Unitset& commandUnits);

		void drawSquadInformation(int x, int y);
	};

}

