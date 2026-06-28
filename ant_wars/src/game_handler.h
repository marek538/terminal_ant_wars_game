/**
 * @file game_handler.h
 * @brief This file manages game loop itself with all actions
 */

#pragma once
#include <ctime>
#include "player_entity.h"
#include "user_interface.h"


/**
 * @brief This class manages game loop itself with all actions
 */
class CGameHandler
{
public:


    //loadMap
    //loadConfig
    //initPlayers
    CGameHandler();

    /**
     * @brief Starts the game and handles game loop
     */
    void playGame();

private:
    //public because of tests
    CGameMap m_gameMap = {0,0,0};

    //CUpgradePanel m_upgradePanel = CUpgradePanel(0, 0, 0);
    std::vector<std::shared_ptr<CEntity>> m_players;
    uint m_livingPlayerIndex = 10;

    //saving variables
    std::string m_commandSequence;
    std::string m_lastCommand;

    //purpose is not to have 2 routes between same anthouses
    std::vector<std::string> m_existentRoutes;

    CWholeWindow m_outputConsole;
    CConfiguration m_configuration;

    int m_turns = 0;
    int m_lastTransfer = 0;

    //game running(1 means over)
    int m_gameStatus = 0;


    /**
     * @brief Handles player input
     * @return false if players command wasn't valid
     */
    bool acceptCommand();


    /**
     * @brief Initializes game including loading map and configuration when game isn't started from save
     */
    void nonSaveInitialization();

    /**
     * @brief Handles actions performed by game itself such as ant moves, ant fights and AI moves
     */
    void updateGameState();

    /**
     * @brief Introduces player to player database and assigns house to his property
     * @param ptr house that is to be included
     */
    void checkPlayer(std::shared_ptr<CPlace> ptr);

    /**
     * @brief Handles players attack to an anthouse
     * @param command
     * @return false if attack can't be executed or command was invalid
     */
    bool playersAttack(std::string &command);

    /**
     * @brief Checks if house with id equals char ch belongs to player
     * @param ch
     */
    bool livingPlayerProperty(char ch);

    /**
     * @brief Fills vector m_players with list of all players with their property
     */
    void initializePlayers();

    /**
     * @brief Adds ant to each house
     */
    void generateAnts();

    /**
     * @brief Finds all ants that are ready to fight and passes them to fightAnts function
     */
    void findAntFighters();


    /**
     * @brief Moves with each ant further down the road
     */
    void moveViaRoutes();

    /**
     * @brief Deletes all routes that has no ant on them
     */
    void cancelEmptyRoutes();

    /**
     * @brief If house is captured, transfers its ownership to a new player so he can execute moves with it
     * @param newOccupant
     * @param oldOccupant
     * @param idHouse
     */
    void transferHouse(char newOccupant, char oldOccupant, char idHouse);

    /**
     * @brief Sets m_gameStatus to 1 if player either lost or won
     */
    void checkVictoryCondition();


    /**
     * @brief If routes was canceled, this deletes it from list of all routes
     * @param leftHouse
     * @param rightHouse
     */
    void deleteRouteFromExistent(char leftHouse, char rightHouse);

    /**
     * @brief When player calls save, this makes a savefile with players requirement and stores game data in it
     * @return if save was successful
     */
    bool saveCurrentState();

    /**
     * @brief This starts game from save file and restores it to state described in save
     * @param location path to save file
     */
    void loadFromSave(std::string &location);

    /**
     * @brief Called from loadFromSave and this executes 1 line from save file
     * @param playerCommand
     * @return false if command wasn't valid
     */
    bool executeRound(std::string &playerCommand);




};


