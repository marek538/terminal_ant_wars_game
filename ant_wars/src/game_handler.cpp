//all macros in configuration file

#include "game_handler.h"
#include <memory>

using namespace std;

//initializes game with configuration and map and initializes players
CGameHandler::CGameHandler()
{

    //m_turns = 100;

    string location = m_outputConsole.playFromSave();
    if((!location.empty() && (location[0] >= 49 && location[0] <= 53) && location.size() == 1))
    {
        loadFromSave(location);
    }
    else
    {
        nonSaveInitialization();
        initializePlayers();
    }
}

void CGameHandler::nonSaveInitialization()
{
    string configPath = m_outputConsole.getConfigLocation();
    string tmp;
    if (!configPath.empty() && (configPath[0] >= 49 && configPath[0] <= 51) && configPath.size() == 1)
    {
        tmp = "./utilities/configuration/actuallInput_";

        tmp.push_back(configPath[0]);
        m_configuration.loadConfiguration(tmp);
    }
    else if (!m_configuration.loadConfiguration(configPath))
    {
        m_outputConsole.print("chyba\n");
    }

    string mapPath = m_outputConsole.getMapLocation();

    m_gameMap = CGameMap(m_configuration.getRadius(), m_configuration.getHealth(), m_configuration.getAttack());

    if (!mapPath.empty() && (mapPath[0] >= 49 && mapPath[0] <= 53) && mapPath.size() == 1)
    {
        tmp = "./utilities/maps/actuallInput_";

        tmp.push_back(mapPath[0]);
        m_gameMap.loadMap(tmp);
    } else if (!m_gameMap.loadMap(mapPath))
    {
        m_outputConsole.print("chyba\n");
    }
}

void CGameHandler::playGame()
{
    m_lastCommand = "save";
    checkVictoryCondition();
    while(m_gameStatus == 0)
    {
        system("clear");
        m_outputConsole.printOptions(m_players[0]->m_upgradePanel.bonuses);
        m_outputConsole.print(m_gameMap.printPlayableMap());
        m_outputConsole.print("available promotions: " + to_string((m_turns - m_players[m_livingPlayerIndex]->getLastPromotion())/m_configuration.getNumberToPromote()) + "\n");
        m_outputConsole.print("available upgrades: " + to_string((m_turns - m_players[m_livingPlayerIndex]->getLastUpgrade())/m_configuration.getNumberToUpgrade()) + "\n");
        m_outputConsole.print("current turn: " + to_string(m_turns));
        while (!acceptCommand())
        {
            m_outputConsole.print("invalid command\n");
            m_outputConsole.print("available promotions: " + to_string((m_turns - m_players[m_livingPlayerIndex]->getLastPromotion())/m_configuration.getNumberToPromote()) + "\n");
            m_outputConsole.print("available upgrades: " + to_string((m_turns - m_players[m_livingPlayerIndex]->getLastUpgrade())/m_configuration.getNumberToUpgrade()) + "\n");
            m_outputConsole.print("current turn: " + to_string(m_turns) + "\n");
        }

        if(m_lastCommand == "exit" || m_lastCommand == "save")
            continue;

        for(auto &player : m_players)
            player->makeMove(m_turns, m_gameMap, m_players, m_existentRoutes);

        updateGameState();

        //add command to save
        if(!(m_lastCommand == "exit" || m_lastCommand == "save"))
        {
            m_commandSequence = m_commandSequence + to_string(m_turns) + ": " + m_lastCommand + "\n";
        }
        if(m_turns - m_lastTransfer > 200)
        {
            system("clear");
            m_outputConsole.print("stalemate\n");
            m_gameStatus = 1;
        }
    }
    m_outputConsole.print(m_gameMap.printPlayableMap());
    m_outputConsole.print("current turn: " + to_string(m_turns));
}

