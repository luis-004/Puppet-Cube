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

//#include "Translations.cpp"

class Puppet {
    public:
        struct Shape{
            uint8_t data[5][5][5];
        };
        struct ShapeBin{
            bool data[5][5][5];
        };
        struct Corners{
            uint8_t data[7];
        };
        struct Edges{
            uint8_t data[9];
        };
        struct Flips{
            bool data[9];
        };
        Puppet();
        void cubeShape(Shape& shape);
        ShapeBin getBinShape(const Shape& shape);
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
        bool testU2LShape(const Shape& shape);
        bool testLShape(const Shape& shape);
        bool testLBShape(const Shape& shape);
        int getVShape(const ShapeBin& shape);
        int getBShape(const ShapeBin& shape);
        int getU2VShape(const ShapeBin& shape);
        int getU2LShape(const ShapeBin& shape);
        bool testLayer(const Shape& shape);
        bool testSameShapeBin(ShapeBin& shape_1,ShapeBin& shape_2);
        void solvedCorners(Corners& corners);
        void rotateCornersCS(Corners& corners, bool dir);
        void mirrorCornersCS(Corners& corners);
        void doTurnCorners(Corners& corners, uint8_t face, uint8_t dir);
        bool testCornersSolved(const Corners& corners);
        void solvedEdges(Edges& edges);
        void doTurnEdges(Edges& edges, uint8_t face, uint8_t dir);
        uint8_t countEdgesSolved(const Edges& edges);
        void printEdges(const Edges& edges);
        void solvedFlips(Flips& flips);
        void doTurnFlips(Flips& flips, uint8_t face, uint8_t dir);
        uint8_t countFlips(const Flips& flips);
        void printFlips(const Flips& flips);
        std::vector<uint8_t> getEdgeCycle(const Edges& edges);
        std::vector<uint8_t> getEdgesSolved(const Edges& edges);
        std::vector<uint8_t> getFlips(const Flips& flips);
        void printCycle(const std::vector<uint8_t> cycle);
        void printFlippedEdges(const std::vector<uint8_t> flippedEdges);
};