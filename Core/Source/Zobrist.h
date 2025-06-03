#pragma once
#include <array>

namespace Core {
	namespace Hash {
		extern const std::array<uint64_t, 64> WHITE_PAWN;
		extern const std::array<uint64_t, 64> WHITE_KNIGHT;
		extern const std::array<uint64_t, 64> WHITE_BISHOP;
		extern const std::array<uint64_t, 64> WHITE_ROOK;
		extern const std::array<uint64_t, 64> WHITE_QUEEN;
		extern const std::array<uint64_t, 64> WHITE_KING;
		
		extern const std::array<uint64_t, 64> BLACK_PAWN;
		extern const std::array<uint64_t, 64> BLACK_KNIGHT;
		extern const std::array<uint64_t, 64> BLACK_BISHOP;
		extern const std::array<uint64_t, 64> BLACK_ROOK;
		extern const std::array<uint64_t, 64> BLACK_QUEEN;
		extern const std::array<uint64_t, 64> BLACK_KING;
		
		extern const uint64_t WQS_CASTLE;
		extern const uint64_t WKS_CASTLE;
		
		extern const uint64_t BQS_CASTLE;
		extern const uint64_t BKS_CASTLE;
		
		extern const uint64_t BLACKS_MOVE;
		
		extern const std::array<uint64_t, 8> EN_PASSANT_FILE;
	}
}