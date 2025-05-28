#pragma once
#include "Core/Actor.h"

namespace Core {
	class PureAI :
		public Actor
	{
	public:
		PureAI();
		~PureAI() override;
		Move getMove(const Board& b) override;
		void trainAndSave();
	private:
	};
};

