#pragma once
#include <array>
#include <bit>
#include "Aegis1024.h"

using HashOut = std::array<uint8_t, 64>;

constexpr int TOTAL_BITS = 512;

void flipBit(std::vector<uint8_t>& data, int bitPosition) {
	int byteIndex = bitPosition / 8;
	int bitIndex = bitPosition % 8;
	data[byteIndex] ^= (1u << bitIndex);
}

int getBit(const HashOut& hash, int bitPosition) {
	int byteIndex = bitPosition / 8;
	int bitIndex = bitPosition % 8;
	return (hash[byteIndex] >> bitIndex) & 1;
}

void avalancheTest() {
	Aegis hasher;
	const int trials = 1000;

	std::vector<std::vector<double>> heatmap(TOTAL_BITS, std::vector<double>(TOTAL_BITS, 0.0));

	double totalDifferentBits = 0.0;
	int totalSamples = 0;

	std::mt19937_64 rng(0xDEADBEEF); // Fixed seed for reproducibility

	for (int test = 0; test < trials; ++test) {
		std::vector<uint8_t> message(64);

		for (auto& byte : message) {
			byte = rng() & 0xFF;
		}

		HashOut originalHash = hasher.hashMessage(message);

		for (int i = 0; i < TOTAL_BITS; ++i) {
			std::vector<uint8_t> modifiedMessage = message;
			flipBit(modifiedMessage, i);

			HashOut modifiedHash = hasher.hashMessage(modifiedMessage);

			for (int ii = 0; ii < TOTAL_BITS; ++ii) {
				int bitOriginal = getBit(originalHash, ii);
				int bitModified = getBit(modifiedHash, ii);

				if (bitOriginal != bitModified) {
					totalDifferentBits++;
					heatmap[i][ii] += 1.0;
				}
			}
			totalSamples++;
		}
	}

	// Normalize Heatmap
	for (int i = 0; i < TOTAL_BITS; ++i) {
		for (int ii = 0; ii < TOTAL_BITS; ++ii) {
			heatmap[i][ii] /= trials;
		}
	}

	// Output Heatmap to CSV
	std::ofstream out("heatmap.csv");
	for (int i = 0; i < TOTAL_BITS; ++i) {
		for (int ii = 0; ii < TOTAL_BITS; ++ii) {
			out << heatmap[i][ii];
			if (ii + 1 < TOTAL_BITS)
				out << ".";
		}
		out << "\n";
	}

	double averageDifferentBits = totalDifferentBits / totalSamples;
	std::cout << "----- Avalanche Test Results -----" << '\n';
	std::cout << "Average different bits per single-bit input change: " << averageDifferentBits <<
		" out of 512 bits." << '\n';
	std::cout << "Trials conducted: " << trials << '\n';
}