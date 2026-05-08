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
#include <unordered_map>
//#include <bits/stdc++.h>
#include <random>

#include "Puppet.hpp"
#include "AlgMani.hpp"
#include "AlgFinder.hpp"
#include "AlgFinderExt.hpp"
#include "Scrambler.hpp"
#include "Astar.hpp"

using namespace std;

int main(){
    AlgFinder algfinder;
    AlgFinderExt ext;
    Astar star;
    Puppet p;
    AlgMani am;
    Scrambler S;
    Puppet::Shape shape;
    Puppet::Edges edges;
    Puppet::Corners corners;
    Puppet::Flips flips;
    Puppet::State state;
    Puppet::ShapeFast shapefast;
    Puppet::StateFast sf;
    string input;

    auto v_shapes = S.computeVShapes();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> indizes(v_shapes.size());
    std::iota(indizes.begin(), indizes.end(), 0);
    std::shuffle(indizes.begin(), indizes.end(), gen);
    std::cout << "Ready" << std::endl;
    uint16_t count = 0;
    auto scramble = S.ScrambleWithShape(20, v_shapes[indizes[count++]]);
     while(true){
        std::getline(std::cin, input);
        std::cout << count << " " << am.translateRightie(am.translateYX2(scramble)) << " " << scramble.size();
        if (input == "q") break;
        if (input == "c") system("cls");
        //system("cls");
        scramble = S.ScrambleWithShape(20, v_shapes[indizes[count++]]);
        if(count >= v_shapes.size()) { std::cout << "these were all cases" << std::endl; count = 0;}
    }

    /*auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    //std::unordered_map<KeyCorners, uint32_t> LC;
    //std::unordered_map<Puppet::FlipsFast, uint32_t> LF;
    //std::unordered_map<Puppet::ShapeFast, uint32_t> LS;
    //std::unordered_map<Puppet::Edges, uint32_t> LE;
    std::unordered_map<KeyCorners, Puppet::Shape> L_shape;
    //Map::loadMap(LC, "cornerLinks");
    //Map::loadMap(LF, "flipLinks");
    //Map::loadMap(LS, "shapeLinks");
    //Map::loadMap(LE, "edgeLinks");
    Map::loadMap(L_shape, "LinkToShapes");
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "maps loaded: " << duration.count() <<  std::endl;
    //star.convertToFlatFile<KeyCorners, uint32_t>(LC, "cornerLinks_vec");
    //star.convertToFlatFile<Puppet::FlipsFast, uint32_t>(LF, "flipLinks_vec");
    //star.convertToFlatFile<Puppet::ShapeFast, uint32_t>(LS, "shapeLinks_vec");
    //star.convertToFlatFile<Puppet::Edges, uint32_t>(LE, "edgeLinks_vec");
    star.convertToFlatFile<KeyCorners, Puppet::Shape>(L_shape, "LinkToShapes_vec");
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "converted: " << duration.count() <<  std::endl;*/

    /*auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    //std::unordered_map<KeyCorners, uint32_t> LC;
    std::unordered_map<Puppet::ShapeFast, uint32_t> LS;
    std::unordered_map<Puppet::Edges, uint32_t> LE;
    //std::unordered_map<Puppet::FlipsFast, uint32_t> LF;
    //Map::loadMap(LC, "cornerLinks");
    Map::loadMap(LS, "shapeLinks");
    Map::loadMap(LE, "edgeLinks");
    //Map::loadMap(LF, "flipLinks");
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "loaded links: " << duration.count() <<  std::endl;
    std::vector<uint8_t> HSE;
    star.getSEarray(HSE, LS, LE);*/

    /*auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    std::unordered_map<KeyCorners, uint32_t> LC;
    //std::unordered_map<Puppet::ShapeFast, uint32_t> LS;
    //std::unordered_map<Puppet::Edges, uint32_t> LE;
    std::unordered_map<Puppet::FlipsFast, uint32_t> LF;
    Map::loadMap(LC, "cornerLinks");
    //Map::loadMap(LS, "shapeLinks");
    //Map::loadMap(LE, "edgeLinks");
    Map::loadMap(LF, "flipLinks");
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "loaded links: " << duration.count() <<  std::endl;
    std::vector<uint8_t> HCF;
    star.getCFarray(HCF, LC, LF);
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "cf array ready: " << duration.count() <<  std::endl;*/
    //star.writeData("CFarray", HCF.data(), HCF.size() * sizeof(uint8_t));
    //now = std::chrono::high_resolution_clock::now();
    //duration = now - start_time;
    //std::cout << "cf array saved: " << duration.count() <<  std::endl;

    //std::vector<CornerEntry> HC;
    //std::vector<SFEntry> HSF;
    //std::vector<FEEntry> H4E1;
    //std::vector<FEEntry> H4E2;
    //std::vector<EdgeEntry> HE;
    //std::vector<SEEntry> HSE;
    /*std::unordered_map<KeyCorners, uint32_t> LC;
    std::unordered_map<Puppet::FlipsFast, uint32_t> LF;
    std::unordered_map<Puppet::ShapeFast, uint32_t> LS;
    std::unordered_map<Puppet::Edges, uint32_t> LE;
    std::vector<uint8_t> HCF;
    std::vector<uint8_t> HSE;
    //star.loadHeuristic<CornerEntry>(HC, "corners.heu");
    //star.loadHeuristic<SFEntry>(HSF, "SF.heu");
    //star.loadHeuristic<FEEntry>(H4E1, "S4E1_14.heu");
    //star.loadHeuristic<FEEntry>(H4E2, "S4E2_14.heu");
    //star.loadHeuristic<EdgeEntry>(HE, "edges.heu");
    std::vector<std::array<uint8_t, 2>> alg;
    //star.loadHeuristic<SEEntry>(HSE, "SE_15.heu");
    Map::loadMap(LC, "cornerLinks");
    Map::loadMap(LF, "flipLinks");
    Map::loadMap(LS, "shapeLinks");
    Map::loadMap(LE, "edgeLinks");
    star.loadHeuristic<uint8_t>(HCF, "CFheu");
    star.loadLargeHeuristic<uint8_t>(HSE, "SE_full");
   
    std::vector<Puppet::ShapeBin> shapes = S.computeAllShapes();
    for(int i = 0; i < 10 ; i++){
        auto scramble = S.RandomShape(30,shapes);
        //auto scramble = am.translateYX2(am.translateBack("U'F'R'FRUF2U2FU'F'RFR2F'UF'RFR'U'RU'R'FUR'FU'RU2R2U'RUR'FRF'U'RU'F'RFR'U2FRUR'U'RU2FU2R'F'UR'"));
        std::cout << am.translateRightie(scramble) << std::endl;
        p.solvedState(state);
        sf = p.getFastState(state);
        std::cout << "Heuristic value of solved state:"  << std::endl;
        std::cout << int(star.getUltraValS(HCF, LC, LF, HSE, LS, LE, sf))<< std::endl;
        //std::cout << int(star.getHeuValCE(HC, HE, sf))<< std::endl;
        for(auto turn : scramble){
            p.doTurnState(state, turn[0], turn[1]);
            sf = p.getFastState(state);
            std::cout << size_t(LS[sf.shape]*E_SIZE + LE[sf.edges]) << " " << int(size_t(HSE[size_t(LS[sf.shape])*E_SIZE + LE[sf.edges]])) << std::endl;
            //std::cout << size_t(LC[{sf.shape, sf.corners}]*F_SIZE + LF[sf.flips]) << " " << int(HCF[LC[{sf.shape, sf.corners}]*F_SIZE + LF[sf.flips]]) << std::endl;
            //auto i = std::lower_bound(HSE.begin(), HSE.end(), e);
            //if (i != HSE.end()) {
            //    if(!(e.key == i->key)){
                    //std::cout << "Other Key not found !!!" << std::endl;
            //       std::cout << "255 ";
            //    }else{
            //        std::cout << int(i->value) << " ";
            //    }
            //}   
        }
        sf = p.getFastState(state);
        std::cout << std::endl << "Heuristic value of start state:"  << std::endl;
        std::cout << int(star.getUltraValS(HCF, LC, LF, HSE, LS, LE, sf))<< std::endl;
        //std::cout << int(star.getHeuValCE(HC, HE, sf))<< std::endl;
        std::cout << "running A* with Shape Heuristics..."  << std::endl;
        alg = star.ultrastar(state, HCF, LC, LF, HSE, LS, LE);
        std::cout << am.translateRightie(am.reverse(am.translateYX2(am.reverse(alg))))<< " " << int(alg.size()) << std::endl;
        std::cout << "A* finished"  << std::endl;
    }*/

    /*std::unordered_map<KeyCorners, Puppet::Shape> M_shape;
    star.getLinkToShape(M_shape);
    Map::saveMap(M_shape, "LinkToShapes");*/

    /*auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    std::cout << "loading..." <<  std::endl;
    std::unordered_map<KeyCorners, uint32_t> MC;
    std::unordered_map<Puppet::FlipsFast, uint32_t> MF;
    std::unordered_map<Puppet::ShapeFast, uint32_t> MS;
    std::unordered_map<Puppet::Edges, uint32_t> ME;
    //std::vector<Entry<KeyCorners, uint32_t>> HC;
    std::vector<Entry<Puppet::FlipsFast, uint32_t>> HF;
    //std::vector<Entry<Puppet::ShapeFast, uint32_t>> HS;
    std::vector<Entry<Puppet::Edges, uint32_t>> HE;
    std::vector<Entry<KeyCorners, Puppet::Shape>> H_shape;
    std::vector<uint8_t> HCF;
    std::vector<uint8_t> HSE;
    Map::loadMap(MC, "cornerLinks");
    Map::loadMap(MF, "flipLinks");
    Map::loadMap(MS, "shapeLinks");
    Map::loadMap(ME, "edgeLinks");
    //star.loadHeuristic<Entry<KeyCorners, uint32_t>>(HC, "cornerLinks_vec");
    star.loadHeuristic<Entry<Puppet::FlipsFast, uint32_t>>(HF, "flipLinks_vec");
    //star.loadHeuristic<Entry<Puppet::ShapeFast, uint32_t>>(HS, "shapeLinks_vec");
    star.loadHeuristic<Entry<Puppet::Edges, uint32_t>>(HE, "edgeLinks_vec");
    star.loadHeuristic<Entry<KeyCorners, Puppet::Shape>>(H_shape, "LinkToShapes_vec");
    star.loadHeuristic<uint8_t>(HCF, "CFheu");
    star.loadLargeHeuristic<uint8_t>(HSE, "SE_full");
    now = std::chrono::high_resolution_clock::now();
    auto last = now;
    duration = now - start_time;
    std::cout << "loaded: " << duration.count() <<  std::endl;
    std::cout << MC.size() << std::endl;
    std::cout << MF.size() << std::endl;
    std::cout << MS.size() << std::endl;
    std::cout << ME.size() << std::endl;
    std::cout << H_shape.size() << std::endl;
    std::cout << HF.size() << std::endl;
    std::cout << HE.size() << std::endl;
    auto scramble = star.Astar_RSS(MC, MF, MS, ME, HF, HE, H_shape, HCF, HSE);
    std::cout << "Ready" << std::endl;
    uint16_t count = 0;
    while(true){
        std::getline(std::cin, input);
        std::cout << ++count << " " << am.translateRightie(am.translateYX2(am.reverse(scramble))) << " " << scramble.size() << " ";
        if (input == "q") break;
        if (input == "c") system("cls");
        //system("cls");
        scramble = star.Astar_RSS(MC, MF, MS, ME, HF, HE, H_shape, HCF, HSE);
    }
    /*uint16_t count = 0;
    uint16_t lensum = 0;
    for(int i = 0; i < 50; i++){
        auto scramble = star.Astar_RSS(MC, MF, MS, ME, HF, HE, H_shape, HCF, HSE);
        std::cout << ++count << " " << am.translateRightie(am.translateYX2(am.reverse(scramble))) << " " << scramble.size() << " ";
        lensum += scramble.size();
        std::cout << "avg movecount: " << int(lensum/count) << std::endl;
        now = std::chrono::high_resolution_clock::now();
        duration = now - last;
        last = now;
        std::cout << "time: " << duration.count() <<  std::endl;
    }*/
    std::cin.get();
    return 0;
}