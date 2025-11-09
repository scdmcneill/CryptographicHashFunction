#include "RNGMatrix.h"

RNGMatrix::RNGMatrix() {
	matrix.assign(ROWS, vector<int8_t>(COLUMNS));	// Allocates 2D vector

	std::mt19937 gen(12345);	// Mersenne Twister Engine					
	std::bernoulli_distribution bit(0.5);	// 50% chance of 1 or 0 with normal distribution

	for (int i = 0; i < ROWS; ++i) {
		for (int ii = 0; ii < COLUMNS; ++ii) {
			matrix[i][ii] = static_cast<int8_t>(bit(gen));
		}
	}
}

void RNGMatrix::displayMatrix() const {
	for (int i = 0; i < ROWS; ++i) {
		for (int ii = 0; ii < COLUMNS; ++ii) {
			cout << int(matrix[i][ii]) << " ";
		}
		cout << '\n';
	}
}