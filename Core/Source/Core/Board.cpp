#include "Board.h"
#include <ranges>
#include <vector>
#include <iostream>

namespace Core {
	Board::Board(const std::string& fen) {
		auto view_split = std::views::split(fen, ' ');
		auto view_it = view_split.begin();
		std::string_view placments(*(view_it++));
		bool isWhitesTurn = ((*(view_it++))[0] == 'w');
		std::string_view castlingRights(*(view_it++));
		std::string_view enpassentSquare(*(view_it++));
		std::string_view halfmoveClockView(*(view_it++));
		std::string_view fullmoveClock(*(view_it++));

		auto rows = std::views::split(placments, '/');
		int8_t r = 8;
		for (const auto& row : rows) {
			char f = 'a';
			for (char c : row) {
				if (std::isdigit(c)) {
					f += c - '0';
					continue;
				}
				this->set(f, r, getPieceFromFenChar(c));
				f++;
			}
			r--;
		}
	}

	Piece Board::get(char file, int8_t rank) const
	{
		return this->get(Position::FromFileRank(file, rank));
	}

	void Board::set(char file, int8_t rank, Piece p)
	{
		this->set(Position::FromFileRank(file, rank), p);
	}

	Piece Board::get(Position pos) const
	{
		return this->pieces[pos.Index()];
	}

	void Board::set(Position pos, Piece p)
	{
		this->pieces[pos.Index()] = p;
		this->setAllZero(pos);
		getBitboardRef(p) |= pos.Bitboard();
	}

	std::vector<Move> Board::getMoves() const
	{
		return std::vector<Move>();
	}

	uint64_t& Board::getBitboardRef(Piece p)
	{
		switch (p)
		{
		case Piece::WhitePawn:
			return this->whitePawns;
		case Piece::WhiteKnight:
			return this->whiteKnights;
		case Piece::WhiteBishop:
			return this->whiteBishops;
		case Piece::WhiteRook:
			return this->whiteRooks;
		case Piece::WhiteQueen:
			return this->whiteQueens;
		case Piece::WhiteKing:
			return this->whiteKings;
		case Piece::BlackPawn:
			return this->blackPawns;
		case Piece::BlackKnight:
			return this->blackKnights;
		case Piece::BlackBishop:
			return this->blackBishops;
		case Piece::BlackRook:
			return this->blackRooks;
		case Piece::BlackQueen:
			return this->blackQueens;
		case Piece::BlackKing:
			return this->blackKings;
		}
		static uint64_t NoneBitboard = 0;
		return NoneBitboard;
	}

	void Board::setAllZero(Position pos)
	{
		uint64_t mask = ~pos.Bitboard();

		this->whitePawns &= mask;
		this->whiteKnights &= mask;
		this->whiteBishops &= mask;
		this->whiteRooks &= mask;
		this->whiteQueens &= mask;
		this->whiteKings &= mask;

		this->blackPawns &= mask;
		this->blackKnights &= mask;
		this->blackBishops &= mask;
		this->blackRooks &= mask;
		this->blackQueens &= mask;
		this->blackKings &= mask;
	}

	uint64_t Board::getBitboard(Piece p) const
	{
		switch (p)
		{
		case Piece::WhitePawn:
			return this->whitePawns;
		case Piece::WhiteKnight:
			return this->whiteKnights;
		case Piece::WhiteBishop:
			return this->whiteBishops;
		case Piece::WhiteRook:
			return this->whiteRooks;
		case Piece::WhiteQueen:
			return this->whiteQueens;
		case Piece::WhiteKing:
			return this->whiteKings;
		case Piece::BlackPawn:
			return this->blackPawns;
		case Piece::BlackKnight:
			return this->blackKnights;
		case Piece::BlackBishop:
			return this->blackBishops;
		case Piece::BlackRook:
			return this->blackRooks;
		case Piece::BlackQueen:
			return this->blackQueens;
		case Piece::BlackKing:
			return this->blackKings;
		}
		return 0;
	}

	uint64_t Board::getWhiteBitboard() const
	{
		return this->whitePawns
			| this->whiteKnights
			| this->whiteBishops
			| this->whiteRooks
			| this->whiteQueens
			| this->whiteKings;
	}

	uint64_t Board::getBlackBitboard() const
	{
		return this->blackPawns
			| this->blackKnights
			| this->blackBishops
			| this->blackRooks
			| this->blackQueens
			| this->blackKings;
	}
}