#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "CommandLineUI.h"
#include "Player.h"
#include "Test.h"

#include <Position.h>
#include <MoveGenLookUp.h>
#include <Utility.h>
#include <Actor.h>
#include <ComActors/SymbolicCom.h>
#include <Board.h>
#include <ComActors/SubsymbolicCom.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <unordered_map>

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
		if (b.getMoves().size() == 0) {
			return;
		}
		Core::Move move = actors[b.getWhoseTurn()]->getMove(b);
		b.make(move);
	}
}

int main() {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);  // UTF-8 Codepage
#endif
	//Core::test();
	//Core::convertCsvPositionsFile();
	//Core::PureAI::trainAndSave();

	//Test::runAll();

	askCompatibility();

	while (true) {
		gameLoop();
	}

	return 0;
}