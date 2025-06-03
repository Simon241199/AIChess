#include "SubsymbolicCom.h"

#include <iostream>
#include <filesystem>

#include "Utility.h"
#include "AiModels.h"

#include <map>

namespace Core {

	SubsymbolicCom::SubsymbolicCom()
	{
	}
	SubsymbolicCom::~SubsymbolicCom()
	{
	}
	Move SubsymbolicCom::getMove(const Board& b)
	{
		Board board = b;
		auto moves = board.getMoves();

		std::multimap<int, Move> sortedMoves;

		for (Move m : moves) {
			board.make(m);

			sortedMoves.insert({ evaluateCombined(board),m });

			board.undo();
		}
		return sortedMoves.begin()->second;
	}

};
