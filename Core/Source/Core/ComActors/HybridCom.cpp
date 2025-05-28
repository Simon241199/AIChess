#include "HybridCom.h"
#include "Core/TorchPch.h"

namespace Core {
	Move HybridCom::getMove(const Board& b)
	{
		return b.getMoves()[0];
	}
};