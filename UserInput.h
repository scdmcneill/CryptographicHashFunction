#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<uint8_t> stringToByte(const std::string& input) {
	std::vector<uint8_t> bytes(input.begin(), input.end());
	
	if (bytes.empty()) {
		throw std::invalid_argument("stringToByte: Input string is empty.");
	}

	return bytes;
}

void writeHashBinaryToFile(const std::string& filename, const std::vector<uint8_t>& hash) {
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		std::cerr << "writeHashBinaryToFile: Error opening file " << filename << " for writing." << '\n';
		return;
	}

	outFile.write(reinterpret_cast<const char*>(hash.data()), hash.size());
}

void writeHashHexToFile(const std::string& filename, const std::vector<uint8_t>& hash) {
	std::ofstream outFile(filename);
	if (!outFile) {
		std::cerr << "writeHashHexToFile: Error opening file " << filename << " for writing." << '\n';
		return;
	}
	for (const auto& byte : hash) {
		outFile << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
	}
	outFile << std::dec << '\n'; // Reset to decimal format
}