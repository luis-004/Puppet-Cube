#include "AlgFinder.hpp"
AlgFinder::AlgFinder(){}

void AlgFinder::searchCubeShapeL(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> new_algs;
    bool new_shape;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testLShape(shape)){
                    found_shape = puppet.getBinShape(shape); 
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testLShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testLShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                if(puppet.testVShape(shape)){
                    continue;
                }else if(puppet.testBShape(shape)){
                    continue;
                }else if(puppet.testU2VShape(shape)){
                    continue;
                }
                bool Vshape = true;
                for(uint8_t f = 0; f < 3; f++){
                    if(puppet.doTurn(shape,f,0)){
                        if(!puppet.testVShape(shape)){
                            if(puppet.doTurn(shape,f,0)){
                                if(!puppet.testVShape(shape)){
                                    if(puppet.doTurn(shape,f,0)){
                                        if(!puppet.testVShape(shape)){
                                            Vshape = false;
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
                if(Vshape){continue;}
                /*int dir = puppet.getLShape(found_shape);
                    if(dir == 1){
                        good_alg = am.twist(good_alg, 0);
                    }else if(dir == 2){
                        good_alg = am.twist(good_alg, 1);
                    }*/
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  
                //am.optimizeRF(am.reverse(am.translateYX2(am.reverse(good_alg))), am.getRUF(am.reverse(am.translateYX2(am.reverse(good_alg)))));
                am.optimizeRUF(am.translateYX2((good_alg)), am.getRUF(am.translateYX2(good_alg)));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                for(uint16_t i = 0; i < found_shapes.size(); i++){
                    if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                        auto& p = found_algs.at(std::floor(i/6));
                        if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                            for(auto& a : new_algs){
                                if(a.first == p.first){
                                    a.first = pair.first;
                                    a.second = pair.second;
                                    break;
                                }
                            }
                            p.first = pair.first;   
                            p.second = pair.second;
                        }
                        new_shape = 0; 
                        break;
                    } 
                }
                if(new_shape == 1){
                    new_algs.push_back(pair);
                    found_algs.push_back(pair);
                    found_shapes.push_back(found_shape);
                    puppet.rotateShapeBin(found_shape, 0);
                    found_shapes.push_back(found_shape);
                    puppet.rotateShapeBin(found_shape, 0);
                    found_shapes.push_back(found_shape);
                    puppet.mirrorShapeBin(found_shape, 1);
                    found_shapes.push_back(found_shape);
                    puppet.rotateShapeBin(found_shape, 0);
                    found_shapes.push_back(found_shape);
                    puppet.rotateShapeBin(found_shape, 0);
                    found_shapes.push_back(found_shape);
                }
            }
        }
        if(alg.size() > num_turns){
            for(auto& alg : new_algs){
                /*puppet.cubeShape(shape);
                for(auto i : am.reverse(am.translateYX2(am.reverse(alg.first)))){
                    puppet.doTurn(shape, i[0], i[1]);
                }
                bool lock = 1;
                lock = puppet.doTurn(shape, 1, 0);
                lock = puppet.doTurn(shape, 1, 0);
                lock = puppet.doTurn(shape, 1, 0);
                lock = puppet.doTurn(shape, 1, 0);
                if(lock){continue;}
                if(puppet.testLayer(shape)){
                    continue;
                }*/
                std::cout << am.translateRightie(alg.first) << " " << am.translateLeftie(alg.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first)) << " " << am.translateLeftie(am.reverse(alg.first)) <<" " << int(num_turns+1) << " " ;
                for(auto i : alg.second){
                    std::cout << int(i);
                }
                /*if(puppet.testYShape(shape)){
                    std::cout << " Y";    
                }
                if(puppet.testLayer(shape)){
                    std::cout << " Layer";
                }*/
                //if(!lock){std::cout << " lock";}
                std::cout << std::endl;
            }
            new_algs.clear();
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
    std::cout << "end" << std::endl;
}

void AlgFinder::searchCubeShapeLcom(uint8_t max_turns, bool stf){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Shape shape_test;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> new_algs;
    bool new_shape;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testVShape(shape)){
                    found_shape = puppet.getBinShape(shape); 
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testVShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testVShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                int dir = puppet.getVShape(found_shape);
                    if(dir == 1){
                        good_alg = am.twist(good_alg, 0);
                    }else if(dir == 2){
                        good_alg = am.twist(good_alg, 1);
                    }
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  
                am.optimizeRF(am.reverse(am.translateYX2(am.reverse(good_alg))), am.getRUF(am.reverse(am.translateYX2(am.reverse(good_alg)))));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                for(uint16_t i = 0; i < found_shapes.size(); i++){
                    if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                        auto& p = found_algs.at(std::floor(i/6));
                        if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                            for(auto& a : new_algs){
                                if(a.first == p.first){
                                    a.first = pair.first;
                                    a.second = pair.second;
                                    break;
                                }
                            }
                            p.first = pair.first;   
                            p.second = pair.second;
                        }
                        new_shape = 0; 
                        break;
                    } 
                }
                if(new_shape == 1){
                    puppet.cubeShape(shape_test);
                    for(auto i : am.reverse(am.translateYX2(am.reverse(pair.first)))){
                        puppet.doTurn(shape_test, i[0], i[1]);
                    }
                    bool lock = 1;
                    lock = puppet.doTurn(shape_test, 1, 0);
                    lock = puppet.doTurn(shape_test, 1, 0);
                    lock = puppet.doTurn(shape_test, 1, 0);
                    lock = puppet.doTurn(shape_test, 1, 0);
                    if(!lock && (puppet.testLayer(shape_test) == stf)){
                        new_algs.push_back(pair);
                        found_algs.push_back(pair);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.mirrorShapeBin(found_shape, 1);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                        puppet.rotateShapeBin(found_shape, 0);
                        found_shapes.push_back(found_shape);
                    }
                }
            }
        }
        if(alg.size() > num_turns){
            for(auto& alg : new_algs){
                puppet.cubeShape(shape);
                for(auto i : am.reverse(am.translateYX2(am.reverse(alg.first)))){
                    puppet.doTurn(shape, i[0], i[1]);
                }
                std::cout << am.translateRightie(alg.first) << " " << am.translateLeftie(alg.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first)) << " " << am.translateLeftie(am.reverse(alg.first)) <<" " << int(num_turns+1) << " " ;
                for(auto i : alg.second){
                    std::cout << int(i);
                }
                if(puppet.testYShape(shape)){
                    std::cout << " Y";    
                }
                if(puppet.testLayer(shape)){
                    std::cout << " Layer";
                }
                std::cout << std::endl;
            }
            new_algs.clear();
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
    int count = 1;
    for(auto& alg: found_algs){
        std::cout << count++ << ": " << am.translateRightie(alg.first) << std::endl;
        searchShape(am.translateRightie((alg.first)), (int)alg.first.size() + 2, 5);
        std::cout << std::endl;
    }
}

