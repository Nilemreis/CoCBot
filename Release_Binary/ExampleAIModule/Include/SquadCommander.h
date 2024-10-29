#pragma once

#include "Includes.h"
//#include "MeleeManager.h"
#include "RangedManager.h"
//#include "DetectorManager.h"
//#include "TransportManager.h"
#include "SquadOrders.h"
//#include "TankManager.h"
//#include "MedicManager.h"

namespace CoCBot
{

    class SquadCommander
    {
        BWAPI::Unitset      m_units;
        std::string         m_name = "Default";
        std::string         m_regroupStatus = "Default";
        int                 m_lastRetreatSwitch = 0;
        bool                m_lastRetreatSwitchVal = false;
        size_t              m_priority = 0;
		BWAPI::Unit			m_commander;
		bool			    m_squadFighting;

        SquadOrder          m_order;
        SquadOrder          m_tmpOrder;
        //MeleeManager        m_meleeManager;
        RangedManager       m_rangedManager;
        //DetectorManager     m_detectorManager;
        //TransportManager    m_transportManager;
        //TankManager         m_tankManager;
        //MedicManager        m_medicManager;

        std::map<BWAPI::Unit, bool>	m_nearEnemy;

        BWAPI::Unit unitClosestToEnemy();

        void updateUnits();
        void addUnitsToMicroManagers();
        void setNearEnemyUnits();
        void setAllUnits();
        

        bool unitNearEnemy(BWAPI::Unit unit);
        bool needsToRegroup();
        bool checkFormation();
        int  squadUnitsNear(BWAPI::Position p);

    public:

        SquadCommander(const std::string& name, SquadOrder order, size_t priority, BWAPI::Unit commander);
        SquadCommander();
        ~SquadCommander();

        void update();
        void setSquadOrder(const SquadOrder& so);
        void addUnit(BWAPI::Unit u);
        void removeUnit(BWAPI::Unit u);
        bool containsUnit(BWAPI::Unit u) const;
        bool isEmpty() const;
        void clear();
        size_t getPriority() const;
        void setPriority(const size_t& priority);
        const std::string& getName() const;

        BWAPI::Position calcCenter();
        BWAPI::Position calcRegroupPosition();

        const BWAPI::Unitset& getUnits() const;
        const SquadOrder& getSquadOrder()	const;
        bool checkCompleted();
    };
}