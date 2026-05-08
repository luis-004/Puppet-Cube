#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <utility>
#include <cstdlib>
#include <unordered_map>

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

    auto start_time = std::chrono::high_resolution_clock::now();
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
    auto scramble = star.Astar_RSS(MC, MF, MS, ME, HF, HE, H_shape, HCF, HSE);
    std::cout << "Ready" << std::endl;
    uint16_t count = 0;
    while(true){
        std::getline(std::cin, input);
        std::cout << ++count << " " << am.translateRightie(am.translateYX2(am.reverse(scramble))) << " " << scramble.size();
        if (input == "q") break;
        if (input == "c") system("cls");
        //system("cls");
        scramble = star.Astar_RSS(MC, MF, MS, ME, HF, HE, H_shape, HCF, HSE);
    }
    std::cin.get();
    return 0;
}