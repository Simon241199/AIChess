#include "Utility.h"

#include "Train.h"
#include "TorchPch.h"

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

	std::vector<ChessGame> getNormalGames(int N, int offset)
	{
		std::ifstream inputFile("resources/chess_games.csv");


		std::string line;

		std::getline(inputFile, line);
		std::cout << line << std::endl;

		std::vector<ChessGame> games;

		int i = 0;
		while (std::getline(inputFile, line)) {
			if (i < offset) {
				i++;
				continue;
			}
			ChessGame game = readCsvLine(line);
			if (game.termination != "Normal") {
				continue;
			}
			games.push_back(game);
			i++;
			if (i >= N + offset) {
				break;
			}
		}

		inputFile.close();
		return games;
	}

	void convertCsvGamesFile()
	{
		std::ifstream inputFile("resources/chess_games.csv");


		std::string line;

		std::getline(inputFile, line);
		std::cout << line << std::endl;

		bool done = false;
		for (int i = 0; !done; i++) {
			std::vector<torch::Tensor> dataVector;
			std::vector<torch::Tensor> labelVector;
			for (int j = 0; j < 5000;) {
				if (!std::getline(inputFile, line)) {
					done = true;
					break;
				}
				ChessGame game = readCsvLine(line);
				if (game.termination != "Normal") {
					continue;
				}
				j++;
				auto [data, label] = encodeChessGame(game);
				dataVector.push_back(data);
				labelVector.push_back(label);
			}
			torch::Tensor dataTensor = torch::cat(dataVector, 0);
			torch::Tensor labelTensor = torch::cat(labelVector, 0);
			std::cout << i << "\n" << dataTensor.sizes() << "\n" << labelTensor.sizes() << "\n";
			torch::save(dataTensor, std::format("resources/processedData/{}_data.pt", i));
			torch::save(labelTensor, std::format("resources/processedData/{}_label.pt", i));
		}
		inputFile.close();
	}

	void convertCsvPositionsFile()
	{
		std::ifstream inputFile("resources/tactic_evals.csv");

		std::string line;
		std::getline(inputFile, line);

		std::vector<float> ratings;
		std::vector<torch::Tensor> positions;
		int i = 0;
		while (std::getline(inputFile, line)) {
			std::vector<std::string> cells = split(line, ',');
			Board b(cells[0]);
			positions.push_back(encodeBoard(b));
			float rating = 0.0;
			if (cells[1].contains('#')) {
				unsigned long movesToMate = std::stoul(cells[1].substr(2));
				rating = 10'000.0 - float(movesToMate);
				if (cells[1].contains('-')) {
					rating *= -1;
				}
			}
			else {
				rating = std::stof(cells[1]);
			}
			if (b.getWhoseTurn() == PieceColor::White) {
				ratings.push_back(rating);
			}
			else {
				ratings.push_back(-rating);
			}
			i++;
			if ((i%10'000) == 0) {
				std::cout << line << std::endl;
				std::cout << ratings.back() << std::endl;
				std::cout << positions.back() << std::endl;
				std::cout << i << std::endl;
			}
		}

		inputFile.close();

		torch::save(torch::stack(positions, 0), "resources/tacticEvalsPositions.pt");
		torch::save(torch::tensor(ratings), "resources/tacticEvalsRatings.pt");
	}

	void test() {
		{
			torch::Tensor trainData;
			torch::load(trainData, std::format("resources/processedData/{}_data.pt", 1));
			torch::Tensor trainLabels;
			torch::load(trainLabels, std::format("resources/processedData/{}_label.pt", 1));

			auto trainDataset = TensorDataset(unpackChannelsData(trainData), unpack2ChannelsLabel(trainLabels)).map(torch::data::transforms::Stack<>());
			auto trainLoader = torch::data::make_data_loader(std::move(trainDataset), 128);

			for (auto& batch : *trainLoader) {
				auto data = batch.data;
				auto targets = batch.target;
				for (int i = 0; i < 10; i++) {
					logEncodedBoard(data[i]);
					std::cout << targets[i] << std::endl;
				}
				break;
			}
		}
		torch::Tensor x = torch::tensor({ 0, 1, 2, 0, 2 }, torch::dtype(torch::kInt64));
		std::cout << x << std::endl;
		torch::Tensor y = unpack2ChannelsLabel(x);
		std::cout << y << std::endl;

		Board b;
		{
			torch::Tensor boardTensor = encodeBoard(b);

			torch::Tensor boardsTensor = boardTensor.unsqueeze(0);

			std::cout << boardsTensor << std::endl;  // Should print: [1, 8, 8]
			std::cout << unpackChannelsData(boardsTensor) << std::endl;  // Should print: [1, 12, 8, 8]
		}
		b.make(b.getMoves()[0]);
		{
			torch::Tensor boardTensor = encodeBoard(b);

			torch::Tensor boardsTensor = boardTensor.unsqueeze(0);

			std::cout << boardsTensor << std::endl;  // Should print: [1, 8, 8]
			std::cout << unpackChannelsData(boardsTensor) << std::endl;  // Should print: [1, 12, 8, 8]
		}
	}
}
