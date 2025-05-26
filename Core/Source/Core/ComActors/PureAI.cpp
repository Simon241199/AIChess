#include "PureAI.h"
namespace Core {
	Move PureAI::getMove(const Board& b)
	{
		return b.getMoves()[0];
	}
};
