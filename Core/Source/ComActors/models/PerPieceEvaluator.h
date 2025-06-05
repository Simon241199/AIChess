#pragma once
#include "TorchPch.h"
#include <vector>
#include <filesystem>

struct PerPieceEvaluatorImpl : torch::nn::Module {
	torch::Tensor square_values; // only separate for readability. Collapses with piece_weights because
	torch::Tensor piece_weights; // there is no activation function between them.

	PerPieceEvaluatorImpl() {
		square_values = register_parameter("square_values", torch::ones({ 12, 8, 8 }));
		piece_weights = register_parameter("piece_weights", torch::tensor(std::vector<float>{ 1, 3, 3, 5, 9, 0, -1, -3, -3, -5, -9, 0 }));
	}

	torch::Tensor forward(torch::Tensor x) {
		auto weighted_squares = x * square_values; // instead of adding a layer one can simply add an activation function here
		auto per_piece_sum = weighted_squares.sum({ 2, 3 });
		auto evaluation = (per_piece_sum * piece_weights).sum(1);
		auto white_prob = torch::sigmoid(evaluation);
		auto black_prob = 1.0 - white_prob;

		auto output = torch::stack({ white_prob, black_prob }, 1);
		return evaluation;
	}
};

TORCH_MODULE(PerPieceEvaluator);

PerPieceEvaluator loadPerPieceEvaluator() {
	PerPieceEvaluator model;
	if (std::filesystem::exists("resources/PerPieceEvaluator.pt")) {
		std::cout << "loaded" << std::endl;
		torch::load(model, "resources/PerPieceEvaluator.pt");
	}
	//std::cout << (100*model->square_values * model->piece_weights.view({ 12, 1, 1 })).to(torch::kLong).rot90(1, { 1,2 }) << std::endl;
	return model;
}

void saveToFile(PerPieceEvaluator model) {
	torch::save(model, "resources/PerPieceEvaluator.pt");
}