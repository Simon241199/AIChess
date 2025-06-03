#include "Train.h"

#include <iostream>
#include <fstream>

namespace Core {

	torch::Tensor encodeBoard(const Board& b) {
		torch::Tensor out = torch::zeros({ 8, 8 }, torch::dtype(torch::kInt8));
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {

				Piece relativePiece = b.get(Position(x, y));
				if (b.getWhoseTurn() == PieceColor::Black) {
					Piece tempPiece = b.get(Position(x, 7 - y));
					relativePiece = pieceFrom(pieceTypeOf(tempPiece), invert(colorOf(tempPiece)));
				}

				switch (relativePiece)
				{
				case Core::Piece::None:
					out[x][y] = -1;
					break;
				case Core::Piece::WhitePawn:
					out[x][y] = 0;
					break;
				case Core::Piece::WhiteKnight:
					out[x][y] = 1;
					break;
				case Core::Piece::WhiteBishop:
					out[x][y] = 2;
					break;
				case Core::Piece::WhiteRook:
					out[x][y] = 3;
					break;
				case Core::Piece::WhiteQueen:
					out[x][y] = 4;
					break;
				case Core::Piece::WhiteKing:
					out[x][y] = 5;
					break;
				case Core::Piece::BlackPawn:
					out[x][y] = 6;
					break;
				case Core::Piece::BlackKnight:
					out[x][y] = 7;
					break;
				case Core::Piece::BlackBishop:
					out[x][y] = 8;
					break;
				case Core::Piece::BlackRook:
					out[x][y] = 9;
					break;
				case Core::Piece::BlackQueen:
					out[x][y] = 10;
					break;
				case Core::Piece::BlackKing:
					out[x][y] = 11;
					break;
				default:
					break;
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

	std::pair<torch::Tensor, torch::Tensor> encodeChessGame(const ChessGame& game)
	{
		Board b;
		std::vector<torch::Tensor> encodedBoards;
		std::vector<int> labels;
		for (std::string moveStr : game.moves) {
			Move move = getMoveFromNotation(b, moveStr);

			/*std::vector<Move> moves = b.getMoves();
			if (std::find(moves.begin(), moves.end(), move) == moves.end()) {
				std::cout << "Warning " << move.toString() << " " << moveStr << std::endl;
			}*/

			b.make(move);

			encodedBoards.push_back(encodeBoard(b));
			PieceColor perspectiveWinner = game.winner;
			if (b.getWhoseTurn() == PieceColor::Black) {
				perspectiveWinner = invert(perspectiveWinner);
			}
			switch (perspectiveWinner)
			{
			case Core::PieceColor::White:
				labels.push_back(0);
				break;
			case Core::PieceColor::None:
				labels.push_back(1);
				break;
			case Core::PieceColor::Black:
				labels.push_back(2);
				break;
			}
		}

		return std::make_pair(std::move(torch::stack(encodedBoards, 0)), std::move(torch::tensor(labels, torch::dtype(torch::kLong))));
	}

	torch::Tensor unpackChannelsData(const torch::Tensor& in) // [ CPUCharType{n,8,8} ] -> [ CPUFloatType{n,12,8,8} ]
	{
		torch::Tensor mask = in >= 0;

		torch::Tensor x_clamped = torch::clamp(in, 0); // [ n,8,8 ]

		torch::Tensor out = torch::nn::functional::one_hot(x_clamped.to(torch::kLong), 12).to(torch::kFloat); // [ CPUFloatType{n,8,8,12} ]

		out *= mask.unsqueeze(-1).to(torch::kFloat); // [ CPUFloatType{n,8,8,12} ]

		return out.permute({ 0,3,1,2 });
	}

	torch::Tensor unpack2ChannelsLabel(torch::Tensor in)
	{
		in.index_put_({ in == 1 }, -1);
		in.index_put_({ in == 2 }, 1);

		torch::Tensor mask = in >= 0;
		torch::Tensor drawMask = in == -1;

		torch::Tensor x_clamped = torch::clamp(in, 0); // [ n,8,8 ]

		torch::Tensor out = torch::nn::functional::one_hot(x_clamped.to(torch::kLong), 2).to(torch::kFloat); // [ CPUFloatType{n,8,8,12} ]

		return out * mask.unsqueeze(-1).to(torch::kFloat) + torch::ones_like(out) * 0.5 * drawMask.unsqueeze(-1).to(torch::kFloat);
	}
	void logEncodedBoard(torch::Tensor t)
	{
		torch::Tensor indices = -torch::ones({ 8,8 });
		float materialWhite = 0;
		float materialBlack = 0;
		for (int i = 0; i < 12; i++) {
			float* material = &materialWhite;
			if (i >= 6) {
				material = &materialBlack;
			}
			switch (i % 6)
			{
			case 0:
				*material += 1 * (t[i].sum()).item<float>();
				break;
			case 1:
				*material += 3 * (t[i].sum()).item<float>();
				break;
			case 2:
				*material += 3 * (t[i].sum()).item<float>();
				break;
			case 3:
				*material += 5 * (t[i].sum()).item<float>();
				break;
			case 4:
				*material += 9 * (t[i].sum()).item<float>();
				break;
			}
			indices += t[i] * (i + 1);
		}
		std::cout << "+---+---+---+---+---+---+---+---+\n";
		for (int x = 0; x < 8; x++) {
			std::cout << "| ";
			for (int y = 0; y < 8; y++) {
				switch (indices[x][y].item<int>()) {
				case 0:
					std::cout << "P";
					break;
				case 1:
					std::cout << "N";
					break;
				case 2:
					std::cout << "B";
					break;
				case 3:
					std::cout << "R";
					break;
				case 4:
					std::cout << "Q";
					break;
				case 5:
					std::cout << "K";
					break;
				case 6:
					std::cout << "p";
					break;
				case 7:
					std::cout << "n";
					break;
				case 8:
					std::cout << "b";
					break;
				case 9:
					std::cout << "r";
					break;
				case 10:
					std::cout << "q";
					break;
				case 11:
					std::cout << "k";
					break;
				default:
					std::cout << " ";
					break;
				}
				std::cout << " | ";
			}
			std::cout << "\n+---+---+---+---+---+---+---+---+\n";
		}
		std::cout << "white: " << materialWhite << "\t black:: " << materialBlack << "\t balance: " << (materialWhite - materialBlack) << std::endl;
	}
};