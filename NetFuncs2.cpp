#include <iostream>
#include <vector>
#include <sstream>

extern "C" std::vector<float> nf2_1(std::vector<float> i_positions, std::vector<float> i_motorPosition) {
    std::vector<float> newPositions = i_motorPosition;

    newPositions[3] = newPositions[3] + 4;
    //  std::cout << "inced to " << newPositions[0];
    return newPositions;
}


extern "C" std::vector<float> nf2_2(const std::vector<float> i_positions, const std::vector<float> i_motorPosition) {
    std::stringstream ss;

    std::vector<float> newPositions;

    for(auto motor : i_motorPosition){
        if(motor > 10)
            newPositions.push_back(0);
        else
            newPositions.push_back(motor);
    }

    // std::cout << "M2" << std::endl;
    return newPositions;
}

