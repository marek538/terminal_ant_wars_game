#include "player_entity.h"

using namespace std;

CEntity::CEntity(char ch): m_name(ch){}

CPlayer::CPlayer(char ch) : CEntity(ch){}

CBotHard::CBotHard(char ch, int upgrade, int promote, int toughness) : CEntity(ch),m_numberToUpgrade(upgrade) , m_numberToPromote(promote), m_botToughness(toughness){}

bool CEntity::bfsAlgorithm(CGameMap * map,int xOrg, int yOrg, int xDst, int yDst, queue<shared_ptr<CPlace>> *bfsQueue)
{
    //add all anthouse neighbours
    addNodeToBfs(xOrg, yOrg, map, bfsQueue, 2, 3);

    while(!bfsQueue->empty())
    {
        shared_ptr<CPlace> tmp = bfsQueue->front();
        shared_ptr<CPlace> countTmp = bfsQueue->front();
        bfsQueue->pop();

        int xTmp = tmp->xCoordinates();
        int yTmp = tmp->yCoordinates();

        //we have reached the destination
        if((xTmp == xDst + 2 && yTmp == yDst) ||
        (xTmp == xDst - 2 && yTmp == yDst) ||
        (xTmp == xDst && yTmp + 3  == yDst) ||
        (xTmp == xDst && yTmp -3 == yDst))
        {

            //counts ants first and if number of ants satisfies route, proceeds to make route
            uint antsNeeded = 0;

            //makes actual route
            string idRoute;
            idRoute.push_back(map->m_playableMap[xOrg][yOrg]->getId());
            idRoute.push_back(map->m_playableMap[xDst][yDst]->getId());
            CRoute route(map->m_playableMap[xOrg][yOrg]->getType(), idRoute);
            map->m_allRoads.push_back(make_shared<CRoute>(route));


            while(tmp->xCoordinates() != 0 && tmp->yCoordinates() != 0)
            {
                tmp->setType('=');
                map->m_allRoads[map->m_allRoads.size()-1]->addPlace(tmp);
                uint first = get<0>(tmp->previousCoordinates);
                uint second = get<1>(tmp->previousCoordinates);
                tmp = map->m_playableMap[first][second];
                antsNeeded++;
            }
            if(antsNeeded > (map->m_playableMap[xOrg][yOrg])->getNumberOfAnts())
            {
                map->m_allRoads[map->m_allRoads.size()-1]->clearRoute();
                map->m_allRoads.pop_back();
                return false;
            }

            if(map->m_allRoads[map->m_allRoads.size()-1]->m_route[map->m_allRoads[map->m_allRoads.size()-1]->m_route.size()-1]->getType() != '=')
                map->m_allRoads[map->m_allRoads.size()-1]->m_route.pop_back();

            map->m_allRoads[map->m_allRoads.size()-1]->m_route.insert
                (map->m_allRoads[map->m_allRoads.size()-1]->m_route.begin(), map->m_playableMap[xDst][yDst]);

            //push source
            map->m_allRoads[map->m_allRoads.size()-1]->m_route.push_back(map->m_playableMap[xOrg][yOrg]);

            return true;
        }

        addNodeToBfs(xTmp, yTmp, map, bfsQueue, 1, 1);
    }

    return false;
}

void clearMap(CGameMap * map)
{
    for(uint i = 0 ; i < map->m_playableMap.size(); i++)
    {
        for(uint j = 0 ; j < map->m_playableMap[0].size(); j++)
        {
            map->m_playableMap[i][j]->visited = false;
        }
    }
}

int CEntity::executeMove(CGameMap * map, tuple<int, int> source, tuple<int, int> destination)
{
    if(source == destination)
        return 0;

    int xSrc = get<0>(source);
    int ySrc = get<1>(source);

    int xDst = get<0>(destination);
    int yDst = get<1>(destination);

    if(xSrc == 0 || ySrc == 0 || xDst == 0 || yDst == 0)
        return 0;

    queue<shared_ptr<CPlace>> bfsQueue;
    if(bfsAlgorithm(map, xSrc, ySrc, xDst, yDst, &bfsQueue))
    {
        clearMap(map);
        return 1;
    }
    else
    {
        clearMap(map);
        return 2;
    }

}

