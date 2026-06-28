/**
 * @file warrior.h
 * @brief This file manages all ant warriors and their options
 */


#pragma once
#include <vector>
#include <iostream>
#include <memory>


class CRegularAnt;

/**
 * @brief Abstract class for managing each ant
 */
class CAbstractWarrior
{
public:

    CAbstractWarrior(int radius, int maxHp, int baseAttack, char owner);

    CAbstractWarrior(int radius, int maxHp, int currentHp, int baseAttack, int currentAttack, char owner);

    virtual ~CAbstractWarrior() = default;

    bool isAlive() const;

    /**
     * @brief Promotes current ant to ant with rank specified as input
     * @param promotion specifies type of promotion
     * @return new promoted ant
     */
    virtual std::shared_ptr<CAbstractWarrior> promoteAnt(char promotion);

    /**
     * @brief Subtracts hit from ants current hp
     * @param hit
     * @return false if ant is dead
     */
    bool getHit(int hit);

    char getOwner() const;

    virtual int getMaxHp() const;

    virtual int getCurrentHp() const;

    virtual int getAttack() const;

    virtual int getBaseAttack() const;

    virtual void setAttack(int attack);

    virtual void setCurrentHp(int hp);

    virtual void setBaseAttack(int attack);

    virtual void setMaxHp(int hp);

    /**
     * @brief Adds range to current range of ant
     * @param range
     */
    virtual void improveRange(int range) = 0;

    virtual char printName() = 0;

    /**
     * @brief empowers stat of input ant by capabilities of this ant
     * @param ant
     * @param distance
     * @return false if ant is out of range
     */
    virtual bool empowers(std::shared_ptr<CAbstractWarrior> ant, int distance);

    virtual int getRange();

    /**
     * @return r if ant is regular, c if ant is captain, b if ant is bomber, m if ant is medic
     */
    virtual bool isPromoted() const = 0;

protected:
    int m_radius;
    int m_maxHp;
    int m_currentHp;
    int m_baseAttack;
    int m_attack;
    char m_owner;
};

/**
 * @brief This class represents an ant which explodes on death
 */
class CBomberAnt : public CAbstractWarrior
{
public:
    CBomberAnt(int maxHp, int baseAttack, char owner, int radius);

    CBomberAnt(CRegularAnt &ant, char owner, int radius);

    void improveRange(int range) override;

    bool isPromoted() const override;

    int getRange() override;

    char printName() override;


private:

};

/**
 * @brief This ant warrior enhances attack power of friendly ants in range in front of him
 */
class CCaptainAnt : public CAbstractWarrior
{
public:
    CCaptainAnt(int maxHp, int baseAttack, int empowerment, char owner, int radius);

    CCaptainAnt(CRegularAnt &ant, int empowerment, char owner, int radius);

    void improveRange(int range) override;

    bool isPromoted() const override;

    int getRange() override;

    char printName() override;

    bool empowers(std::shared_ptr<CAbstractWarrior> ant, int distance) override;

private:
    int m_empowerment;

};

/**
 * @brief This ant heals bruised friendly ant in range in front of him
 */
class CMedicAnt : public CAbstractWarrior
{
public:
    CMedicAnt(int maxHp, int baseAttack, int healPerTimeUnit, char owner, int radius);

    CMedicAnt(CRegularAnt &ant, int healPerTimeUnit, char owner, int radius);

    void improveRange(int range) override;

    bool isPromoted() const override;

    int getRange() override;

    char printName() override;

    bool empowers(std::shared_ptr<CAbstractWarrior> ant, int distance) override;

private:
    int m_healPerTimeUnit;

};

/**
 * @brief This is basic ant which spawns by default in each anthouse and can be promoted to other ants
 */
class CRegularAnt : public CAbstractWarrior
{
public:
    CRegularAnt(int radius, int maxHp, int baseAttack, char owner);

    void improveRange(int range) override;

    std::shared_ptr<CAbstractWarrior> promoteAnt(char promotion) override;

    bool isPromoted() const override;

    char printName() override;
};
