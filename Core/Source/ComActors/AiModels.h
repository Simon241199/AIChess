#pragma once
#include <Board.h>
namespace Core {
	int evaluateCombined(const Board& b);
	int evaluatePerPiece(const Board& b);
	void trainAndSave();
};