//updates game - ant fights, ant movement, creation of new ants
void CGameHandler::updateGameState()
{
    m_turns++;

    //fight ants with medic and captain bonuses and explode bomber (only one bomber can explode - easier to implement)
    findAntFighters();

    //move ants one square + resolve capturing anthouse
    moveViaRoutes();

    cancelEmptyRoutes();

    generateAnts();
}


//XaY    -attack from X to Y
//up-a   -upgrade attack
//up-h   -upgrade health
//up-r   -upgrade range
//       -enter to continue cycle
//promXc -promote first ant in X to captain
//promXb -promote first ant in X to bomber
//promXm -promote first ant in X to medic
//save   -save game
//exit   -exit
bool CGameHandler::acceptCommand()
{
    m_lastCommand = m_outputConsole.readCommand();



    if(m_lastCommand.size() == 3 && m_lastCommand[1] == 'a')
    {
        //attack
        return playersAttack(m_lastCommand);
    }
    else if(m_lastCommand.size() == 4 && m_lastCommand.substr(0, 3) == "up-")
    {
        //upgrade
        string failure;
        if(!m_players[m_livingPlayerIndex]->upgrade(m_lastCommand, failure, m_turns, m_configuration.getNumberToUpgrade(), m_gameMap))
        {
            m_outputConsole.print(failure);
            return false;
        }
        else
            return true;

    }
    else if(m_lastCommand.empty())
    {
        return true;
    }
    else if(m_lastCommand.size() == 6 && m_lastCommand.substr(0, 4) == "prom")
    {
        string failure;
        if(!m_players[m_livingPlayerIndex]->promote(m_gameMap.getAllIdList(), m_lastCommand, failure, m_turns, 10))
        {
            m_outputConsole.print(failure);
            return false;
        }
        else
            return true;
    }
    else if(m_lastCommand == "exit")
    {

        system("clear");

        m_gameStatus = 1;
        return true;
    }
    else if(m_lastCommand == "save")
    {
        //save
        if(saveCurrentState())
        {
            m_outputConsole.print("save successful\n");
        }
        else
        {
            m_outputConsole.print("save failed\n");
            return false;
        }
        return true;
    }

    return false;
}

//checks if char ch belongs to living player
bool CGameHandler::livingPlayerProperty(char ch)
{
    for(auto &i : m_players[m_livingPlayerIndex]->m_property)
    {
        if(i->getId() == ch)
            return true;
    }
    return false;
}

//executes players move
bool CGameHandler::playersAttack(string &command)
{
    if(!livingPlayerProperty(command[0]) || command[0] == command[2])
    {
        m_outputConsole.print("not valid source or target");
        return false;
    }

    string failure;

    if(!m_players[m_livingPlayerIndex]->playersAttack(command, m_existentRoutes, failure, m_gameMap, m_players))
    {
        m_outputConsole.print(failure);
        return false;
    }
    //true if move can be executed
    return true;
}


//gets list of players with their property
void CGameHandler::initializePlayers()
{
    for(auto & i : m_gameMap.m_playableMap)
    {
        for(uint j = 0; j < m_gameMap.m_playableMap[0].size(); j++)
        {
            checkPlayer(i[j]);
        }
    }
}

//if place on position is anthouse, adds new player and/or adds house to players property
//@is living player
void CGameHandler::checkPlayer(shared_ptr<CPlace> ptr)
{
    if(ptr->isHouse())
    {
        bool found = false;
        for(auto & player : m_players)
        {
            if( player->getName() == ptr->getType())
            {
                found = true;
                player->m_property.push_back(ptr);
                break;
            }
        }
        if(!found)
        {
            if(ptr->getType() == '@')
            {
                m_players.push_back(make_shared<CPlayer>('@'));
                m_players[m_players.size() - 1]->m_upgradePanel = CUpgradePanel(m_configuration.getAttackBonus(), m_configuration.getHealthBonus(), m_configuration.getRadiusBonus());
                m_livingPlayerIndex = m_players.size() - 1;
                m_players[m_players.size() - 1]->m_property.push_back(ptr);
            }
            else
            {
                m_players[m_players.size() - 1]->m_upgradePanel = CUpgradePanel(m_configuration.getAttackBonus(), m_configuration.getHealthBonus(), m_configuration.getRadiusBonus());
                m_players.push_back(make_shared<CBotHard>(ptr->getType(), m_configuration.getNumberToUpgrade(), m_configuration.getNumberToPromote(), m_configuration.getBotToughness()));
                m_players[m_players.size() - 1]->m_property.push_back(ptr);
            }
        }
    }
}

