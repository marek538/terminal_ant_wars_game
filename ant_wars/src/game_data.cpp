#include "game_data.h"

using namespace std;
//max of 3 gaming entities   "$,%,&"
//# is unbreakable wall
//anthouses can't be adjacent, corner to corner is good (x,y and x+1,y+1)
bool CGameMap::loadMap(const string &inFile)
{
    m_path = inFile;
    m_gamePlan.clear();
    m_gamePlan.emplace_back(vector<char>{});
    ifstream input(inFile);

    uint lineSize = 0;

    if(!input.good())
    {
        throw invalid_argument("not reachable file\n");
    }

    char ch;

    while(input.read(&ch,1))
    {
        if(!input.good())
            throw invalid_argument("not reachable file\n");

        if(!validateCharInMap(ch, &lineSize))
            throw invalid_argument("wrong map layout\n");
    }

    m_gamePlan.pop_back();

    if(!validateObjectsPosition())
        throw invalid_argument("wrong map layout\n");

    transferToPlayableMap();

    return true;
}

//# stands for a wall
//@ stands for an anthouse wall
//' ' stands for free space
//there are 3 chars in the house, 1 for id, 2 for number of ants
//checks if the frame (except the last line) is correctly written
//checks if the map is rectangle
bool CGameMap::validateCharInMap(const char ch, uint *lineSize)
{
    //too large
    if(m_gamePlan.size() > 49)
        return false;

    if(m_gamePlan.empty() || m_gamePlan.size() == 1)
    {
        if(ch == '\n')
        {
            *lineSize = m_gamePlan[0].size();

            //too large
            if(*lineSize > 50)
                return false;

            m_gamePlan.emplace_back();
        }
        else if(ch == '#')
            m_gamePlan[0].push_back(ch);
        else return false;
    }
    else if ((m_gamePlan[m_gamePlan.size()-1].empty() || m_gamePlan[m_gamePlan.size()-1].size() == *lineSize - 1) && ch != '#')
    {
        return false;
    }
    else
    {
        checkChar(ch, lineSize);
    }

    if(m_gamePlan[m_gamePlan.size()-1].size() > 50)
        return false;

    return true;
}

bool CGameMap::checkChar(const char ch, const uint *lineSize)
{
    if(ch == '#' || ch == '@' || ch == '%' || ch == '&' || ch == ' ' || isdigit(ch) || isalpha(ch))
    {
        m_gamePlan[m_gamePlan.size()-1].push_back(ch);
    }
    else if (ch == '\n')
    {
        if(m_gamePlan[m_gamePlan.size()-1].size() != *lineSize)
        {
            return false;
        }
        m_gamePlan.emplace_back();
    }
    else
        return false;
    return true;
}

//validate anthome positioning and walls and not colliding objects
//validate last row!!! == ####
bool CGameMap::validateObjectsPosition()
{
    for(char i : m_gamePlan[m_gamePlan.size()-1])
    {
        if(i != '#')
            return false;
    }

//go char by char, if I find @, try, if there is one above or to the left, else check whole anthome
    for(uint i = 1; i < m_gamePlan.size() -1 ; i++)//don't have to check last line
    {
        for(uint j = 1; j < m_gamePlan[0].size() -1 ; j++)//don't have to check the corners
        {
            if(m_gamePlan[i][j] == '@')
            {
                if(!(m_gamePlan[i-1][j] == '@' || m_gamePlan[i][j-1] == '@'))
                {
                    if(!validateAntHouse(i,j, '@'))
                        return false;
                }
            }
            else if(m_gamePlan[i][j] == '&')
            {
                if(!(m_gamePlan[i-1][j] == '&' || m_gamePlan[i][j-1] == '&'))
                {
                    if(!validateAntHouse(i,j, '&'))
                        return false;
                }
            }
            else if(m_gamePlan[i][j] == '%')
            {
                if(!(m_gamePlan[i-1][j] == '%' || m_gamePlan[i][j-1] == '%'))
                {
                    if(!validateAntHouse(i,j, '%'))
                        return false;
                }
            }
            else if( isdigit(m_gamePlan[i][j]) || isalpha(m_gamePlan[i][j]))
            {
                if(m_gamePlan[i-1][j] != '@' && m_gamePlan[i-1][j] != '&' &&  m_gamePlan[i-1][j] != '%')
                    return false;
            }

        }
    }

    return true;
}

