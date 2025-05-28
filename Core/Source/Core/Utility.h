#pragma once
#include "Board.h"
#include <vector>

namespace Core {
	struct ChessGame {
		std::string event;
		std::string userWhite;
		std::string userBlack;
		PieceColor winner;
		std::string utcDate;
		std::string utcTime;
		int whiteElo;
		int blackElo;
		float whiteRatingDiff;
		float blackRatingDiff;
		std::string eco;
		std::string opening;
		int timeInSeconds;
		int incrementInSeconds;
		std::string termination;
		std::vector<std::string> moves;
	};
	ChessGame readCsvLine(const std::string& str);

	std::vector<ChessGame> getNormalGames(int N = 50000);

	void test();

	int countMovesDebug(Board& b, int remainingDepth);

	int countMoves(Board& b, int remainingDepth);

	Move getRandomMove(const Board& b);

	void logBitboard(uint64_t bitboard);
};