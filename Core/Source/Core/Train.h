#pragma once
#include "Board.h"
#include <string>

#include "TorchPch.h"
#include "Utility.h"

namespace Core {
	torch::Tensor encodeBoard(const Board& b);
	torch::Tensor encodeChessGame(const ChessGame& game);
};
