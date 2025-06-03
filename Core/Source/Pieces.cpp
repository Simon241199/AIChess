#include "Pieces.h"

namespace Core {
	Piece whiteOf(PieceType t)
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

	Piece blackOf(PieceType t)
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

	PieceColor colorOf(Piece p)
	{
		switch (p)
		{
		case Core::Piece::WhitePawn:
		case Core::Piece::WhiteKnight:
		case Core::Piece::WhiteBishop:
		case Core::Piece::WhiteRook:
		case Core::Piece::WhiteQueen:
		case Core::Piece::WhiteKing:
			return PieceColor::White;
		case Core::Piece::BlackPawn:
		case Core::Piece::BlackKnight:
		case Core::Piece::BlackBishop:
		case Core::Piece::BlackRook:
		case Core::Piece::BlackQueen:
		case Core::Piece::BlackKing:
			return PieceColor::Black;
		}
		return PieceColor::None;
	}

	PieceType pieceTypeOf(Piece p)
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

	Piece pieceFrom(PieceType t, PieceColor c)
	{
		switch (c)
		{
		case Core::PieceColor::White:
			return whiteOf(t);
		case Core::PieceColor::Black:
			return blackOf(t);
		}
		return Piece::None;
	}

	Piece pawnOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhitePawn;
		case PieceColor::Black:
			return Piece::BlackPawn;
		}
		return Piece::None;
	}

	Piece knightOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhiteKnight;
		case PieceColor::Black:
			return Piece::BlackKnight;
		}
		return Piece::None;
	}

	Piece bishopOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhiteBishop;
		case PieceColor::Black:
			return Piece::BlackBishop;
		}
		return Piece::None;
	}

	Piece rookOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhiteRook;
		case PieceColor::Black:
			return Piece::BlackRook;
		}
		return Piece::None;
	}

	Piece queenOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhiteQueen;
		case PieceColor::Black:
			return Piece::BlackQueen;
		}
		return Piece::None;
	}

	Piece kingOf(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return Piece::WhiteKing;
		case PieceColor::Black:
			return Piece::BlackKing;
		}
		return Piece::None;
	}

	bool sameType(Piece p1, Piece p2)
	{
		return isPawn(p1) && isPawn(p2)
			|| isKnight(p1) && isKnight(p2)
			|| isBishop(p1) && isBishop(p2)
			|| isRook(p1) && isRook(p2)
			|| isQueen(p1) && isQueen(p2)
			|| isKing(p1) && isKing(p2);
	}

	bool isNone(Piece p)
	{
		return p == Piece::None;
	}

	bool isPawn(Piece p)
	{
		return p == Piece::WhitePawn || p == Piece::BlackPawn;
	}

	bool isKnight(Piece p)
	{
		return p == Piece::WhiteKnight || p == Piece::BlackKnight;
	}

	bool isBishop(Piece p)
	{
		return p == Piece::WhiteBishop || p == Piece::BlackBishop;
	}

	bool isRook(Piece p)
	{
		return p == Piece::WhiteRook || p == Piece::BlackRook;
	}

	bool isQueen(Piece p)
	{
		return p == Piece::WhiteQueen || p == Piece::BlackQueen;
	}

	bool isKing(Piece p)
	{
		return p == Piece::WhiteKing || p == Piece::BlackKing;
	}

	bool isWhite(Piece p)
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

	bool isBlack(Piece p)
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

	bool isSameColor(Piece p1, Piece p2)
	{
		return isWhite(p1) && isWhite(p2) || isBlack(p1) && isBlack(p2);
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
		return isRook(p) || isQueen(p);
	}

	bool movesDiagonal(Piece p)
	{
		return isBishop(p) || isQueen(p);
	}

	PieceColor invert(PieceColor c)
	{
		switch (c)
		{
		case PieceColor::White:
			return PieceColor::Black;
		case PieceColor::Black:
			return PieceColor::White;
		}
		return PieceColor::None;
	}

	bool isSameColor(PieceColor c, Piece p)
	{
		return colorOf(p) == c;
	}

	char toAscii(Core::Piece p) {
		switch (p)
		{
		case Core::Piece::WhitePawn:
			return 'P';
		case Core::Piece::WhiteKnight:
			return 'N';
		case Core::Piece::WhiteBishop:
			return 'B';
		case Core::Piece::WhiteRook:
			return 'R';
		case Core::Piece::WhiteQueen:
			return 'Q';
		case Core::Piece::WhiteKing:
			return 'K';
		case Core::Piece::BlackPawn:
			return 'p';
		case Core::Piece::BlackKnight:
			return 'n';
		case Core::Piece::BlackBishop:
			return 'b';
		case Core::Piece::BlackRook:
			return 'r';
		case Core::Piece::BlackQueen:
			return 'q';
		case Core::Piece::BlackKing:
			return 'k';
		}
		return ' ';
	}
};