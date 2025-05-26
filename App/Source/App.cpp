#ifdef _WIN32
#include <windows.h>
#endif

#include "Core/Board.h"
#include "CommandLineUI.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <Core/Position.h>
#include <Core/MoveGenLookUp.h>
#include <Core/Utility.h>

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);  // Konsole auf UTF-8 Codepage umstellen
#endif

	Core::Board b;

	while (true)
	{
		Display(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

		Core::Move randomMove = Core::getRandomMove(b);
		b.make(randomMove);
	}

	return 0;
}