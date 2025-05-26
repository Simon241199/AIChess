#include "HybridCom.h"

namespace Core {
	Move HybridCom::getMove(const Board& b)
	{
		return b.getMoves()[0];
	}
};