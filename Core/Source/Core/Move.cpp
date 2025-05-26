#include "Move.h"
#include <cmath>
namespace Core {

	namespace PawnMoveTyp {
		enum PawnMoveTyp : uint16_t {
			Normal = 0,
			Double = 1,
			EnPassent = 2,
			QueenPromotion = 3,
			RookPromotion = 4,
			BishopPromotion = 5,
			KnightPromotion = 6,
		};
	};

	namespace PawnMoveDir {
		enum PawnMoveTyp : uint16_t {
			FrontLeft = 0b010,
			Front = 0b000,
			FrontRight = 0b001,
			BackLeft = 0b110,
			Back = 0b100,
			BackRight = 0b101,
		};
	};

	Move::Move(Position from, Position to, Piece p, Piece pieceOnTargetSquare, PieceType promotion) {
		this->data = 0;
		if (isPawn(p)) {
			this->data |= uint16_t{ 1 } << 15;

			uint16_t pawnMoveDir = (to.y() > from.y()) ? PawnMoveDir::Front : PawnMoveDir::Back;
			if (to.x() < from.x()) {
				pawnMoveDir = (to.y() > from.y()) ? PawnMoveDir::FrontLeft : PawnMoveDir::BackLeft;
			}
			else if (to.x() > from.x()) {
				pawnMoveDir = (to.y() > from.y()) ? PawnMoveDir::FrontRight : PawnMoveDir::BackRight;
			}
			this->data |= uint16_t{ pawnMoveDir } << 12;

			uint16_t pawnMoveType = PawnMoveTyp::Normal;
			if (std::abs(from.y() - to.y()) == 2) {
				pawnMoveType = PawnMoveTyp::Double;
			}
			else if (to.x() != from.x() && pieceOnTargetSquare == Piece::None) {
				pawnMoveType = PawnMoveTyp::EnPassent;
			}
			else if (promotion == PieceType::Queen) {
				pawnMoveType = PawnMoveTyp::QueenPromotion;
			}
			else if (promotion == PieceType::Rook) {
				pawnMoveType = PawnMoveTyp::RookPromotion;
			}
			else if (promotion == PieceType::Bishop) {
				pawnMoveType = PawnMoveTyp::BishopPromotion;
			}
			else if (promotion == PieceType::Knight) {
				pawnMoveType = PawnMoveTyp::KnightPromotion;
			}
			this->data |= pawnMoveType << 9;

			this->data |= uint16_t{ to.index() } << 3;

			uint16_t pieceOnTargetSquareData = uint16_t(PieceType::None);
			if (isPawn(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Pawn);
			}
			else if (isKnight(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Knight);
			}
			else if (isBishop(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Bishop);
			}
			else if (isRook(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Rook);
			}
			else if (isQueen(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Queen);
			}
			else if (isKing(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::King);
			}
			this->data |= pieceOnTargetSquareData;
			return;
		}
		else {
			this->data |= uint16_t{ from.index() } << 9;

			this->data |= uint16_t{ to.index() } << 3;

			uint16_t pieceOnTargetSquareData = uint16_t(PieceType::None);
			if (isPawn(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Pawn);
			}
			else if (isKnight(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Knight);
			}
			else if (isBishop(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Bishop);
			}
			else if (isRook(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Rook);
			}
			else if (isQueen(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Queen);
			}
			else if (isKing(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::King);
			}
			this->data |= pieceOnTargetSquareData;
			return;
		}
	}

	bool Move::isPawnMove() const
	{
		return (this->data >> 15) == 1;
	}

	bool Move::isDoublePawnMove() const
	{
		return this->isPawnMove() && ((this->data >> 9) & 0b0111) == PawnMoveTyp::Double;
	}

	bool Move::isEnPassent() const
	{
		return this->isPawnMove() && ((this->data >> 9) & 0b0111) == PawnMoveTyp::EnPassent;
	}

	bool Move::isPromotion() const
	{
		uint16_t pawnMoveType = (this->data >> 9) & 0b0111;
		return this->isPawnMove()
			&& (pawnMoveType == PawnMoveTyp::QueenPromotion
				|| pawnMoveType == PawnMoveTyp::RookPromotion
				|| pawnMoveType == PawnMoveTyp::BishopPromotion
				|| pawnMoveType == PawnMoveTyp::KnightPromotion);
	}

	bool Move::isCapture() const
	{
		return this->isEnPassent() || (this->data & 0b0111) != 0;
	}

	PieceType Move::getCapturedPiece() const
	{
		if (this->isEnPassent()) {
			return PieceType::Pawn;
		}
		return PieceType(this->data & 0b0111);
	}

	PieceType Move::getPromotionPiece() const
	{
		if (!this->isPawnMove()) {
			return PieceType::None;
		}
		switch ((this->data >> 9) & 0b0111)
		{
		case PawnMoveTyp::KnightPromotion:
			return PieceType::Knight;
		case PawnMoveTyp::BishopPromotion:
			return PieceType::Bishop;
		case PawnMoveTyp::RookPromotion:
			return PieceType::Rook;
		case PawnMoveTyp::QueenPromotion:
			return PieceType::Queen;
		}
		return PieceType::None;
	}

	PieceType Move::getPieceOnTargetSquare() const
	{
		return PieceType(this->data & 0b0111);
	}

	Position Move::getFrom() const
	{
		if (!this->isPawnMove()) {
			return Position((this->data >> 9) & 0b0011'1111);
		}
		Position out = getTo();
		uint8_t dir = (this->data >> 12) & 0b111;
		if ((dir & 0b100) == 0) {
			out = out.add(0, (this->isDoublePawnMove()) ? -2 : -1);
		}
		else {
			out = out.add(0, (this->isDoublePawnMove()) ? 2 : 1);
		}

		switch (dir) {
		case PawnMoveDir::FrontLeft:
		case PawnMoveDir::BackLeft:
			out = out.add(1, 0);
			break;
		case PawnMoveDir::FrontRight:
		case PawnMoveDir::BackRight:
			out = out.add(-1, 0);
			break;
		}
		return out;
	}

	Position Move::getTo() const
	{
		return Position((this->data >> 3) & 0b0011'1111);
	}

	std::string Move::toString() const {
		return this->getFrom().toString() + this->getTo().toString();
	}
};