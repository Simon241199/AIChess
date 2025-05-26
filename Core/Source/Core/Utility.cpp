#include "Utility.h"
#include <iostream>
#include <random>

namespace Core {

	int countMoves(Board& b, int remainingDepth)
	{
		if (remainingDepth <= 0) {
			return 1;
		}
		int count = 0;
		std::vector<Move> moves = b.getMoves();

		for (int i = 0; i < moves.size(); i++) {
			b.make(moves[i]);
			count += countMoves(b, remainingDepth - 1);
			b.undo();
		}
		return count;
	}

	int countMovesDebug(Board& b, int remainingDepth)
	{
		if (remainingDepth <= 0) {
			return 1;
		}
		int count = 0;
		std::vector<Move> moves = b.getMoves();

		for (int i = 0; i < moves.size(); i++) {
			b.make(moves[i]);
			int currentCount = countMoves(b, remainingDepth - 1);
			std::cout << i << " " << moves[i].toString() << " " << currentCount << std::endl;
			count += currentCount;
			b.undo();
		}
		return count;
	}

	Move getRandomMove(const Board& b)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::vector<Core::Move> moves = b.getMoves();
		int i = std::uniform_int_distribution<>(0, moves.size() - 1)(gen);
		return moves[i];
	}

	void logBitboard(uint64_t bitboard) {
		std::cout << "----------" << std::endl;
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				std::cout << (bitboard & 1);
				bitboard = bitboard >> 1;
			}
			std::cout << std::endl;
		}
	}
}
