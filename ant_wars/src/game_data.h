/**
 * @file game_data.h
 * @brief This class manages data input from files such as map and ant stats
 */

#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "place_on_map.h"

/**
 * @brief This class handles all data regarding map
 */
class CGameMap
{
public:
    std::vector<std::shared_ptr<CRoute>> m_allRoads;
    std::vector<std::vector<std::shared_ptr<CPlace>>> m_playableMap;

    CGameMap(int radius, int baseHealth, int baseAttack);

    /**
     * @brief Loads file which then transfers to playable map
     *
     * @param inFile path to map
     * @return true if map is valid and loading was successful
     */
    bool loadMap(const std::string &inFile);

    /**
     * @brief Transforms map with all objects to printable string
     *
     * @return transformed map into string
     */
    std::string printPlayableMap();

    /**
     * @brief Changes owner of house on printable map, called when house is for example captured
     * @param xCoordinates
     * @param yCoordinates
     * @param newOwner
     */
    void refactorAntHouse(int xCoordinates, int yCoordinates, char newOwner);

    std::string getPath() const;

    void setPath(std::string &path);

    std::string getAllIdList();

private:
    std::string m_allIdList;
    std::string m_path;
    int m_baseRadius;
    int m_baseHealth;
    int m_baseAttack;
    std::vector<std::vector<char>> m_gamePlan;

    /**
     * @brief Checks if map has correct size and if first row is made of #
     * @param ch
     * @param lineSize
     */
    bool validateCharInMap(char ch, uint *lineSize);

    /**
     * @brief Goes through inside of map borders and finds potentional houses
     */
    bool validateObjectsPosition();

    /**
     * @brief Checks if char can even be used in map
     * @param ch
     * @param lineSize
     */
    bool checkChar(char ch, const uint *lineSize);

    /**
     * @brief Checks if house with upper left corner with coordinates posx, posy is correctly defined
     * @param posx
     * @param posy
     * @param border
     */
    bool validateAntHouse(uint posx, uint posy,  char border);

    /**
     * @brief Transforms strings into objects than can be used by engine
     * @return false if house has not valid number of ants
     */
    bool transferToPlayableMap();

};

/**
 * @brief This class stores base stats information
 */
class CConfiguration
{
public:
    /**
     * @brief Loads configuration file with base game parameters
     * @param inFile path to configuration file
     * @return if configuration has bad layout
     */
    bool loadConfiguration(const std::string &inFile);

    /**
     * @brief Gets line from save file and parses it to executable command
     * @param lineFromSave
     * @param correct flag is set to false when command doesn't have right format
     * @return executable command
     */
    static std::string parseSaveCommand(std::string &lineFromSave, bool *correct);

    int getAttack() const;

    int getHealth() const;

    int getRadius() const;

    int getAttackBonus() const;

    int getHealthBonus() const;

    int getRadiusBonus() const;

    /**
     * @brief Makes save file consisting path to used map, path to used configuration and game commands itself
     *
     * @param configurationPath path to configuration used in this game
     * @param mapPath path to map used in this game
     * @param savePath path to save file
     * @param commands command sequence that was used during the game
     * @return false if unable to reach savePath
     */
    static bool saveGame(const std::string &configurationPath, const std::string &mapPath, const std::string &savePath, const std::string &commands);

    std::string getPath() const;

    int getTurnsToGenerate() const;

    int getNumberToUpgrade() const;

    int getNumberToPromote() const;

    int getBotToughness() const;

private:
    int m_turnsToGenerateAnts = 10;
    int m_numberToUpgrade = 20;
    int m_numberToPromote = 10;
    int m_botToughness = 3;

    std::string m_path;
    int m_attack = 5;
    int m_health = 20;
    int m_radius = 1;
    int m_attackBonus = 2;
    int m_healthBonus = 2;
    int m_radiusBonus = 1;

    /**
     * @brief checks if current line is loaded correctly
     * @param linePos
     * @param original
     */
    bool validateConfigLine(int linePos, std::string &original);

    /**
     * @brief checks if line indication is correct
     *
     * @param base
     * @param original
     */
    int checkLabel(std::string &base, std::string &original);

};