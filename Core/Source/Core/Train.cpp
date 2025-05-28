#include "Train.h"

#include <iostream>
#include <fstream>

namespace Core {

	torch::Tensor encodeBoard(const Board& b) {
		torch::Tensor out = torch::zeros({ 8, 8, 12 });
		if (b.getWhoseTurn() == PieceColor::White) {
			for (int x = 0; x < 8; x++) {
				for (int y = 0; y < 8; y++) {
					switch (b.get(Position(x, y)))
					{
					case Core::Piece::WhitePawn:
						out[x][y][0] = 1.0;
						break;
					case Core::Piece::WhiteKnight:
						out[x][y][1] = 1.0;
						break;
					case Core::Piece::WhiteBishop:
						out[x][y][2] = 1.0;
						break;
					case Core::Piece::WhiteRook:
						out[x][y][3] = 1.0;
						break;
					case Core::Piece::WhiteQueen:
						out[x][y][4] = 1.0;
						break;
					case Core::Piece::WhiteKing:
						out[x][y][5] = 1.0;
						break;
					case Core::Piece::BlackPawn:
						out[x][y][6] = 1.0;
						break;
					case Core::Piece::BlackKnight:
						out[x][y][7] = 1.0;
						break;
					case Core::Piece::BlackBishop:
						out[x][y][8] = 1.0;
						break;
					case Core::Piece::BlackRook:
						out[x][y][9] = 1.0;
						break;
					case Core::Piece::BlackQueen:
						out[x][y][10] = 1.0;
						break;
					case Core::Piece::BlackKing:
						out[x][y][11] = 1.0;
						break;
					default:
						break;
					}
				}
			}
		}
		else {
			for (int x = 0; x < 8; x++) {
				for (int y = 7; y >= 0; y--) {
					switch (b.get(Position(x, y)))
					{
					case Core::Piece::BlackPawn:
						out[x][y][0] = 1.0;
						break;
					case Core::Piece::BlackKnight:
						out[x][y][1] = 1.0;
						break;
					case Core::Piece::BlackBishop:
						out[x][y][2] = 1.0;
						break;
					case Core::Piece::BlackRook:
						out[x][y][3] = 1.0;
						break;
					case Core::Piece::BlackQueen:
						out[x][y][4] = 1.0;
						break;
					case Core::Piece::BlackKing:
						out[x][y][5] = 1.0;
						break;
					case Core::Piece::WhitePawn:
						out[x][y][6] = 1.0;
						break;
					case Core::Piece::WhiteKnight:
						out[x][y][7] = 1.0;
						break;
					case Core::Piece::WhiteBishop:
						out[x][y][8] = 1.0;
						break;
					case Core::Piece::WhiteRook:
						out[x][y][9] = 1.0;
						break;
					case Core::Piece::WhiteQueen:
						out[x][y][10] = 1.0;
						break;
					case Core::Piece::WhiteKing:
						out[x][y][11] = 1.0;
						break;
					default:
						break;
					}
				}
			}
		}
		return out;
	}

	Move getMoveFromNotation(const Board& b, std::string moveStr) {
		std::string originalMove = moveStr;

		bool isCapture = moveStr.contains("x");
		bool isCheck = moveStr.contains("+");
		bool isCheckmate = moveStr.contains("#");
		bool isPromotion = moveStr.contains("=");

		std::erase_if(moveStr, [](const char& c) {
			return c == 'x' || c == '+' || c == '#' || c == '=' || c == '!' || c == '?';
			});

		if (moveStr.contains("-")) { // castle
			int8_t fromX = 4;
			int8_t y = (b.getWhoseTurn() == PieceColor::White) ? 0 : 7;
			int8_t toX = (moveStr.length() == std::string("O-O-O").length()) ? 2 : 6; // queenside vs kingside castle
			return Move(Position(fromX, y), Position(toX, y), false);
		}

		Piece movingPiece = pieceFrom(PieceType::Pawn, b.getWhoseTurn());
		if (std::string("PNBRQK").contains(moveStr[0])) {
			movingPiece = pieceFrom(pieceTypeOf(getPieceFromFenChar(moveStr[0])), b.getWhoseTurn());
			moveStr = moveStr.substr(1);
		}

		PieceType promotionType = PieceType::None;
		if (!isdigit(moveStr.back())) {
			promotionType = pieceTypeOf(getPieceFromFenChar(moveStr.back()));
			if (!isPawn(movingPiece) || promotionType == PieceType::None) {
				std::cout << moveStr << std::endl;
			}
			moveStr = moveStr.substr(0, moveStr.length() - 1);
		}

		Position toPos = Position::fromFileRank(moveStr[moveStr.length() - 2], moveStr[moveStr.length() - 1] - '0');
		moveStr = moveStr.substr(0, moveStr.length() - 2);

		std::vector<Move> moves = b.getMoves();

		std::erase_if(moves, [isCapture, &b, movingPiece, toPos, promotionType](const Move& m) {
			return !m.isCapture() == isCapture
				|| b.get(m.getFrom()) != movingPiece
				|| m.getTo() != toPos
				|| m.isPromotion() == (promotionType == PieceType::None)
				|| m.getPromotionPiece() != promotionType;
			});

		while (!moveStr.empty()) {
			if (std::isdigit(moveStr[0])) {
				int8_t y = moveStr[0] - '1';
				std::erase_if(moves, [y](const Move& m) {
					return m.getFrom().y() != y;
					});
			}
			else {
				int8_t x = moveStr[0] - 'a';
				std::erase_if(moves, [x](const Move& m) {
					return m.getFrom().x() != x;
					});
			}
			moveStr = moveStr.substr(1);
		}

		if (moves.size() != 1) {
			std::cout << "OG: " << originalMove << "\t remaining: " << moveStr << std::endl;
		}

		return moves[0];
	}

	torch::Tensor encodeChessGame(const ChessGame& game)
	{
		Board b;
		std::vector<torch::Tensor> encodedBoards;
		for (std::string moveStr : game.moves) {
			Move move = getMoveFromNotation(b, moveStr);

			std::vector<Move> moves = b.getMoves();
			if (std::find(moves.begin(), moves.end(), move) == moves.end()) {
				std::cout << "Warning " << move.toString() << " " << moveStr << std::endl;
			}

			b.make(move);

			encodedBoards.push_back(encodeBoard(b));
		}

		return torch::stack(encodedBoards, 0);
	}
};