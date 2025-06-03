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

	enum class PieceColor : uint8_t {
		None,
		White,
		Black,
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

	Piece whiteOf(PieceType t);
	Piece blackOf(PieceType t);
	PieceColor colorOf(Piece p);
	PieceType pieceTypeOf(Piece p);
	Piece pieceFrom(PieceType t,PieceColor c);

	Piece pawnOf(PieceColor c);
	Piece knightOf(PieceColor c);
	Piece bishopOf(PieceColor c);
	Piece rookOf(PieceColor c);
	Piece queenOf(PieceColor c);
	Piece kingOf(PieceColor c);

	bool sameType(Piece p1, Piece p2);

	bool isNone(Piece p);
	bool isPawn(Piece p);
	bool isKnight(Piece p);
	bool isBishop(Piece p);
	bool isRook(Piece p);
	bool isQueen(Piece p);
	bool isKing(Piece p);

	bool isWhite(Piece p);
	bool isBlack(Piece p);

	bool isSameColor(Piece p1, Piece p2);
	bool isSameColor(PieceColor c, Piece p);

	char getFenChar(Piece p);

	Piece getPieceFromFenChar(char c);

	bool movesStraight(Piece p);
	bool movesDiagonal(Piece p);

	PieceColor invert(PieceColor c);

	char toAscii(Core::Piece p);
};