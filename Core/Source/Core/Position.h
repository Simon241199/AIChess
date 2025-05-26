#pragma once
#include <cstdint>
#include <string>

namespace Core {
	class Position
	{
	public:
		static Position fromBitboard(uint64_t bitboard);
		static Position fromFileRank(char file, int8_t rank);
		Position(uint8_t index);
		Position(int8_t x, int8_t y);
		int8_t x() const;
		int8_t y() const;
		Position add(int8_t x, int8_t y) const;
		bool isOnBoard() const;
		uint8_t index() const;
		uint64_t bitboard() const;
		std::string toString() const;
		bool operator==(const Position& other) const;
	private:
		uint8_t m_index;
	};

	Position popBit(uint64_t& bitboard);
};

