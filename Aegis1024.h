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

	// AEGIS-1024 Round Constants derived from first 12 SHA-512 constants
	constexpr std::array<StateLane, 12> AEGIS_CONSTANTS = {
		0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
		0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
		0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
		0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
		0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
		0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL
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