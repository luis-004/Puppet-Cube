#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <utility>
#include <cmath>
#include <array>
#include <algorithm>
#include <queue>
#include <random>

#include "Puppet.hpp"
#include "AlgMani.hpp"

class Scrambler{
    private:
        Puppet puppet;
        AlgMani am;
    public:
        std::vector<std::array<uint8_t, 2>> RMS(uint8_t movecount);
        std::vector<Puppet::ShapeBin> computeAllShapes();
        auto findShapeFromShape(const Puppet::Shape& start, const Puppet::ShapeBin& end);
        std::vector<std::array<uint8_t, 2>> RandomShape(uint8_t movecount, std::vector<Puppet::ShapeBin> allShapes);
};