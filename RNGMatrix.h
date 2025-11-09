#include <unordered_map>
#include <vector>
#include <random>
#include <cstdint>
#include <iostream>

using std::vector;
using std::cout;



class RNGMatrix {
public:
	// Constructor
	RNGMatrix();
	
    // Methods
    void displayMatrix() const;
	// Expand Data
	// Swap position at random
	// condense members to digest of 128 bits or more

private:
	vector<vector<int8_t>> matrix;
	static constexpr int8_t ROWS = 32;
	static constexpr int8_t COLUMNS = 32;
};