void AlgFinder::searchCubeShapeLB(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Shape shape_test;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> new_algs;
    bool new_shape;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testBShape(shape)){ 
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testBShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testBShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                int dir = puppet.getBShape(found_shape);
                if(dir == 1){
                    good_alg = am.twist(good_alg, 0);
                }else if(dir == 2){
                    good_alg = am.twist(good_alg, 1);
                }
                puppet.cubeShape(shape_test);
                puppet.doAlgExpectPossible(shape_test, good_alg);
                found_shape = puppet.getBinShape(shape_test);
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  
                am.optimizeRF(good_alg, am.getRUF(good_alg));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                for(uint16_t i = 0; i < found_shapes.size(); i++){
                    if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                        auto& p = found_algs.at(std::floor(i/8)); //!!!!!
                        if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                            for(auto& a : new_algs){
                                if(a.first == p.first){
                                    a.first = pair.first;
                                    a.second = pair.second;
                                    break;
                                }
                            }
                            p.first = pair.first;   
                            p.second = pair.second;
                        }
                        new_shape = 0; 
                        break;
                    } 
                }
                if(new_shape == 1){
                    new_algs.push_back(pair);
                    found_algs.push_back(pair);
                    found_shapes.push_back(found_shape);
                    puppet.mirrorShapeBin(found_shape, 1);
                    found_shapes.push_back(found_shape);
                    puppet.cubeShape(shape_test);
                    puppet.doAlgExpectPossible(shape_test, pair.first);
                    for(int i = 0; i < 3; ++i){
                        puppet.doTurn(shape_test, 1, 0);
                        found_shape = puppet.getBinShape(shape_test);
                        found_shapes.push_back(found_shape);
                        puppet.mirrorShapeBin(found_shape, 1);
                        found_shapes.push_back(found_shape);
                    }
                }
            }
        }
        if(alg.size() > num_turns){
            for(auto& alg : new_algs){
                alg.first = am.translateYX2(alg.first);
                std::cout << am.translateRightie(alg.first) << " " << am.translateLeftie(alg.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first)) << " " << am.translateLeftie(am.reverse(alg.first)) <<" " << int(num_turns+1) << " " ;
                    for(auto i : alg.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
            }
            new_algs.clear();
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
    std::cout << "end" << std::endl;
}

