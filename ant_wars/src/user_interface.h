/**
 * @file user_interface.h
 * @brief This file handles communication between program and user
 */

#pragma once
#include <string>
#include <iostream>
#include "race_bonus_tree.h"

/**
 * @brief This class manages communication with player
 */
class CWholeWindow
{
public:
    CWholeWindow();

    ~CWholeWindow();

    void print(const std::string &input);

    void errPrint(const std::string &input);

    std::string readCommand();

    /**
     * @brief Handles communication regarding path to map
     * @return path to map or index of default map file
     */
    std::string getMapLocation();

    /**
     * @brief Handles communication regarding path to configuration
     * @return path to configuration or index of default configuration file
     */
    std::string getConfigLocation();

    /**
     * @brief Prints all possible commands that player can execute
     */
    void printOptions(std::vector<std::shared_ptr<CAbstractBonus>> &bonuses);

    /**
     * @brief Handles communication regarding playing game from save
     * @return index of save file
     */
    std::string playFromSave();

private:
    std::string partialCommand;
    int m_prints = 0; //only debugging

};

