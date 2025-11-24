#include "Aegis1024.h"
#include "UserInput.h"
#include "empiricalTesting.h"
#include <fstream>

int main() {
	std::ofstream outFile("HashOutput.txt");
	if (!outFile) {
		std::cerr << "Error: Unable to open output file." << '\n';
	}

	Aegis aegisHasher;
	
	// Example message to hash
	std::string message = "The quick brown fox jumps over the lazy dog!";
	std::vector<uint8_t> messageBytes = stringToByte(message);
	
	// Compute the hash
	RateBlock hashOutput = aegisHasher.hashMessage(messageBytes);
	
	// Print the hash in hexadecimal format
	writeHashHexToFile("HashOutput.txt", std::vector<uint8_t>(hashOutput.begin(), hashOutput.end()));

	avalancheTest();
	
}