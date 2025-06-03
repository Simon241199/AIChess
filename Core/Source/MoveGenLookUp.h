#pragma once
#include <array>

namespace Core {

	extern const std::array<uint64_t, 64> KNIGHT_MASKS;
	extern const std::array<uint64_t, 64> BISHOP_MASKS;
	extern const std::array<uint64_t, 64> ROOK_MASKS;
	extern const std::array<uint64_t, 64> QUEEN_MASKS;
	extern const std::array<uint64_t, 64> KING_MASKS;

	extern const std::array<uint64_t, 64> WHITE_PAWN_MASKS;
	extern const std::array<uint64_t, 64> BLACK_PAWN_MASKS;

	extern const std::array<uint64_t, 64> WHITE_PAWN_INFRONT_MASKS;
	extern const std::array<uint64_t, 64> BLACK_PAWN_INFRONT_MASKS;

	extern const std::array<uint64_t, 64> STRAIGHT_BLOCKER_MASKS;
	extern const std::array<uint64_t, 64> DIAGONAL_BLOCKER_MASKS;

	extern const std::array<std::pair<uint64_t, uint8_t>, 64> STRAIGHT_MAGIC;
	extern const std::array<std::pair<uint64_t, uint8_t>, 64> DIAGONAL_MAGIC;

	extern const std::array<uint64_t, 2096576> STRAIGHT_MAGIC_LOOKUP;
	extern const std::array<uint64_t, 262144> DIAGONAL_MAGIC_LOOKUP;
};

