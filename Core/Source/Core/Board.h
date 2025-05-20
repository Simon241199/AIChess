#pragma once
#include <array>
#include "Pieces.h"
#include <string>
#include <vector>
#include "Move.h"
#include "Position.h"


namespace Core {

	const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	class Board
	{
	public:
		Board(const std::string& fen = START_FEN);
		Piece get(char file, int8_t rank) const;
		void set(char file, int8_t rank, Piece p);
		Piece get(Position pos) const;
		void set(Position pos, Piece p);
		std::string getFen() const;
		std::vector<Move> getMoves() const;
		uint64_t getBitboard(Piece p) const;
		uint64_t getWhiteBitboard() const;
		uint64_t getBlackBitboard() const;
	private:
		uint64_t& getBitboardRef(Piece p);
		void setAllZero(Position pos);

		std::array<Piece, 64> pieces;

		uint64_t whitePawns;
		uint64_t whiteKnights;
		uint64_t whiteBishops;
		uint64_t whiteRooks;
		uint64_t whiteQueens;
		uint64_t whiteKings;

		uint64_t blackPawns;
		uint64_t blackKnights;
		uint64_t blackBishops;
		uint64_t blackRooks;
		uint64_t blackQueens;
		uint64_t blackKings;
	};
}