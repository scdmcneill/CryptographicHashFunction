#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <array>

// =========================================================================================================
//											AEGIS-1024-bit Sponge Hash
// State: 16 lanes x 64-bit = 1024 bits
// Rate: 8 lanes x 64-bit = 512 bits (64 bytes per absorb / squeeze)
// Capacity: 8 lanes x 64-bit = 512 bits
// 
// =========================================================================================================

namespace HashFunction {
	constexpr size_t LANE_BITS		=	64;
	constexpr size_t LANE_BYTES		=	LANE_BITS / 8;

	constexpr size_t STATE_BITS		=	1024;
	constexpr size_t RATE_BITS		=	512;
	constexpr size_t CAPACITY_BITS	=	512;

	constexpr size_t STATE_LANES	=	STATE_BITS / LANE_BITS; // 16 lanes
	constexpr size_t RATE_LANES		=	RATE_BITS / LANE_BITS;   // 8 lanes
	constexpr size_t CAPACITY_LANES	=	CAPACITY_BITS / LANE_BITS; // 8 lanes

	constexpr size_t STATE_BYTES	=	STATE_LANES * LANE_BYTES; // 128 bytes
	constexpr size_t RATE_BYTES		=	RATE_LANES * LANE_BYTES;   // 64 bytes

	using StateLane		= std::uint64_t;
	using StateBlock	= std::array<StateLane, STATE_LANES>;
	using RateBlock		= std::array<uint8_t, RATE_BYTES>;

	// AEGIS-1024 Round Constants derived from first 12 SHA-512 K constants
	constexpr std::array<StateLane, 12> AEGIS_CONSTANTS_A = {
		0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
		0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
		0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
		0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
		0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
		0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL
	};
}

using namespace HashFunction;

class Aegis {
public: 
	
	void absorbBlock(StateBlock& state, const RateBlock& messageBlock);

	void squeezeBlock(const StateBlock& state, RateBlock& outputBlock);

	void applyPermutation(StateBlock& state);

	RateBlock hashMessage(const std::vector<uint8_t>& message);

	
private:
	StateBlock state;
};