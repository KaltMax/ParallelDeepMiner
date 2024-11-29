#include "VisualsAndUtility.hpp"

void VisualsAndUtility::clearScreen(){
    #ifdef _WIN32
            system("cls");
    #else
            // Assume POSIX
            system("clear");
    #endif
}

void VisualsAndUtility::enterToContinue() {
    cout << "\nPress ENTER to continue!" << endl;
    cin.clear();
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    cin.get(); // Warten auf die Eingabetaste
}

void VisualsAndUtility::enterToContinue2() {
    cout << "\nPress ENTER to continue!" << endl;
    cin.get(); // Warten auf die Eingabetaste
}

void VisualsAndUtility::printTitle() {
    cout << "                                   " << "______               _ _      _ " << endl;
    cout << "                                   " << "| ___ \\             | | |    | |" << endl;
    cout << "                                   " << "| |_/ /_ _ _ __ __ _| | | ___| |" << endl;
    cout << "                                   " << "|  __/ _` | '__/ _` | | |/ _ \\ |" << endl;
    cout << "                                   " << "| | | (_| | | | (_| | | |  __/ |" << endl;
    cout << "                                   " << "\\_|  \\__,_|_|  \\__,_|_|_|\\___|_|" << endl;

    cout << "         __                ______                 ___  ____                             __\n"
        " _(\\    |@@|               |  _  \\                |  \\/  (_)                        _  |@@|\n"
        "(__/\\__ \\--/ __            | | | |___  ___ _ __   | .  . |_ _ __   ___ _ __        / \\ \\--/ __           \n"
        "   \\___|----|  |   __      | | | / _ \\/ _ \\ |_ \\  | |\\/| | | '_ \\ / _ \\ |__|       ) O|----|  |   __            \n"
        "       \\ }{ /\\ )_ / _\\     | |/ /  __/  __/ |_) | | |  | | | | | |  __/ |         / / \\ }{ /\\ )_ / _\\           \n"
        "       /\\__/\\ \\__O (__     |___/ \\___|\\___| |__/  \\_|  |_/_|_| |_|\\___|_|         )/  /\\__/\\" " \\__O (__           \n"
        "      (--/\\--)    \\__/                    | |                                    |/  (--/\\--)    \\__/           \n"
        "      _)(  )(_                            |_|                                    /   _)(  )(_           \n"
        "     `---''---`                                                                     `---''---`      \n" << endl;
    enterToContinue2();
    clearScreen();
}

void VisualsAndUtility::printGoodbye() {
    cout << "Goodbye and thank you for playing!" << endl;
    enterToContinue2();
}