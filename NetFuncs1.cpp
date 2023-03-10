#include <iostream>
#include <vector>
#include <sstream>

extern "C" std::vector<float> pos0(std::vector<float> i_positions, std::vector<float> i_motorPosition) {
    std::vector<float> newPositions = i_motorPosition;

    newPositions[0] = newPositions[0] + 4;
    return newPositions;
}


extern "C" std::vector<float> zeroMotors(const std::vector<float> i_positions, const std::vector<float> i_motorPosition) {
    std::stringstream ss;

    std::vector<float> newPositions;

    for(auto motor : i_motorPosition){
        if(motor > 10)
            newPositions.push_back(0);
        else
            newPositions.push_back(motor);
    }

    return newPositions;
}

extern "C" std::vector<float> dec1(const std::vector<float> i_positions, const std::vector<float> i_motorPosition) {
    std::vector<float> newPositions = i_motorPosition;

    newPositions[0] = newPositions[0] - 4;
    return newPositions;
}