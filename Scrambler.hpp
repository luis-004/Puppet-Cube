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
        std::vector<std::array<uint8_t, 2>> RMS(uint16_t movecount);
        std::vector<Puppet::ShapeBin> computeAllShapes();
        std::vector<Puppet::ShapeBin> computeVShapes();
        auto findShapeFromShape(const Puppet::Shape& start, const Puppet::ShapeBin& end);
        std::vector<std::array<uint8_t, 2>> RandomShape(uint8_t movecount, std::vector<Puppet::ShapeBin> allShapes);
        std::vector<std::array<uint8_t, 2>> ScrambleWithShape(uint8_t movecount, Puppet::ShapeBin goal);
};