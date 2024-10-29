#pragma once

#include "Includes.h"
#include "SquadCommander.h"

namespace CoCBot
{

    class Squads
    {
        std::map<std::string, SquadCommander> m_squads;

        void updateAllSquads();
        void verifySquadUniqueMembership();

    public:

        Squads();

        void update();
        void clearSquadData();
        void assignUnitToSquad(BWAPI::Unit unit, SquadCommander& squad);
        void addSquad(const std::string& squadName, const SquadCommander& squad);
        void removeSquad(const std::string& squadName);
        void drawSquadInformation(int x, int y);

        bool canAssignUnitToSquad(BWAPI::Unit unit, const SquadCommander& squad) const;
        bool squadExists(const std::string& squadName);
        bool unitIsInSquad(BWAPI::Unit unit) const;
        const SquadCommander* getUnitSquad(BWAPI::Unit unit) const;
        SquadCommander* getUnitSquad(BWAPI::Unit unit);
        SquadCommander& getSquad(const std::string& squadName);
        std::map<std::string, SquadCommander>& getSquads();
    };
}