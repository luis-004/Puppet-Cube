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

class AlgMani {
    public:
        AlgMani();
        std::string translateRightie(const std::vector<std::array<uint8_t, 2>> alg);
        std::string translateLeftie(const std::vector<std::array<uint8_t, 2>> alg);
        std::vector<std::array<uint8_t, 2>> translateBack(const std::string alg);
        std::vector<std::array<uint8_t, 2>> translateYX2(std::vector<std::array<uint8_t, 2>> alg);
        std::vector<std::array<uint8_t, 2>> reverse(std::vector<std::array<uint8_t, 2>> alg);
        std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> optimizeRUF(std::vector<std::array<uint8_t, 2>> alg, std::array<uint8_t, 6> turns);
        std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> optimizeRF(std::vector<std::array<uint8_t, 2>> alg, std::array<uint8_t, 6> turns);
        std::array<uint8_t, 6> getRUF(std::vector<std::array<uint8_t, 2>> alg);
        std::vector<std::array<uint8_t, 2>> swap(std::vector<std::array<uint8_t, 2>> alg, uint8_t stay);
        std::vector<std::array<uint8_t, 2>> twist(std::vector<std::array<uint8_t, 2>> alg, bool dir);
};