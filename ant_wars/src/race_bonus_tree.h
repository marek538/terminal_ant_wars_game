/**
 * @file race_bonus_tree.h
 * @brief This file manages all ant-stats bonuses available to certain player
 */

#pragma once
#include "warrior.h"
#include <memory>


class CAbstractBonus;

/**
 * @brief Manages all upgrades available to each player
 */
class CUpgradePanel
{
public:
    std::vector<std::shared_ptr<CAbstractBonus>> bonuses;

    CUpgradePanel(int attack, int hp, int range);

    /**
     * @brief Apply bonus to warrior
     * @param bonus
     * @param warrior
     */
    void upgradeAnt(std::shared_ptr<CAbstractBonus> bonus, std::shared_ptr<CAbstractWarrior> warrior);

    /**
     * @brief Checks if bonus is available and makes progress on bonus tree
     * @param bonus
     */
    bool validateUpgrade(std::shared_ptr<CAbstractBonus> bonus);


private:
    //can start upgrading range after hp and attack are both 3+
    int m_progressInAttack = 0;
    int m_progressInHealth = 0;

};

/**
 * @brief Abstract class for buffing each individual ant
 */
class CAbstractBonus
{
public:
    virtual bool checkValidity(int &attackProgress, int &healthProgress) = 0;

    virtual void aplicateUpgrade(std::shared_ptr<CAbstractWarrior> warrior) = 0;

    virtual ~CAbstractBonus()= default;

    virtual std::string printUserOption() const = 0;
};


/**
 * @brief Bonus that enhances ants attack number
 */
class CAttackBonus : public CAbstractBonus
{
public:
    explicit CAttackBonus(int attack);

    bool checkValidity(int &attackProgress, int &healthProgress) override;

    void aplicateUpgrade(std::shared_ptr<CAbstractWarrior> warrior) override;

    std::string printUserOption() const override;
private:
    int m_attackBonus;
};

/**
 * @brief Bonus that enhances ants health number
 */
class CHpBonus : public CAbstractBonus
{
public:
    bool checkValidity(int &attackProgress, int &healthProgress) override;

    explicit CHpBonus(int hp);

    void aplicateUpgrade(std::shared_ptr<CAbstractWarrior> warrior) override;

    std::string printUserOption() const override;
private:
    int m_hpBonus;
};

/**
 * @brief Bonus used for support ants such as medic and captain and this bonus enhances their range
 */
class CRangeBonus : public CAbstractBonus
{
public:
    bool checkValidity(int &attackProgress, int &healthProgress) override;

    explicit CRangeBonus(int range);

    void aplicateUpgrade(std::shared_ptr<CAbstractWarrior> warrior) override;

    std::string printUserOption() const override;
private:
    int m_rangeBonus;
};