void AlgFinder::search4cycle(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<Puppet::Edges> found_Eperms;
    std::vector<Puppet::Flips> found_flips;
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    //std::vector<std::pair<std::pair<std::vector<std::array<uint8_t, 2>>,std::array<uint8_t, 6>>, int>> new_algs;
    bool new_shape;
    int new_perm;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!puppet.testCornersSolved(corners)){
                    continue;
                }
                puppet.solvedEdges(edges);
                for(auto turn : good_alg){
                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                }
                uint8_t edge_count = 9-puppet.countEdgesSolved(edges);
                if(edge_count != 4){
                    continue;
                }
                puppet.solvedFlips(flips);
                for(auto turn : good_alg){
                    puppet.doTurnFlips(flips, turn[0], turn[1]);
                }
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                new_shape = 1;
                new_perm = 1;
                for(uint16_t i = 0; i < found_algs.size(); i++){
                    if(found_algs[i] == good_alg){
                        new_shape = 0; 
                        break;
                    } 
                }
                new_perm = 0;
                for(uint16_t i = 0; i < found_Eperms.size(); i++){
                    if(std::memcmp(found_Eperms[i].data, edges.data, sizeof(Puppet::Edges::data)) == 0){
                        break;
                    } 
                    new_perm++;
                }
                if(new_shape == 1){
                    std::cout << "#" << std::floor(new_perm/12);
                    std::cout << " " << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                        am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(num_turns+1)<< " " ;
                    for(auto i : pair.second){
                        std::cout << int(i);
                    }
                    found_algs.push_back(good_alg);
                    found_algs.push_back((am.twist(good_alg,0)));
                    found_algs.push_back((am.twist(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,0)));
                    found_algs.push_back((am.swap(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),2)));
                    found_algs.push_back((am.reverse(good_alg)));
                    found_algs.push_back((am.reverse(am.twist(good_alg,0))));
                    found_algs.push_back((am.reverse(am.twist(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,0))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),2))));                
                    if(new_perm >= found_Eperms.size()){
                        found_Eperms.push_back(edges);
                        for(uint8_t i = 0; i<11; i++){
                            if(i == 2 || i == 8){
                                good_alg = am.swap(good_alg, 0);
                            }else if( i == 5){
                                good_alg = am.reverse(good_alg);
                            }else{
                                good_alg = am.twist(good_alg, 0);
                            }
                            puppet.solvedEdges(edges);
                            for(auto turn : good_alg){
                                puppet.doTurnEdges(edges, turn[0], turn[1]);
                            }                   
                            found_Eperms.push_back(edges);
                        }
                    }
                    puppet.solvedFlips(flips);
                    for(auto i : good_alg){
                        puppet.doTurnFlips(flips, i[0], i[1]);
                    }
                    std::cout << " " << int(puppet.countFlips(flips));
                    for(auto i : good_alg){
                        puppet.doTurnFlips(flips, i[0], i[1]);
                    }
                    std::cout << " " << int(puppet.countFlips(flips));
                    int new_flip = 0;
                    for(uint16_t i = 0; i < found_flips.size(); i++){
                        if(std::memcmp(found_flips[i].data, flips.data, sizeof(Puppet::Flips::data)) == 0){
                            break;
                        } 
                        new_flip++;
                    }
                    if(new_flip >= found_flips.size()){
                        found_flips.push_back(flips);
                        for(uint8_t i = 0; i<11; i++){
                            if(i == 2 || i == 8){
                                good_alg = am.swap(good_alg, 0);
                            }else if( i == 5){
                                good_alg = am.reverse(good_alg);
                            }else{
                                good_alg = am.twist(good_alg, 0);
                            }
                            puppet.solvedFlips(flips);
                            for(auto turn : good_alg){
                                puppet.doTurnFlips(flips, turn[0], turn[1]);
                            }     
                            for(auto turn : good_alg){
                                puppet.doTurnFlips(flips, turn[0], turn[1]);
                            }                  
                            found_flips.push_back(flips);
                        }
                    }
                    std::cout << " $" << std::floor(new_flip/12);
                    std::cout << std::endl;
                }
            }
        }
        if(alg.size() > num_turns){
            //new_algs.clear();
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "duration: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}

void AlgFinder::search3cycle(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>,std::array<uint8_t, 6>>> new_algs;
    std::vector<std::vector<uint8_t>> cycles;
    bool new_alg;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 0;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!puppet.testCornersSolved(corners)){
                    continue;
                }
                puppet.solvedEdges(edges);
                for(auto turn : good_alg){
                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                }
                if(9-puppet.countEdgesSolved(edges) > 3){
                    continue;
                }
                new_alg = 1;
                for(uint16_t i = 0; i < found_algs.size(); i++){
                    if(found_algs[i] == good_alg){
                        new_alg = 0; 
                        break;
                    } 
                }
                if(new_alg == 1){
                    auto pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                    std::cout << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                    am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(num_turns+1)<< " ";
                    std::vector<uint8_t> cycle = puppet.getEdgeCycle(edges);
                    puppet.printCycle(cycle);
                    for(auto i : pair.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
                    cycles.push_back(cycle);
                    new_algs.push_back(pair);
                    found_algs.push_back(good_alg);
                    found_algs.push_back((am.twist(good_alg,0)));
                    found_algs.push_back((am.twist(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,0)));
                    found_algs.push_back((am.swap(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),2)));
                    found_algs.push_back((am.reverse(good_alg)));
                    found_algs.push_back((am.reverse(am.twist(good_alg,0))));
                    found_algs.push_back((am.reverse(am.twist(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,0))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),2))));
                }
            }
        }
        if(alg.size() > num_turns){
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "duration: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
    bool new_case = 0;
    int index;
    Puppet::Edges cycle;
    std::vector<Puppet::Flips> found_flips;
    for(uint8_t a = 0; a < 9; a++){
        for(uint8_t b = 0; b < 9; b++){
            for(uint8_t c = 0; c < 9; c++){
                new_case = 1;
                found_flips.clear();
                for(uint8_t d = 0; d < cycles.size(); d++){
                    if(cycles.at(d).at(0) == a && cycles.at(d).at(1) == b && cycles.at(d).at(2) == c ){
                        puppet.solvedFlips(flips);
                        puppet.solvedEdges(edges);
                        std::vector<std::array<uint8_t, 2>> alg = am.translateYX2(new_algs.at(d).first);
                        if(new_case){
                            new_case = 0;
                            std::cout << std::endl;
                            for(auto i : alg){
                                puppet.doTurnEdges(edges, i[0], i[1]);
                                puppet.doTurnFlips(flips, i[0], i[1]);
                            }
                            cycle = edges;
                            found_flips.push_back(flips);
                            std::cout << "#0 ";
                        }else{
                            puppet.solvedEdges(edges);
                            puppet.solvedFlips(flips);
                            for(auto turn : alg){
                                puppet.doTurnEdges(edges, turn[0], turn[1]);
                                puppet.doTurnFlips(flips, turn[0], turn[1]);
                            }
                            for(uint8_t i = 0; i<11; i++){
                                if(edges.data == cycle.data){
                                    break;
                                }
                                if(i == 2 || i == 8){
                                    alg = am.swap(alg, 0);
                                }else if( i == 5){
                                    alg = am.reverse(alg);
                                }else{
                                    alg = am.twist(alg, 0);
                                }
                                puppet.solvedEdges(edges);
                                puppet.solvedFlips(flips);
                                for(auto turn : alg){
                                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                                    puppet.doTurnFlips(flips, turn[0], turn[1]);
                                }                   
                            }
                            index = 0;
                            for(auto i:found_flips){
                                if(memcmp(i.data, flips.data, sizeof(Puppet::Flips::data)) == 0){
                                    break;
                                }
                                index++;
                            }
                            if(index>=found_flips.size()){
                                found_flips.push_back(flips);
                            }
                            std::cout << "#" << index << " ";
                        }
                        std::cout << int(a) << int(b) << int(c) <<  ": ";
                        std::cout << am.translateRightie(new_algs.at(d).first) << " " << am.translateLeftie(new_algs.at(d).first) <<" " << 
                        am.translateRightie(am.reverse(new_algs.at(d).first)) << " " << am.translateLeftie(am.reverse(new_algs.at(d).first)) << " ";
                        uint8_t len = new_algs.at(d).second[0] + new_algs.at(d).second[1] + new_algs.at(d).second[2];
                        for(auto i : new_algs.at(d).second){
                            std::cout << int(i);
                        }
                        std::cout << " " << int(len);
                        std::cout << " " << int(puppet.countFlips(flips));
                        std::vector<uint8_t> solvedEdges =  puppet.getEdgesSolved(edges);
                        std::vector<uint8_t> flippedEdges =  puppet.getFlips(flips);
                        bool internalFlip = 1;
                        for(auto i : solvedEdges){
                            for(auto j : flippedEdges){
                                if(i == j){
                                    internalFlip = 0;
                                }
                            }
                        }
                        std::cout << (internalFlip?" int":" ext");
                        std::cout << std::endl;
                    }
                }
            }
        }    
    }
}

