#pragma once
#include "Board.h"
#include <string>

#include "TorchPch.h"
#include "Utility.h"

namespace Core {
	torch::Tensor encodeBoard(const Board& b);
	std::pair<torch::Tensor, torch::Tensor> encodeChessGame(const ChessGame& game);

	torch::Tensor unpackChannelsData(const torch::Tensor& in);

	torch::Tensor unpack2ChannelsLabel(torch::Tensor in);

	void logEncodedBoard(torch::Tensor t);

	struct TensorDataset : torch::data::datasets::Dataset<TensorDataset> {
		torch::Tensor data, labels;

		TensorDataset(torch::Tensor data, torch::Tensor labels)
			: data(data), labels(labels) {
		}

		torch::data::Example<> get(size_t index) override {
			return { data[index], labels[index] };
		}

		torch::optional<size_t> size() const override {
			return data.size(0);
		}
	};
};
