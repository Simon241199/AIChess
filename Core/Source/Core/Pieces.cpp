#include "Pieces.h"

namespace Core {
	Piece WhiteOf(PieceType t)
	{
		switch (t)
		{
		case PieceType::Pawn:
			return Piece::WhitePawn;
		case PieceType::Knight:
			return Piece::WhiteKnight;
		case PieceType::Bishop:
			return Piece::WhiteBishop;
		case PieceType::Rook:
			return Piece::WhiteRook;
		case PieceType::Queen:
			return Piece::WhiteQueen;
		case PieceType::King:
			return Piece::WhiteKing;
		};
		return Piece::None;;
	}

	Piece BlackOf(PieceType t)
	{
		switch (t)
		{
		case PieceType::Pawn:
			return Piece::BlackPawn;
		case PieceType::Knight:
			return Piece::BlackKnight;
		case PieceType::Bishop:
			return Piece::BlackBishop;
		case PieceType::Rook:
			return Piece::BlackRook;
		case PieceType::Queen:
			return Piece::BlackQueen;
		case PieceType::King:
			return Piece::BlackKing;
		};
		return Piece::None;;
	}

	PieceType PieceTypeOf(Piece p)
	{
		switch (p)
		{
		case Piece::WhitePawn:
		case Piece::BlackPawn:
			return PieceType::Pawn;
		case Piece::WhiteKnight:
		case Piece::BlackKnight:
			return PieceType::Knight;
		case Piece::WhiteBishop:
		case Piece::BlackBishop:
			return PieceType::Bishop;
		case Piece::WhiteRook:
		case Piece::BlackRook:
			return PieceType::Rook;
		case Piece::WhiteQueen:
		case Piece::BlackQueen:
			return PieceType::Queen;
		case Piece::WhiteKing:
		case Piece::BlackKing:
			return PieceType::King;
		}
		return PieceType::None;
	}

	bool SameType(Piece p1, Piece p2)
	{
		return IsPawn(p1) && IsPawn(p2)
			|| IsKnight(p1) && IsKnight(p2)
			|| IsBishop(p1) && IsBishop(p2)
			|| IsRook(p1) && IsRook(p2)
			|| IsQueen(p1) && IsQueen(p2)
			|| IsKing(p1) && IsKing(p2);
	}

	bool IsNone(Piece p)
	{
		return p == Piece::None;
	}

	bool IsPawn(Piece p)
	{
		return p == Piece::WhitePawn || p == Piece::BlackPawn;
	}

	bool IsKnight(Piece p)
	{
		return p == Piece::WhiteKnight || p == Piece::BlackKnight;
	}

	bool IsBishop(Piece p)
	{
		return p == Piece::WhiteBishop || p == Piece::BlackBishop;
	}

	bool IsRook(Piece p)
	{
		return p == Piece::WhiteRook || p == Piece::BlackRook;
	}

	bool IsQueen(Piece p)
	{
		return p == Piece::WhiteQueen || p == Piece::BlackQueen;
	}

	bool IsKing(Piece p)
	{
		return p == Piece::WhiteKing || p == Piece::BlackKing;
	}

	bool IsWhite(Piece p)
	{
		switch (p) {
		case Piece::WhitePawn:
		case Piece::WhiteKnight:
		case Piece::WhiteBishop:
		case Piece::WhiteRook:
		case Piece::WhiteQueen:
		case Piece::WhiteKing:
			return true;
		}
		return false;
	}

	bool IsBlack(Piece p)
	{
		switch (p) {
		case Piece::BlackPawn:
		case Piece::BlackKnight:
		case Piece::BlackBishop:
		case Piece::BlackRook:
		case Piece::BlackQueen:
		case Piece::BlackKing:
			return true;
		}
		return false;
	}

	bool IsSameColor(Piece p1, Piece p2)
	{
		return IsWhite(p1) && IsWhite(p2) || IsBlack(p1) && IsBlack(p2);
	}

	char getFenChar(Piece p)
	{
		switch (p) {
		case Piece::WhitePawn:
			return 'P';
		case Piece::WhiteKnight:
			return 'N';
		case Piece::WhiteBishop:
			return 'B';
		case Piece::WhiteRook:
			return 'R';
		case Piece::WhiteQueen:
			return 'Q';
		case Piece::WhiteKing:
			return 'K';
		case Piece::BlackPawn:
			return 'p';
		case Piece::BlackKnight:
			return 'n';
		case Piece::BlackBishop:
			return 'b';
		case Piece::BlackRook:
			return 'r';
		case Piece::BlackQueen:
			return 'q';
		case Piece::BlackKing:
			return 'k';
		}
		return ' ';
	}

	Piece getPieceFromFenChar(char c)
	{
		switch (c) {
		case 'P':
			return Piece::WhitePawn;
		case 'N':
			return Piece::WhiteKnight;
		case 'B':
			return Piece::WhiteBishop;
		case 'R':
			return Piece::WhiteRook;
		case 'Q':
			return Piece::WhiteQueen;
		case 'K':
			return Piece::WhiteKing;
		case 'p':
			return Piece::BlackPawn;
		case 'n':
			return Piece::BlackKnight;
		case 'b':
			return Piece::BlackBishop;
		case 'r':
			return Piece::BlackRook;
		case 'q':
			return Piece::BlackQueen;
		case 'k':
			return Piece::BlackKing;
		}
		return Piece::None;
	}

	bool movesStraight(Piece p)
	{
		return IsRook(p) || IsQueen(p);
	}

	bool movesDiagonal(Piece p)
	{
		return IsBishop(p) || IsQueen(p);
	}
};