bool CGameMap::validateAntHouse(uint posx, uint posy, char border)
{
    if (m_gamePlan.size() + 1 <= posx + 2 || m_gamePlan[0].size() + 1 <= posy + 4)
        return false;

    //first row
    for (uint i = 0; i < 5; i++)
    {
        if (m_gamePlan[posx][posy + i] != border)
            return false;
    }
    if (m_gamePlan[posx][posy + 5] != ' ' && m_gamePlan[posx][posy + 5] != '#')
        return false;


    //second row
    if (m_gamePlan[posx + 1][posy] != border)
        return false;

    //id char
    if (!isalpha(m_gamePlan[posx + 1][posy + 1]))
        return false;

    if(m_allIdList.find(m_gamePlan[posx + 1][posy + 1]) != std::string::npos)
        throw invalid_argument("duplicate id\n");

    m_allIdList.push_back((m_gamePlan[posx + 1][posy + 1]));

    //1st number
    if (!isdigit(m_gamePlan[posx + 1][posy + 2]))
        return false;

    //    2nd number
    if (!isdigit(m_gamePlan[posx + 1][posy + 3]))
        return false;

    if (m_gamePlan[posx + 1][posy + 4] != border)
        return false;

    if (m_gamePlan[posx + 1][posy + 5] != ' ' && m_gamePlan[posx + 1][posy + 5] != '#')
        return false;

    //3rd row
    for (uint i = 0; i < 5; i++)
    {
        if (m_gamePlan[posx + 2][posy + i] != border)
            return false;

    }

    if (m_gamePlan[posx + 2][posy + 5] != ' ' && m_gamePlan[posx + 2][posy + 5] != '#')
        return false;

    //no adjacent homes
    for (uint i = 0; i < 5; i++)
    {
        if (m_gamePlan[posx + 3][posy + i] != ' ' && m_gamePlan[posx + 3][posy + i] != '#')
            return false;

    }
    return true;
}

bool CGameMap::transferToPlayableMap()
{
    bool firstNumber = true;
    for(uint i = 0; i < m_gamePlan.size(); i++)
    {
        m_playableMap.emplace_back();
        for(uint j = 0; j < m_gamePlan[0].size(); j++)
        {

            if(isdigit(m_gamePlan[i][j]) && firstNumber)
            {
                firstNumber = false;
                //int x, int y, char playerId, char houseId, int numberOfAnts)

                int count = (m_gamePlan[i][j]-48) * 10 + (m_gamePlan[i][j+1]-48);

                shared_ptr<CAntHouse> h = make_shared<CAntHouse>(i, j, m_gamePlan[i][j-2], m_gamePlan[i][j-1], count, m_baseRadius, m_baseHealth, m_baseAttack);
                m_playableMap[m_playableMap.size() - 1].push_back(h);
            }
            else
            {
                firstNumber = true;
                m_playableMap[m_playableMap.size() - 1].push_back(make_shared<CRegularPlace>(i, j, m_gamePlan[i][j]));
            }
        }
    }
    return true;
}

string CGameMap::printPlayableMap()
{
    string output;
    for(auto & row : m_playableMap)
    {
        for(uint j = 0; j < m_playableMap[0].size(); j++)
        {
            if(isalpha(row[j]->getOriginalSquare()))
            {
                output.push_back(row[j]->getOriginalSquare());
                output.push_back(char(row[j + 1]->getNumberOfAnts() / 10 + 48));
                output.push_back(char(row[j + 1]->getNumberOfAnts() % 10 + 48));
                j+=2;
            }
            else if(row[j]->getOccupant() == nullptr)
                output.push_back(row[j]->getOriginalSquare());
            else
                output.push_back(row[j]->getOccupant()->printName());
        }
        output.push_back('\n');
    }

    return output;
}

CGameMap::CGameMap(int radius, int baseHealth, int baseAttack) : m_baseRadius(radius), m_baseHealth(baseHealth), m_baseAttack(baseAttack) {}

void CGameMap::refactorAntHouse(int xCoordinates, int yCoordinates, char newOwner)
{
    for(int i = yCoordinates - 2; i <= yCoordinates + 2 ; i++)
        m_playableMap[xCoordinates - 1][i]->setType(newOwner);

    m_playableMap[xCoordinates][yCoordinates - 2]->setType(newOwner);
    m_playableMap[xCoordinates][yCoordinates + 2]->setType(newOwner);

    for(int i = yCoordinates - 2; i <= yCoordinates + 2 ; i++)
        m_playableMap[xCoordinates + 1][i]->setType(newOwner);
}

string CGameMap::getPath() const
{
    return m_path;
}

void CGameMap::setPath(string &path)
{
    m_path = path;
}

std::string CGameMap::getAllIdList()
{
    return m_allIdList;
}



bool CConfiguration::loadConfiguration(const string &inFile)
{
    m_path = inFile;
    ifstream input(inFile);
    char ch;
    int currentSize = 0;
    string line;
    int linePos = 0;

    if(!input.good())
        throw invalid_argument("not reachable file\n");

    while(input.read(&ch,1))
    {
        if(!input.good())
            throw invalid_argument("not reachable file\n");

        if(ch == '\n')
        {
            if(!validateConfigLine(linePos, line))
                throw invalid_argument("wrong file layout\n");

            currentSize = 0;
            linePos++;
            line = "";
        }
        else
        {
            line.push_back(ch);
            currentSize++;
        }

        if(currentSize > 25)
            throw invalid_argument("wrong file layout\n");

    }

    if(linePos != 10)
        throw invalid_argument("wrong file layout\n");

    return true;
}

