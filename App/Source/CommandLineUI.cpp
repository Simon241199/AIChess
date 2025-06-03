#include "CommandLineUI.h"
#include <iostream>
#include <vector>
#include <ComActors/SubsymbolicCom.h>
#include <ComActors/SymbolicCom.h>
#include <ComActors/NeuroSymbolicCom.h>
#include <Player.h>
#include <Pieces.h>

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

// print using ANSI escape code
void displayColored(const Core::Board& b)
{
	std::cout << std::endl;
	std::cout << "\033[2J\033[H"; // erase screen and reset cursor position
	for (int r = 8; r >= 1; r--) {
		int rank = (b.getWhoseTurn() == Core::PieceColor::White) ? r : (9 - r);
		for (char f = 'a'; f <= 'h'; f++) {
			char file = (b.getWhoseTurn() == Core::PieceColor::White) ? f : ('h' - (f - 'a'));
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
		std::cout << r << std::endl;
	}
	for (char f = 'a'; f <= 'h'; f++) {
		char file = (b.getWhoseTurn() == Core::PieceColor::White) ? f : ('h' - (f - 'a'));
		std::cout << file << " ";
	}
	std::cout << std::endl;
}

void displayCompatibility(const Core::Board& b)
{
	std::cout << std::endl;
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	for (int rank = 8; rank >= 1; rank--) {
		std::cout << "| ";
		for (char file = 'a'; file <= 'h'; file++) {
			std::cout << Core::toAscii(b.get(file, rank)) << " | ";
		}
		std::cout << rank << std::endl;
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	}
	std::cout << "  a   b   c   d   e   f   g   h" << std::endl;
}

void askCompatibility()
{
	displayColored(Core::Board());
	std::cout << std::endl;
	std::cout << "If the above chessboard is displayed correctly, you can remain in colour mode by entering \"y\" otherwise enter \"n\" to activate compatibility mode." << std::endl;
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

std::unique_ptr<Core::Actor> getActor()
{
	do {
		std::cout << "Your answer [b/n/s/u]: ";
		std::string str;
		std::cin >> str;
		if (str.length() == 1) {
			switch (str[0]) {
			case 'b':
				return std::make_unique<Core::SubsymbolicCom>();
			case 'n':
				return std::make_unique<Core::NeuroSymbolicCom>();
			case 's':
				return std::make_unique<Core::SymbolicCom>();
			case 'u':
				return std::make_unique<Player>();
			}
		}
		std::cout << "Invalid input!" << std::endl;
	} while (true);
}

std::pair<std::unique_ptr<Core::Actor>, std::unique_ptr<Core::Actor>> getMatchUp()
{
	std::cout << std::endl;
	std::cout << "Should white be a SubsymbolicCom [b], a NeuroSymbolicCom [n], a SymbolicCom [s] or a user-controlled player [u]?" << std::endl;
	std::unique_ptr<Core::Actor> whiteActor = getActor();

	std::cout << std::endl;
	std::cout << "Should black be a SubsymbolicCom [b], a NeuroSymbolicCom [n], a SymbolicCom [s] or a user-controlled player [u]?" << std::endl;
	std::unique_ptr<Core::Actor> blackActor = getActor();

	return std::pair<std::unique_ptr<Core::Actor>, std::unique_ptr<Core::Actor>>(std::move(whiteActor), std::move(blackActor));
}

void display(const Core::Board& b)
{
	if (compatibility) {
		displayCompatibility(b);
	}
	else {
		displayColored(b);
	}
}

Core::Move getUserMove(const Core::Board& b)
{
	std::cout << "Enter one of these possible moves: ";
	std::vector<Core::Move> moves = b.getMoves();
	std::vector<std::string> movesStrings;
	for (int i = 0; i < moves.size() - 1; i++) {
		std::cout << moves[i].toString() << ", ";
		movesStrings.push_back(moves[i].toString());
	}
	std::cout << moves[moves.size() - 1].toString() << std::endl;
	movesStrings.push_back(moves[moves.size() - 1].toString());

	int i;
	do {
		std::string input;
		std::cout << "Your move: ";
		std::cin >> input;
		auto it = std::find(movesStrings.begin(), movesStrings.end(), input);
		if (it != movesStrings.end()) {
			i = it - movesStrings.begin();
			break;
		}
		std::cout << "Invalid move!" << std::endl;
	} while (true);

	return moves[i];
}
