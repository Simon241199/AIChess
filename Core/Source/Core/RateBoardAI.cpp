#include "RateBoardAI.h"
#include <torch/torch.h>

float Core::AIRating(Board& board)
{
	torch::Tensor tensor = torch::rand({ 2, 3 });
	std::cout << tensor << std::endl;
	return 0.0f;
}
