/**
 * @file place_on_map.h
 * @brief This file manages all objects on map
 */

#pragma once
#include <queue>
#include <list>
#include <memory>
#include "race_bonus_tree.h"

class CPlace;
class CAntHouse;

/**
 * @brief This class manages routes by which ants move from house on position 0 to house on position size-1
 */
class CRoute
{
public:
    //destination is first
    //includes both anthouses on edges
    std::vector<std::shared_ptr<CPlace>> m_route;

    explicit CRoute(char owner, std::string idRoute);

    /**
     * @brief Sets all places back to passagable
     */
    void clearRoute();

    /**
     * @brief Adds place to this route
     * @param place
     */
    void addPlace(std::shared_ptr<CPlace> place);

    /**
     *
     * @return true if route contains at least one ant
     */
    bool isOccupied();

    char getOwner() const;

    bool getAvailability() const;

    void setAvailability(bool availability);

    int moveViaRoutes(int &first);

    /**
     * @brief Updates stats of fighting ants via support ants and starts fightItself
     * @param first
     * @param second
     */
    void fightAnts(int first, int second);

private:
    std::string m_idRoute;
    char m_owner;

    bool m_available = true;

    /**
     * @brief Fights ant - subtracts stats and kills dead ants
     * @param route
     * @param first
     * @param second
     * @param firstIndex location of first ant in current route
     */
    void fightOnTheSquareItself(std::shared_ptr<CAbstractWarrior> first,
                                       std::shared_ptr<CAbstractWarrior> second, int firstIndex);

    /**
    * @brief Moves each ant one step forward and handles if it captures enemy house
    * @param first
    * @param route
    */
    int moveAntOneStep(const int &first);


    bool
    empowerAntPreFight(std::shared_ptr<CAbstractWarrior> first, std::shared_ptr<CAbstractWarrior> second, int distance);
};

/**
 * @brief Abstract class for managing individual places on map
 */
class CPlace
{
public:
    std::tuple<int, int> previousCoordinates;
    bool visited = false;

    CPlace(int x, int y);

    virtual ~CPlace() = default;

    int xCoordinates() const;

    int yCoordinates() const;

    virtual void setType(char ch);

    virtual bool isHouse() const = 0;

    /**
     * @brief Upgrades every ant located on current place
     * @param ptr which upgrade to aplicate
     * @param owner
     */
    virtual void upgradeUnits(std::shared_ptr<CAbstractBonus> ptr, char owner) = 0;

    //in regular place those 3 do the exact same thing,
    // which doesn't check in runtime and doesn't slow the program
    virtual char getType() const = 0;

    virtual char getId() const = 0;

    virtual char getOriginalSquare() const = 0;

    /**
     *
     * @return number of ants located on this place
     */
    virtual uint getNumberOfAnts() ;

    /**
     *
     * @return first occupant that is on this place
     */
    virtual std::shared_ptr<CAbstractWarrior> getOccupant(int pos = 0) const = 0;

    /**
     * @brief Kills first ant, that ant which was supposed to be fighting for this place
     */
    virtual void killOccupant() = 0;

    virtual bool setOccupant(std::shared_ptr<CAbstractWarrior> occupant) = 0;

    virtual bool promoteOccupant(char rank);

    virtual void spawnRegularAnt();

protected:
    std::shared_ptr<CAbstractWarrior> m_occupant = nullptr;

private:

    int m_xUpDown;
    int m_yLeftRight;

};

/**
 * @brief Class handles places such as normal passable squares or walls
 */
class CRegularPlace : public CPlace
{
public:

    CRegularPlace(int x, int y, char terrain);

    bool setOccupant(std::shared_ptr<CAbstractWarrior> occupant) override;

    std::shared_ptr<CAbstractWarrior> getOccupant(int pos = 0) const override;

    void killOccupant() override;

    void setType(char ch) override;

    char getType() const override;

    char getId() const override;

    char getOriginalSquare() const override;

    void upgradeUnits(std::shared_ptr<CAbstractBonus> ptr, char owner) override;

    bool isHouse() const override;
private:
    char m_terrain;


};

/**
 * @brief Class representing anthouse on map, stores information about home army
 */
class CAntHouse : public CPlace
{
public:


    //captures anthouse
    bool setOccupant(std::shared_ptr<CAbstractWarrior> occupant) override;

    bool promoteOccupant(char rank) override;

    std::shared_ptr<CAbstractWarrior> getOccupant(int pos = 0) const override;

    void killOccupant() override;

    void setType(char ch) override;

    CAntHouse(int x, int y, char playerId, char houseId, int numberOfAnts, int radius, int health, int attack);

    /**
     * @return name of owner
     */
    char getType() const override;

    char getId() const override;

    char getOriginalSquare() const override;

    void spawnRegularAnt() override;

    uint getNumberOfAnts() override;

    void upgradeUnits(std::shared_ptr<CAbstractBonus> ptr, char owner) override;

    bool isHouse() const override;
private:
    std::vector<std::shared_ptr<CAbstractWarrior>> m_homeArmy;
    std::list<char> currentRoutes;

    int m_radius;
    int m_health;
    int m_attack;

    char m_houseId;
    char m_player;
    char m_originalSquare;

};
