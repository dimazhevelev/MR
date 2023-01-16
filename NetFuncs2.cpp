#include <iostream>
#include <vector>
#include <sstream>

extern "C" std::vector<float> inc3(std::vector<float> i_positions, std::vector<float> i_motorPosition) {
    std::vector<float> newPositions = i_motorPosition;

    newPositions[3] = newPositions[3] + 4;
    return newPositions;
}


extern "C" std::vector<float> dec2(const std::vector<float> i_positions, const std::vector<float> i_motorPosition) {
    std::vector<float> newPositions = i_motorPosition;

    newPositions[2] = newPositions[2] - 3;
    return newPositions;
}

