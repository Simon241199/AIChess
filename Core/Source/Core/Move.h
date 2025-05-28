#pragma once
#include "Position.h"
#include "Pieces.h"
namespace Core {

	class Move
	{
	public:
		Move(Position from, Position to, bool isPawnMove, Piece pieceOnTargetSquare = Piece::None, PieceType promotion = PieceType::None);

		bool isPawnMove() const;
		bool isDoublePawnMove() const;
		bool isEnPassent() const;
		bool isPromotion() const;
		bool isCapture() const;
		PieceType getCapturedPiece() const;
		PieceType getPromotionPiece() const;
		PieceType getPieceOnTargetSquare() const;
		Position getFrom() const;
		Position getTo() const;
		std::string toString() const;
		bool operator==(const Move& other) const;
	private:
		uint16_t data;
	};
};