void CGameHandler::generateAnts()
{
    if(m_turns % m_configuration.getTurnsToGenerate() == 0)
    {
        for(shared_ptr<CEntity> &player : m_players)
        {
            for(auto &house : player->m_property)
            {
                house->spawnRegularAnt();
            }
        }
    }
}

void CGameHandler::findAntFighters()
{
    for (auto &route: m_gameMap.m_allRoads)
    {
        for(uint i = 0; i < route->m_route.size()-1; i++)
        {
            if(route->m_route[i]->getOccupant() == nullptr || route->m_route[i+1]->getOccupant() == nullptr)
                break;
            if(route->m_route[i]->getOccupant()->getOwner() != route->m_route[i+1]->getOccupant()->getOwner())
                //fight
                route->fightAnts((int)i, (int)i+1);
        }
    }
}

void CGameHandler::moveViaRoutes()
{

    for(auto &route: m_gameMap.m_allRoads)
    {
        int first = 0;
        char oldOwner = route->m_route[0]->getType();
        if(route->moveViaRoutes(first) == 2 && route->m_route[0]->getOccupant() != nullptr)
        {
            m_lastTransfer = m_turns;
            transferHouse(route->m_route[0]->getOccupant()->getOwner(), oldOwner, route->m_route[0]->getId());
            m_gameMap.refactorAntHouse(route->m_route[0]->xCoordinates(),
                                       route->m_route[0]->yCoordinates(),
                                       route->m_route[0]->getOccupant()->getOwner());

            checkVictoryCondition();
        }
    }
}