int CConfiguration::checkLabel(string &base, string &original)
{
    if(original.length() <= base.length())
        return -1;

    if (original.substr(0, base.size()) != base)
        return -1;

    int result = 0;

    uint i = base.length();
    while(i < original.length())
    {
        if(original[i] - 48 < 0 || original[i] - 48 > 9)
            return -1;

        result = result * 10 + (original[i] - 48);
        i++;
    }

    if(result > 100 || result == 0)
        return -1;
    return result;
}

/*
#define turnsToGenerateAnts 10
#define numberToUpgrade 20
#define numberToPromote 10
#define botToughness 3  */
bool CConfiguration::validateConfigLine(int linePos, string &original)
{
    string base;
    switch (linePos)
    {
        case 0:            
            base = "base attack:";

            m_attack = checkLabel(base, original);
            if(m_attack < 0)
                return false;
            
            return true;

        case 1:
            base = "base health:";

            m_health = checkLabel(base, original);
            if(m_health < 0)
                return false;

            return true;

        case 2:
            base = "base radius:";

            m_radius = checkLabel(base, original);
            if(m_radius < 0 || m_radius >= 10)
                return false;

            return true;

        case 3:
            base = "base attackbonus:";

            m_attackBonus = checkLabel(base, original);
            if(m_attackBonus < 0 )
                return false;

            return true;

        case 4:
            base = "base healthbonus:";

            m_healthBonus = checkLabel(base, original);
            if(m_healthBonus < 0 )
                return false;

            return true;

        case 5:
            base = "base radiusbonus:";

            m_radiusBonus = checkLabel(base, original);
            if(m_radiusBonus < 0 || m_radiusBonus >= 5)
                return false;

            return true;

        case 6:
            base = "turnsToGenerateAnts:";
            m_turnsToGenerateAnts = checkLabel(base, original);
            if(m_turnsToGenerateAnts < 0 || m_turnsToGenerateAnts >= 20)
                return false;

            return true;

        case 7:
            base = "numberToUpgrade:";
            m_numberToUpgrade  = checkLabel(base, original);
            if(m_numberToUpgrade < 0 || m_numberToUpgrade >= 30)
                return false;

            return true;

        case 8:
            base = "numberToPromote:";
            m_numberToPromote = checkLabel(base, original);
            if(m_numberToPromote < 0 || m_numberToPromote >= 30)
                return false;

            return true;


        case 9:
            base = "botToughness:";
            m_botToughness = checkLabel(base, original);
            if(m_botToughness < 2 || m_botToughness > 4)
                return false;

            return true;


        default:
            return false;
    }
    
}

int CConfiguration::getAttack() const
{
    return m_attack;
}

int CConfiguration::getHealth() const
{
    return m_health;
}

int CConfiguration::getRadius() const
{
    return m_radius;
}

int CConfiguration::getAttackBonus() const
{
    return m_attackBonus;
}

int CConfiguration::getHealthBonus() const
{
    return m_healthBonus;
}

int CConfiguration::getRadiusBonus() const
{
    return m_radiusBonus;
}

bool CConfiguration::saveGame(const string &configurationPath, const string &mapPath, const string &savePath,
                              const string &commands)
{
    ofstream of;
    of.open(savePath, ios::trunc);
    if(!of.good())
        return false;

    of << configurationPath << "\n" << mapPath << "\n" << commands;
    of.close();
    return true;
}

string CConfiguration::getPath() const
{
    return m_path;
}

string CConfiguration::parseSaveCommand(string &lineFromSave, bool *correct)
{
    bool wasDigit = false;
    bool wasDoubleDot = false;
    bool wasSpace = false;

    string parsed;
    uint i = 0;
    for(;i < lineFromSave.size(); i++)
    {
        if(isdigit(lineFromSave[i]))
            wasDigit = true;

        if(lineFromSave[i] == ':')
        {
            wasDoubleDot = true;
            break;
        }
    }
    i++;
    if(i < lineFromSave.size() && lineFromSave[i] == ' ')
        wasSpace = true;

    if(!(wasDoubleDot && wasDigit && wasSpace))
    {
        *correct = false;
        return "badInput";
    }

    i++;
    parsed = lineFromSave.substr(i, lineFromSave.size() - i);
    return parsed;
}

int CConfiguration::getTurnsToGenerate() const
{
    return m_turnsToGenerateAnts;
}

int CConfiguration::getNumberToUpgrade() const
{
    return m_numberToUpgrade;
}

int CConfiguration::getNumberToPromote() const
{
    return m_numberToPromote;
}

int CConfiguration::getBotToughness() const
{
    return m_botToughness;
}
