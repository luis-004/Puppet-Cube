#include "AlgFinderExt.hpp"
AlgFinderExt::AlgFinderExt(){}

auto AlgFinderExt::computeAlgsFast(uint8_t max_turns){
    Puppet::Shape shape;
    uint8_t last_turn = 0;
    std::array<std::array<uint8_t, 2>, 9> turns = {{{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}}};
    std::vector<std::array<uint8_t, 2>> alg = {};
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::vector<std::array<uint8_t, 2>>> valid_turns(max_turns + 1);
    valid_turns[0].push_back({{0,0}});
    valid_turns[0].push_back({{0,1}});
    //valid_turns[0].push_back({{0,2}});
    bool new_alg = 1;
    bool new_step = 0;
    puppet.cubeShape(shape);
    while(true){
        if(alg.size() == max_turns){
            found_algs.push_back(alg);
            found_shapes.push_back(puppet.getBinShape(shape));
            puppet.doTurn(shape, alg.back()[0], 2 - alg.back()[1]);
            alg.pop_back();
            valid_turns.at(alg.size()).pop_back();
            new_step = 0;
        }
        if(new_step){
            last_turn = alg.back()[0];
            valid_turns.at(alg.size()).clear();
            for(const auto& t : turns) {
                if(t[0] != last_turn && puppet.testTurn(shape, t[0], t[1])){
                    valid_turns.at(alg.size()).push_back(t);
                }
            }
        }
        if(valid_turns.at(alg.size()).size() == 0){
            if(alg.empty()){
                break;
            }
            puppet.doTurn(shape, alg.back()[0], 2 - alg.back()[1]);
            alg.pop_back();
            valid_turns.at(alg.size()).pop_back();
            new_step = 0;
            continue;   
        }
        puppet.doTurn(shape, valid_turns.at(alg.size()).back()[0], valid_turns.at(alg.size()).back()[1]);
        alg.push_back({valid_turns.at(alg.size()).back()[0], valid_turns.at(alg.size()).back()[1]});
        new_step = 1;
    }
    return std::make_pair(found_algs, found_shapes);
}