void CGameHandler::transferHouse(char newOccupant, char oldOccupant, char idHouse)
{
    for(auto &playerNew : m_players)
    {
        if(playerNew->getName() == newOccupant)
        {
            for(auto &playerOld : m_players)
            {
                if(playerOld->getName() == oldOccupant)
                {
                    for(int i = 0; i < (int)playerOld->m_property.size(); i++)
                    {
                        if(playerOld->m_property[i]->getId() == idHouse)
                        {
                            playerNew->m_property.push_back(playerOld->m_property[i]);
                            playerOld->m_property.erase(playerOld->m_property.begin() + i);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void CGameHandler::cancelEmptyRoutes()
{
    for(int i = 0; i < (int)m_gameMap.m_allRoads.size(); i++)
    {
        if(!m_gameMap.m_allRoads[i]->isOccupied())
        {
            deleteRouteFromExistent(m_gameMap.m_allRoads[i]->m_route[0]->getId(), m_gameMap.m_allRoads[i]->m_route[m_gameMap.m_allRoads[i]->m_route.size()-1]->getId());
            m_gameMap.m_allRoads[i]->clearRoute();
            m_gameMap.m_allRoads.erase(m_gameMap.m_allRoads.begin() + i);
            i--;
        }
    }
}

void CGameHandler::checkVictoryCondition()
{
    if(m_livingPlayerIndex > 8 || m_players[m_livingPlayerIndex]->m_property.empty())
    {
        m_gameStatus = 1;
        system("clear");
        m_outputConsole.print("GGWP, you lost\n");
    }

    bool victory = true;
    for(uint i = 0; i < m_players.size(); i++)
    {
        if(i != m_livingPlayerIndex && !m_players[i]->m_property.empty())
        {
            victory = false;
            break;
        }
    }
    if(victory)
    {
        m_gameStatus = 1;
        system("clear");
        m_outputConsole.print("GGWP, you won\n");
    }
}

void CGameHandler::deleteRouteFromExistent(char left, char right)
{
    for(int i = 0; i < (int)m_existentRoutes.size(); i++)
    {
        if((left == m_existentRoutes[i][0] && right == m_existentRoutes[i][1]) || (left == m_existentRoutes[i][1] && right == m_existentRoutes[i][0]))
        {
            m_existentRoutes.erase(m_existentRoutes.begin() + i);
            break;
        }
    }
}


//it is enough to save only players command because everything will play out the same way
bool CGameHandler::saveCurrentState()
{
    system("clear");
    m_outputConsole.print("use save 1-5:\n");
    string location = m_outputConsole.readCommand();

    while (!(!location.empty() && (location[0] >= 49 && location[0] <= 53) && location.size() == 1))
    {
        m_outputConsole.print("save 1-5:\n");
        location = m_outputConsole.readCommand();
    }

    string path = "./utilities/saves/save_no_";

    path.push_back(location[0]);
    path = path + ".txt";

    return CConfiguration::saveGame(m_configuration.getPath(), m_gameMap.getPath(), path, m_commandSequence);
}

//turn number is there just for player, code doesn't check it
void CGameHandler::loadFromSave(string &location)
{
    string path = "./utilities/saves/save_no_" + location + ".txt";

    ifstream is(path);
    string configPath;
    getline(is, configPath);
    if(!is.good())
        throw invalid_argument("not reachable file\n");
    m_configuration.loadConfiguration(configPath);

    string mapPath;
    getline(is, mapPath);
    if(!is.good())
        throw invalid_argument("not reachable file\n");

    m_gameMap = CGameMap(m_configuration.getRadius(), m_configuration.getHealth(), m_configuration.getAttack());

    m_gameMap.loadMap(mapPath);

    initializePlayers();

    string command;
    while(getline(is, command))
    {
        if(!is.good())
            throw invalid_argument("not reachable file\n");

        //the command execution and game progression itself
        executeRound(command);
    }

    is.close();
}

bool CGameHandler::executeRound(string &playerCommand)
{
    bool correct = true;
    playerCommand = CConfiguration::parseSaveCommand(playerCommand, &correct);
    m_lastCommand = playerCommand;

    if(!correct)
        throw invalid_argument("wrong save layout\n");

    if(playerCommand.size() == 3 && playerCommand[1] == 'a')
    {
        if(!playersAttack(playerCommand))
            throw invalid_argument("wrong save layout\n");
    }
    else if(playerCommand.size() == 4 && playerCommand.substr(0, 3) == "up-")
    {
        //upgrade
        string failure;
        if(!m_players[m_livingPlayerIndex]->upgrade(m_lastCommand, failure, m_turns, m_configuration.getNumberToUpgrade(), m_gameMap))
            throw invalid_argument("wrong save layout\n");
    }
    else if(playerCommand.empty())
    {   //happily continue cycle
    }
    else if(playerCommand.size() == 6 && playerCommand.substr(0, 4) == "prom")
    {
        string failure;

        string tmp = m_gameMap.getAllIdList();
        if(!m_players[m_livingPlayerIndex]->promote(tmp, m_lastCommand,failure, m_turns, 10 ))
            throw invalid_argument("wrong save layout\n");
    }
    else
        throw invalid_argument("wrong save layout\n");

    for(auto &player : m_players)
        player->makeMove(m_turns, m_gameMap, m_players, m_existentRoutes);

    updateGameState();


    if(!(m_lastCommand == "exit" || m_lastCommand == "save"))
    {
        m_commandSequence = m_commandSequence + to_string(m_turns) + ": " + m_lastCommand + "\n";
    }

    return true;
}


