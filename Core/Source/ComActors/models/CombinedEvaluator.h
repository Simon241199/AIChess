#pragma once
#include "TorchPch.h"
#include <filesystem>

struct CombinedEvaluatorImpl : torch::nn::Module {

	torch::nn::Sequential convolutionSequence{ nullptr };
	torch::Tensor square_values;
	torch::Tensor piece_weights;

	CombinedEvaluatorImpl() {

		convolutionSequence = torch::nn::Sequential(
			torch::nn::Conv2d(torch::nn::Conv2dOptions(12, 12, 15).padding(7)),
			torch::nn::Tanh(),
			torch::nn::Conv2d(torch::nn::Conv2dOptions(12, 3, 15).padding(7)),
			torch::nn::Flatten(),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8 * 3, 8 * 8 * 3),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8 * 3, 8 * 8),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8, 1)
		);

		square_values = register_parameter("square_values", torch::ones({ 12, 8, 8 }));
		piece_weights = register_parameter("piece_weights", torch::tensor(std::vector<float>{ 100, 300, 300, 500, 900, 0, -100, -300, -300, -500, -900, 0 }));

		register_module("convolutionSequence", convolutionSequence);
	}

	torch::Tensor forward(torch::Tensor x) {
		auto weighted_squares = x * square_values;
		auto per_piece_sum = weighted_squares.sum({ 2, 3 });
		auto perPieceEvaluation = (per_piece_sum * piece_weights).sum(1);
		auto convolutionEvaluation = convolutionSequence->forward(x).squeeze(1);
		auto predicted = convolutionEvaluation + perPieceEvaluation;
		//std::cout << x.sizes() << per_piece_sum.sizes() << perPieceEvaluation.sizes() << convolutionEvaluation.sizes() << predicted.sizes() << std::endl;
		return predicted;
	}
};

TORCH_MODULE(CombinedEvaluator);

CombinedEvaluator loadCombinedEvaluator() {
	CombinedEvaluator model;
	if (std::filesystem::exists("resources/CombinedEvaluator.pt")) {
		std::cout << "loaded" << std::endl;
		torch::load(model, "resources/CombinedEvaluator.pt");
	}
	return model;
}

void saveToFile(CombinedEvaluator model) {
	torch::save(model, "resources/CombinedEvaluator.pt");
}
