#include "Aegis1024.h"

void Aegis::absorbBlock(StateBlock& state, const RateBlock& messageBlock) {
	
	// Byte-level access to the state and message block
	auto* stateBytes = reinterpret_cast<uint8_t*>(state.data());

	// XOR the message block into the first RATE_SIZE lanes of the state
	for (size_t i = 0; i < RATE_BYTES; ++i) {
		stateBytes[i] ^= messageBlock[i];
	}

	applyPermutation(state);
}

void Aegis::squeezeBlock(const StateBlock& state, RateBlock& outputBlock) {
	const auto* stateBytes = reinterpret_cast<const uint8_t*>(state.data());

	for (size_t i = 0; i < RATE_BYTES; ++i) {
		outputBlock[i] = stateBytes[i];
	}
}

StateLane rotateLeft(StateLane& lane, unsigned int shiftAmount) {
	return (lane << shiftAmount) | (lane >> (64 - shiftAmount));
}

void quarterRound(StateLane& lane1, StateLane& lane2, StateLane& lane3, StateLane& lane4, 
									StateLane roundConstants1, StateLane roundConstants2) {
	lane1 += lane2 + roundConstants1;
	lane4 ^= lane1;
	lane4 = rotateLeft(lane4, 32);

	lane3 += lane4;
	lane2 ^= lane3;
	lane2 = rotateLeft(lane2, 24);

	lane1 += lane2 + roundConstants2;
	lane4 ^= lane1;
	lane4 = rotateLeft(lane4, 16);

	lane3 += lane4;
	lane2 ^= lane3;
	lane2 = rotateLeft(lane2, 63);
}

void Aegis::applyPermutation(StateBlock& state) {
	for (size_t round = 0; round < 12; ++round) {
		const StateLane roundConstants1 = AEGIS_CONSTANTS_A[round];
		const StateLane roundConstants2 = AEGIS_CONSTANTS_A[11 - round];

		// Column shuffle
		quarterRound(state[0], state[4], state[8], state[12], roundConstants1, roundConstants2);
		quarterRound(state[1], state[5], state[9], state[13], roundConstants1, roundConstants2);
		quarterRound(state[2], state[6], state[10], state[14], roundConstants1, roundConstants2);
		quarterRound(state[3], state[7], state[11], state[15], roundConstants1, roundConstants2);

		// Diagonal shuffle
		quarterRound(state[0], state[5], state[10], state[15], roundConstants1, roundConstants2);
		quarterRound(state[1], state[6], state[11], state[12], roundConstants1, roundConstants2);
		quarterRound(state[2], state[7], state[8], state[13], roundConstants1, roundConstants2);
		quarterRound(state[3], state[4], state[9], state[14], roundConstants1, roundConstants2);
	}
}

RateBlock Aegis::hashMessage(const std::vector<uint8_t>& message) {
	// Initialize state to zero
	state.fill(0);

	size_t messageOffset = 0;
	const size_t messageSize = message.size();

	// Absorb full 64-byte blocks
	while (messageSize - messageOffset >= RATE_BYTES) {
		RateBlock messageBlock;
		std::memcpy(messageBlock.data(), message.data() + messageOffset, RATE_BYTES);
		absorbBlock(state, messageBlock);
		messageOffset += RATE_BYTES;
	}
	
	// Build and absorb the final padded block
	RateBlock finalBlock{};
	const size_t remainingBytes = messageSize - messageOffset;
	
	// Copy remaining bytes
	for (size_t i = 0; i < remainingBytes; ++i) {
		finalBlock[i] = message[messageOffset + i];
	}
	
	// Padding: append 0x01 byte
	finalBlock[remainingBytes] ^= 0x01;
	
	// final '1' bit in most significant bit of last byte
	finalBlock[RATE_BYTES - 1] ^= 0x80;
	
	// Absorb the final padded block
	absorbBlock(state, finalBlock);
	
	// Squeeze out 64 bytes of output (one block)
	RateBlock outputBlock;
	squeezeBlock(state, outputBlock);

	return outputBlock;
}