void AlgFinder::searchCornerCase(std::string algStr, uint8_t max_turns){
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Corners corners_to_find;
    uint8_t last_turn = 8;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    puppet.cubeShape(shape);
    puppet.solvedCorners(corners_to_find);
    std::vector<std::array<uint8_t, 2>> alg = am.translateYX2(am.translateBack(algStr));
    for(auto turn : alg){
        if(!puppet.doTurn(shape, turn[0], turn[1])){
            std::cout << "no valid alg" << std::endl;
            return;
        };
        puppet.doTurnCorners(corners_to_find, turn[0], turn[1]);
    }
    if(!puppet.testCubeShape(shape)){
        std::cout << "no cubeshape alg" << std::endl;
        return;
    }
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    bool new_alg;
    algs.push({{0,0}});
    algs.push({{0,1}});
    algs.push({{0,2}});
    algs.push({{1,0}});
    algs.push({{1,1}});
    algs.push({{1,2}});
    algs.push({{2,0}});
    algs.push({{2,1}});
    algs.push({{2,2}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){ 
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!std::memcmp(corners.data, corners_to_find.data, sizeof(Puppet::Corners::data)) == 0){
                    continue;
                }
                new_alg = 1;
                for(uint16_t i = 0; i < found_algs.size(); i++){
                    if(found_algs[i] == good_alg){
                        new_alg = 0; 
                        break;
                    } 
                }
                if(new_alg == 1){
                    std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                    std::cout << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                    am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(num_turns+1) << " " ;
                    for(auto i : pair.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
                    found_algs.push_back(good_alg);
                    found_algs.push_back((am.twist(good_alg,0)));
                    found_algs.push_back((am.twist(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,0)));
                    found_algs.push_back((am.swap(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),2)));
                    found_algs.push_back((am.reverse(good_alg)));
                    found_algs.push_back((am.reverse(am.twist(good_alg,0))));
                    found_algs.push_back((am.reverse(am.twist(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,0))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),2))));
                }
            }
        }
        if(alg.size() > num_turns){
            num_turns = alg.size();
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}

