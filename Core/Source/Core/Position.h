#pragma once
#include <cstdint>

namespace Core {
	class Position
	{
	public:
		static Position FromBitboard(uint64_t bitboard);
		static Position FromFileRank(char file, int8_t rank);
		Position(uint8_t index);
		Position(int8_t x, int8_t y);
		int8_t X() const;
		int8_t Y() const;
		Position Add(int8_t x, int8_t y) const;
		bool IsOnBoard() const;
		uint8_t Index() const;
		uint64_t Bitboard() const;
	private:
		uint8_t index;
	};
};

