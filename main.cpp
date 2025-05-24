#include "Storage.hpp"

#include "CppWld/main.cpp"
CppWld MainWin;

using namespace std;

int main() {
    consoleWindow = GetConsoleWindow();
    SetDllDirectory("libs");

    MainWin.setup();
    MainWin.run();

    return 0;
}
