#pragma once
#include "Actor.h"
#include <unordered_map>

namespace Core {
	class SymbolicCom
		: public Actor
	{
	public:
		Move getMove(const Board& b) override;
		int alphaBeta(Board& b, int lower, int upper, int depth = 0);
		int quiesce(Board& b, int lower, int upper, int depth);
		int evaluate(Board& b);
	private:
		enum class EvalType {
			exact,
			lower,
			upper,
		};
		struct TableEntry {
			int remainingDepth;
			int currentDepth;
			int eval;
			EvalType type;
			Move bestMove;
		};
		std::unordered_map<uint64_t, TableEntry> transpositionTable;
		int maxDepth = 0;
	};
};