void AlgFinderExt::searchFlipsFast(uint8_t max_turns, bool odd){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto pair = computeAlgsFast(max_turns);
    auto now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    auto found_algs= pair.first;
    auto found_shapes = pair.second;
    std::cout << found_algs.size() << std::endl;
    auto copy = pair;
    if(odd){
        copy = computeAlgsFast(max_turns-1);
        now = std::chrono::high_resolution_clock::now();
        duration = now - start_time;
        std::cout << "time: " << duration.count() <<  std::endl;
    }
    Puppet::ShapeBin found_shape;
    std::vector<std::vector<std::array<uint8_t, 2>>> new_algs;
    size_t all_size = copy.first.size();
    for(size_t i = 0; i < all_size; i++){
        copy.first.push_back(am.twist(copy.first[i], 0));
        copy.first.push_back(am.twist(copy.first[i], 1));
        copy.first.push_back(am.twist(am.swap(copy.first[i], 1), 0));
        copy.first.push_back(am.twist(am.swap(copy.first[i], 1), 1));
        copy.first.push_back(am.swap(copy.first[i], 1));
        found_shape = copy.second[i];
        puppet.rotateShapeBin(found_shape, 0);
        copy.second.push_back(found_shape);
        puppet.rotateShapeBin(found_shape, 0);
        copy.second.push_back(found_shape);
        puppet.mirrorShapeBin(found_shape, 1);
        copy.second.push_back(found_shape);
        puppet.rotateShapeBin(found_shape, 0);
        copy.second.push_back(found_shape);
        puppet.rotateShapeBin(found_shape, 0);
        copy.second.push_back(found_shape);
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    auto all_algs= copy.first;
    auto all_shapes = copy.second;
    std::cout << all_algs.size() << std::endl;
    bool new_alg = 1;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    std::vector<std::array<uint8_t, 2>> alg;
    std::vector<std::array<uint8_t, 2>> alg2;
    for(size_t i = 0; i < found_shapes.size(); i++){
        for(size_t j = (size_t(!odd)*i+size_t(!odd)); j < all_shapes.size(); j++){
            if(std::memcmp(all_shapes[j].data, found_shapes[i].data, sizeof(Puppet::ShapeBin::data)) == 0){
                if(found_algs[i].back()[0] == all_algs[j].back()[0]){
                    continue;
                }
                alg = am.reverse(all_algs[j]);
                alg.insert(alg.begin(),found_algs[i].begin(),found_algs[i].end());
                new_alg = 1;
                for(uint64_t k = 0; k < new_algs.size(); k++){
                    if(new_algs[k] == alg){
                        new_alg = 0;
                        break;
                    } 
                }
                if(!new_alg){
                    continue;
                }
                puppet.solvedCorners(corners);
                for(auto turn : alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!puppet.testCornersSolved(corners)){
                    continue;
                }
                puppet.solvedEdges(edges);
                for(auto turn : alg){
                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                }
                if(puppet.countEdgesSolved(edges) != 9){
                    continue;
                }
                puppet.solvedFlips(flips);
                for(auto turn : alg){
                    puppet.doTurnFlips(flips, turn[0], turn[1]);
                }
                if(puppet.countFlips(flips) == 0){
                    continue;
                }
                auto pair =  am.optimizeRUF(am.translateYX2(alg), am.getRUF(am.translateYX2(alg)));
                std::cout << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(alg.size())<< " ";
                for(auto n : pair.second){
                    std::cout << int(n);
                }
                std::cout << " ";
                puppet.printFlippedEdges(puppet.getFlips(flips));
                std::cout << std::endl;
                new_algs.push_back(alg);
                new_algs.push_back((am.twist(alg,0)));
                new_algs.push_back((am.twist(alg,1)));
                new_algs.push_back((am.swap(alg,0)));
                new_algs.push_back((am.swap(alg,1)));
                new_algs.push_back((am.swap(alg,2)));
                new_algs.push_back((am.swap(am.twist(alg,0),0)));
                new_algs.push_back((am.swap(am.twist(alg,0),1)));
                new_algs.push_back((am.swap(am.twist(alg,0),2)));
                new_algs.push_back((am.swap(am.twist(alg,1),0)));
                new_algs.push_back((am.swap(am.twist(alg,1),1)));
                new_algs.push_back((am.swap(am.twist(alg,1),2)));
                new_algs.push_back((am.reverse(alg)));
                new_algs.push_back((am.reverse(am.twist(alg,0))));
                new_algs.push_back((am.reverse(am.twist(alg,1))));
                new_algs.push_back((am.reverse(am.swap(alg,0))));
                new_algs.push_back((am.reverse(am.swap(alg,1))));
                new_algs.push_back((am.reverse(am.swap(alg,2))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,0),0))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,0),1))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,0),2))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,1),0))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,1),1))));
                new_algs.push_back((am.reverse(am.swap(am.twist(alg,1),2))));
            }
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
}