void CEntity::addNodeToBfs(int xTmp, int yTmp, CGameMap * map, queue<shared_ptr<CPlace>> *bfsQueue, int xIncerement, int yIncrement)
{
    if( map->m_playableMap[xTmp + xIncerement][yTmp]->getType() == ' ' && !map->m_playableMap[xTmp + xIncerement][yTmp]->visited)
    {
        map->m_playableMap[xTmp + xIncerement][yTmp]->visited = true;
        map->m_playableMap[xTmp + xIncerement][yTmp]->previousCoordinates = {xTmp, yTmp};
        bfsQueue->push(map->m_playableMap[xTmp + xIncerement][yTmp]);
    }

    if( map->m_playableMap[xTmp - xIncerement][yTmp]->getType() == ' ' && !map->m_playableMap[xTmp - xIncerement][yTmp]->visited)
    {
        map->m_playableMap[xTmp - xIncerement][yTmp]->visited = true;
        map->m_playableMap[xTmp - xIncerement][yTmp]->previousCoordinates = {xTmp, yTmp};
        bfsQueue->push(map->m_playableMap[xTmp - xIncerement][yTmp]);
    }

    if( map->m_playableMap[xTmp][yTmp + yIncrement]->getType() == ' ' && !map->m_playableMap[xTmp][yTmp + yIncrement]->visited)
    {
        map->m_playableMap[xTmp][yTmp + yIncrement]->visited = true;
        map->m_playableMap[xTmp][yTmp + yIncrement]->previousCoordinates = {xTmp, yTmp};
        bfsQueue->push(map->m_playableMap[xTmp][yTmp + yIncrement]);
    }

    if( map->m_playableMap[xTmp][yTmp - yIncrement]->getType() == ' ' && !map->m_playableMap[xTmp][yTmp - yIncrement]->visited)
    {
        map->m_playableMap[xTmp][yTmp - yIncrement]->visited = true;
        map->m_playableMap[xTmp][yTmp - yIncrement]->previousCoordinates = {xTmp, yTmp};
        bfsQueue->push(map->m_playableMap[xTmp][yTmp - yIncrement]);
    }
}

bool CEntity::upgradeArmy(int upgradeIndex, CGameMap * map)
{
    bool valid = m_upgradePanel.validateUpgrade(m_upgradePanel.bonuses[upgradeIndex]);
    if(!valid)
        return false;

    for(auto &row : map->m_playableMap)
    {
        for (auto &place : row)
        {
            place->upgradeUnits(m_upgradePanel.bonuses[upgradeIndex], m_name);
        }
    }
    return true;
}

bool CEntity::promoteAnt(char idHouse, char promoteType)
{
    for(auto &house : m_property)
    {
        if(house->getId() == idHouse)
        {
            if(house->getOccupant() == nullptr)
                return false;

            if(promoteType == 'm')
                return house->promoteOccupant('m');
            else if(promoteType == 'b')
                return house->promoteOccupant('b');
            else if(promoteType == 'c')
                return house->promoteOccupant('c');
            else
                return false;
        }
    }
    return false;
}

void CBotHard::twoBestHouses(uint &mostPowerfulHouse, uint &secondHouse)
{
    if(m_property.size() > 1)
    {
        secondHouse = 1;
        for (uint i = 0; i < m_property.size(); i++)
        {
            if (m_property[i]->getNumberOfAnts() >
                m_property[mostPowerfulHouse]->getNumberOfAnts())
            {
                mostPowerfulHouse = i;
                if(i == 1)
                    secondHouse = 0;
            }
            else if(m_property[i]->getNumberOfAnts() <
                    m_property[mostPowerfulHouse]->getNumberOfAnts()
                    &&
                    m_property[i]->getNumberOfAnts() >
                    m_property[secondHouse]->getNumberOfAnts()
                    )
            { secondHouse = i; }
        }
    }
}

