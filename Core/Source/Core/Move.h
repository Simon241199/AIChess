#pragma once
#include "Position.h"
#include "Pieces.h"
namespace Core {

	class Move
	{
		Move(Position from, Position to, Piece p, Piece pieceOnTargetSquare = Piece::None, Piece promotion = Piece::None);

		bool IsPawnMove();
		bool IsDoublePawnMove();
		bool IsEnPassent();
		bool IsPromotion();
		bool IsCapture();
		PieceType GetCapturedPiece();
		PieceType GetPromotionPiece();
		Position GetFrom();
		Position GetTo();
	private:
		uint16_t data;
	};

};