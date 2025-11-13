#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <utility>
//#include <cmath>
//#include <array>
//#include <algorithm>
//#include <queue>
#include <cstdlib>

#include "Puppet.hpp"
#include "AlgMani.hpp"
#include "AlgFinder.hpp"
#include "AlgFinderExt.hpp"
#include "Scrambler.hpp"

int main()
{
    AlgFinder algfinder;
    AlgFinderExt ext;
    Puppet p;
    AlgMani am;
    Scrambler S;
    Puppet::Shape shape;
    Puppet::Edges edges;
    Puppet::Corners corners;
    Puppet::Flips flips;
    std::string input;
    std::vector<Puppet::ShapeBin> shapes = S.computeAllShapes();
    auto scramble = S.RandomShape(40,shapes);
    std::cout << "Ready" << std::endl;
    while(true){
        std::getline(std::cin, input);
        std::cout << am.translateRightie(am.translateYX2(scramble)) << std::endl;
        if (input == "q") break;
        if (input == "c") system("cls");
        //system("cls");
        scramble = S.RandomShape(40,shapes);
    }
    //std::cout << am.translateRightie(am.reverse(std::vector<std::array<uint8_t, 2>> {}));
    //std::cout << am.translateLeftie(am.swap(am.translateBack("URU2R'F'U'F'R2FRF'RFUF2U'R'URU2R'U'"), 2)) << std::endl;
    //std::cout << am.translateRightie(am.reverse(am.swap(am.translateBack("U'RF'U2FRUFR2F'R'U'R'U"), 2))) << std::endl;
    //std::cout << am.translateRightie(am.swap(am.translateBack("U2R'F'UFRU2R'U'RFR2F'U2"), 2)) << std::endl;
    //std::cout << am.translateRightie(am.reverse(am.swap(am.translateBack("U2R'F'UFRU2R'U'RFR2F'U2"), 2))) << std::endl;
    //algfinder.searchCubeShapeL(13);
    //std::cout << am.translateRightie(am.translateYX2(am.translateBack("RU'R2UR2U2R2F2R2U2R2F2U'R2UR'"))) << std::endl;
    // ext.printLtoLayer();
    //ext.searchFlipsFast(8);
    // algfinder.searchCubeShapeL(13);
    //algfinder.searchAllShapes();
    //algfinder.searchShape("RU'FR2U'R'FURU'RF2R'", 14, 5);
    // algfinder.searchCubeShapeL(16);
    ///algfinder.search4cycle(18);
    // algfinder.searchEdges(12, 3);
    //algfinder.searchCubeShapeLB(10);
    // algfinder.searchCornerCase("URUR'FU'R2UR'F2RU2",13);
    // std::cout << am.translateRightie(am.swap(am.translateBack("U'R'F2RU2RF'R'FRUR"), 0)) << std::endl;
    // algfinder.searchShape("R'FR'U'R2FU'", 10);
    /*std::cout << am.translateRightie(am.twist(am.translateBack("RUR2U'F'R"), 0)) << std::endl;
    std::cout << am.translateRightie(am.twist(am.translateBack("RUR2U'F'R"), 1)) << std::endl;
    std::cout << am.translateRightie(am.twist(am.swap(am.translateBack("RUR2U'F'R"), 1), 0)) << std::endl;
    std::cout << am.translateRightie(am.twist(am.swap(am.translateBack("RUR2U'F'R"), 1), 1)) << std::endl;
    std::cout << am.translateRightie(am.swap(am.translateBack("RUR2U'F'R"), 1)) << std::endl;*/
    //std::cin.get();
    return 0;
}