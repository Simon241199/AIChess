#include "Utility.h"

#include "Core/Train.h"
#include "Core/TorchPch.h"

#include <iostream>
#include <random>
#include <fstream>

namespace Core {

	int countMoves(Board& b, int remainingDepth)
	{
		if (remainingDepth <= 0) {
			return 1;
		}
		int count = 0;
		std::vector<Move> moves = b.getMoves();

		for (int i = 0; i < moves.size(); i++) {
			b.make(moves[i]);
			count += countMoves(b, remainingDepth - 1);
			b.undo();
		}
		return count;
	}

	int countMovesDebug(Board& b, int remainingDepth)
	{
		if (remainingDepth <= 0) {
			return 1;
		}
		int count = 0;
		std::vector<Move> moves = b.getMoves();

		for (int i = 0; i < moves.size(); i++) {
			b.make(moves[i]);
			int currentCount = countMoves(b, remainingDepth - 1);
			std::cout << i << " " << moves[i].toString() << " " << currentCount << std::endl;
			count += currentCount;
			b.undo();
		}
		return count;
	}

	Move getRandomMove(const Board& b)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::vector<Core::Move> moves = b.getMoves();
		int i = std::uniform_int_distribution<>(0, moves.size() - 1)(gen);
		return moves[i];
	}

	void logBitboard(uint64_t bitboard) {
		std::cout << "----------" << std::endl;
		for (size_t i = 0; i < 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				std::cout << (bitboard & 1);
				bitboard = bitboard >> 1;
			}
			std::cout << std::endl;
		}
	}

	std::vector<std::string> split(const std::string& str, char delimiter) {
		std::vector<std::string> tokens;

		std::string token = "";
		bool inQuotes = false;

		for (char c : str) {
			if (token.empty() && c == '\"') {
				inQuotes = true;
				continue;
			}
			if (inQuotes && c == '\"') {
				inQuotes = false;
				continue;
			}
			if (c == delimiter && !inQuotes) {
				tokens.push_back(token);
				token = "";
				continue;
			}
			token += c;
		}
		tokens.push_back(token);

		return tokens;
	}

	ChessGame readCsvLine(const std::string& str) {

		std::vector<std::string> tokens = split(str, ',');

		std::vector<std::string> numberedMoves = split(tokens[14], ' ');
		std::vector<std::string> moves;
		for (std::string numberedMove : numberedMoves) {
			if (numberedMove.length() == 0) {
				continue;
			}
			if (numberedMove.back() == '.' || numberedMove[0] == '{' || numberedMove[0] == '[' || numberedMove.back() == ']' || numberedMove.back() == '}') {
				continue;
			}
			if (numberedMove == "1-0" || numberedMove == "1/2-1/2" || numberedMove == "0-1") {
				continue;
			}
			moves.push_back(numberedMove);
		}
		int time = -1;
		int increment = 0;
		if (tokens[12] != "-" && !tokens[12].empty()) {
			std::vector<std::string> timeSetting = split(tokens[12], '+');
			time = std::stoi(timeSetting[0]);
			increment = 0;
			if (timeSetting.size() == 2) {
				increment = std::stoi(timeSetting[1]);
			}
		}

		PieceColor winner = PieceColor::None;
		if (tokens[3] == "1-0") {
			winner = PieceColor::White;
		}
		else if (tokens[3] == "0-1") {
			winner = PieceColor::Black;
		}
		else if (tokens[3] == "1/2-1/2") {
			winner = PieceColor::None;
		}
		else {
			std::cout << tokens[3] << std::endl;
		}

		return ChessGame{
			tokens[0],
			tokens[1],
			tokens[2],
			winner,
			tokens[4],
			tokens[5],
			std::stoi(tokens[6]),
			std::stoi(tokens[7]),
			std::stof("0" + tokens[8]),
			std::stof("0" + tokens[9]),
			tokens[10],
			tokens[11],
			time,
			increment,
			tokens[13],
			moves,
		};
	}

	std::vector<ChessGame> getNormalGames(int N)
	{
		std::ifstream inputFile("C:/Users/simon/source/repos/AIChess/Core/Source/Core/ComActors/resources/chess_games.csv");


		std::string line;

		std::getline(inputFile, line);
		std::cout << line << std::endl;

		std::vector<ChessGame> games;

		int i = 0;
		while (std::getline(inputFile, line)) {
			ChessGame game = readCsvLine(line);
			if (game.termination != "Normal") {
				continue;
			}
			games.push_back(game);
			i++;
			if (i >= N) {
				break;
			}
		}

		inputFile.close();
		return games;
	}

	void test() {
		Board b;

		torch::Tensor out = encodeBoard(b);

		std::cout << out << std::endl;  // Should print: [12, 8, 8]
	}
}
