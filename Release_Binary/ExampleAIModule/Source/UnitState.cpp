#include "Include/UnitState.h"


namespace CoCBot::UnitState
{
    bool IsCombatUnit(BWAPI::Unit unit)
    {
        //UAB_ASSERT(unit != nullptr, "Unit was null");
        if (!unit)
        {
            return false;
        }

        // no workers or buildings allowed
        if (unit && unit->getType().isWorker() || unit->getType().isBuilding())
        {
            return false;
        }

        // check for various types of combat units
        if (unit->getType().canAttack() ||
            unit->getType() == BWAPI::UnitTypes::Terran_Medic ||
            unit->getType() == BWAPI::UnitTypes::Protoss_High_Templar ||
            unit->getType() == BWAPI::UnitTypes::Protoss_Observer ||
            unit->isFlying() && unit->getType().spaceProvided() > 0)
        {
            return true;
        }

        return false;
    }

    bool IsValidUnit(BWAPI::Unit unit)
    {
        if (!unit)
        {
            return false;
        }

        if (unit->isCompleted()
            && unit->getHitPoints() > 0
            && unit->exists()
            && unit->getType() != BWAPI::UnitTypes::Unknown
            && unit->getPosition().x != BWAPI::Positions::Unknown.x
            && unit->getPosition().y != BWAPI::Positions::Unknown.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }


    CoCBot::Rect GetRect(BWAPI::Unit unit)
    {
        CoCBot::Rect r;

        r.x = unit->getLeft();
        r.y = unit->getTop();
        r.height = unit->getBottom() - unit->getTop();
        r.width = unit->getLeft() - unit->getRight();

        return r;
    }

    double GetDistanceBetweenTwoRectangles(CoCBot::Rect& rect1, CoCBot::Rect& rect2)
    {
        CoCBot::Rect& mostLeft = rect1.x < rect2.x ? rect1 : rect2;
        CoCBot::Rect& mostRight = rect2.x < rect1.x ? rect1 : rect2;
        CoCBot::Rect& upper = rect1.y < rect2.y ? rect1 : rect2;
        CoCBot::Rect& lower = rect2.y < rect1.y ? rect1 : rect2;

        int diffX = std::max(0, mostLeft.x == mostRight.x ? 0 : mostRight.x - (mostLeft.x + mostLeft.width));
        int diffY = std::max(0, upper.y == lower.y ? 0 : lower.y - (upper.y + upper.height));

        return std::sqrtf(static_cast<float>(diffX * diffX + diffY * diffY));
    }

    bool CanAttack(BWAPI::Unit attacker, BWAPI::Unit target)
    {
        return CoCBot::UnitState::GetWeapon(attacker, target) != BWAPI::WeaponTypes::None;
    }

    bool CanAttackAir(BWAPI::Unit unit)
    {
        return unit->getType().airWeapon() != BWAPI::WeaponTypes::None;
    }

    bool CanAttackGround(BWAPI::Unit unit)
    {
        return unit->getType().groundWeapon() != BWAPI::WeaponTypes::None;
    }

    double CalculateLTD(BWAPI::Unit attacker, BWAPI::Unit target)
    {
        BWAPI::WeaponType weapon = CoCBot::UnitState::GetWeapon(attacker, target);

        if (weapon == BWAPI::WeaponTypes::None)
        {
            return 0;
        }

        return static_cast<double>(weapon.damageAmount()) / weapon.damageCooldown();
    }

    BWAPI::WeaponType GetWeapon(BWAPI::Unit attacker, BWAPI::Unit target)
    {
        return target->isFlying() ? attacker->getType().airWeapon() : attacker->getType().groundWeapon();
    }

    BWAPI::WeaponType GetWeapon(BWAPI::UnitType attacker, BWAPI::UnitType target)
    {
        return target.isFlyer() ? attacker.airWeapon() : attacker.groundWeapon();
    }

    int GetAttackRange(BWAPI::Unit attacker, BWAPI::Unit target)
    {
        BWAPI::WeaponType weapon = CoCBot::UnitState::GetWeapon(attacker, target);

        if (weapon == BWAPI::WeaponTypes::None)
        {
            return 0;
        }

        int range = weapon.maxRange();

        if ((attacker->getType() == BWAPI::UnitTypes::Protoss_Dragoon)
            && (attacker->getPlayer() == BWAPI::Broodwar->self())
            && BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Singularity_Charge))
        {
            range = 6 * 32;
        }

        return range;
    }

    int GetAttackRange(BWAPI::UnitType attacker, BWAPI::UnitType target)
    {
        BWAPI::WeaponType weapon = CoCBot::UnitState::GetWeapon(attacker, target);

        if (weapon == BWAPI::WeaponTypes::None)
        {
            return 0;
        }

        return weapon.maxRange();
    }

    size_t GetAllUnitCount(BWAPI::UnitType type)
    {
        size_t count = 0;
        for (const auto& unit : BWAPI::Broodwar->self()->getUnits())
        {
            // trivial case: unit which exists matches the type
            if (unit->getType() == type)
            {
                count++;
            }

            // case where a zerg egg contains the unit type
            if (unit->getType() == BWAPI::UnitTypes::Zerg_Egg && unit->getBuildType() == type)
            {
                count += type.isTwoUnitsInOneEgg() ? 2 : 1;
            }

            // case where a building has started constructing a unit but it doesn't yet have a unit associated with it
            if (unit->getRemainingTrainTime() > 0)
            {
                BWAPI::UnitType trainType = unit->getLastCommand().getUnitType();

                if (trainType == type && unit->getRemainingTrainTime() == trainType.buildTime())
                {
                    count++;
                }
            }
        }

        return count;
    }


    BWAPI::Unit GetClosestUnitTypeToTarget(BWAPI::UnitType type, BWAPI::Position target)
    {
        BWAPI::Unit closestUnit = nullptr;
        double closestDist = 100000;

        for (auto& unit : BWAPI::Broodwar->self()->getUnits())
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
}
