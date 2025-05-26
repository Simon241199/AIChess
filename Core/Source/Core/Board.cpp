#include "Board.h"
#include <ranges>
#include <vector>
#include <iostream>
#include "MoveGenLookUp.h"
#include "Move.h"
#include "Utility.h"

namespace Core {
	Board::Board(const std::string& fen) {
		auto view_split = std::views::split(fen, ' ');
		auto view_it = view_split.begin();

		std::string_view placments(*(view_it++));

		this->whoseTurn = ((*(view_it++))[0] == 'w') ? PieceColor::White : PieceColor::Black;

		std::string_view castlingRights(*(view_it++));
		this->castlingRightsHistory.push(
			CastlingRights{
				castlingRights.contains("Q"),
				castlingRights.contains("K"),
				castlingRights.contains("q"),
				castlingRights.contains("k")
			});

		this->enPassentFile = (*(view_it++))[0];

		std::string_view halfmoveClockView(*(view_it++));
		this->halfmoveClockStack.push(stoi(std::string(halfmoveClockView)));

		std::string_view fullmoveClockView(*(view_it++));
		this->fullmoveClock = stoi(std::string(fullmoveClockView));

		auto rows = std::views::split(placments, '/');
		int8_t r = 8;
		for (const auto& row : rows) {
			char f = 'a';
			for (char c : row) {
				if (std::isdigit(c)) {
					f += c - '0';
					continue;
				}
				this->set(f, r, getPieceFromFenChar(c));
				f++;
			}
			r--;
		}
		logBitboard(this->blackKings);
	}

	Piece Board::get(char file, int8_t rank) const
	{
		return this->get(Position::fromFileRank(file, rank));
	}

	void Board::set(char file, int8_t rank, Piece p)
	{
		this->set(Position::fromFileRank(file, rank), p);
	}

	Piece Board::get(Position pos) const
	{
		return this->pieces[pos.index()];
	}

	void Board::set(Position pos, Piece p)
	{
		this->pieces[pos.index()] = p;
		this->setAllZero(pos);
		getBitboardRef(p) |= pos.bitboard();
	}

	int8_t enPassentFromRank(PieceColor color) {
		if (color == PieceColor::White) {
			return 4;
		}
		return 3;
	}

	void Board::insertMoveIfOk(Move move, std::vector<Move>& moves) const
	{
		uint64_t dummyBlockers = move.getTo().bitboard();
		uint64_t forcedFree = move.getFrom().bitboard();
		if (move.isEnPassent()) {
			forcedFree |= Position(move.getTo().x(), move.getFrom().y()).bitboard();
		}
		Position kingPos = Position::fromBitboard(this->getBitboard(kingOf(this->whoseTurn)));
		if (isKing(this->get(move.getFrom()))) {
			kingPos = move.getTo();
		}

		if (!this->isAttackedBy(kingPos, invert(this->whoseTurn), forcedFree, dummyBlockers)) {
			moves.push_back(move);
		}
	}

