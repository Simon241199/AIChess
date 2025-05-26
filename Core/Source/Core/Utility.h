#pragma once
#include "Board.h"

namespace Core {
	int countMovesDebug(Board& b, int remainingDepth);

	int countMoves(Board& b, int remainingDepth);

	Move getRandomMove(const Board& b);

	void logBitboard(uint64_t bitboard);
};