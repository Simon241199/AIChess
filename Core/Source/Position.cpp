#include "Position.h"

#include <bit>

namespace Core {

	Position Position::fromBitboard(uint64_t bitboard)
	{
		return Position(std::countr_zero(bitboard));
	}

	Position Position::fromFileRank(char file, int8_t rank)
	{
		return Position(file - 'a', rank - 1);
	}

	Position::Position(uint8_t index)
		: m_index(index)
	{
	}

	Position::Position(int8_t x, int8_t y)
		: m_index(x + y * 8)
	{
		if (x < 0 || x>7 || y < 0 || y>7) {
			m_index = 128;    // set to invalid position
		}
	}

	int8_t Position::x() const
	{
		return this->m_index % 8;
	}

	int8_t Position::y() const
	{
		return this->m_index / 8;
	}

	Position Position::add(int8_t x, int8_t y) const
	{
		if (this->isOnBoard()) {
			return Position(this->x() + x, this->y() + y);
		}
		return *this;
	}

	bool Position::isOnBoard() const
	{
		return this->m_index < 64;
	}

	uint8_t Position::index() const
	{
		return this->m_index;
	}

	uint64_t Position::bitboard() const
	{
		return uint64_t{ 1 } << this->m_index;
	}

	std::string Position::toString() const {
		return char{ this->x() + 'a' } + std::to_string(int{ this->y() + 1 });
	}

	bool Position::operator==(const Position& other) const
	{
		return this->m_index == other.m_index;
	}

	Position popBit(uint64_t& bitboard)
	{
		Position out(std::countr_zero(bitboard));
		bitboard ^= uint64_t{ 1 } << out.index();
		return out;
	}
};