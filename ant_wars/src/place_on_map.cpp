

#include "place_on_map.h"

using namespace std;

CRoute::CRoute(char owner, string idRoute) : m_idRoute(idRoute), m_owner(owner){}



CPlace::CPlace(int x, int y) : m_xUpDown(x), m_yLeftRight(y){}

void CPlace::setType(char ch){}

void CRegularPlace::setType(char ch)
{
    m_terrain = ch;
}

void CAntHouse::setType(char ch)
{}

int CPlace::xCoordinates() const
{return m_xUpDown;}

int CPlace::yCoordinates() const
{return m_yLeftRight;}

uint CPlace::getNumberOfAnts()
{
    return 0;
}

void CPlace::spawnRegularAnt()
{}

bool CPlace::promoteOccupant(char rank)
{
    return true;
}

char CRegularPlace::getType() const
{ return m_terrain; }

char CAntHouse::getType() const
{ return m_player; }

char CRegularPlace::getId() const
{ return m_terrain; }

char CAntHouse::getId() const
{ return m_houseId; }

char CRegularPlace::getOriginalSquare() const
{ return m_terrain; }

char CAntHouse::getOriginalSquare() const
{ return m_originalSquare; }


CRegularPlace::CRegularPlace(int x, int y, char terrain) : CPlace(x,y), m_terrain(terrain){}

void CRegularPlace::upgradeUnits(shared_ptr<CAbstractBonus> ptr, char owner)
{
    if(m_occupant != nullptr && m_occupant->getOwner() == owner)
        ptr->aplicateUpgrade(m_occupant);
}

void CRegularPlace::killOccupant()
{
    m_occupant = nullptr;
}

shared_ptr<CAbstractWarrior> CRegularPlace::getOccupant(int pos) const
{
    return m_occupant;
}

bool CRegularPlace::setOccupant(shared_ptr<CAbstractWarrior> occupant)
{
    m_occupant = occupant;
    return true;
}



bool CRegularPlace::isHouse() const
{
    return false;
}

CAntHouse::CAntHouse(int x, int y, char playerId, char houseId, int numberOfAnts, int radius, int health, int attack)
: CPlace(x,y), m_radius(radius), m_health(health), m_attack(attack), m_houseId(houseId),m_player(playerId), m_originalSquare((char)(numberOfAnts/10 + 48))
{
    for(int i = 0; i < numberOfAnts; i++)
    {
        if(m_homeArmy.size() < 99)
            m_homeArmy.push_back(make_shared<CRegularAnt>(m_radius, m_health, m_attack, m_player));
    }
}

void CAntHouse::spawnRegularAnt()
{
    if(m_homeArmy.size() < 99)
        m_homeArmy.push_back(make_shared<CRegularAnt>(m_radius, m_health, m_attack, m_player));
}

uint CAntHouse::getNumberOfAnts()
{
    return m_homeArmy.size();
}

void CAntHouse::upgradeUnits(shared_ptr<CAbstractBonus> ptr, char owner)
{
    if(m_player == owner)
    {
        for (auto &ant: m_homeArmy)
            ptr->aplicateUpgrade(ant);
    }
}

void CAntHouse::killOccupant()
{
    if(!m_homeArmy.empty())
        m_homeArmy.erase(m_homeArmy.begin());
}

shared_ptr<CAbstractWarrior> CAntHouse::getOccupant(int pos) const
{
    if((int)m_homeArmy.size() <= pos)
        return nullptr;
    return m_homeArmy[pos];
}

bool CAntHouse::setOccupant(shared_ptr<CAbstractWarrior> occupant)
{

    if(m_homeArmy.empty())
        m_homeArmy.push_back(occupant);

    m_player = m_homeArmy[0]->getOwner();
    return false;
}

bool CAntHouse::promoteOccupant(char rank)
{
    for(auto & i : m_homeArmy)
    {
        if(i->promoteAnt(rank) != nullptr)
        {
            i = i->promoteAnt(rank);
            return true;
        }

    }

    return false;
}

bool CAntHouse::isHouse() const
{
    return true;
}


void CRoute::clearRoute()
{
    for(auto & i : m_route)
    {
        i->setType(' ');
    }

    for(uint i = 1; i < m_route.size() - 1; i++)
        m_route[i]->killOccupant();

}

void CRoute::addPlace(shared_ptr<CPlace> place)
{
    m_route.push_back(place);
}

bool CRoute::isOccupied()
{
    for(int i = 1; i < (int)m_route.size()-1; i++)
    {
        if(m_route[i]->getOccupant() != nullptr)
            return true;
    }

    return false;
}