void AlgFinder::searchShape(std::string algStr, uint8_t max_turns, uint8_t max_count){
    int count = 0;
    Puppet::Shape shape;
    uint8_t last_turn = 8;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    puppet.cubeShape(shape);
    std::vector<std::array<uint8_t, 2>> alg = am.translateYX2(am.translateBack(algStr));
    for(auto turn : alg){
        if(!puppet.doTurn(shape, turn[0], turn[1])){
            std::cout << "no valid alg" << std::endl;
            return;
        };
    }
    Puppet::ShapeBin shapeBin = puppet.getBinShape(shape);
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    bool new_alg;
    algs.push({{0,0}});
    algs.push({{0,1}});
    algs.push({{0,2}});
    algs.push({{1,0}});
    algs.push({{1,1}});
    algs.push({{1,2}});
    algs.push({{2,0}});
    algs.push({{2,1}});
    algs.push({{2,2}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(std::memcmp(puppet.getBinShape(shape).data, shapeBin.data, sizeof(Puppet::ShapeBin::data)) == 0){ 
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&std::memcmp(puppet.getBinShape(shape).data, shapeBin.data, sizeof(Puppet::ShapeBin::data)) == 0){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&std::memcmp(puppet.getBinShape(shape).data, shapeBin.data, sizeof(Puppet::ShapeBin::data)) == 0){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                new_alg = 1;
                for(uint16_t i = 0; i < found_algs.size(); i++){
                    if(found_algs[i] == good_alg){
                        new_alg = 0; 
                        break;
                    } 
                }
                if(new_alg == 1){
                    std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                    std::cout << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                    am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(num_turns+1) << " " ;
                    for(auto i : pair.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
                    found_algs.push_back(good_alg);
                    found_algs.push_back((am.twist(good_alg,0)));
                    found_algs.push_back((am.twist(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,0)));
                    found_algs.push_back((am.swap(good_alg,1)));
                    found_algs.push_back((am.swap(good_alg,2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,0),2)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),0)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),1)));
                    found_algs.push_back((am.swap(am.twist(good_alg,1),2)));
                    found_algs.push_back((am.reverse(good_alg)));
                    found_algs.push_back((am.reverse(am.twist(good_alg,0))));
                    found_algs.push_back((am.reverse(am.twist(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,0))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,1))));
                    found_algs.push_back((am.reverse(am.swap(good_alg,2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),2))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),0))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),1))));
                    found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),2))));
                    count++;
                    if(count == max_count){
                        return;
                    }
                }
            }
        }
        if(alg.size() > num_turns){
            num_turns = alg.size();
            //std::cout << int(num_turns + 1) << std::endl;
        }
    }
}

void AlgFinder::searchCubeShapeY(uint8_t max_turns){
    Puppet::Shape shape;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> new_algs;
    bool new_shape;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testYShape(shape)){
                    found_shape = puppet.getBinShape(shape); 
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testYShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testYShape(shape)){
                    found_shape = puppet.getBinShape(shape);
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                for(uint16_t i = 0; i < found_shapes.size(); i++){
                    if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                        auto& p = found_algs.at(std::floor(i/6));
                        if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                            for(auto& a : new_algs){
                                if(a.first == p.first){
                                    a.first = pair.first;
                                    a.second = pair.second;
                                    break;
                                }
                            }
                            p.first = pair.first;   
                            p.second = pair.second;
                        }
                        new_shape = 0; 
                        break;
                    } 
                }
                if(new_shape == 1){
                    new_algs.push_back(pair);
                    found_algs.push_back(pair);
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
                }
            }
        }
        if(alg.size() > num_turns){
            for(auto& alg : new_algs){
                std::cout << am.translateRightie(alg.first) << " " << am.translateLeftie(alg.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first)) << " " << am.translateLeftie(am.reverse(alg.first)) <<" " << int(num_turns+1) << " " ;
                    for(auto i : alg.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
            }
            new_algs.clear();
            num_turns = alg.size();
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}
void AlgFinder::searchCorners(uint8_t max_turns){
    Puppet::Shape shape;
    Puppet::Corners corners;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<Puppet::Corners> found_Cperms;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> new_algs;
    bool new_shape;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                for(uint16_t i = 0; i < found_Cperms.size(); i++){
                    if(std::memcmp(found_Cperms[i].data, corners.data, sizeof(Puppet::Corners::data)) == 0){
                        auto& p = found_algs[std::floor(i/6)];
                        if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                            for(auto& a : new_algs){
                                if(a.first == p.first){
                                    a.first = pair.first;
                                    a.second = pair.second;
                                    break;
                                }
                            }
                            p.first = pair.first;   
                            p.second = pair.second;
                        }
                        new_shape = 0; 
                        break;
                    } 
                }
                if(new_shape == 1){
                    new_algs.push_back(pair);
                    found_algs.push_back(pair);
                    found_Cperms.push_back(corners);
                    puppet.rotateCornersCS(corners, 0);
                    found_Cperms.push_back(corners);
                    puppet.rotateCornersCS(corners, 0);
                    found_Cperms.push_back(corners);
                    puppet.mirrorCornersCS(corners);
                    found_Cperms.push_back(corners);
                    puppet.rotateCornersCS(corners, 0);
                    found_Cperms.push_back(corners);
                    puppet.rotateCornersCS(corners, 0);
                    found_Cperms.push_back(corners);
                }
            }
        }
        if(alg.size() > num_turns){
            for(auto& alg : new_algs){
                std::cout << am.translateRightie(alg.first) << " " << am.translateLeftie(alg.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first)) << " " << am.translateLeftie(am.reverse(alg.first)) <<" " << int(num_turns+1) << " " ;
                    for(auto i : alg.second){
                        std::cout << int(i);
                    }
                    std::cout << std::endl;
            }
            new_algs.clear();
            new_algs.shrink_to_fit();
            num_turns = alg.size();
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}