//attack with most populated house to house with the least ants (can be his own as a support)
//subtract something like distance estimate
string CBotHard::attackMostValidHouse(bool firstHouse, vector<shared_ptr<CEntity>> &players, CGameMap &gameMap)
{

    if(m_property.empty())
        return "";

    uint mostPowerfulHouse = 0;
    uint secondHouse = 0;

    twoBestHouses(mostPowerfulHouse, secondHouse);

    int biggestScore = 0;

    string s;
    s.push_back(m_property[mostPowerfulHouse]->getId());
    s.push_back(m_property[secondHouse]->getId());

    vector<tuple<char,int>> houses;

    //order all targets
    for(auto & player : players)
    {
        for(auto & house : player->m_property)
        {
            int tempScore = 0;
            if(firstHouse)
            {
                tempScore = probableLengthAndPower(
                        m_property[mostPowerfulHouse]->xCoordinates(),
                        m_property[mostPowerfulHouse]->yCoordinates(),
                        house->xCoordinates(),
                        house->yCoordinates(),
                        gameMap);
            }
            else
            {
                tempScore = probableLengthAndPower(
                        m_property[secondHouse]->xCoordinates(),
                        m_property[secondHouse]->yCoordinates(),
                        house->xCoordinates(),
                        house->yCoordinates(),
                        gameMap);
            }
            if(tempScore > 0)
            {
                biggestScore = tempScore;
                bool pushed = false;
                for(uint pos = 0; pos < houses.size(); pos++)
                    if(get<1>(houses[pos]) < tempScore )
                    {
                        pushed = true;
                        houses.insert(houses.begin() + pos, {house->getId(), tempScore});
                        break;
                    }
                if(!pushed)
                    houses.emplace_back(house->getId(), tempScore);
            }
        }
    }

    if(biggestScore == 0)
        return "";

    for(auto & house : houses)
        s.push_back(get<0>(house));

    return s;
}

void CEntity::doMove(vector<std::shared_ptr<CEntity>> &players, CGameMap &gameMap,
                     vector<std::string> &existentRoutes) {}

void CEntity::makeMove(int turns, CGameMap &gameMap, vector<shared_ptr<CEntity>> &players, vector<string> &existentRoutes)
{}

int CBotHard::probableLengthAndPower(int x1, int y1, int x2, int y2, CGameMap &gameMap)
{
    return (int)gameMap.m_playableMap[x1][y1]->getNumberOfAnts() - (int)gameMap.m_playableMap[x2][y2]->getNumberOfAnts() - (abs(x1 - x2) + abs(y1 - y2));
}

bool CEntity::routeExistent(string &route, vector<string> &existentRoutes)
{
    for(auto &routeNew : existentRoutes)
    {
        if((route[0] == routeNew[0] && route[1] == routeNew[1]) || (route[0] == routeNew[1] && route[1] == routeNew[0]))
            return true;
    }
    return false;
}

void CBotHard::doMove(vector<shared_ptr<CEntity>> &players, CGameMap &gameMap, vector<string> &existentRoutes)
{
    string s = attackMostValidHouse(true, players, gameMap);
    if(s.size() <= 2)
        return;

    int executed = 0;
    //call attack command with s parameters
    for(uint i = 2; i < s.length(); i++)
    {
        string tmp;
        tmp.push_back(s[0]);
        tmp.push_back(s[i]);
        if(routeExistent(tmp, existentRoutes))
            continue;

        executed = executeMove(&gameMap, destinationCoordinates(s[0], players),
                                                               destinationCoordinates(s[i], players));
        if (executed == 1)
        {
            existentRoutes.push_back(tmp);
            return;
        }
    }
    if(executed == 1)
        return;

    s = attackMostValidHouse(false, players, gameMap);
    if(s.size()<=3)
        return;

    executed = 0;
    //call attack command with s parameters
    for(uint i = 2; i < s.length(); i++)
    {
        string tmp;
        tmp.push_back(s[1]);
        tmp.push_back(s[i]);
        if(routeExistent(tmp, existentRoutes))
            continue;

        executed = executeMove(&gameMap, destinationCoordinates(s[1], players),
                                                               destinationCoordinates(s[i], players));
        if (executed == 1)
        {

            existentRoutes.push_back(tmp);
            return;
        }
    }
}

