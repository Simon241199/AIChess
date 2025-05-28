#include "PureAI.h"

#include <iostream>

#include "Core/TorchPch.h"
#include <Core/Train.h>
#include <Core/Utility.h>

namespace Core {

	struct RatingAiImpl : torch::nn::Module {

		torch::nn::Sequential chessPred{ nullptr };

		RatingAiImpl() {

			chessPred = torch::nn::Sequential(
				torch::nn::Flatten(),
				torch::nn::Linear(8 * 8 * 12, 8 * 8 * 3),
				torch::nn::Dropout(0.1),
				torch::nn::Tanh(),
				torch::nn::Linear(8 * 8 * 3, 8 * 8),
				torch::nn::Dropout(0.1),
				torch::nn::Tanh(),
				torch::nn::Linear(8 * 8, 1),
				torch::nn::Sigmoid()
			);

			register_module("chessPred", chessPred);
		}

		torch::Tensor forward(torch::Tensor x) {
			auto predicted = chessPred->forward(x);
			return predicted;
		}
	};

	TORCH_MODULE(RatingAi);

	PureAI::PureAI()
	{
	}
	PureAI::~PureAI()
	{
	}
	Move PureAI::getMove(const Board& b)
	{
		return b.getMoves()[0];
	}

	void PureAI::trainAndSave()
	{
		std::vector<ChessGame> games = getNormalGames(5000);

		std::vector<torch::Tensor> trainDataVector;

		int i = 0;
		while (i < (games.size() * 4) / 5) {
			trainDataVector.push_back(encodeChessGame(games[i]));
			i++;
		}
		torch::Tensor trainData = torch::cat(trainDataVector, 0);

		std::vector<torch::Tensor> testDataVector;
		while (i < games.size()) {
			testDataVector.push_back(encodeChessGame(games[i]));
			i++;
		}
		torch::Tensor testData = torch::cat(testDataVector, 0);

		std::cout << "Trainings data " << trainData.sizes() << std::endl;
		std::cout << "Testing data " << testData.sizes() << std::endl;
	}
};