void AlgFinder::searchEdges(uint8_t max_turns, uint8_t count){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::pair<Puppet::Edges, Puppet::Flips>> found_Eperms;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::pair<std::pair<std::vector<std::array<uint8_t, 2>>,std::array<uint8_t, 6>>, bool>> new_algs;
    std::vector<uint8_t> edge_counts;
    bool new_shape;
    bool new_perm;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 1;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(!hit&&puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!puppet.testCornersSolved(corners)){
                    continue;
                }
                puppet.solvedEdges(edges);
                for(auto turn : good_alg){
                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                }
                uint8_t edge_count = 9-puppet.countEdgesSolved(edges);
                if(edge_count > count){
                    continue;
                }
                puppet.solvedFlips(flips);
                for(auto turn : good_alg){
                    puppet.doTurnFlips(flips, turn[0], turn[1]);
                }
                std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                uint8_t Hcount = pair.second[0] + pair.second[1] +pair.second[2];
                uint8_t Qcount = pair.second[3] + pair.second[4] +pair.second[5];
                new_shape = 1;
                new_perm = 1;
                for(uint16_t i = 0; i < found_Eperms.size(); i++){
                    if(std::memcmp(found_Eperms[i].first.data, edges.data, sizeof(Puppet::Edges::data)) == 0){
                        new_perm = 0;
                        if((std::memcmp(found_Eperms[i].second.data, flips.data, sizeof(Puppet::Flips::data)) == 0)){
                            auto& p = found_algs.at(std::floor(i/12)); 
                            if((Hcount == (p.second[0] + p.second[1] + p.second[2])) && (Qcount < (p.second[3] + p.second[4] + p.second[5]))){
                                for(auto& a : new_algs){
                                    if(a.first.first == p.first){
                                        a.first.first = pair.first;
                                        a.first.second = pair.second;
                                        break;
                                    }
                                }
                                p.first = pair.first;   
                                p.second = pair.second;
                            }
                            new_shape = 0; 
                            break;
                        }
                    } 
                }
                if(new_shape == 1){
                    new_algs.push_back(std::make_pair(pair, new_perm));
                    found_algs.push_back(pair);                
                    found_Eperms.push_back(std::make_pair(edges, flips));
                    edge_counts.push_back(edge_count);
                    for(uint8_t i = 0; i<11; i++){
                        if(i == 2 || i == 8){
                            good_alg = am.swap(good_alg, 0);
                        }else if( i == 5){
                            good_alg = am.reverse(good_alg);
                        }else{
                            good_alg = am.twist(good_alg, 0);
                        }
                        puppet.solvedEdges(edges);
                        puppet.solvedFlips(flips);
                        for(auto turn : good_alg){
                            puppet.doTurnEdges(edges, turn[0], turn[1]);
                            puppet.doTurnFlips(flips, turn[0], turn[1]);
                        }                   
                        found_Eperms.push_back(std::make_pair(edges, flips));
                    }
                }
            }
        }
        if(alg.size() > num_turns){
            for(uint8_t i = 0; i < new_algs.size(); i++){
                auto& alg = new_algs.at(i);
                std::cout << am.translateRightie(alg.first.first) << " " << am.translateLeftie(alg.first.first) <<" " << 
                    am.translateRightie(am.reverse(alg.first.first)) << " " << am.translateLeftie(am.reverse(alg.first.first)) <<" " << int(num_turns+1)<<" " << int(edge_counts.at(i))<< " " ;
                for(auto i : alg.first.second){
                    std::cout << int(i);
                }
                puppet.solvedFlips(flips);
                for(auto i : am.translateYX2(alg.first.first)){
                    puppet.doTurnFlips(flips, i[0], i[1]);
                }
                std::cout << " " << int(puppet.countFlips(flips));
                if(alg.second){
                    std::cout << " new" << std::endl;
                }else{
                    std::cout << std::endl;
                }
            }
            edge_counts.clear();
            new_algs.clear();
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "duration: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}
void AlgFinder::searchAllShapes(){
    Puppet::Shape shape;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    std::vector<std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>>> found_algs;
    std::vector<std::array<uint8_t, 2>> alg;
    found_shape = puppet.getBinShape(shape); 
    found_shapes.push_back(found_shape);
    found_algs.push_back(std::make_pair(alg, std::array<uint8_t, 6>{0,0,0,0,0,0}));
    uint8_t num_turns = 0;
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
            break;
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
                        std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> pair =  am.optimizeRUF(am.translateYX2(alg), am.getRUF(am.translateYX2(alg)));
                        if(pair.first.size() > num_turns){
                            num_turns = alg.size();
                            std::cout << int(num_turns) << std::endl;
                        }
                        std::cout << int(found_algs.size()) << " " << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                            am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(pair.first.size()) << " " ;
                        for(auto i : pair.second){
                            std::cout << int(i);
                        }
                        found_algs.push_back(pair);
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
                        uint8_t pos_turns = 0;
                        uint8_t new_turns = 0;
                        uint8_t lf_turns = 0;
                        for(uint8_t f = 0; f < 3; f++){
                            if(f == face){
                                for(uint8_t d = 0; d < 3; d++){
                                    if(puppet.testTurn(shape,f,d)){
                                        lf_turns++;
                                    }
                                }  
                                continue;  
                            }
                            for(uint8_t d = 0; d < 3; d++){
                                if(puppet.testTurn(shape,f,d)){
                                    puppet.doTurn(shape,f,d);
                                    pos_turns++;
                                    found_shape = puppet.getBinShape(shape);
                                    for(uint16_t i = 0; i < found_shapes.size(); i++){
                                        if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                                            new_turns++;
                                            goto b;
                                        }
                                    }
                                    b:
                                    puppet.doTurn(shape, f, 2 - d);
                                }
                            }
                        }
                        std::cout << " $" << int(pos_turns) <<" #" << int(pos_turns - new_turns) <<" %" << int(lf_turns);
                        found_shape = puppet.getBinShape(shape);
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
                        for(uint8_t f = 0; f < 3; f++){
                            if(puppet.doTurn(shape,f,0)){
                                if(puppet.doTurn(shape,f,0)){
                                    if(puppet.doTurn(shape,f,0)){
                                        std::cout << " FREE";
                                        puppet.doTurn(shape,f,2);
                                    }
                                    puppet.doTurn(shape,f,2);
                                } 
                                puppet.doTurn(shape,f,2);
                            }
                        }
                        std::cout << std::endl;
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    /*AlgFinderExt ext;
    std::vector<std::pair<Puppet::Shape, std::vector<std::array<uint8_t, 2>>>> Lshapes = ext.searchShapes();
    //std::vector<std::array<uint8_t, 2>> alg = am.translateYX2(am.translateBack("FR2FUFU2FR2F'UF2U'R2FR2"));
    for(int i = 0; i < Lshapes.size(); i++){
        /*puppet.cubeShape(shape);
        for(auto turn : s.second){
            if(!puppet.doTurn(shape, turn[0], turn[1])){
                std::cout << "no valid alg" << std::endl;
                return;
            }
        }
        found_shape = puppet.getBinShape(Lshapes[i].first); 
        for(uint16_t i = 0; i < found_shapes.size(); i++){
            if(std::memcmp(found_shapes[i].data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                //std::cout << "alg is at " << int(std::floor(i/6)) <<std::endl;
                goto found;
            }
        }
        std::cout <<i<< " not found" <<std::endl;
        found: ;
    }*/
}

