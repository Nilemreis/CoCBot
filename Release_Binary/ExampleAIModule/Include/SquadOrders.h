#pragma once

#include "Includes.h"

namespace CoCBot
{

    namespace SquadOrderTypes
    {
        enum { None, Idle, Attack, Defend, Regroup, Tighten, Drop, SquadOrderTypes };
    }

    class SquadOrder
    {
        size_t              m_type = SquadOrderTypes::None;
        int                 m_radius = 0;
        BWAPI::Position     m_position;
        std::string         m_status;
        std::string         m_name;

    public:

        SquadOrder()
        {
        }

        SquadOrder(int type, BWAPI::Position position, int radius, std::string status = "Default", std::string name = "")
            : m_type(type)
            , m_position(position)
            , m_radius(radius)
            , m_status(status)
            , m_name(name)
        {
        }

        const std::string& getStatus() const
        {
            return m_status;
        }

        void Assigned()
        {
            m_status = "assigned";
        }

        void Completed()
        {
            m_status = "completed";
        }

        const std::string& getName() const
        {
            return m_name;
        }

        const BWAPI::Position& getPosition() const
        {
            return m_position;
        }

        const int getRadius() const
        {
            return m_radius;
        }

        const size_t getType() const
        {
            return m_type;
        }
    };
}