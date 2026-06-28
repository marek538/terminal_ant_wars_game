/**
 * @file player_entity.h
 * @brief This file manages move execution by all players on map
 */

#pragma once
#include <vector>
#include <queue>
#include "game_data.h"

/**
 * @brief This class represents each player in game meaning real player and all bots and is capable of executing moves
 */
class CEntity
{
public:
    std::vector<std::shared_ptr<CPlace>> m_property;
    CUpgradePanel m_upgradePanel = {0,0,0};

    explicit CEntity(char ch);

    virtual ~CEntity() = default;

    /**
     * @brief Promotes first available ant in anthouse idHouse
     * @param idHouse
     * @param promoteType
     * @param attack
     * @param radius
     * @return false if promote isn't available for current house
     */
    bool promoteAnt(char idHouse, char promoteType);

    /**
     * @brief Find coordinates to house with id == dstId
     * @param dstId
     * @return coordinates to ant house
     */
    static std::tuple<int, int> destinationCoordinates(char dstId, std::vector<std::shared_ptr<CEntity>> &players);

    /**
     * @brief Upgrades all ants of this player
     * @param upgradeIndex
     * @param map
     * @return false if upgrade can't be executed
     */
    bool upgradeArmy(int upgradeIndex, CGameMap * map);

    virtual void makeMove(int turns, CGameMap &gameMap, std::vector<std::shared_ptr<CEntity>> &players, std::vector<std::string> &existentRoutes);

    virtual void doMove(std::vector<std::shared_ptr<CEntity>> &players, CGameMap &gameMap, std::vector<std::string> &existentRoutes);

    /**
     * @brief Makes a route from source to destination
     * @param map
     * @param source
     * @param destination
     * @return 0 if route isn't valid, 2 if there isn't enough ants to create the route, 1 if action is successful
     */
    virtual int executeMove(CGameMap * map, std::tuple<int, int> source, std::tuple<int, int> destination);
    /**
     * @brief Makes estimate how many ants can survive when attacking from house on coordinates x1, y1 to house with coordinates x2, y2
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     */

    bool playersAttack(std::string &command, std::vector<std::string> &existentRoutes, std::string &failure, CGameMap &gameMap, std::vector<std::shared_ptr<CEntity>> &players);



    /**
     * @brief Promotes all ants of one player
     * @param allIdList
     * @param command
     * @param failure
     * @param turns
     * @param numberToPromoteAnt
     * @return
     */
    virtual bool promote(std::string allIdList, std::string &command, std::string &failure, int turns, int numberToPromoteAnt);

    /**
     * @brief Promotes first available ant in anthous
     * @param command
     * @param failure
     * @param turns
     * @param numberToUpgradeAnt
     * @param gameMap
     * @return
     */
    virtual bool upgrade(std::string &command, std::string &failure, int turns, int numberToUpgradeAnt, CGameMap &gameMap);

    char getName() const;

    int getLastUpgrade() const;

    int getLastPromotion() const;
protected:
    char m_name;
    int m_lastUpgrade = 0;
    int m_lastPromotion = 0;

    bool routeExistent(std::string &route, std::vector<std::string> &existentRoutes);

private:

    /**
     * @brief Finds shortest route between anthouses
     * @param map
     * @param xOrg
     * @param yOrg
     * @param xDst
     * @param yDst
     * @param bfsQueue
     * @return false if the route is too long for ants from source to cover
     */
    static bool bfsAlgorithm(CGameMap * map,int xOrg, int yOrg, int xDst, int yDst, std::queue<std::shared_ptr<CPlace>> *bfsQueue);

    /**
     * @brief Adds one layer to bfs queue
     * @param xTmp
     * @param yTmp
     * @param map
     * @param bfsQueue
     * @param xIncrement
     * @param yIncrement
     */
    static void addNodeToBfs(int xTmp, int yTmp, CGameMap * map, std::queue<std::shared_ptr<CPlace>> *bfsQueue, int xIncrement, int yIncrement);
};

/**
 * @brief Player who's moves are generated via UI
 */
class CPlayer : public CEntity
{
public:
    bool promote(std::string allIdList, std::string &command, std::string &failure, int turns, int numberToPromoteAnt) override;

    bool upgrade(std::string &command, std::string &failure, int turns, int numberToUpgradeAnt, CGameMap &gameMap) override;

    explicit CPlayer(char ch);

private:
};

/**
 * @brief AI operated player with better move choices
 */
class CBotHard : public CEntity
{
public:


    void makeMove(int turns, CGameMap &gameMap, std::vector<std::shared_ptr<CEntity>> &players, std::vector<std::string> &existentRoutes) override;

    void doMove(std::vector<std::shared_ptr<CEntity>> &players, CGameMap &gameMap, std::vector<std::string> &existentRoutes) override;

    CBotHard(char ch, int upgrade, int promote, int toughness);
private:
    int m_numberToUpgrade;
    int m_numberToPromote;
    int m_botToughness;

    int m_botUpgrade = 0;
    int m_botPromote = 0;
    std::string m_promotions = "mbc";    //promote type
    int m_whereToUpgrade = 0;


    int probableLengthAndPower(int x1, int y1, int x2, int y2, CGameMap &gameMap);

          /**
     * @brief Generates most valid attack path for current AI player
     * @param attackerIndex
     * @param firstHouse decides if it will attack with most or second most powerful house
     * @return command which contains first 2 strongest houses of current AI player and then sequence of most promising attack paths
     */
    std::string attackMostValidHouse(bool firstHouse, std::vector<std::shared_ptr<CEntity>> &m_players, CGameMap &gameMap);

    void twoBestHouses(uint &mostPowerfulHouse, uint &secondHouse);
};