std::tuple<int, int> CEntity::destinationCoordinates(char dstId, std::vector<std::shared_ptr<CEntity>> &players)
{
    int xDest = 0;
    int yDest = 0;
    bool found = false;

    for(uint i = 0; i < players.size() && !found; i++)
    {
        for (auto & j : players[i]->m_property)
        {

            if (j->getId() == dstId)
            {
                found = true;
                xDest = j->xCoordinates();
                yDest = j->yCoordinates();
                break;
            }

        }
    }
    return {xDest, yDest};
}

void CBotHard::makeMove(int turns, CGameMap &gameMap, vector<shared_ptr<CEntity>> &players, vector<string> &existentRoutes)
{
    if(m_property.empty())
        return;

    if(turns % (m_botToughness * m_numberToUpgrade) == 0)
    {
        upgradeArmy(m_botUpgrade, &gameMap);
        m_botUpgrade = (m_botUpgrade + 1) % 3;
    }
    else if(turns % (m_botToughness * m_numberToPromote) == 0)
    {
        //each bot upgrades an ant
        char idTmp = m_property[m_whereToUpgrade % m_property.size()]->getId();
        promoteAnt(idTmp, m_promotions[m_botPromote]);
        m_botPromote = (m_botPromote + 1) % 3;
    }
    else if(turns % (m_botToughness * 3 ) == 0)
    {
        //each bot tries to make a move

        doMove(players, gameMap, existentRoutes);

    }
}

bool CEntity::playersAttack(string &command, vector<string> &existentRoutes, string &failure, CGameMap &gameMap, std::vector<std::shared_ptr<CEntity>> &players)
{
    string tmp;
    tmp.push_back(command[0]);
    tmp.push_back(command[2]);
    if(routeExistent(tmp, existentRoutes))
    {
        failure = "route already exists";
        return false;
    }

    //attack
    int executed = executeMove(&gameMap,
                               CEntity::destinationCoordinates(command[0], players),
                               CEntity::destinationCoordinates(command[2], players));
    if(executed == 2)
    {
        failure = "not enough ants";
        return false;
    }
    else if (executed == 0)
    {
        failure = "not valid source or target";
        return false;
    }

    string s;
    s.push_back(command[0]);
    s.push_back(command[2]);

    existentRoutes.push_back(s);
    //true if move can be executed
    return true;
}

bool CEntity::promote(string allIdList, string &command, string &failure, int turns, int numberToPromoteAnt){return true;}

bool CEntity::upgrade(string &command, string &failure, int turns, int numberToUpgradeAnt, CGameMap &gameMap)
{
    return true;
}

char CEntity::getName() const
{
    return m_name;
}

int CEntity::getLastUpgrade() const
{
    return m_lastUpgrade;
}

int CEntity::getLastPromotion() const
{
    return m_lastPromotion;
}

bool CPlayer::promote(std::string allIdList, string &command, string &failure, int turns, int numberToPromoteAnt)
{
    if(command[5] != 'c' && command[5] != 'b' && command[5] != 'm' && allIdList.find(command[4]) == std::string::npos)
    {
        failure = "invalid promotion\n";
        return false;
    }

    int roundsForUpgrade = turns - m_lastPromotion;
    if(roundsForUpgrade < numberToPromoteAnt)
    {
        failure = "not enough promotion points\n";
        return false;
    }

    if (!promoteAnt(command[4], command[5]))
    {
        failure = "invalid promotion\n";
        return false;
    }

    m_lastPromotion += numberToPromoteAnt;
    return true;
}

bool CPlayer::upgrade(string &command, string &failure, int turns, int numberToUpgradeAnt, CGameMap &gameMap)
{
    int roundsForUpgrade = turns - m_lastUpgrade;
    if(roundsForUpgrade < numberToUpgradeAnt)
    {
        failure = "not enough upgrade points\n";
        return false;
    }

    if(command[3] == 'a')
    {
        if (!upgradeArmy(0, &gameMap))
        {
            failure = "invalid upgrade\n";
            return false;
        }
    }
    else if(command[3] == 'h')
    {
        if (!upgradeArmy(1, &gameMap))
        {
            failure = "invalid upgrade\n";
            return false;
        }
    }
    else if(command[3] == 'r')
    {
        if (!upgradeArmy(2, &gameMap))
        {
            failure = "invalid upgrade\n";
            return false;
        }
    }
    else
        return false;

    m_lastUpgrade += numberToUpgradeAnt;
    return true;
}
