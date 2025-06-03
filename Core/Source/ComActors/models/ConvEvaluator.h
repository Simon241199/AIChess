#pragma once
#include "TorchPch.h"
#include <filesystem>

struct ConvEvaluatorImpl : torch::nn::Module {

	torch::nn::Sequential chessPred{ nullptr };

	ConvEvaluatorImpl() {

		chessPred = torch::nn::Sequential(
			torch::nn::Conv2d(torch::nn::Conv2dOptions(12, 12, 15).padding(7)),
			torch::nn::Tanh(),
			torch::nn::Conv2d(torch::nn::Conv2dOptions(12, 3, 15).padding(7)),
			torch::nn::Flatten(),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8 * 3, 8 * 8 * 3),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8 * 3, 8 * 8),
			torch::nn::Tanh(),
			torch::nn::Linear(8 * 8, 2)
		);

		register_module("chessPred", chessPred);
	}

	torch::Tensor forward(torch::Tensor x) {
		auto predicted = chessPred->forward(x);
		return predicted;
	}
};

TORCH_MODULE(ConvEvaluator);

ConvEvaluator loadConvEvaluator() {
	ConvEvaluator model;
	if (std::filesystem::exists("resources/ConvEvaluator.pt")) {
		std::cout << "loaded" << std::endl;
		torch::load(model, "resources/ConvEvaluator.pt");
	}
	return model;
}

void saveToFile(ConvEvaluator model) {
	torch::save(model, "resources/ConvEvaluator.pt");
}