char CRoute::getOwner() const
{
    return m_owner;
}

bool CRoute::getAvailability() const
{
    return m_available;
}

void CRoute::setAvailability(bool availability)
{
    m_available = availability;
}

int CRoute::moveViaRoutes(int &first)
{
    int returnValue = 0;
    for(int i = 0; i < (int)(m_route.size())-2; i++)
    {
        if(m_route[i]->getOccupant() == nullptr && m_route[i + 1]->getOccupant() != nullptr)
        { //move
            int tmp = moveAntOneStep(i);
            returnValue = max(returnValue, tmp);
            if(tmp == 2)
                first = i;
        }
    }
    if((m_route[m_route.size()-2]->getOccupant() == nullptr && m_route[(m_route.size()) - 1]->getOccupant() != nullptr)
       && getAvailability()
       && getOwner() == m_route[(m_route.size()) - 1]->getType())
    { //move
        int tmp = moveAntOneStep((int) (m_route.size() - 2));
        returnValue = max(returnValue, tmp);
        if(tmp == 2)
            first = (int) (m_route.size() - 2);

    }
    return returnValue;
}


int CRoute::moveAntOneStep(const int &first)
{
    int returnVal = 0;


    if(!m_route[first]->setOccupant(m_route[first + 1]->getOccupant()) && first == 0)
    {
        returnVal = 2;
    }
    m_route[first + 1]->killOccupant();
    return returnVal;
}

void CRoute::fightOnTheSquareItself(shared_ptr<CAbstractWarrior> first,
                               shared_ptr<CAbstractWarrior> second, int firstIndex)
{
    first->getHit(second->getAttack());
    second->getHit(first->getAttack());
    if(first->printName() == 'b' && !first->isAlive())
    {
        for (int i = firstIndex + 1; i < (int) (m_route.size()) - 1 && i - firstIndex <= first->getRange(); i++)
            m_route[i]->killOccupant();
    }

    if(second != nullptr && second->printName() == 'b' && !second->isAlive())
    {
        int i = firstIndex;
        int range = second->getRange();
        while(range > 0)
        {
            m_route[i]->killOccupant();
            if(i > 0)
                i--;

            range--;
        }
    }

    if(!first->isAlive())
        m_route[firstIndex]->killOccupant();

    if(!second->isAlive())
        m_route[firstIndex + 1]->killOccupant();
}

void CRoute::fightAnts(int first, int second)
{
    //can be made faster, regular ant can return radius too but need to upgrade it as well
    int firstTmp = first;
    int secondTmp = second;
    int rowInAntHouse = 0;

    while(true)
    {
        if(firstTmp > 1)
        {
            firstTmp--;
            if(!empowerAntPreFight(m_route[first]->getOccupant(),
                                   m_route[firstTmp]->getOccupant(),
                                   abs(first - firstTmp)))
                break;
        }
        else
        {
            if((int)(m_route[0]->getNumberOfAnts())-1 < rowInAntHouse)
            {
                break;
            }

            if(!empowerAntPreFight(m_route[first]->getOccupant(),
                                   m_route[0]->getOccupant(rowInAntHouse),
                                   abs(first - firstTmp) + rowInAntHouse + 1))
                break;

            rowInAntHouse++;
        }
    }

    rowInAntHouse = 0;
    while(true)
    {
        if(secondTmp < (int)(m_route.size()) -2)
        {
            secondTmp++;
            if(!empowerAntPreFight(m_route[second]->getOccupant(),
                                   m_route[secondTmp]->getOccupant(),
                                   abs(second - secondTmp)))
                break;
        }
        else
        {
            if((int)(m_route[0]->getNumberOfAnts())-1 < rowInAntHouse)
            {
                break;
            }

            if(!empowerAntPreFight(m_route[second]->getOccupant(),
                                   m_route[0]->getOccupant(rowInAntHouse),
                                   abs(second - secondTmp) + rowInAntHouse + 1))
                break;

            rowInAntHouse++;
        }
    }

    CRoute::fightOnTheSquareItself(m_route[first]->getOccupant(), m_route[second]->getOccupant(), first);
}


/**
 * @brief If second ant isn't regular then it improves first ant
 * @param first
 * @param second
 * @param distance
 * @return false if distance is too big for second ant to reach
 */
bool CRoute::empowerAntPreFight(shared_ptr<CAbstractWarrior> first,
                                      shared_ptr<CAbstractWarrior> second, int distance)
{
    if(first == nullptr || second == nullptr)
        return false;
    return second->empowers(first, distance);
}