void AlgFinder::searchFlips(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    bool new_alg;
    algs.push({{0,0}});
    algs.push({{0,1}});
    uint8_t num_turns = 0;
    while(num_turns < max_turns){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg = algs.front();
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            bool hit = false;
            std::vector<std::array<uint8_t, 2>> good_alg;
            if(puppet.doTurn(shape, face, 0)){
                alg.push_back({face, 0});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 2);
                HT_possible = true;
            }
            if(puppet.doTurn(shape, face, 2)){
                alg.push_back({face, 2});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 0);
                HT_possible = true;
            }
            if(HT_possible && puppet.doTurn(shape, face, 1)){
                alg.push_back({face, 1});
                algs.push(alg);
                if(puppet.testCubeShape(shape)){
                    good_alg = alg;
                    hit = true;
                }
                alg.pop_back();
                puppet.doTurn(shape, face, 1);
            }
            if(hit){
                new_alg = 1;
                for(uint16_t i = 0; i < found_algs.size(); i++){
                    if(found_algs[i] == good_alg){
                        new_alg = 0;
                        break;
                    } 
                }
                if(!new_alg){
                    continue;
                }
                puppet.solvedCorners(corners);
                for(auto turn : good_alg){
                    puppet.doTurnCorners(corners, turn[0], turn[1]);
                }
                if(!puppet.testCornersSolved(corners)){
                    continue;
                }
                puppet.solvedEdges(edges);
                for(auto turn : good_alg){
                    puppet.doTurnEdges(edges, turn[0], turn[1]);
                }
                if(puppet.countEdgesSolved(edges) != 9){
                    continue;
                }
                puppet.solvedFlips(flips);
                for(auto turn : good_alg){
                    puppet.doTurnFlips(flips, turn[0], turn[1]);
                }
                if(puppet.countFlips(flips) == 0){
                    continue;
                }
                auto pair =  am.optimizeRUF(am.translateYX2(good_alg), am.getRUF(am.translateYX2(good_alg)));
                std::cout << am.translateRightie(pair.first) << " " << am.translateLeftie(pair.first) <<" " << 
                am.translateRightie(am.reverse(pair.first)) << " " << am.translateLeftie(am.reverse(pair.first)) <<" " << int(num_turns+1)<< " ";
                for(auto i : pair.second){
                    std::cout << int(i);
                }
                std::cout << std::endl;
                found_algs.push_back(good_alg);
                found_algs.push_back((am.twist(good_alg,0)));
                found_algs.push_back((am.twist(good_alg,1)));
                found_algs.push_back((am.swap(good_alg,0)));
                found_algs.push_back((am.swap(good_alg,1)));
                found_algs.push_back((am.swap(good_alg,2)));
                found_algs.push_back((am.swap(am.twist(good_alg,0),0)));
                found_algs.push_back((am.swap(am.twist(good_alg,0),1)));
                found_algs.push_back((am.swap(am.twist(good_alg,0),2)));
                found_algs.push_back((am.swap(am.twist(good_alg,1),0)));
                found_algs.push_back((am.swap(am.twist(good_alg,1),1)));
                found_algs.push_back((am.swap(am.twist(good_alg,1),2)));
                found_algs.push_back((am.reverse(good_alg)));
                found_algs.push_back((am.reverse(am.twist(good_alg,0))));
                found_algs.push_back((am.reverse(am.twist(good_alg,1))));
                found_algs.push_back((am.reverse(am.swap(good_alg,0))));
                found_algs.push_back((am.reverse(am.swap(good_alg,1))));
                found_algs.push_back((am.reverse(am.swap(good_alg,2))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),0))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),1))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,0),2))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),0))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),1))));
                found_algs.push_back((am.reverse(am.swap(am.twist(good_alg,1),2))));
            }
        }
        if(alg.size() > num_turns){
            num_turns = alg.size();
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << int(num_turns + 1) << std::endl;
        }
    }
}

