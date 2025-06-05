#pragma once
#include "TorchPch.h"
#include <filesystem>

struct CombinedEvaluatorImpl : torch::nn::Module {

	torch::nn::Sequential convolutionSequence{ nullptr };
	torch::Tensor square_values; // only separate for readability. Collapses with piece_weights because
	torch::Tensor piece_weights; // there is no activation function between them.

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
		auto initPW = torch::tensor(std::vector<float>{ 100, 300, 300, 500, 900, 0, -100, -300, -300, -500, -900, 0 });
		piece_weights = register_parameter("piece_weights", initPW);

		register_module("convolutionSequence", convolutionSequence);
	}

	torch::Tensor forward(torch::Tensor x) {
		auto weighted_squares = x * square_values; // instead of adding a layer one can simply add an activation function here
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
	//std::cout << (model->square_values * model->piece_weights.view({ 12, 1, 1 })).to(torch::kLong).rot90(1, {1,2}) << std::endl;
	return model;
}

void saveToFile(CombinedEvaluator model) {
	torch::save(model, "resources/CombinedEvaluator.pt");
}
