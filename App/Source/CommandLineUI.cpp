#include "CommandLineUI.h"
#include <iostream>

static bool compatibility = true;

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

std::string toAscii(Core::Piece p) {
	switch (p)
	{
	case Core::Piece::WhitePawn:
		return "P";
	case Core::Piece::WhiteKnight:
		return "N";
	case Core::Piece::WhiteBishop:
		return "B";
	case Core::Piece::WhiteRook:
		return "R";
	case Core::Piece::WhiteQueen:
		return "Q";
	case Core::Piece::WhiteKing:
		return "K";
	case Core::Piece::BlackPawn:
		return "p";
	case Core::Piece::BlackKnight:
		return "n";
	case Core::Piece::BlackBishop:
		return "b";
	case Core::Piece::BlackRook:
		return "r";
	case Core::Piece::BlackQueen:
		return "q";
	case Core::Piece::BlackKing:
		return "k";
	}
	return " ";
}

// print using ANSI escape code
void DisplayColored(const Core::Board& b)
{
	std::cout << std::endl;
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
			if (isWhite(p)) {
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

void DisplayCompatibility(const Core::Board& b)
{
	std::cout << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	for (int rank = 8; rank >= 1; rank--) {
		std::cout << "| ";
		for (char file = 'a'; file <= 'h'; file++) {
			std::cout << toAscii(b.get(file, rank)) << " | ";
		}
		std::cout << rank << std::endl;
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	}
	std::cout << "  a   b   c   d   e   f   g   h" << std::endl;
}

void askCompatibility()
{
	DisplayColored(Core::Board());
	std::cout << std::endl;
	std::cout << "If the above chessboard is displayed correctly, you can remain in colour mode by entering \"y\" otherwise enter \"n\" to activate compatibility mode."<<std::endl;
	std::cout << "Is the chessboard displayed correctly? [y/n]: ";
	std::string str;
	std::cin >> str;
	if (str.contains("y")) {
		compatibility = false;
	}
	else {
		compatibility = true;
	}
}

void Display(const Core::Board& b)
{
	if (compatibility) {
		DisplayCompatibility(b);
	}
	else {
		DisplayColored(b);
	}
}
