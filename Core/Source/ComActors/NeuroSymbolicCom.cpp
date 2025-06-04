#include "NeuroSymbolicCom.h"
#include "TorchPch.h"

#include <chrono>
#include <iostream>
#include "AiModels.h"

namespace Core {

	Move Core::NeuroSymbolicCom::getMove(const Board& b)
	{
		this->transpositionTable.clear();
		this->transpositionTable.reserve(5'000'000);
		auto start = std::chrono::high_resolution_clock::now();
		Board board = b;

		this->maxDepth = 1;
		while (true) {
			alphaBeta(board, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
			if (board.getOccupiedBitboard() != b.getOccupiedBitboard()) {
				std::cout << "board.getOccupiedBitboard() != b.getOccupiedBitboard()" << std::endl;
			}

			this->maxDepth++;
			std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
			if (elapsed.count() > 1.0) {
				break;
			}
		}

		return this->transpositionTable[b.hash()].bestMove;
	}

	int NeuroSymbolicCom::alphaBeta(Board& b, int lower, int upper, int depth)
	{
		uint64_t boardHash = b.hash();

		int bestEval = -std::numeric_limits<int>::max();
		Move bestMove = Move(Position(0), Position(0), false);

		bool overwriteTranspos = true;

		if (this->transpositionTable.contains(boardHash)) {
			TableEntry entry = this->transpositionTable[boardHash];
			overwriteTranspos = (entry.remainingDepth <= (maxDepth - depth));
			if (entry.remainingDepth >= (maxDepth - depth)) {
				switch (entry.type) {
				case EvalType::exact:
					return entry.eval;
				case  EvalType::lower:
					bestEval = entry.eval;
					bestMove = entry.bestMove;
					lower = std::max(lower, entry.eval);
					break;
				case  EvalType::upper:
					if (entry.eval <= lower) {
						return entry.eval; // Alpha cut-off
					}
					upper = std::min(upper, entry.eval);
					break;
				}
			}
		}

		if (depth >= this->maxDepth) {
			int currentEval = quiesce(b, lower, upper, depth);
			if (overwriteTranspos) {
				this->transpositionTable.insert_or_assign(boardHash, TableEntry{
					maxDepth - depth,                     // remainingDepth
					depth,                                // currentDepth
					currentEval,                          // eval
					EvalType::exact,                      // type
					Move(Position(0), Position(0), false) // bestMove
					});
			}
			return currentEval;
		}

		std::vector<Move> moves = b.getMoves();

		if (moves.size() == 0) {
			int eval = b.isInCheck(b.getWhoseTurn()) ? -std::numeric_limits<int>::max() + depth : 0;
			// Consider storing this in the transposition table
			return eval;
		}

		Move priorityMove = Move(Position(0), Position(0), false);
		if (this->transpositionTable.contains(boardHash)) {
			priorityMove = this->transpositionTable[boardHash].bestMove;
			if (!priorityMove.isNull()) {
				moves.push_back(priorityMove);
				std::swap(moves.front(), moves.back());
			}
		}
		bool isExact = false;
		int i = 0;
		for (Move m : moves) {
			if (m == priorityMove && i != 0) {
				continue; // already searched the suggestion from the transposition table
			}
			i++;
			b.make(m);
			int currentEval = -alphaBeta(b, -upper, -lower, depth + 1);
			b.undo();

			if (currentEval > bestEval) {
				bestEval = currentEval;
				bestMove = m;
				if (currentEval > lower) {
					lower = currentEval;
					isExact = true;
				}
			}

			if (currentEval >= upper) {
				if (overwriteTranspos) {
					this->transpositionTable.insert_or_assign(boardHash, TableEntry{
						maxDepth - depth, // remainingDepth
						depth,            // currentDepth
						bestEval,         // eval
						EvalType::lower,  // type
						bestMove          // bestMove
						});
				}
				return bestEval;
			}
		}
		if (overwriteTranspos) {
			if (isExact) {
				this->transpositionTable.insert_or_assign(boardHash, TableEntry{
					maxDepth - depth, // remainingDepth
					depth,            // currentDepth
					bestEval,         // eval
					EvalType::exact,  // type
					bestMove          // bestMove
					});
			}
			else {
				this->transpositionTable.insert_or_assign(boardHash, TableEntry{
					maxDepth - depth, // remainingDepth
					depth,            // currentDepth
					bestEval,         // eval
					EvalType::upper,  // type
					bestMove          // bestMove
					});
			}
		}
		return bestEval;
	}

	int NeuroSymbolicCom::quiesce(Board& b, int lower, int upper, int depth)
	{
		{
			uint64_t boardHash = b.hash();
			if (this->transpositionTable.contains(boardHash)) {
				TableEntry entry = this->transpositionTable[boardHash];
				if (entry.type == EvalType::exact) {
					return entry.eval;
				}
			}
		}


		std::vector<Move> moves = b.getMoves();

		if (moves.size() == 0) {
			return b.isInCheck(b.getWhoseTurn()) ? -std::numeric_limits<int>::max() + depth : 0;
		}

		int bestEval = evaluateCombined(b);
		if (depth > (this->maxDepth + 6)) {
			return bestEval;
		}

		for (Move m : moves) {
			b.make(m);
			if (!m.isCapture() && !m.isPromotion()) {
				b.undo();
				continue;
			}
			int currentEval = -quiesce(b, -upper, -lower, depth + 1);
			b.undo();

			if (currentEval > bestEval) {
				bestEval = currentEval;
				lower = std::max(lower, currentEval);
			}

			if (currentEval >= upper) {
				return bestEval;
			}
		}

		return bestEval;
	}
};
