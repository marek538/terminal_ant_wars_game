

#include "race_bonus_tree.h"

using namespace std;

bool CUpgradePanel::validateUpgrade(shared_ptr<CAbstractBonus> bonus)
{
    return bonus->checkValidity(m_progressInAttack, m_progressInHealth);
}

void CUpgradePanel::upgradeAnt(shared_ptr<CAbstractBonus> bonus, shared_ptr<CAbstractWarrior> warrior)
{
    bonus->aplicateUpgrade(warrior);
}

CUpgradePanel::CUpgradePanel(int attack, int hp, int range)
{
    bonuses.push_back(make_shared<CAttackBonus>(attack));
    bonuses.push_back(make_shared<CHpBonus>(hp));
    bonuses.push_back(make_shared<CRangeBonus>(range));
}

CAttackBonus::CAttackBonus(int attack) : m_attackBonus(attack){}

CHpBonus::CHpBonus(int hp) : m_hpBonus(hp){}

CRangeBonus::CRangeBonus(int range) : m_rangeBonus(range){}

void CAttackBonus::aplicateUpgrade(shared_ptr<CAbstractWarrior> warrior)
{
    warrior->setAttack(warrior->getAttack() + m_attackBonus);
    warrior->setBaseAttack(warrior->getBaseAttack() + m_attackBonus);
}

bool CAttackBonus::checkValidity(int &attackProgress, int &healthProgress)
{
    attackProgress++;
    return true;
}

std::string CAttackBonus::printUserOption() const
{
    return "up-a -upgrade attack";
}

void CHpBonus::aplicateUpgrade(shared_ptr<CAbstractWarrior> warrior)
{
    warrior->setMaxHp(warrior->getMaxHp() + m_hpBonus);
    warrior->setCurrentHp(warrior->getCurrentHp() + m_hpBonus);
}

bool CHpBonus::checkValidity(int &attackProgress, int &healthProgress)
{
    healthProgress++;
    return true;
}

std::string CHpBonus::printUserOption() const
{
    return "up-h -upgrade health";
}

void CRangeBonus::aplicateUpgrade(shared_ptr<CAbstractWarrior> warrior)
{
    warrior->improveRange(m_rangeBonus);
}

bool CRangeBonus::checkValidity(int &attackProgress, int &healthProgress)
{
    if(attackProgress < 3 || healthProgress < 3)
        return false;

    return true;
}

std::string CRangeBonus::printUserOption() const
{
    return "up-r -upgrade range";
}

