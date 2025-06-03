#include "SymbolicCom.h"

#include <chrono>
#include <iostream>

namespace Core {

	Move Core::SymbolicCom::getMove(const Board& b)
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

	int SymbolicCom::alphaBeta(Board& b, int lower, int upper, int depth)
	{
		uint64_t boardHash = b.hash();

		int bestEval = std::numeric_limits<int>::lowest();
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
				this->transpositionTable.insert({ boardHash, TableEntry{
					maxDepth - depth,                     // remainingDepth
					depth,                                // currentDepth
					currentEval,                             // eval
					EvalType::exact,                      // type
					Move(Position(0), Position(0), false) // bestMove
					} });
			}
			return currentEval;
		}

		std::vector<Move> moves = b.getMoves();

		if (moves.size() == 0) {
			float eval = b.isInCheck(b.getWhoseTurn()) ? std::numeric_limits<int>::lowest() + depth : 0;
		}

		Move priorityMove = Move(Position(0), Position(0), false);
		if (this->transpositionTable.contains(boardHash)) {
			priorityMove = this->transpositionTable[boardHash].bestMove;
			if (!priorityMove.isNull()) {
				moves.push_back(priorityMove);
				std::swap(moves.front(), moves.back());
			}
		}

		for (Move m : moves) {
			if (m == priorityMove) {
				continue; // already searched the suggestion from the transposition table
			}
			b.make(m);
			int currentEval = -alphaBeta(b, -upper, -lower, depth + 1);
			b.undo();

			if (currentEval > bestEval) {
				bestEval = currentEval;
				bestMove = m;
				lower = std::max(lower, currentEval);
			}

			if (currentEval >= upper) {
				if (overwriteTranspos) {
					this->transpositionTable.insert({ boardHash, TableEntry{
						maxDepth - depth, // remainingDepth
						depth,            // currentDepth
						bestEval,         // eval
						EvalType::lower,  // type
						bestMove          // bestMove
						} });
				}
				return bestEval;
			}
		}
		if (overwriteTranspos) {
			if (bestEval <= lower) {
				this->transpositionTable.insert({ boardHash, TableEntry{
					maxDepth - depth, // remainingDepth
					depth,            // currentDepth
					bestEval,         // eval
					EvalType::upper,  // type
					bestMove          // bestMove
					} });
			}
			else {
				this->transpositionTable.insert({ boardHash, TableEntry{
					maxDepth - depth, // remainingDepth
					depth,            // currentDepth
					bestEval,         // eval
					EvalType::exact,  // type
					bestMove          // bestMove
					} });
			}
		}
		return bestEval;
	}

	int SymbolicCom::quiesce(Board& b, int lower, int upper, int depth)
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
			return b.isInCheck(b.getWhoseTurn()) ? std::numeric_limits<int>::lowest() + depth : 0;
		}

		int bestEval = this->evaluate(b);
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

	int SymbolicCom::evaluate(Board& b)
	{
		int material = 0;
		uint64_t occupied = b.getOccupiedBitboard();
		while (occupied != 0) {
			Position pos = popBit(occupied);
			switch (b.get(pos)) {
			case Core::Piece::WhitePawn:
				material += 100;
				break;
			case Core::Piece::WhiteKnight:
				material += 250;
				break;
			case Core::Piece::WhiteBishop:
				material += 300;
				break;
			case Core::Piece::WhiteRook:
				material += 500;
				break;
			case Core::Piece::WhiteQueen:
				material += 900;
				break;
			case Core::Piece::BlackPawn:
				material -= 100;
				break;
			case Core::Piece::BlackKnight:
				material -= 250;
				break;
			case Core::Piece::BlackBishop:
				material -= 300;
				break;
			case Core::Piece::BlackRook:
				material -= 500;
				break;
			case Core::Piece::BlackQueen:
				material -= 900;
				break;
			}
		}
		material = (b.getWhoseTurn() == PieceColor::White) ? material : -material;
		std::vector teamMoves = b.getMoves();
		b.make(teamMoves.front());
		std::vector opponentMoves = b.getMoves();
		b.undo();
		b.make(teamMoves.back());
		int opponentMoveCount = (b.getMoves().size() + opponentMoves.size()) / 2;
		b.undo();
		int teamMoveCount = teamMoves.size();
		return material + teamMoveCount - opponentMoveCount;
	}
};