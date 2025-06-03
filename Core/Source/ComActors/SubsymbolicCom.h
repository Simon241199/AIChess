#pragma once
#include "Actor.h"

namespace Core {
	class SubsymbolicCom :
		public Actor
	{
	public:
		SubsymbolicCom();
		~SubsymbolicCom() override;
		Move getMove(const Board& b) override;
	private:
		static const char* fileName;
	};
};

