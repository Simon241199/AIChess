#pragma once
#include "Core/Actor.h"

namespace Core {
	class SymbolicCom 
		: public Actor
	{
		Move getMove(const Board& b) override;
	};
};