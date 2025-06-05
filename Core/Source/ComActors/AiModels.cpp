#include "AiModels.h"

#include "Board.h"
#include "TorchPch.h"
#include "Train.h"
#include "models/PerPieceEvaluator.h"
#include "models/ConvEvaluator.h"
#include "models/CombinedEvaluator.h"

namespace Core {
	int evaluateCombined(const Board& b)
	{
		static CombinedEvaluator model = loadCombinedEvaluator();

		model->eval();
		torch::NoGradGuard no_grad;

		torch::Tensor encodedBoard = unpackChannelsData(encodeBoard(b).unsqueeze(0));
		auto output = model->forward(encodedBoard);

		float rating = output[0].item<float>();
		return int(rating);
	}

	int evaluatePerPiece(const Board& b)
	{
		static PerPieceEvaluator model = loadPerPieceEvaluator();

		model->eval();
		torch::NoGradGuard no_grad;

		torch::Tensor encodedBoard = unpackChannelsData(encodeBoard(b).unsqueeze(0));
		auto output = model->forward(encodedBoard);

		float rating = 100*output[0].item<float>();
		return int(rating);
	}

	void trainAndSave()
	{
		torch::Tensor data;
		torch::load(data, "resources/chessDataPositions.pt");
		std::cout << data.sizes() << std::endl;

		torch::Tensor labels;
		torch::load(labels, "resources/chessDataRatings.pt");
		std::cout << labels.sizes() << std::endl;

		torch::Tensor permutation = torch::randperm(labels.size(0), torch::kLong);

		int trainSize = permutation.size(0) * 0.8;

		torch::Tensor trainPerm = permutation.slice(0, 0, trainSize);
		std::cout << trainPerm.sizes() << std::endl;
		torch::Tensor testPerm = permutation.slice(0, trainSize);
		std::cout << testPerm.sizes() << std::endl;

		torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
		std::cout << device << std::endl;
		auto model = loadCombinedEvaluator();
		std::cout << model->square_values << std::endl;
		std::cout << model->piece_weights << std::endl;
		model->to(device);
		model->train();

		auto lossFunction = torch::nn::MSELoss();
		torch::optim::Adam optimizer(model->parameters(), torch::optim::AdamOptions(1e-3));
		size_t num_epochs = 2;
		bool show = true;
		float summedLoss = 0.0;
		int lossCount = 0;

		torch::Tensor trainData = data.index_select(0, trainPerm);
		std::cout << "trainData.sizes(): " << trainData.sizes() << std::endl;
		torch::Tensor trainLabels = labels.index_select(0, trainPerm);
		std::cout << "trainLabels.sizes(): " << trainLabels.sizes() << std::endl;
		auto trainDataset = TensorDataset(trainData, trainLabels).map(torch::data::transforms::Stack<>());
		auto trainLoader = torch::data::make_data_loader(std::move(trainDataset), 64);
		for (size_t epoch = 1; epoch <= num_epochs; ++epoch) {
			size_t batch_index = 0;
			for (auto& batch : *trainLoader) {
				auto data = unpackChannelsData(batch.data).to(device);
				auto targets = batch.target.to(device);

				optimizer.zero_grad();
				auto output = model->forward(data);

				if (show) {
					show = false;
					logEncodedBoard(data[0]);
					std::cout << "data: " << data.sizes() << std::endl;
					std::cout << "targets: " << targets.sizes() << std::endl;
					std::cout << "output: " << output.sizes() << std::endl;
				}

				auto loss = lossFunction(output, targets);

				loss.backward();
				optimizer.step();

				summedLoss += loss.item<float>();
				lossCount++;
				if (batch_index++ % 150 == 0) {
					std::cout << "Epoch [" << epoch << "/" << num_epochs << "], Batch [" << batch_index << "], Loss: " << (summedLoss / std::max(1.0f, float(lossCount))) << std::endl;
					summedLoss = 0;
					lossCount = 0;
				}
			}
			saveToFile(model);
		}
		model->eval();
		torch::NoGradGuard no_grad;
		float totalError = 0;
		size_t evaluationCount = 0;

		auto testDataset = TensorDataset(unpackChannelsData(data.index_select(0, testPerm)), labels.index_select(0, testPerm)).map(torch::data::transforms::Stack<>());
		auto testLoader = torch::data::make_data_loader(std::move(testDataset), 128);
		for (const auto& batch : *testLoader) {
			auto output = model->forward(batch.data);
			auto target = batch.target;

			totalError += (output - target).pow(2).sum().item<float>();
			evaluationCount += target.size(0);
		}
		std::cout << "Accuracy: " << (totalError / evaluationCount) << std::endl;

		saveToFile(model);
	}
};