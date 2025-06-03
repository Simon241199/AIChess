#pragma once
#include "Move.h"
#include "Board.h"

namespace Core {
	class Actor
	{
	public:
		virtual ~Actor() = default;
		virtual Move getMove(const Board& b) = 0;
	};
};