#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

constexpr size_t STATE_SIZE = 16;
constexpr size_t RATE_BYTES = 64;
constexpr size_t CAPACITY_BYTES = 64;
constexpr size_t BLOCK_SIZE = RATE_BYTES;

struct State {
	// Initialize the state to 0.
	std::array<uint64_t, STATE_SIZE> lanes = { 0 }; 
};

std::vector<uint8_t> padMessage(const std::string& msg) {
	std::vector<uint8_t> data(msg.begin(), msg.end());
	data.push_back(0x80); // start of padding
	while ((data.size() % BLOCK_SIZE) != (BLOCK_SIZE - 1))
		data.push_back(0x00);
	data.push_back(0x01); // end of padding
	return data;
}

void absorb(State& state, const std::vector<uint8_t>& data) {
	size_t numBlocks = data.size(); // Block size
	
	for (size_t block = 0; block < numBlocks; ++block) {
		// XOR message block into the rate portion
		for (size_t i = 0; i < RATE_BYTES / 8; ++i) {
			uint64_t msgWord = 0;
			for (size_t ii = 0; ii < 8; ++ii)
				msgWord |= (uint64_t)data[block * BLOCK_SIZE + i * 8 + ii] << (8 * ii);
			state.lanes[i] ^= msgWord;
		}

		permutation(state);
	}
}

void permutation(State& state) {
	
}