void AlgFinder::searchFlipsDFS(uint8_t max_turns){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Edges edges;
    Puppet::Flips flips;
    uint8_t last_turn = 0;
    std::array<std::array<uint8_t, 2>, 9> turns = {{{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}}};
    std::vector<std::array<uint8_t, 2>> alg = {};
    std::vector<std::vector<std::array<uint8_t, 2>>> found_algs;
    std::vector<std::vector<std::array<uint8_t, 2>>> valid_turns(max_turns + 1);
    valid_turns[0].push_back({{0,0}});
    valid_turns[0].push_back({{0,1}});
    bool new_alg = 1;
    bool new_step = 0;
    puppet.cubeShape(shape);
    while(true){
        if(alg.size() == max_turns){
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
        if(puppet.testCubeShape(shape)){
            new_alg = 1;
            for(uint16_t i = 0; i < found_algs.size(); i++){
                if(found_algs[i] == alg){
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
            for(auto i : pair.second){
                std::cout << int(i);
            }
            std::cout << std::endl;
            found_algs.push_back(alg);
            found_algs.push_back((am.twist(alg,0)));
            found_algs.push_back((am.twist(alg,1)));
            found_algs.push_back((am.swap(alg,0)));
            found_algs.push_back((am.swap(alg,1)));
            found_algs.push_back((am.swap(alg,2)));
            found_algs.push_back((am.swap(am.twist(alg,0),0)));
            found_algs.push_back((am.swap(am.twist(alg,0),1)));
            found_algs.push_back((am.swap(am.twist(alg,0),2)));
            found_algs.push_back((am.swap(am.twist(alg,1),0)));
            found_algs.push_back((am.swap(am.twist(alg,1),1)));
            found_algs.push_back((am.swap(am.twist(alg,1),2)));
            found_algs.push_back((am.reverse(alg)));
            found_algs.push_back((am.reverse(am.twist(alg,0))));
            found_algs.push_back((am.reverse(am.twist(alg,1))));
            found_algs.push_back((am.reverse(am.swap(alg,0))));
            found_algs.push_back((am.reverse(am.swap(alg,1))));
            found_algs.push_back((am.reverse(am.swap(alg,2))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,0),0))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,0),1))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,0),2))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,1),0))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,1),1))));
            found_algs.push_back((am.reverse(am.swap(am.twist(alg,1),2))));
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
}