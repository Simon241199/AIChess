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

	Move::Move(Position from, Position to, Piece p, Piece pieceOnTargetSquare, Piece promotion) {
		this->data = 0;
		if (IsPawn(p)) {
			this->data |= uint16_t{ 1 } << 15;

			uint16_t pawnMoveDir = (to.Y() > from.Y()) ? PawnMoveDir::Front : PawnMoveDir::Back;
			if (to.X() < from.X()) {
				pawnMoveDir = (to.Y() > from.Y()) ? PawnMoveDir::FrontLeft : PawnMoveDir::BackLeft;
			}
			else if (to.X() > from.X()) {
				pawnMoveDir = (to.Y() > from.Y()) ? PawnMoveDir::FrontRight : PawnMoveDir::BackRight;
			}
			this->data |= uint16_t{ pawnMoveDir } << 12;

			uint16_t pawnMoveType = PawnMoveTyp::Normal;
			if (std::abs(from.Y() - to.Y()) == 2) {
				pawnMoveType = PawnMoveTyp::Double;
			}
			else if (to.X() != from.X() && pieceOnTargetSquare == Piece::None) {
				pawnMoveType = PawnMoveTyp::EnPassent;
			}
			else if (IsQueen(promotion)) {
				pawnMoveType = PawnMoveTyp::QueenPromotion;
			}
			else if (IsRook(promotion)) {
				pawnMoveType = PawnMoveTyp::RookPromotion;
			}
			else if (IsBishop(promotion)) {
				pawnMoveType = PawnMoveTyp::BishopPromotion;
			}
			else if (IsKnight(promotion)) {
				pawnMoveType = PawnMoveTyp::KnightPromotion;
			}
			this->data |= pawnMoveType << 9;

			this->data |= uint16_t{ to.Index() } << 3;

			uint16_t pieceOnTargetSquareData = uint16_t(PieceType::None);
			if (IsPawn(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Pawn);
			}
			else if (IsKnight(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Knight);
			}
			else if (IsBishop(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Bishop);
			}
			else if (IsRook(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Rook);
			}
			else if (IsQueen(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Queen);
			}
			else if (IsKing(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::King);
			}
			this->data |= pieceOnTargetSquareData;
			return;
		}
		else {
			this->data |= uint16_t{ from.Index() } << 9;

			this->data |= uint16_t{ to.Index() } << 3;

			uint16_t pieceOnTargetSquareData = uint16_t(PieceType::None);
			if (IsPawn(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Pawn);
			}
			else if (IsKnight(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Knight);
			}
			else if (IsBishop(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Bishop);
			}
			else if (IsRook(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Rook);
			}
			else if (IsQueen(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::Queen);
			}
			else if (IsKing(pieceOnTargetSquare)) {
				pieceOnTargetSquareData = uint16_t(PieceType::King);
			}
			this->data |= pieceOnTargetSquareData;
			return;
		}
	}

	bool Move::IsPawnMove()
	{
		return (this->data >> 15) == 1;
	}

	bool Move::IsDoublePawnMove()
	{
		return this->IsPawnMove() && ((this->data >> 9) & 0b0111) == PawnMoveTyp::Double;
	}

	bool Move::IsEnPassent()
	{
		return this->IsPawnMove() && ((this->data >> 9) & 0b0111) == PawnMoveTyp::EnPassent;
	}

	bool Move::IsPromotion()
	{
		uint16_t pawnMoveType = (this->data >> 9) & 0b0111;
		return this->IsPawnMove()
			&& (pawnMoveType == PawnMoveTyp::QueenPromotion
				|| pawnMoveType == PawnMoveTyp::RookPromotion
				|| pawnMoveType == PawnMoveTyp::BishopPromotion
				|| pawnMoveType == PawnMoveTyp::KnightPromotion);
	}

	bool Move::IsCapture()
	{
		return this->IsEnPassent() || (this->data & 0b0111) != 0;
	}

	PieceType Move::GetCapturedPiece()
	{
		if (this->IsEnPassent()) {
			return PieceType::Pawn;
		}
		return PieceType(this->data & 0b0111);
	}

	PieceType Move::GetPromotionPiece()
	{
		if (!this->IsPawnMove()) {
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

	Position Move::GetFrom()
	{
		if (!this->IsPawnMove()) {
			return Position((this->data >> 9) & 0b0011'1111);
		}
		Position out = GetTo();
		uint8_t dir = (this->data >> 12) & 0b111;
		if ((dir & 0b100) == 0) {
			out = out.Add(0, (this->IsDoublePawnMove()) ? -2 : -1);
		}
		else {
			out = out.Add(0, (this->IsDoublePawnMove()) ? 2 : 1);
		}

		switch (dir) {
		case PawnMoveDir::FrontLeft:
		case PawnMoveDir::BackLeft:
			out = out.Add(1, 0);
			break;
		case PawnMoveDir::FrontRight:
		case PawnMoveDir::BackRight:
			out = out.Add(-1, 0);
			break;
		}
	}

	Position Move::GetTo()
	{
		return Position((this->data >> 3) & 0b0011'1111);
	}
};