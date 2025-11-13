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
#include "AlgFinderExt.hpp"

class AlgFinder{
    private:
        Puppet puppet;
        AlgMani am;
    public:
        AlgFinder();
        void searchCubeShapeY(uint8_t max_turns);
        void searchCubeShapeL(uint8_t max_turns);
        void searchCubeShapeLcom(uint8_t max_turns, bool stf);
        void searchCubeShapeLB(uint8_t max_turns);
        void searchCorners(uint8_t max_turns);
        void searchEdges(uint8_t max_turns, uint8_t count);
        void searchShape(std::string alg, uint8_t max_turns, uint8_t max_count);
        void searchCornerCase(std::string alg, uint8_t max_turns);
        void search3cycle(uint8_t max_turns);
        void search4cycle(uint8_t max_turns);
        void searchAllShapes();
        void searchFlips(uint8_t max_turns);
        void searchFlipsDFS(uint8_t max_turns);
};