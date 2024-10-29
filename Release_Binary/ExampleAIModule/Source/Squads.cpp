#include "include/Squads.h"
//#include "Global.h"
//#include "WorkerManager.h"

using namespace CoCBot;

Squads::Squads()
{

}

void Squads::update()
{
    updateAllSquads();
    verifySquadUniqueMembership();
}

void Squads::clearSquadData()
{
    // give back workers who were in squads
    for (auto& kv : m_squads)
    {
        SquadCommander & squad = kv.second;

        const BWAPI::Unitset& units = squad.getUnits();

        for (auto& unit : units)
        {
            if (unit->getType().isWorker())
            {
                //Global::Workers().finishedWithWorker(unit);
            }
        }
    }

    m_squads.clear();
}

void Squads::removeSquad(const std::string& squadName)
{
    auto& squadPtr = m_squads.find(squadName);

    //UAB_ASSERT_WARNING(squadPtr != m_squads.end(), "Trying to clear a squad that didn't exist: %s", squadName.c_str());
    if (squadPtr == m_squads.end())
    {
        return;
    }

    for (auto& unit : squadPtr->second.getUnits())
    {
        if (unit->getType().isWorker())
        {
            //Global::Workers().finishedWithWorker(unit);
        }
    }

    m_squads.erase(squadName);
}

std::map<std::string, SquadCommander>& Squads::getSquads()
{
    return m_squads;
}

bool Squads::squadExists(const std::string& squadName)
{
    return m_squads.find(squadName) != m_squads.end();
}

void Squads::addSquad(const std::string& squadName, const SquadCommander& squad)
{
    m_squads[squadName] = squad;
}

void Squads::updateAllSquads()
{
    for (auto& kv : m_squads)
    {
        kv.second.update();
    }
}

void Squads::drawSquadInformation(int x, int y)
{
    //if (!Config::Debug::DrawSquadInfo)
    //{
    //    return;
    //}

    BWAPI::Broodwar->drawTextScreen(x, y, "\x04Squads");
    BWAPI::Broodwar->drawTextScreen(x, y + 20, "\x04NAME");
    BWAPI::Broodwar->drawTextScreen(x + 150, y + 20, "\x04SIZE");
    BWAPI::Broodwar->drawTextScreen(x + 200, y + 20, "\x04LOCATION");

    int yspace = 0;

    for (auto& kv : m_squads)
    {
        const SquadCommander& squad = kv.second;

        const BWAPI::Unitset& units = squad.getUnits();
        const SquadOrder& order = squad.getSquadOrder();

        BWAPI::Broodwar->drawTextScreen(x, y + 40 + ((yspace) * 10), "\x03%s", squad.getName().c_str());
        BWAPI::Broodwar->drawTextScreen(x + 150, y + 40 + ((yspace) * 10), "\x03%d", units.size());
        BWAPI::Broodwar->drawTextScreen(x + 200, y + 40 + ((yspace++) * 10), "\x03(%d,%d)", order.getPosition().x, order.getPosition().y);

        BWAPI::Broodwar->drawCircleMap(order.getPosition(), 10, BWAPI::Colors::Green, true);
        BWAPI::Broodwar->drawCircleMap(order.getPosition(), order.getRadius(), BWAPI::Colors::Red, false);
        BWAPI::Broodwar->drawTextMap(order.getPosition() + BWAPI::Position(0, 12), "%s", squad.getName().c_str());

        for (const BWAPI::Unit unit : units)
        {
            BWAPI::Broodwar->drawTextMap(unit->getPosition() + BWAPI::Position(0, 10), "%s", squad.getName().c_str());
        }
    }
}

void Squads::verifySquadUniqueMembership()
{
    BWAPI::Unitset assigned;

    for (const auto& kv : m_squads)
    {
        for (auto& unit : kv.second.getUnits())
        {
            if (assigned.contains(unit))
            {
                BWAPI::Broodwar->printf("Unit is in at least two squads: %s", unit->getType().getName().c_str());
            }

            assigned.insert(unit);
        }
    }
}

bool Squads::unitIsInSquad(BWAPI::Unit unit) const
{
    return getUnitSquad(unit) != nullptr;
}

const SquadCommander* Squads::getUnitSquad(BWAPI::Unit unit) const
{
    for (const auto& kv : m_squads)
    {
        if (kv.second.getUnits().contains(unit))
        {
            return &kv.second;
        }
    }

    return nullptr;
}

SquadCommander* Squads::getUnitSquad(BWAPI::Unit unit)
{
    for (auto& kv : m_squads)
    {
        if (kv.second.getUnits().contains(unit))
        {
            return &kv.second;
        }
    }

    return nullptr;
}

void Squads::assignUnitToSquad(BWAPI::Unit unit, SquadCommander& squad)
{
    //UAB_ASSERT_WARNING(canAssignUnitToSquad(unit, squad), "We shouldn't be re-assigning this unit!");

    SquadCommander* previousSquad = getUnitSquad(unit);

    if (previousSquad)
    {
        previousSquad->removeUnit(unit);
    }

    squad.addUnit(unit);
}

bool Squads::canAssignUnitToSquad(BWAPI::Unit unit, const SquadCommander& squad) const
{
    const SquadCommander* unitSquad = getUnitSquad(unit);

    // make sure strictly less than so we don't reassign to the same squad etc
    return !unitSquad || (unitSquad->getPriority() < squad.getPriority());
}

SquadCommander& Squads::getSquad(const std::string& squadName)
{
    UAB_ASSERT_WARNING(squadExists(squadName), "Trying to access squad that doesn't exist: %s", squadName);

    return m_squads[squadName];
}