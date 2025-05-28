#ifdef _WIN32
#include <windows.h>
#endif

#include "Core/Board.h"
#include "CommandLineUI.h"
#include "Player.h"
#include "Test.h"

#include <Core/Position.h>
#include <Core/MoveGenLookUp.h>
#include <Core/Utility.h>
#include <Core/Actor.h>
#include <Core/ComActors/SymbolicCom.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <Core/ComActors/PureAI.h>

void gameLoop() {
	Core::Board b;

	std::unordered_map<Core::PieceColor, std::unique_ptr<Core::Actor>> actors;
	auto matchUp = getMatchUp();
	actors[Core::PieceColor::White] = std::move(matchUp.first);
	actors[Core::PieceColor::Black] = std::move(matchUp.second);

	while (true)
	{
		display(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (b.getMoves().size() == 0 && b.isInCheck(b.getWhoseTurn())) {
			return;
		}
		Core::Move move = actors[b.getWhoseTurn()]->getMove(b);
		b.make(move);
	}
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);  // Konsole auf UTF-8 Codepage umstellen
#endif

	Test::runAll();

	Core::PureAI ai;
	ai.trainAndSave();

	askCompatibility();

	while (true) {
		gameLoop();
	}

	return 0;
}