std::vector<std::pair<Puppet::Shape, std::vector<std::array<uint8_t, 2>>>> AlgFinderExt::searchShapes(){
    Puppet::Shape shape;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<Puppet::Shape, std::vector<std::array<uint8_t, 2>>>> ret_shapes;
    std::vector<std::array<uint8_t, 2>> alg;
    found_shape = puppet.getBinShape(shape); 
    found_shapes.push_back(found_shape);
    ret_shapes.push_back(std::make_pair(shape, alg));
    bool s = true;
    while(true){
        puppet.cubeShape(shape);
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            return ret_shapes;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getBinShape(shape); 
                    hit = true;
                    for(uint16_t i = 0; i < found_shapes.size(); i++){
                        if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                            hit = false;
                            break;
                        }
                    }
                    if(hit){
                        algs.push(alg);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.mirrorShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        ret_shapes.push_back(std::make_pair(shape, alg));
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}

void AlgFinderExt::searchToLayer(const Puppet::Shape& start){
    Puppet::Shape shape;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape = puppet.getBinShape(start);
    std::vector<Puppet::ShapeBin> found_shapes = {puppet.getBinShape(start)};
    shape = start;
    //std::memcpy(shape.data, start.data, sizeof(Puppet::Shape::data));
    std::vector<std::array<uint8_t, 2>> alg;
    if(puppet.testCubeShape(shape)||puppet.testVShape(shape)||puppet.testLayer(shape)
        ||puppet.testYShape(shape)||puppet.testBShape(shape)
        ||puppet.testU2VShape(shape)||puppet.testLShape(shape)
        ||puppet.testU2LShape(shape)/*||puppet.testLBShape(shape)*/){
        std::cout << "#0" << std::endl;
        return;
    }
    bool s = true;
    while(true){
        //std::memcpy(shape.data, start.data, sizeof(Puppet::Shape::data));
        shape = start;
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            break;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    alg.push_back({face, uint8_t(t % 3)});
                    hit = true;
                    found_shape = puppet.getBinShape(shape);
                    for(uint16_t i = 0; i < found_shapes.size(); i++){
                        if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                            hit = false;
                            break;
                        }
                    }
                    if(hit){
                        if(puppet.testCubeShape(shape)||puppet.testVShape(shape)||puppet.testLayer(shape)
                        ||puppet.testYShape(shape)||puppet.testBShape(shape)
                        ||puppet.testU2VShape(shape)||puppet.testLShape(shape)
                        ||puppet.testU2LShape(shape)/*||puppet.testLBShape(shape)*/){
                            //auto pair =  am.reverse(am.translateYX2(am.reverse(alg)));
                            //auto pair = am.translateYX2(alg);
                            auto pair = alg;
                            std::cout << am.translateRightie(pair) << " " << am.translateLeftie(pair) <<" " << 
                            am.translateRightie(am.reverse(pair)) << " " << am.translateLeftie(am.reverse(pair)) <<" #" << int(alg.size());
                            std::cout << std::endl;
                            return;
                        } 
                        algs.push(alg);
                        found_shapes.push_back(found_shape);
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}

void AlgFinderExt::printLtoLayer(){
    auto Lshapes = searchShapes();
    Puppet::Shape shape;
    for(int i = 0; i < Lshapes.size(); i++){
        puppet.cubeShape(shape);
        if(!puppet.testAlg(shape, Lshapes[i].second)){
            std::cout << "yoooooooo" << std::endl;
        }
        auto alg =  am.translateYX2(Lshapes[i].second);
        std::cout << i << " " << am.translateRightie(alg) << " " << am.translateLeftie(alg) <<" " << 
        am.translateRightie(am.reverse(alg)) << " " << am.translateLeftie(am.reverse(alg)) <<" " << int(alg.size())<< " ";
        Puppet::ShapeBin found_shape = puppet.getBinShape(shape);
        if(puppet.testVShape(shape)){
            if(puppet.testLayer(shape)){
                std::cout << " #Layer";
            }else{
                uint8_t f = puppet.getVShape(found_shape) == 2? 2: !puppet.getVShape(found_shape);
                if(!puppet.testTurn(shape,f,0) || !puppet.testTurn(shape,f,2) || !puppet.testTurn(shape,f,1)){
                    std::cout << " #lockedV";
                }else{
                    std::cout << " #V";
                }
            }
        }else if(puppet.testBShape(shape)){
            std::cout << " #B";
        }else if(puppet.testLShape(shape)){
            for(uint8_t f = 0; f < 3; f++){
                if(puppet.doTurn(shape,f,0)){
                    if(!puppet.testVShape(shape)){
                        if(puppet.doTurn(shape,f,0)){
                            if(!puppet.testVShape(shape)){
                                if(puppet.doTurn(shape,f,0)){
                                    if(!puppet.testVShape(shape)){
                                        std::cout << " #L";
                                    }
                                    puppet.doTurn(shape,f,2);
                                }
                            }
                            puppet.doTurn(shape,f,2);
                        } 
                    }
                    puppet.doTurn(shape,f,2);
                }
            }
        }else if(puppet.testU2VShape(shape)){
            uint8_t f = puppet.getU2VShape(found_shape) == 2? 2: !puppet.getU2VShape(found_shape);
            if(!puppet.testTurn(shape,f,1)){
                std::cout << " #lockedU2V";
            }
        }else if(puppet.testU2LShape(shape)){
            uint8_t f = puppet.getU2LShape(found_shape) == 2? 2: !puppet.getU2LShape(found_shape);
            if(!puppet.testTurn(shape,f,1)){
                std::cout << " #lockedU2L";
            }
        }else if(puppet.testLBShape(shape)){
            std::cout << " #LB";
        }
        std::cout << std::endl;
        searchToLayer(Lshapes[i].first);
    }
}