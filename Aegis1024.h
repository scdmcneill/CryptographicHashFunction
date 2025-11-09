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
// Cap: 8 lanes x 64-bit = 512 bits
// =========================================================================================================

namespace chf {
	// ----- Parameters -----
	static constexpr size_t STATE_LANES = 16;
	static constexpr size_t RATE_LANES = 8;
	static constexpr size_t RATE_BYTES = RATE_LANES * 8;
	static constexpr size_t BLOCK_SIZE = RATE_BYTES;
	static constexpr int	ROUNDS = 12;

	// ----- Helpers -----
	static inline uint64_t rotate(uint64_t x, int rotations) {
		return (x << rotations) | (x >> (64 - rotations));
	}
}

using namespace chf;
using std::cout;
using std::vector;

struct State {
	std::array<uint64_t, STATE_LANES> state{};
};

// "Nothing-up-my-sleeve" round constants go here
static constexpr std::array<uint64_t, ROUNDS> ROUND_CONSTANTS = {

};

class Aegis {
public: 
	Aegis();

	void resetState() {
		
	}


	// ARX permutation over 16 lanes
	static inline void permutation(State& S) {
		for (int round = 0; round < ROUNDS; ++round) {
			// Mix groups of 4 lanes
			for (int i = 0; i < 16; i += 4) {
				uint64_t& laneA = S.state[i + 0];
				uint64_t& laneB = S.state[i + 1];
				uint64_t& laneC = S.state[i + 2];
				uint64_t& laneD = S.state[i + 3];

				laneA += laneB; laneD ^= laneA; laneD = rotate(laneD, 32);
				laneC += laneD; laneB ^= laneC; laneB = rotate(laneB, 24);
				laneA += laneB; laneD ^= laneA; laneD = rotate(laneD, 16);
				laneC += laneD; laneB ^= laneC; laneB = rotate(laneB, 63);

			}

			// Cross-Lane Diffusion: shuffle and XOR neighbors
			// Rotate lanes by (round + 1) and XOR in a neighbor rotated value
			std::array<uint64_t, 16> tempArray{};
			for (int i = 0; i < 16; ++i)
				tempArray[i] = S.state[i];
			int shift = (round + 1) % 16;
			for (int i = 0; i < 16; ++i) {
				S.state[i] = tempArray[(i + shift) & 15] ^ rotate(tempArray[(i + 7) & 15], (i + round) % 64);
			}

			// Inject Round constant into lane 0 and lane 8
			S.state[0] ^= ROUND_CONSTANTS[round];
			S.state[8] ^= rotate(ROUND_CONSTANTS[round], 17);
		}
	}



	// Squeeze bytes
	vector<uint8_t> squeeze(size_t targetByteSize) {
		vector<uint8_t> squeezedBlock;

		return squeezedBlock;
	}
	
private:
	State state_{};
	std::array<uint8_t, BLOCK_SIZE>	buffer_;
	size_t bufferUsed_ = 0;
	bool finalized_ = false;

	std::vector<uint8_t> squeezeCache_{};
	size_t squeezePtr_ = 0;

	void absorb_block(const uint8_t* block) {
		// XOR 64 bytes into rate (8 lanes)
		
	}
};