#include "Position.h"
#include <bit>

namespace Core {

	Position Position::FromBitboard(uint64_t bitboard)
	{
		return Position(std::countr_zero(bitboard));
	}

	Position Position::FromFileRank(char file, int8_t rank)
	{
		return Position(file - 'a', rank - 1);
	}

	Position::Position(uint8_t index)
		: index(index)
	{
	}

	Position::Position(int8_t x, int8_t y)
		: index(x + y * 8)
	{
		if (x < 0 || x>7 || y < 0 || y>7) {
			index = 128;    // set to invalid position
		}
	}

	int8_t Position::X() const
	{
		return this->index % 8;
	}

	int8_t Position::Y() const
	{
		return this->index / 8;
	}

	Position Position::Add(int8_t x, int8_t y) const
	{
		if (this->IsOnBoard()) {
			return Position(this->X() + x, this->Y() + y);
		}
		return *this;
	}

	bool Position::IsOnBoard() const
	{
		return this->index < 64;
	}

	uint8_t Position::Index() const
	{
		return this->index;
	}

	uint64_t Position::Bitboard() const
	{
		return uint64_t{ 1 } << this->index;
	}
};