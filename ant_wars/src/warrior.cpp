#include "warrior.h"

using namespace std;

CAbstractWarrior::CAbstractWarrior(int radius, int maxHp, int baseAttack, char owner)
:m_radius(radius), m_maxHp(maxHp), m_currentHp(maxHp), m_baseAttack(baseAttack), m_attack(baseAttack), m_owner(owner){}

CAbstractWarrior::CAbstractWarrior(int radius,int maxHp, int currentHp, int baseAttack, int currentAttack, char owner)
: m_radius(radius), m_maxHp(maxHp), m_currentHp(currentHp), m_baseAttack(baseAttack), m_attack(currentAttack), m_owner(owner){}

int CAbstractWarrior::getMaxHp() const
{return m_maxHp;}

int CAbstractWarrior::getCurrentHp() const
{return m_currentHp;}

int CAbstractWarrior::getAttack() const
{return m_attack;}

int CAbstractWarrior::getBaseAttack() const
{return m_baseAttack;}

void CAbstractWarrior::setAttack(int attack)
{m_attack = attack;}

void CAbstractWarrior::setCurrentHp(int hp)
{
    m_currentHp = min(hp, m_maxHp);
}

void CAbstractWarrior::setBaseAttack(int attack)
{m_baseAttack = attack;}

void CAbstractWarrior::setMaxHp(int hp)
{m_maxHp = hp;}

char CAbstractWarrior::getOwner() const
{
    return m_owner;
}

bool CAbstractWarrior::isAlive() const
{
    return m_currentHp > 0;
}

bool CAbstractWarrior::getHit(int hit)
{
    m_currentHp -= hit;
    return isAlive();
}

int CAbstractWarrior::getRange()
{
    return 0;
}

std::shared_ptr<CAbstractWarrior> CAbstractWarrior::promoteAnt(char promotion)
{
    return nullptr;
}

bool CAbstractWarrior::empowers(std::shared_ptr<CAbstractWarrior> ant, int distance)
{
    return distance <= m_radius;
}


CBomberAnt::CBomberAnt(int maxHp, int baseAttack, char owner, int radius)
: CAbstractWarrior(radius, maxHp, baseAttack, owner){}

CBomberAnt::CBomberAnt(CRegularAnt &ant, char owner, int radius)
: CAbstractWarrior(radius, ant.getMaxHp(), ant.getCurrentHp(), ant.getBaseAttack(), ant.getAttack(), owner){}

void CBomberAnt::improveRange(int range)
{
    m_radius += range;
}

bool CBomberAnt::isPromoted() const
{
    return true;
}

int CBomberAnt::getRange()
{
    return m_radius;
}

char CBomberAnt::printName()
{
    return 'b';
}

CCaptainAnt::CCaptainAnt(int maxHp, int baseAttack, int empowerment, char owner, int radius)
: CAbstractWarrior(radius, maxHp, baseAttack, owner), m_empowerment(empowerment){}

CCaptainAnt::CCaptainAnt(CRegularAnt& ant, int empowerment, char owner, int radius)
: CAbstractWarrior(radius, ant.getMaxHp(), ant.getCurrentHp(), ant.getBaseAttack(), ant.getAttack(), owner), m_empowerment(empowerment){}

void CCaptainAnt::improveRange(int range)
{
    m_radius += range;
}

bool CCaptainAnt::isPromoted() const
{
    return true;
}

int CCaptainAnt::getRange()
{
    return m_radius;
}

char CCaptainAnt::printName()
{
    return 'c';
}

bool CCaptainAnt::empowers(shared_ptr<CAbstractWarrior> ant, int distance)
{
    if(distance > m_radius)
        return false;

    ant->setAttack(ant->getBaseAttack() + m_empowerment);
    return true;
}

CMedicAnt::CMedicAnt(CRegularAnt& ant, int healPerTimeUnit, char owner, int radius)
: CAbstractWarrior(radius, ant.getMaxHp(), ant.getCurrentHp(), ant.getBaseAttack(), ant.getAttack(), owner), m_healPerTimeUnit(healPerTimeUnit){}

void CMedicAnt::improveRange(int range)
{
    m_radius += range;
}

bool CMedicAnt::isPromoted() const
{
    return true;
}

int CMedicAnt::getRange()
{
    return m_radius;
}

CRegularAnt::CRegularAnt(int radius, int maxHp, int baseAttack, char owner)
 : CAbstractWarrior(radius, maxHp, baseAttack, owner){}

void CRegularAnt::improveRange(int range){}



CMedicAnt::CMedicAnt(int maxHp, int baseAttack, int healPerTimeUnit, char owner, int radius)
: CAbstractWarrior(radius, maxHp, baseAttack, owner), m_healPerTimeUnit(healPerTimeUnit){}

char CMedicAnt::printName()
{
    return 'm';
}

bool CMedicAnt::empowers(shared_ptr<CAbstractWarrior> ant, int distance)
{
    if(distance > m_radius)
        return false;

    ant->setCurrentHp(ant->getCurrentHp() + m_healPerTimeUnit);
    return true;
}


bool CRegularAnt::isPromoted() const
{
    return false;
}

std::shared_ptr<CAbstractWarrior> CRegularAnt::promoteAnt(char promotion)
{
    if(promotion == 'm')
    {
        return make_shared<CMedicAnt>(*this, m_baseAttack/2, m_owner, m_radius);
    }
    else if(promotion == 'c')
    {
        return make_shared<CCaptainAnt>(*this, m_baseAttack/2, m_owner, m_radius);
    }
    else if(promotion == 'b')
    {
        return make_shared<CBomberAnt>(*this, m_owner, m_radius);
    }
    return nullptr;
}

char CRegularAnt::printName()
{
    return 'r';
}
