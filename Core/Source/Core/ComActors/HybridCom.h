#pragma once
#include "Core/Actor.h"

namespace Core {
	class HybridCom :
		public Actor
	{
		Move getMove(const Board& b) override;
	};
};

