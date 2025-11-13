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
#include <chrono>

#include "Puppet.hpp"
#include "AlgMani.hpp"

class AlgFinderExt{
    private:
        Puppet puppet;
        AlgMani am;
    public:
        AlgFinderExt();
        auto computeAlgsFast(uint8_t max_turns);
        void searchFlipsFast(uint8_t max_turns, bool odd);
        std::vector<std::pair<Puppet::Shape, std::vector<std::array<uint8_t, 2>>>> searchShapes();
        void searchToLayer(const Puppet::Shape& start);
        void printLtoLayer();
    };