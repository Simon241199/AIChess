#pragma once
#include <array>
#include "Pieces.h"
#include <string>
#include <vector>
#include "Move.h"
#include "Position.h"
#include <stack>


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
		std::vector<Move> getMoves() const;
		uint64_t getBitboard(Piece p) const;
		uint64_t getWhiteBitboard() const;
		uint64_t getBlackBitboard() const;
		uint64_t getBitboardOf(PieceColor color) const;
		uint64_t getOccupiedBitboard() const;
		bool isAttackedBy(Position pos, PieceColor color, uint64_t forcedFree = 0, uint64_t dummyBlockers = 0) const;
		bool isInCheck(PieceColor color) const;
		void make(Move m);
		void undo();
		void insertMoveIfOk(Move move, std::vector<Move>& moves) const;
	private:
		bool areClearAndNotAttackedBy(uint64_t bitboard, PieceColor attackerColor) const;
		uint64_t& getBitboardRef(Piece p);
		void setAllZero(Position pos);
		void updateCaslingRights(Move m);

		std::array<Piece, 64> pieces{ Piece::None };

		uint64_t whitePawns{ 0 };
		uint64_t whiteKnights{ 0 };
		uint64_t whiteBishops{ 0 };
		uint64_t whiteRooks{ 0 };
		uint64_t whiteQueens{ 0 };
		uint64_t whiteKings{ 0 };

		uint64_t blackPawns{ 0 };
		uint64_t blackKnights{ 0 };
		uint64_t blackBishops{ 0 };
		uint64_t blackRooks{ 0 };
		uint64_t blackQueens{ 0 };
		uint64_t blackKings{ 0 };


		PieceColor whoseTurn;
		char enPassentFile;
		std::stack<int> halfmoveClockStack;
		int fullmoveClock;
		struct CastlingRights {
			bool canWhiteQueenSideCastle : 1;
			bool canWhiteKingSideCastle : 1;
			bool canBlackQueenSideCastle : 1;
			bool canBlackKingSideCastle : 1;
		};
		std::stack<CastlingRights> castlingRightsHistory;
		std::stack<Move> moveHistory;
	};
}