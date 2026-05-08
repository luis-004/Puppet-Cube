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

//#include "Translations.cpp"

class Puppet {
    public:
        struct Shape{
            uint8_t data[5][5][5];
        };
        struct ShapeBin{
            bool data[5][5][5];
        };
        struct ShapeFast{
            uint64_t bits[2];

            ShapeFast() { bits[0] = 0; bits[1] = 0; }

            // Umwandlung von 3D-Koordinaten zu Bits
            void setPixel(int x, int y, int z) {
                int bitPos = x + (5 * y) + (25 * z);
                // Wähle Zahl 0 oder 1, dann schiebe die 1 an die richtige Stelle
                bits[bitPos / 64] |= (1ULL << (bitPos % 64));
            }

            bool operator==(const ShapeFast& other) const {
                return bits[0] == other.bits[0] && bits[1] == other.bits[1];
            }
            bool operator<(const ShapeFast& other) const{
                if (bits[0] != other.bits[0])
                    return bits[0] < other.bits[0];
                if (bits[1] != other.bits[1])
                    return bits[1] < other.bits[1];
                return false;
            }
        };
        struct Corners{
            uint8_t data[7];
            bool operator==(const Corners& other) const {
                return std::memcmp(data, other.data, 7) == 0;
            }
        };
        struct Edges{
            uint8_t data[9];
            bool operator==(const Edges& other) const {
                return std::memcmp(data, other.data, 9) == 0;
            }
            bool operator<(const Edges& other) const{
                for(int i = 0; i < 9; ++i) {
                    if (data[i] != other.data[i])
                        return data[i] < other.data[i];
                }
                return false;
            }
        };
        struct fourE{
            uint8_t data[4];
            bool operator==(const fourE& other) const {
                return std::memcmp(data, other.data, 4) == 0;
            }
        };
        struct Flips{
            bool data[9];
        };
        struct FlipsFast {
            // 9 Bits passen locker in ein uint16_t
            uint16_t bits = 0; 

            void setFlip(int index, bool value) {
                if (value) bits |= (1U << index);
                else bits &= ~(1U << index);
            }
            bool getFlip(int index) const {
                return (bits & (1U << index)) != 0;
            }
            bool operator==(const FlipsFast& other) const {
                return bits == other.bits;
            }
            bool operator<(const FlipsFast& other) const{
                return bits < other.bits;
            }
        };
        struct State {
            Shape shape;
            Corners corners;
            Edges edges;
            Flips flips;
        };
        struct StateFast {
            ShapeFast shape;
            Corners corners;
            Edges edges;
            FlipsFast flips;
            // WICHTIG: Der Vergleichsoperator für die unordered_map
            bool operator==(const StateFast& other) const {
                return shape == other.shape &&
                    corners == other.corners &&
                    edges == other.edges &&
                    flips == other.flips;
            }
        };
        Puppet();
        void cubeShape(Shape& shape);
        ShapeBin getBinShape(const Shape& shape);
        template <typename T>
        ShapeFast getFastShape(const T& shape){
            ShapeFast shapefast = {};
            for(uint8_t x = 0; x <5; ++x){
                for(uint8_t y = 0; y < 5; ++y){
                    for(uint8_t z = 0; z < 5; ++z){
                        if(shape.data[x][y][z] > 0){
                            shapefast.setPixel(x, y, z);
                        }
                    }
                }
            }
            return shapefast;
        }
        void rotateShapeBin(ShapeBin& shape, bool dir);
        void mirrorShapeBin(ShapeBin& shape, uint8_t stay);
        bool doTurn(Shape& shape, uint8_t face, uint8_t dir);
        bool testTurn(Shape& shape, uint8_t face, uint8_t dir);
        void doAlgExpectPossible(Shape& shape, std::vector<std::array<uint8_t, 2>> alg);
        bool testAlg(Shape& shape, std::vector<std::array<uint8_t, 2>> alg);
        bool testCubeShape(const Shape& shape);
        bool testYShape(const Shape& shape);
        bool testVShape(const Shape& shape);
        bool testBShape(const Shape& shape);
        bool testU2VShape(const Shape& shape);
        bool test90VShape(const Shape& shape);
        bool testU2LShape(const Shape& shape);
        bool testL1Shape(const Shape& shape);
        bool testL2Shape(const Shape& shape);
        bool testL3Shape(const Shape& shape);
        bool testL4Shape(const Shape& shape);
        bool testLShape(const Shape& shape);
        bool testLBShape(const Shape& shape);
        int getVShape(const ShapeBin& shape);
        int getBShape(const ShapeBin& shape);
        int getU2VShape(const ShapeBin& shape);
        int get90VShape(const ShapeBin& shape);
        int getU2LShape(const ShapeBin& shape);
        int getL1Shape(const ShapeBin& shape);
        int getL2Shape(const ShapeBin& shape);
        int getL3Shape(const ShapeBin& shape);
        int getL4Shape(const ShapeBin& shape);
        bool testLayer(const Shape& shape);
        bool testSameShapeBin(ShapeBin& shape_1,ShapeBin& shape_2);
        void solvedCorners(Corners& corners);
        void rotateCornersCS(Corners& corners, bool dir);
        void mirrorCornersCS(Corners& corners);
        void rotateCorners(Corners& corners);
        void mirrorCorners(Corners& corners);
        void doTurnCorners(Corners& corners, uint8_t face, uint8_t dir);
        bool testCornersSolved(const Corners& corners);
        void solvedEdges(Edges& edges);
        void doTurnEdges(Edges& edges, uint8_t face, uint8_t dir);
        uint8_t countEdgesSolved(const Edges& edges);
        void printEdges(const Edges& edges);
        void rotateEdges(Edges& edges);
        void mirrorEdges(Edges& edges);
        void solvedFlips(Flips& flips);
        void doTurnFlips(Flips& flips, uint8_t face, uint8_t dir);
        uint8_t countFlips(const Flips& flips);
        void printFlips(const Flips& flips);
        FlipsFast getFastFlips(const Flips& flips);
        Flips getFlipsFromFast(const FlipsFast& ff);
        std::vector<uint8_t> getEdgeCycle(const Edges& edges);
        std::vector<uint8_t> getEdgesSolved(const Edges& edges);
        std::vector<uint8_t> getFlips(const Flips& flips);
        void printCycle(const std::vector<uint8_t> cycle);
        void printFlippedEdges(const std::vector<uint8_t> flippedEdges);
        void rotateFlips(Flips& flips);
        void mirrorFlips(Flips& flips);
        bool doTurnState(State& state, uint8_t face, uint8_t dir);
        void solvedState(State& state);
        StateFast getFastState(const State& state);
        fourE get4E(const Edges& edges, bool G);
        void solveParity(const Corners& corners, Edges& edges);
};