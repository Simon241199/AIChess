#pragma once
#include <cstdint>

namespace Core {
	enum class Piece : uint8_t {
		None,

		WhitePawn,
		WhiteKnight,
		WhiteBishop,
		WhiteRook,
		WhiteQueen,
		WhiteKing,

		BlackPawn,
		BlackKnight,
		BlackBishop,
		BlackRook,
		BlackQueen,
		BlackKing,
	};

	enum class PieceType : uint8_t {
		None = 0,
		Pawn = 1,
		Knight = 2,
		Bishop = 3,
		Rook = 4,
		Queen = 5,
		King = 6,
	};

	Piece WhiteOf(PieceType t);
	Piece BlackOf(PieceType t);
	PieceType PieceTypeOf(Piece p);

	bool SameType(Piece p1, Piece p2);

	bool IsNone(Piece p);
	bool IsPawn(Piece p);
	bool IsKnight(Piece p);
	bool IsBishop(Piece p);
	bool IsRook(Piece p);
	bool IsQueen(Piece p);
	bool IsKing(Piece p);

	bool IsWhite(Piece p);
	bool IsBlack(Piece p);

	bool IsSameColor(Piece p1, Piece p2);

	char getFenChar(Piece p);

	Piece getPieceFromFenChar(char c);

	bool movesStraight(Piece p);
	bool movesDiagonal(Piece p);
};