	std::vector<Move> Board::getMoves() const
	{
		std::vector<Move> moves;
		PieceColor opponentColor = invert(this->whoseTurn);

		uint64_t occupied = this->getOccupiedBitboard();
		uint64_t thisTeam = this->getBitboardOf(this->whoseTurn);
		uint64_t opponentTeam = this->getBitboardOf(opponentColor);

		for (uint8_t index = 0; index < 64; index++) {
			Position from(index);
			Piece piece = this->get(from);
			if (!isSameColor(this->whoseTurn, piece)) {
				continue;
			}
			PieceType type = pieceTypeOf(piece);

			if (type == PieceType::Pawn) {
				uint64_t targetBits = (this->whoseTurn == PieceColor::White) ? WHITE_PAWN_MASKS[from.index()] : BLACK_PAWN_MASKS[from.index()];
				targetBits &= ~thisTeam;
				uint64_t captureTargetsBitboard = targetBits & opponentTeam;
				uint64_t quietTargetsBitboard = targetBits & ~opponentTeam;

				uint64_t infrontBitboard = (this->whoseTurn == PieceColor::White) ? WHITE_PAWN_INFRONT_MASKS[from.index()] : BLACK_PAWN_INFRONT_MASKS[from.index()];
				Position infrontPos(infrontBitboard);
				bool infrontOccupied = (infrontBitboard & occupied) != 0;

				while (captureTargetsBitboard != 0) {
					Position targetPos = popBit(captureTargetsBitboard);
					if (targetPos.x() == from.x()) {
						continue;
					}
					if (targetPos.y() == 0 || targetPos.y() == 7) {
						for (PieceType promotionType : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight}) {
							this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos), promotionType), moves);
						}
					}
					else {
						this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
					}
				}

				while (quietTargetsBitboard != 0) {
					Position targetPos = popBit(quietTargetsBitboard);
					if (targetPos.x() != from.x()) {
						continue;
					}
					if (std::abs(targetPos.y() - from.y()) == 2) {
						if (!infrontOccupied) {
							this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
						}
						continue;
					}
					if (targetPos.y() == 0 || targetPos.y() == 7) {
						for (PieceType promotionType : {PieceType::Queen, PieceType::Rook, PieceType::Bishop, PieceType::Knight}) {
							this->insertMoveIfOk(Move(from, targetPos, piece, Piece::None, promotionType), moves);
						}
					}
					else {
						this->insertMoveIfOk(Move(from, targetPos, piece), moves);
					}
				}

				if (std::abs((from.x() + 'a') - this->enPassentFile) == 1 && enPassentFromRank(this->whoseTurn) == from.y()) {
					this->insertMoveIfOk(Move(from, Position(this->enPassentFile - 'a', (this->whoseTurn == PieceColor::White) ? from.y() + 1 : from.y() - 1), piece), moves);
				}
			}

			if (type == PieceType::Knight) {
				uint64_t targetBits = KNIGHT_MASKS[from.index()] & ~thisTeam;
				while (targetBits != 0) {
					Position targetPos = popBit(targetBits);
					this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
				}
			}

			if (type == PieceType::Bishop) {
				auto [magicMult, magicShift] = DIAGONAL_MAGIC[from.index()];
				uint64_t targetBits = DIAGONAL_MAGIC_LOOKUP[(((DIAGONAL_BLOCKER_MASKS[from.index()] & occupied) * magicMult) >> magicShift) * 64 + from.index()] & ~thisTeam;
				while (targetBits != 0) {
					Position targetPos = popBit(targetBits);
					this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
				}
			}

			if (type == PieceType::Rook) {
				auto [magicMult, magicShift] = STRAIGHT_MAGIC[from.index()];
				uint64_t targetBits = STRAIGHT_MAGIC_LOOKUP[(((STRAIGHT_BLOCKER_MASKS[from.index()] & occupied) * magicMult) >> magicShift) * 64 + from.index()] & ~thisTeam;
				while (targetBits != 0) {
					Position targetPos = popBit(targetBits);
					this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
				}
			}

			if (type == PieceType::Queen) {
				auto [magicMultD, magicShiftD] = DIAGONAL_MAGIC[from.index()];
				uint64_t diagBits = DIAGONAL_MAGIC_LOOKUP[(((DIAGONAL_BLOCKER_MASKS[from.index()] & occupied) * magicMultD) >> magicShiftD) * 64 + from.index()];

				auto [magicMultS, magicShiftS] = STRAIGHT_MAGIC[from.index()];
				uint64_t straBits = STRAIGHT_MAGIC_LOOKUP[(((STRAIGHT_BLOCKER_MASKS[from.index()] & occupied) * magicMultS) >> magicShiftS) * 64 + from.index()];

				uint64_t targetBits = (diagBits | straBits) & ~thisTeam;

				while (targetBits != 0) {
					Position targetPos = popBit(targetBits);
					this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
				}
			}

			if (type == PieceType::King) {
				uint64_t targetBits = KING_MASKS[from.index()] & ~thisTeam;
				while (targetBits != 0) {
					Position targetPos = popBit(targetBits);
					this->insertMoveIfOk(Move(from, targetPos, piece, this->get(targetPos)), moves);
				}
				if (!this->isInCheck(this->whoseTurn)) {
					CastlingRights rights = this->castlingRightsHistory.top();
					if (this->whoseTurn == PieceColor::White) {
						if (areClearAndNotAttackedBy((int64_t{ 1 } << 1) | (int64_t{ 1 } << 2) | (int64_t{ 1 } << 3), opponentColor) && rights.canWhiteQueenSideCastle) {
							this->insertMoveIfOk(Move(from, from.add(-2, 0), piece), moves);
						}
						if (areClearAndNotAttackedBy((int64_t{ 1 } << 5) | (int64_t{ 1 } << 6), opponentColor) && rights.canWhiteKingSideCastle) {
							this->insertMoveIfOk(Move(from, from.add(2, 0), piece), moves);
						}
					}
					else { // Black
						if (areClearAndNotAttackedBy((int64_t{ 1 } << 57) | (int64_t{ 1 } << 58) | (int64_t{ 1 } << 59), opponentColor) && rights.canBlackQueenSideCastle) {
							this->insertMoveIfOk(Move(from, from.add(-2, 0), piece), moves);
						}
						if (areClearAndNotAttackedBy((int64_t{ 1 } << 61) | (int64_t{ 1 } << 62), opponentColor) && rights.canBlackKingSideCastle) {
							this->insertMoveIfOk(Move(from, from.add(2, 0), piece), moves);
						}
					}
				}
			}
		}
		return moves;
	}

	bool Board::areClearAndNotAttackedBy(uint64_t bitboard, PieceColor attackerColor) const
	{
		uint64_t occupied = this->getOccupiedBitboard();
		if ((bitboard & occupied) != 0) {
			return false;
		}
		while (bitboard != 0) {
			Position pos = popBit(bitboard);
			if (this->isAttackedBy(pos, attackerColor)) {
				return false;
			}
		}
		return true;
	}

	uint64_t& Board::getBitboardRef(Piece p)
	{
		switch (p)
		{
		case Piece::WhitePawn:
			return this->whitePawns;
		case Piece::WhiteKnight:
			return this->whiteKnights;
		case Piece::WhiteBishop:
			return this->whiteBishops;
		case Piece::WhiteRook:
			return this->whiteRooks;
		case Piece::WhiteQueen:
			return this->whiteQueens;
		case Piece::WhiteKing:
			return this->whiteKings;
		case Piece::BlackPawn:
			return this->blackPawns;
		case Piece::BlackKnight:
			return this->blackKnights;
		case Piece::BlackBishop:
			return this->blackBishops;
		case Piece::BlackRook:
			return this->blackRooks;
		case Piece::BlackQueen:
			return this->blackQueens;
		case Piece::BlackKing:
			return this->blackKings;
		}
		static uint64_t NoneBitboard = 0;
		return NoneBitboard;
	}

	void Board::setAllZero(Position pos)
	{
		uint64_t mask = ~pos.bitboard();

		this->whitePawns &= mask;
		this->whiteKnights &= mask;
		this->whiteBishops &= mask;
		this->whiteRooks &= mask;
		this->whiteQueens &= mask;
		this->whiteKings &= mask;

		this->blackPawns &= mask;
		this->blackKnights &= mask;
		this->blackBishops &= mask;
		this->blackRooks &= mask;
		this->blackQueens &= mask;
		this->blackKings &= mask;
	}

	uint64_t Board::getBitboard(Piece p) const
	{
		switch (p)
		{
		case Piece::WhitePawn:
			return this->whitePawns;
		case Piece::WhiteKnight:
			return this->whiteKnights;
		case Piece::WhiteBishop:
			return this->whiteBishops;
		case Piece::WhiteRook:
			return this->whiteRooks;
		case Piece::WhiteQueen:
			return this->whiteQueens;
		case Piece::WhiteKing:
			return this->whiteKings;
		case Piece::BlackPawn:
			return this->blackPawns;
		case Piece::BlackKnight:
			return this->blackKnights;
		case Piece::BlackBishop:
			return this->blackBishops;
		case Piece::BlackRook:
			return this->blackRooks;
		case Piece::BlackQueen:
			return this->blackQueens;
		case Piece::BlackKing:
			return this->blackKings;
		}
		return 0;
	}

	uint64_t Board::getBitboardOf(PieceColor color) const
	{
		if (color == PieceColor::White) {
			return this->getWhiteBitboard();
		}
		if (color == PieceColor::Black) {
			return this->getBlackBitboard();
		}
		return 0;
	}

	uint64_t Board::getOccupiedBitboard() const
	{
		return this->whitePawns
			| this->whiteKnights
			| this->whiteBishops
			| this->whiteRooks
			| this->whiteQueens
			| this->whiteKings
			| this->blackPawns
			| this->blackKnights
			| this->blackBishops
			| this->blackRooks
			| this->blackQueens
			| this->blackKings;
	}

	bool Board::isAttackedBy(Position pos, PieceColor color, uint64_t forcedFree, uint64_t dummyBlockers) const // attacked != capturable  <-- relevant when concidering a pinned piece or EnPassent
	{
		if (color == PieceColor::None) {
			return false;
		}

		uint64_t opponentPawns = this->getBitboard(pawnOf(color)) & ~forcedFree & ~dummyBlockers;
		if ((pos.add(-1, (color == PieceColor::White) ? -1 : 1).bitboard() & opponentPawns) != 0
			|| (pos.add(1, (color == PieceColor::White) ? -1 : 1).bitboard() & opponentPawns) != 0) {
			return true;
		}

		uint64_t opponentKnights = this->getBitboard(knightOf(color)) & ~forcedFree & ~dummyBlockers;
		uint64_t possibleKnightAttackers = KNIGHT_MASKS[pos.index()];
		if ((opponentKnights & possibleKnightAttackers) != 0) {
			return true;
		}


		uint64_t occupied = (this->getOccupiedBitboard() | dummyBlockers) & ~forcedFree;

		uint64_t opponentDiagonals = (this->getBitboard(bishopOf(color)) | this->getBitboard(queenOf(color))) & ~forcedFree & ~dummyBlockers;
		auto [magicMultD, magicShiftD] = DIAGONAL_MAGIC[pos.index()];
		uint64_t possibleDigaonalAttackers = DIAGONAL_MAGIC_LOOKUP[(((DIAGONAL_BLOCKER_MASKS[pos.index()] & occupied) * magicMultD) >> magicShiftD) * 64 + pos.index()];
		if ((possibleDigaonalAttackers & opponentDiagonals) != 0) {
			return true;
		}

		uint64_t opponentStraights = (this->getBitboard(rookOf(color)) | this->getBitboard(queenOf(color))) & ~forcedFree & ~dummyBlockers;
		auto [magicMultS, magicShiftS] = STRAIGHT_MAGIC[pos.index()];
		uint64_t possibleStraightAttackers = STRAIGHT_MAGIC_LOOKUP[(((STRAIGHT_BLOCKER_MASKS[pos.index()] & occupied) * magicMultS) >> magicShiftS) * 64 + pos.index()];
		if ((possibleStraightAttackers & opponentStraights) != 0) {
			return true;
		}

		uint64_t opponentKing = this->getBitboard(kingOf(color)) & ~forcedFree & ~dummyBlockers;
		uint64_t possibleKingAttackers = KING_MASKS[pos.index()];
		if ((opponentKing & possibleKingAttackers) != 0) {
			return true;
		}

		return false;
	}

	bool Board::isInCheck(PieceColor color) const
	{
		Position kingPos = Position::fromBitboard(this->getBitboard(kingOf(color)));
		return isAttackedBy(kingPos, invert(color));
	}

	void Board::updateCaslingRights(Move m) {
		Position from = m.getFrom();
		Position to = m.getTo();
		Piece p = this->get(m.getFrom());
		CastlingRights rights = this->castlingRightsHistory.top();

		if (p == Piece::WhiteKing) {
			rights.canWhiteKingSideCastle = false;
			rights.canWhiteQueenSideCastle = false;
		}
		else if (p == Piece::BlackKing) {
			rights.canBlackKingSideCastle = false;
			rights.canBlackQueenSideCastle = false;
		}
		if (from == Position::fromFileRank('a', 1) || to == Position::fromFileRank('a', 1)) {
			rights.canWhiteQueenSideCastle = false;
		}
		if (from == Position::fromFileRank('h', 1) || to == Position::fromFileRank('h', 1)) {
			rights.canWhiteKingSideCastle = false;
		}
		if (from == Position::fromFileRank('a', 8) || to == Position::fromFileRank('a', 8)) {
			rights.canBlackQueenSideCastle = false;
		}
		if (from == Position::fromFileRank('h', 8) || to == Position::fromFileRank('h', 8)) {
			rights.canBlackKingSideCastle = false;
		}

		this->castlingRightsHistory.push(rights);
	}

	void Board::make(Move m)
	{
		Position from = m.getFrom();
		Position to = m.getTo();
		Piece p = this->get(from);

		updateCaslingRights(m);

		this->set(to, p);
		this->set(from, Core::Piece::None);

		this->halfmoveClockStack.push(this->halfmoveClockStack.top() + 1);
		if (isPawn(p) || m.isCapture()) {
			this->halfmoveClockStack.top() = 0;
		}

		if (isKing(p) && std::abs(from.x() - to.x()) == 2) { // if is castle
			Position rookFromPosition((to.x() < from.x()) ? 0 : 7, to.y());
			Position rookToPosition((to.x() + from.x()) / 2, to.y());
			this->set(rookToPosition, this->get(rookFromPosition));
			this->set(rookFromPosition, Core::Piece::None);
		}

		if (m.isEnPassent()) {
			Position capturedPawnPosition(to.x(), from.y());
			this->set(capturedPawnPosition, Core::Piece::None);
		}

		this->enPassentFile = '-';
		if (m.isDoublePawnMove()) {
			this->enPassentFile = from.x() + 'a';
		}

		if (m.isPromotion()) {
			this->set(to, pieceFrom(m.getPromotionPiece(), colorOf(p)));
		}

		if (whoseTurn == PieceColor::Black) {
			this->fullmoveClock++;
		}
		this->whoseTurn = invert(this->whoseTurn);
		this->moveHistory.push(m);
	}

	void Board::undo()
	{
		Move m = this->moveHistory.top();
		this->moveHistory.pop();
		Position from = m.getFrom();
		Position to = m.getTo();
		Piece p = this->get(to);

		this->castlingRightsHistory.pop();

		this->set(to, pieceFrom(m.getPieceOnTargetSquare(), invert(colorOf(p))));
		this->set(from, p);

		if (m.isPromotion()) {
			this->set(from, pieceFrom(PieceType::Pawn, colorOf(p)));
		}

		if (isKing(p) && std::abs(from.x() - to.x()) == 2) { // if is castle
			Position rookFromPosition((to.x() < from.x()) ? 0 : 7, to.y());
			Position rookToPosition((to.x() + from.x()) / 2, to.y());
			this->set(rookFromPosition, this->get(rookToPosition));
			this->set(rookToPosition, Piece::None);
		}

		if (m.isEnPassent()) {
			Position capturedPawnPosition(to.x(), from.y());
			this->set(capturedPawnPosition, pawnOf(invert(colorOf(p))));
		}

		this->enPassentFile = '-';
		if (!this->moveHistory.empty()) {
			if (this->moveHistory.top().isDoublePawnMove()) {
				this->enPassentFile = this->moveHistory.top().getFrom().x() + 'a';
			}
		}

		this->halfmoveClockStack.pop();

		this->whoseTurn = invert(this->whoseTurn);

		if (whoseTurn == PieceColor::Black) {
			this->fullmoveClock--;
		}
	}

	uint64_t Board::getWhiteBitboard() const
	{
		return this->whitePawns
			| this->whiteKnights
			| this->whiteBishops
			| this->whiteRooks
			| this->whiteQueens
			| this->whiteKings;
	}

	uint64_t Board::getBlackBitboard() const
	{
		return this->blackPawns
			| this->blackKnights
			| this->blackBishops
			| this->blackRooks
			| this->blackQueens
			| this->blackKings;
	}
}