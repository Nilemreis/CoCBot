#pragma once

#include "Includes.h"
#include "MicroManager.h"

namespace CoCBot
{
	class RangedManager : public CoCBot::MicroManager
	{
	public:

		RangedManager();
		void executeMicro(const BWAPI::Unitset& targets);
		void assignTargetsNew(const BWAPI::Unitset& targets);
		void assignTargetsOld(const BWAPI::Unitset& targets);

		int getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target);
		BWAPI::Unit closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit>& rangedUnitsToAssign);
		BWAPI::Unit getTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset& targets);
		std::pair<BWAPI::Unit, BWAPI::Unit> findClosestUnitPair(const BWAPI::Unitset& attackers, const BWAPI::Unitset& targets);
	};
}