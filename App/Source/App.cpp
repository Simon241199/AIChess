#ifdef _WIN32
#include <windows.h>
#endif

#include "Core/Board.h"
#include "CommandLineUI.h"
#include <iostream>
#include <Core/Position.h>


int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // Konsole auf UTF-8 Codepage umstellen
#endif
    Core::Board b;
    Display(b);

    return 0;
}