#pragma once
#include "Core/Actor.h"
namespace Core {
	class PureAI :
		public Actor
	{
		// Inherited via Actor
		Move getMove(const Board& b) override;
	};
};

