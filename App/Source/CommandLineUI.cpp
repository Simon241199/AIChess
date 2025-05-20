#include "CommandLineUI.h"
#include <iostream>

std::string toUnicode(Core::Piece p) {
	switch (p)
	{
	case Core::Piece::WhitePawn:
		return std::string(reinterpret_cast<const char*>(u8"♙"));
	case Core::Piece::WhiteKnight:
		return std::string(reinterpret_cast<const char*>(u8"♘"));
	case Core::Piece::WhiteBishop:
		return std::string(reinterpret_cast<const char*>(u8"♗"));
	case Core::Piece::WhiteRook:
		return std::string(reinterpret_cast<const char*>(u8"♖"));
	case Core::Piece::WhiteQueen:
		return std::string(reinterpret_cast<const char*>(u8"♕"));
	case Core::Piece::WhiteKing:
		return std::string(reinterpret_cast<const char*>(u8"♔"));
	case Core::Piece::BlackPawn:
		return std::string(reinterpret_cast<const char*>(u8"♙"));
	case Core::Piece::BlackKnight:
		return std::string(reinterpret_cast<const char*>(u8"♞"));
	case Core::Piece::BlackBishop:
		return std::string(reinterpret_cast<const char*>(u8"♝"));
	case Core::Piece::BlackRook:
		return std::string(reinterpret_cast<const char*>(u8"♜"));
	case Core::Piece::BlackQueen:
		return std::string(reinterpret_cast<const char*>(u8"♛"));
	case Core::Piece::BlackKing:
		return std::string(reinterpret_cast<const char*>(u8"♚"));
	}
	return " ";
}

void Display(const Core::Board& b)
{
	std::cout << "\033[2J\033[H"; // erase screen and reset cursor position
	for (int rank = 8; rank >= 1; rank--) {
		for (char file = 'a'; file <= 'h'; file++) {
			if ((rank + file) % 2 == 0) {
				std::cout << "\033[48;5;22m"; // dark square
			}
			else {
				std::cout << "\033[48;5;34m"; // light square
			}
			Core::Piece p = b.get(file, rank);
			if (IsWhite(p)) {
				std::cout << "\033[38;5;15m"; // white piece
			}
			else {
				std::cout << "\033[38;5;0m"; // black piece
			}
			std::cout << toUnicode(b.get(file, rank)) << " \033[0m";
		}
		std::cout << std::endl;
	}
}
