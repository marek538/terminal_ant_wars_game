#include "user_interface.h"


using namespace std;

CWholeWindow::CWholeWindow() = default;

CWholeWindow::~CWholeWindow() = default;

void CWholeWindow::print(const string &input)
{
    cout << input << endl;
}


string CWholeWindow::getMapLocation()
{
    cout << "enter location of your map or type 1-5 for default map" << endl;
    string s;
    getline(cin,s);
    system("clear");
    return s;
}

string CWholeWindow::playFromSave()
{
    cout << "if you want to play from save, choose 1-5 else press any buttons (not power button pls) and enter" << endl;
    string s;
    getline(cin,s);;
    system("clear");

    return s;
}

string CWholeWindow::getConfigLocation()
{

    cout << "enter location of your configuration or type 1-3 for default ones" << endl;
    string s;
    getline(cin,s);
    system("clear");

    return s;
}

void CWholeWindow::errPrint(const string &input)
{
    cerr << input << endl;
}

string CWholeWindow::readCommand()
{
    string s;
    getline(cin, s);

    return s;
}

void CWholeWindow::printOptions(std::vector<std::shared_ptr<CAbstractBonus>> &bonuses)
{
    cout << "enter to continue cycle\n"
            "XaY -attack from X to Y\n";

    for(auto &bonus : bonuses)
        cout << bonus->printUserOption() << "\n";

    cout << "promXc -promote first ant in X to captain\n"
            "promXb -promote first ant in X to bomber\n"
            "promXm -promote first ant in X to medic\n"
            "save -save game\n"
            "exit -exit \n\n" << endl;
}

