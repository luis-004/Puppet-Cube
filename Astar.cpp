#include "Astar.hpp"
Astar::Astar(){}

std::unordered_map<KeyCorners, uint8_t> Astar::getCornerMap(){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    KeyCorners key;
    Puppet::Shape shape;
    Puppet::Corners corners;
    Puppet::Corners cor_cpy;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedCorners(corners);
    found_shape = puppet.getFastShape(shape); 
    std::unordered_map<KeyCorners, uint8_t> Map = {{{found_shape, corners}, 0}};
    Map.reserve(500000);
    uint8_t num_turns = 0;
    int Map_oldsize = 1;
    bool s = true;
    std::cout << am.translateRightie(alg) << " " << int(num_turns) << " " << int(Map.size()) << std::endl;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedCorners(corners);
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnCorners(corners, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            std::cout << "fertig" << std::endl;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return Map;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnCorners(corners, face, t % 3);
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getFastShape(shape); 
                    key = {found_shape, corners};
                    auto inserted = Map.try_emplace(key, num_turns = uint8_t(alg.size()));
                    if(inserted.second){ 
                        /*if(alg.size() > num_turns){
                            num_turns = alg.size();
                            std::cout << int(num_turns) << std::endl;
                            std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
                            Map_oldsize = int(Map.size());
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "time: " << duration.count() <<  std::endl;
                        }*/
                        std::memcpy(cor_cpy.data, corners.data, 7);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        Map.insert({{found_shape, cor_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        Map.insert({{found_shape, cor_cpy},num_turns});
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.mirrorCorners(cor_cpy);
                        Map.insert({{found_shape, cor_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        Map.insert({{found_shape, cor_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        Map.insert({{found_shape, cor_cpy},num_turns});
                        algs.push(alg);
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnCorners(corners, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
std::unordered_map<KeyEdges, uint8_t> Astar::getEdgeMap(){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    KeyEdges key;
    Puppet::Edges edges;
    Puppet::Edges e_cpy;
    Puppet::Flips flips;
    Puppet::Flips f_cpy;
    Puppet::FlipsFast f_fast;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.solvedEdges(edges);
    puppet.solvedFlips(flips);
    f_fast = puppet.getFastFlips(flips); 
    std::unordered_map<KeyEdges, uint8_t> Map = {{{edges, f_fast}, 0}};
    Map.reserve(100000000);
    uint8_t num_turns = 0;
    int Map_oldsize = 1;
    bool s = true;
    while(true){
        puppet.solvedEdges(edges);
        puppet.solvedFlips(flips);
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurnEdges(edges, turn[0], turn[1]);
                puppet.doTurnFlips(flips, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            std::cout << "fertig" << std::endl;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return Map;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            for(uint8_t t = 0; t < 3; t++){
                puppet.doTurnEdges(edges, face, t);
                puppet.doTurnFlips(flips, face, t);
                alg.push_back({face, t});
                f_fast = puppet.getFastFlips(flips); 
                key = {edges, f_fast};
                if(alg.size() > num_turns){
                    num_turns = alg.size();
                    std::cout << int(num_turns) << std::endl;
                    std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
                    Map_oldsize = int(Map.size());
                    now = std::chrono::high_resolution_clock::now();
                    duration = now - start_time;
                    std::cout << "time: " << duration.count() <<  std::endl;
                }
                /*if(num_turns > 3){
                    return Map;
                }*/
                auto inserted = Map.try_emplace(key, num_turns = uint8_t(alg.size()));
                //puppet.printEdges(edges);
                //puppet.printFlips(flips);
                if(inserted.second){ 
                    std::memcpy(e_cpy.data, edges.data, 9);
                    std::memcpy(f_cpy.data, flips.data, 9);
                    puppet.mirrorEdges(e_cpy);
                    puppet.mirrorFlips(f_cpy);
                    //puppet.printEdges(e_cpy);
                    //puppet.printFlips(f_cpy);
                    f_fast = puppet.getFastFlips(f_cpy);
                    Map.insert({{e_cpy, f_fast},num_turns});
                    algs.push(alg);
                    //std::cout << am.translateRightie(alg) << " " << int(alg.size()) << " " << int(Map.size()) <<  std::endl;
                }
                alg.pop_back();
                puppet.doTurnEdges(edges, face, 2 - t);
                puppet.doTurnFlips(flips, face, 2 - t);
            }
        }
    }
}
std::unordered_map<KeySF, uint8_t> Astar::getSFMap(){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    KeySF key;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;    
    Puppet::Flips flips;
    Puppet::Flips f_cpy;
    Puppet::FlipsFast f_fast;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedFlips(flips);
    found_shape = puppet.getFastShape(shape); 
    f_fast = puppet.getFastFlips(flips);
    std::unordered_map<KeySF, uint8_t> Map = {{{found_shape, f_fast}, 0}};
    Map.reserve(3500000);
    uint8_t num_turns = 0;
    int Map_oldsize = 1;
    bool s = true;
    std::cout << am.translateRightie(alg) << " " << int(num_turns) << " " << int(Map.size()) << std::endl;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedFlips(flips);
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnFlips(flips, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            std::cout << "fertig" << std::endl;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return Map;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnFlips(flips, face, t % 3);
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getFastShape(shape); 
                    f_fast = puppet.getFastFlips(flips);
                    key = {found_shape, f_fast};
                    auto inserted = Map.try_emplace(key, /*num_turns =*/ uint8_t(alg.size()));
                    if(inserted.second){ 
                        if(alg.size() > num_turns){
                            num_turns = alg.size();
                            std::cout << int(num_turns) << std::endl;
                            std::cout << int(Map.size()) - Map_oldsize << " " << int(Map.size()) << std::endl;
                            Map_oldsize = int(Map.size());
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "time: " << duration.count() <<  std::endl;
                        }
                        std::memcpy(f_cpy.data, flips.data, 9);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.mirrorFlips(f_cpy);
                        f_fast = puppet.getFastFlips(f_cpy);
                        Map.insert({{found_shape, f_fast},num_turns});
                        algs.push(alg);
                        std::cout << am.translateRightie(alg) << " " << int(alg.size()) << " " << int(Map.size()) <<  std::endl;
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnFlips(flips, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
void Astar::getS4EMap(bool G, uint8_t max, std::unordered_map<Key4E, uint8_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    //Puppet::ShapeBin bin_shape;    
    Puppet::Edges edges;
    //Puppet::Edges e_cpy;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedEdges(edges);
    found_shape = puppet.getFastShape(shape); 
    Map = {{{found_shape, puppet.get4E(edges, G)}, 0}};
    Map.reserve(40000000);
    uint8_t num_turns = 0;
    bool s = true;
    std::cout << am.translateRightie(alg) << " " << int(num_turns) << " " << int(Map.size()) << std::endl;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedEdges(edges);
        if(!algs.empty()){
            alg = algs.front();
            algs.pop();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnEdges(edges, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << "Speicher der queue freigeben" << std::endl;
            std::queue<std::vector<std::array<uint8_t, 2>>>().swap(algs);
            std::cout << "queue freigegeben" << std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnEdges(edges, face, t % 3);
                    alg.push_back({face, uint8_t(t % 3)});
                    if(alg.size() > num_turns){
                        num_turns = alg.size();
                        std::cout << int(num_turns) << std::endl;
                        std::cout << int(num_turns) << " " << int(Map.size()) << std::endl;
                        now = std::chrono::high_resolution_clock::now();
                        duration = now - start_time;
                        std::cout << "time: " << duration.count() <<  std::endl;
                        if(num_turns > max){
                            std::cout << "fertig" << std::endl;
                            std::cout << int(num_turns) << std::endl;
                            std::cout << int(Map.size()) << std::endl;
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "time: " << duration.count() <<  std::endl;
                            std::cout << "Speicher der queue freigeben" << std::endl;
                            std::queue<std::vector<std::array<uint8_t, 2>>>().swap(algs);
                            std::cout << "queue freigegeben" << std::endl;
                            return;
                        }
                    }
                    found_shape = puppet.getFastShape(shape); 
                    Map.insert({{found_shape, puppet.get4E(edges, G)}, num_turns});
                    algs.push(alg);
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnEdges(edges, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
void Astar::getSEMap(const uint8_t max, std::unordered_map<KeySE, uint8_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;    
    Puppet::Edges edges;
    Puppet::Edges e_cpy;
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedEdges(edges);
    found_shape = puppet.getFastShape(shape); 
    Map = {{{found_shape, edges}, 0}};
    Map.reserve(2000000000);
    uint8_t num_turns = 1;
    bool s = true;
    std::cout << am.translateRightie(alg) << " " << int(num_turns) << " " << int(Map.size()) << std::endl;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedEdges(edges);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnEdges(edges, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            num_turns ++;
            v = !v;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(num_turns) << " " << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            if(num_turns > max){
                std::cout << "fertig" << std::endl;
                std::cout << int(num_turns) << std::endl;
                std::cout << int(Map.size()) << std::endl;
                now = std::chrono::high_resolution_clock::now();
                duration = now - start_time;
                std::cout << "time: " << duration.count() <<  std::endl;
                return;
            }
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(num_turns) << std::endl;
            std::cout << int(Map.size()) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnEdges(edges, face, t % 3);
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getFastShape(shape); 
                    auto inserted = Map.try_emplace({found_shape, edges}, num_turns);
                    if(inserted.second){
                        std::memcpy(e_cpy.data, edges.data, 9);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateEdges(e_cpy);
                        Map.insert({{found_shape, e_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateEdges(e_cpy);
                        Map.insert({{found_shape, e_cpy},num_turns});
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.mirrorEdges(e_cpy);
                        Map.insert({{found_shape, e_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateEdges(e_cpy);
                        Map.insert({{found_shape, e_cpy},num_turns});
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateEdges(e_cpy);
                        Map.insert({{found_shape, e_cpy},num_turns});
                        algs[!v].push_back(alg);
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnEdges(edges, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}

void Astar::getLinkToShape(std::unordered_map<KeyCorners, Puppet::Shape>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;  
    Puppet::Corners corners;
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedCorners(corners);
    found_shape = puppet.getFastShape(shape); 
    uint32_t index = 0;
    Map = {{{found_shape, corners}, shape}};
    Map.reserve(500000);
    bool s = true;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedCorners(corners);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnCorners(corners, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            v = !v;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnCorners(corners, face, t % 3);
                    found_shape = puppet.getFastShape(shape); 
                    auto inserted = Map.try_emplace({found_shape, corners}, shape);
                    if(inserted.second){
                        index++;
                        alg.push_back({face, uint8_t(t % 3)});
                        algs[!v].push_back(alg);
                        alg.pop_back();
                    }
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnCorners(corners, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}

void Astar::getLinkS(std::unordered_map<Puppet::ShapeFast, uint32_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;    
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    found_shape = puppet.getFastShape(shape); 
    uint32_t index = 0;
    Map = {{found_shape, index++}};
    Map.reserve(20000);
    bool s = true;
    while(true){
        puppet.cubeShape(shape);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            v = !v;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getFastShape(shape); 
                    auto inserted = Map.try_emplace(found_shape, index);
                    if(inserted.second){
                        index++;
                        bin_shape = puppet.getBinShape(shape);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        inserted = Map.try_emplace(found_shape,index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        inserted = Map.try_emplace(found_shape,index);
                        if(inserted.second) index++;
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        inserted = Map.try_emplace(found_shape,index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        inserted = Map.try_emplace(found_shape,index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        inserted = Map.try_emplace(found_shape,index);
                        if(inserted.second) index++;
                        algs[!v].push_back(alg);
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
void Astar::getLinkC(std::unordered_map<KeyCorners, uint32_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;    
    Puppet::Corners corners;
    Puppet::Corners cor_cpy;
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedCorners(corners);
    found_shape = puppet.getFastShape(shape); 
    uint32_t index = 0;
    Map = {{{found_shape, corners}, index++}};
    Map.reserve(500000);
    bool s = true;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedCorners(corners);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnCorners(corners, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            v = !v;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnCorners(corners, face, t % 3);
                    alg.push_back({face, uint8_t(t % 3)});
                    found_shape = puppet.getFastShape(shape); 
                    auto inserted = Map.try_emplace({found_shape, corners}, index);
                    if(inserted.second){
                        index++;
                        std::memcpy(cor_cpy.data, corners.data, 7);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        inserted = Map.try_emplace({found_shape, cor_cpy},index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        inserted = Map.try_emplace({found_shape, cor_cpy},index);
                        if(inserted.second) index++;
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.mirrorCorners(cor_cpy);
                        inserted = Map.try_emplace({found_shape, cor_cpy},index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        inserted = Map.try_emplace({found_shape, cor_cpy},index);
                        if(inserted.second) index++;
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        puppet.rotateCorners(cor_cpy);
                        inserted = Map.try_emplace({found_shape, cor_cpy},index);
                        if(inserted.second) index++;
                        algs[!v].push_back(alg);
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnCorners(corners, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
void Astar::getLinkE(std::unordered_map<Puppet::Edges, uint32_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;   
    Puppet::Edges edges;
    Puppet::Edges e_cpy;
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.solvedEdges(edges);
    uint32_t index = 0;
    Map = {{edges, index++}};
    Map.reserve(400000);
    bool s = true;
    while(true){
        puppet.solvedEdges(edges);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurnEdges(edges, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            v = !v;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            for(uint8_t t = 0; t < 3; t++){
                puppet.doTurnEdges(edges, face, t);
                alg.push_back({face, t});
                auto inserted = Map.try_emplace(edges, index);
                if(inserted.second){
                    index++;
                    std::memcpy(e_cpy.data, edges.data, 9);
                    puppet.rotateEdges(e_cpy);
                    inserted = Map.try_emplace(e_cpy,index);
                    if(inserted.second) index++;
                    puppet.rotateEdges(e_cpy);
                    inserted = Map.try_emplace(e_cpy,index);
                    if(inserted.second) index++;
                    puppet.mirrorEdges(e_cpy);
                    inserted = Map.try_emplace(e_cpy,index);
                    if(inserted.second) index++;
                    puppet.rotateEdges(e_cpy);
                    inserted = Map.try_emplace(e_cpy,index);
                    if(inserted.second) index++;
                    puppet.rotateEdges(e_cpy);
                    inserted = Map.try_emplace(e_cpy,index);
                    if(inserted.second) index++;
                    algs[!v].push_back(alg);
                }
                alg.pop_back();
                puppet.doTurnEdges(edges, face, 2 - t);
            }
        }
    }
}
void Astar::getLinkF(std::unordered_map<Puppet::FlipsFast, uint32_t>& Map){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;   
    Puppet::Flips flips;
    Puppet::Flips f_cpy;
    Puppet::FlipsFast f_fast;
    int8_t last_turn = -1;
    bool v = 0;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.solvedFlips(flips);
    f_fast = puppet.getFastFlips(flips);
    uint32_t index = 0;
    Map = {{f_fast, index++}};
    Map.reserve(500);
    bool s = true;
    while(true){
        puppet.solvedFlips(flips);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurnFlips(flips, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            v = !v;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(index) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            for(uint8_t t = 0; t < 3; t++){
                puppet.doTurnFlips(flips, face, t);
                alg.push_back({face, t});
                f_fast = puppet.getFastFlips(flips);
                auto inserted = Map.try_emplace(f_fast, index);
                if(inserted.second){
                    index++;
                    std::memcpy(f_cpy.data, flips.data, 9);
                    puppet.mirrorFlips(f_cpy);
                    f_fast = puppet.getFastFlips(flips);
                    inserted = Map.try_emplace(f_fast,index);
                    if(inserted.second) index++;
                    algs[!v].push_back(alg);
                }
                alg.pop_back();
                puppet.doTurnFlips(flips, face, 2 - t);
            }
        }
    }
}

void Astar::getCFarray(std::vector<uint8_t>& HCF, 
    std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;   
    Puppet::Corners corners;
    Puppet::Corners c_cpy; 
    Puppet::Flips flips;
    Puppet::Flips f_cpy;
    Puppet::FlipsFast f_fast;
    int8_t last_turn = -1;
    bool v = 0;
    size_t index;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedCorners(corners);
    puppet.solvedFlips(flips);
    found_shape = puppet.getFastShape(shape); 
    f_fast = puppet.getFastFlips(flips);
    //HCF.reserve(CF_SIZE); 
    HCF.assign(CF_SIZE, 255);
    HCF[MC[{found_shape, corners}]*F_SIZE + MF[f_fast]] = 0;
    uint8_t num_turns = 1;
    size_t c = 1;
    bool s = true;
    while(true){
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            puppet.cubeShape(shape);
            puppet.solvedFlips(flips);
            puppet.solvedCorners(corners);
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnFlips(flips, turn[0], turn[1]);
                puppet.doTurnCorners(corners, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            num_turns ++;
            v = !v;
            std::cout << int(num_turns) << " " << c <<  std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << "save progress" <<  std::endl;
            //HCF.shrink_to_fit();
            writeData("CFarray", HCF.data(), HCF.size() * sizeof(uint8_t));
            std::cout << "saved" <<  std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << "alles fertig" << std::endl;
            std::cout << int(num_turns) << " " << c << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
             std::cout << "save progress" <<  std::endl;
            writeData("CFarray", HCF.data(), HCF.size() * sizeof(uint8_t));
            std::cout << "saved" <<  std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnCorners(corners, face, t % 3);
                    puppet.doTurnFlips(flips, face, t % 3);
                    found_shape = puppet.getFastShape(shape); 
                    f_fast = puppet.getFastFlips(flips);
                    index = MC[{found_shape, corners}]*F_SIZE + MF[f_fast];
                    uint8_t& cur = HCF[index];
                    if(num_turns < cur){ 
                        cur = num_turns;
                        std::memcpy(f_cpy.data, flips.data, 9);
                        std::memcpy(c_cpy.data, corners.data, 7);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.mirrorFlips(f_cpy);
                        puppet.mirrorCorners(c_cpy);
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        f_fast = puppet.getFastFlips(f_cpy);
                        HCF[MC[{found_shape, c_cpy}]*F_SIZE + MF[f_fast]] = num_turns;
                        alg.push_back({face, uint8_t(t % 3)});
                        algs[!v].push_back(alg);
                        alg.pop_back();
                        c += 2;
                    }
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnFlips(flips, face, 2 - (t % 3));
                    puppet.doTurnCorners(corners, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}
void Astar::getSEarray(std::vector<uint8_t>& HSE,
    std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    Puppet::Shape shape;
    Puppet::ShapeFast found_shape;
    Puppet::ShapeBin bin_shape;   
    Puppet::Edges edges;
    Puppet::Edges e_cpy;
    int8_t last_turn = -1;
    bool v = 0;
    size_t index;
    std::array<std::vector<std::vector<std::array<uint8_t, 2>>>, 2> algs;
    std::vector<std::array<uint8_t, 2>> alg = {};
    puppet.cubeShape(shape);
    puppet.solvedEdges(edges);
    found_shape = puppet.getFastShape(shape); 
    //HSE.reserve(SE_SIZE); 
    HSE.assign(SE_SIZE, 255);
    HSE[MS[found_shape]*E_SIZE + ME[edges]] = 0;
    uint8_t num_turns = 1; 
    bool s = true;
    size_t c = 1;
    while(true){
        puppet.cubeShape(shape);
        puppet.solvedEdges(edges);
        if(!algs[v].empty()){
            alg = algs[v].back();
            algs[v].pop_back();
            for(auto turn : alg){
                puppet.doTurn(shape, turn[0], turn[1]);
                puppet.doTurnEdges(edges, turn[0], turn[1]);
            }
            last_turn = alg.back()[0];
        }else if(!algs[!v].empty()){
            std::cout << int(num_turns) << " " << c << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            if(num_turns >= 12){
                std::cout << "save progress" <<  std::endl;
                //HSE.shrink_to_fit();
                writeData("SE_arr", HSE.data(), HSE.size());
                now = std::chrono::high_resolution_clock::now();
                duration = now - start_time;
                std::cout << "time: " << duration.count() <<  std::endl;
            }
            num_turns ++;
            v = !v;
            continue;
        }else if (s){
            s = false;
        }else{
            std::cout << int(num_turns) << " " << c << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            std::cout << "save progress" <<  std::endl;
            //HSE.shrink_to_fit();
            writeData("SE_arr", HSE.data(), HSE.size());
            std::cout << "FLIP THE SHIT!!!!!!!" << std::endl;
            return;
        }
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_turn){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurn(shape, face, t % 3)){
                    puppet.doTurnEdges(edges, face, t % 3);
                    found_shape = puppet.getFastShape(shape); 
                    //index = MS[found_shape]*E_SIZE + ME[edges];
                    index = static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[edges];
                    uint8_t& cur = HSE[index];
                    if(num_turns < cur){
                        cur = num_turns;
                        std::memcpy(e_cpy.data, edges.data, 9);
                        bin_shape = puppet.getBinShape(shape);
                        puppet.rotateEdges(e_cpy);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        //HSE[MS[found_shape]*E_SIZE + ME[e_cpy]] = num_turns;
                        HSE[static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[e_cpy]] = num_turns;
                        puppet.rotateEdges(e_cpy);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        //HSE[MS[found_shape]*E_SIZE + ME[e_cpy]] = num_turns;
                        HSE[static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[e_cpy]] = num_turns;
                        puppet.mirrorEdges(e_cpy);
                        puppet.mirrorShapeBin(bin_shape,2);
                        found_shape = puppet.getFastShape(bin_shape);
                        //HSE[MS[found_shape]*E_SIZE + ME[e_cpy]] = num_turns;
                        HSE[static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[e_cpy]] = num_turns;
                        puppet.rotateEdges(e_cpy);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        //HSE[MS[found_shape]*E_SIZE + ME[e_cpy]] = num_turns;
                        HSE[static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[e_cpy]] = num_turns;
                        puppet.rotateEdges(e_cpy);
                        puppet.rotateShapeBin(bin_shape,0);
                        found_shape = puppet.getFastShape(bin_shape);
                        //HSE[MS[found_shape]*E_SIZE + ME[e_cpy]] = num_turns;
                        HSE[static_cast<size_t>(MS[found_shape]) * E_SIZE + ME[e_cpy]] = num_turns;
                        alg.push_back({face, uint8_t(t % 3)});
                        algs[!v].push_back(alg);
                        alg.pop_back();
                        c+= 6;
                    }
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    puppet.doTurnEdges(edges, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
}

uint8_t Astar::getMapValCE(std::unordered_map<KeyCorners, uint8_t>& HeuC, std::unordered_map<KeyEdges, uint8_t>& HeuE, Puppet::StateFast& sf){
    return HeuC[{sf.shape, sf.corners}] > HeuE[{sf.edges, sf.flips}] ? HeuC[{sf.shape, sf.corners}] : HeuE[{sf.edges, sf.flips}];
}

KeyCorners makeKey(const Entry<KeyCorners, uint8_t>*, const Puppet::StateFast& sf) {
    return {sf.shape, sf.corners};
}
KeyEdges makeKey(const Entry<KeyEdges, uint8_t>*, const Puppet::StateFast& sf) {
    return {sf.edges, sf.flips};
}
KeySF makeKey(const Entry<KeySF, uint8_t>*, const Puppet::StateFast& sf) {
    return {sf.shape, sf.flips};
}
KeySE makeKey(const Entry<KeySE, uint8_t>*, const Puppet::StateFast& sf) {
    return {sf.shape, sf.edges};
}

template <typename E>  uint8_t getHeuVal(const std::vector<E>& heuristic, const Puppet::StateFast& sf){
    E e;
    e.key = makeKey(static_cast<E*>(nullptr), sf);
    auto i = std::lower_bound(heuristic.begin(), heuristic.end(), e);
    if (i != heuristic.end()/*&& std::memcmp(&cor->key, &targetC.key, sizeof(KeyCorners)) == 0*/) {
        if(!(e.key == i->key)){
            //std::cout << "Other Key not found !!!" << std::endl;
            return 255;
        }
        return  i->value;
    }   
    return 255;
}

uint8_t Astar::getHeuValCE(const std::vector<Entry<KeyCorners, uint8_t>>& heuC, const std::vector<Entry<KeyEdges, uint8_t>>& heuE, const Puppet::StateFast& sf){
    uint8_t cVal = getHeuVal<Entry<KeyCorners, uint8_t>>(heuC, sf);
    uint8_t eVal = getHeuVal<Entry<KeyEdges, uint8_t>>(heuE, sf);
    return cVal > eVal ? cVal : eVal;
}
uint8_t Astar::getHeuValS4(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, 
            const std::vector<Entry<Key4E, uint8_t>>& H4E1, const std::vector<Entry<Key4E, uint8_t>>& H4E2, const Puppet::StateFast& sf, const uint8_t max){
    uint8_t cVal = getHeuVal<Entry<KeyCorners, uint8_t>>(HC, sf);
    uint8_t sfVal = getHeuVal<Entry<KeySF, uint8_t>>(HSF, sf);
    uint8_t e1Val = getHeuVal4E(H4E1, 0, sf, max);
    uint8_t e2Val = getHeuVal4E(H4E2, 1, sf, max);
    uint8_t ret1 = cVal > sfVal ? cVal : sfVal;
    uint8_t ret2 = e1Val > e2Val ? e1Val : e2Val;
    return ret1 > ret2 ? ret1 : ret2;
}
uint8_t Astar::getHeuValSE(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, const std::vector<Entry<KeyEdges, uint8_t>>& HE, const Puppet::StateFast& sf){
    uint8_t cVal = getHeuVal<Entry<KeyCorners, uint8_t>>(HC, sf);
    uint8_t sfVal = getHeuVal<Entry<KeySF, uint8_t>>(HSF, sf);
    uint8_t eVal = getHeuVal<Entry<KeyEdges, uint8_t>>(HE, sf);
    uint8_t ret1 = cVal > sfVal ? cVal : sfVal;
    return ret1 > eVal ? ret1 : eVal;
}
uint8_t Astar::getHeuValS(const std::vector<Entry<KeyCorners, uint8_t>>& HC, const std::vector<Entry<KeySF, uint8_t>>& HSF, const std::vector<Entry<KeySE, uint8_t>>& HSE, const Puppet::StateFast& sf, const uint8_t max){
    uint8_t cVal = getHeuVal<Entry<KeyCorners, uint8_t>>(HC, sf);
    uint8_t sfVal = getHeuVal<Entry<KeySF, uint8_t>>(HSF, sf);
    uint8_t eVal = getHeuVal<Entry<KeySE, uint8_t>>(HSE, sf); 
    if(eVal == 255){
        eVal = max + 1;
    }
    uint8_t ret1 = cVal > sfVal ? cVal : sfVal;
    return ret1 > eVal ? ret1 : eVal;
}



std::vector<std::array<uint8_t, 2>> Astar::astarCE(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HeuC, std::vector<Entry<KeyEdges, uint8_t>>& HeuE){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::vector<std::array<uint8_t, 2>> alg; //keinen alg mehr speichern! stattdessen pointer auf vorgänger und last_turn??
        uint8_t value;
    };
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    std::vector<std::array<uint8_t, 2>> alg = {};
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t val = 0;
    uint8_t newval = 0;
    std::array<std::queue<node>, 3> found;
    found[0].push({state, alg, getHeuValCE(HeuC, HeuE, sf)});
    std::unordered_set<Puppet::StateFast> checked;
    checked.reserve(1000000);
    checked.insert(sf);
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(alg.size() + newval)<< " alg: " << am.translateRightie(alg) << " val: " << int(newval)<< std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        state = found[queue].front().state;
        alg = found[queue].front().alg;
        val = found[queue].front().value;
        if(!alg.empty()) last_face = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    auto i = checked.insert(sf);
                    if(i.second){  //i.second is false if sf was already in the set
                        alg.push_back({face, uint8_t(t % 3)});
                        newval = getHeuValCE(HeuC, HeuE, sf);
                        if(newval == 0){
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "time: " << duration.count() <<  std::endl;
                            std::cout << am.translateRightie(am.reverse(am.translateYX2(am.reverse(alg))))<< " " << int(alg.size()) << std::endl;
                            return alg;
                        }
                        found[(queue + 1 + newval - val)%3].push({state, alg, newval});
                        alg.pop_back();
                    }
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
        found[queue].pop();
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    return {{0,0},{0,0}};
}
std::vector<std::array<uint8_t, 2>> Astar::AstarShape4(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, 
            std::vector<Entry<Key4E, uint8_t>>& H4E1, std::vector<Entry<Key4E, uint8_t>>& H4E2, const uint8_t max){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::vector<std::array<uint8_t, 2>> alg; 
        uint8_t value;
    };
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    std::vector<std::array<uint8_t, 2>> alg = {};
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t val = 0;
    uint8_t newval = 0;
    uint8_t min_val = 255;
    std::vector<std::array<uint8_t, 2>> min_alg = {};
    std::array<std::queue<node>, 3> found;
    found[0].push({state, alg, getHeuValS4(HC, HSF, H4E1, H4E2, sf, max)});
    std::unordered_set<Puppet::StateFast> checked;
    checked.reserve(1000000);
    checked.insert(sf);
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(min_alg.size() + min_val)<< " alg: " << am.translateRightie(min_alg) << " val: " << int(min_val)<< std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        state = found[queue].front().state;
        alg = found[queue].front().alg;
        val = found[queue].front().value;
        if(!alg.empty()) last_face = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    auto i = checked.insert(sf);
                    if(i.second){  //i.second is false if sf was already in the set
                        alg.push_back({face, uint8_t(t % 3)});
                        newval = getHeuValS4(HC, HSF, H4E1, H4E2, sf, max);
                        if(newval == 0){
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "time: " << duration.count() <<  std::endl;
                            return alg;
                        }
                        /*if(newval - val > 1 ||val - newval > 1){
                            std::cout << int(val) << " " << int(newval) << std::endl;
                            std::cout << am.translateRightie(found[queue].front().alg) << " " << am.translateRightie(alg) << std::endl;

                        }*/
                        if(newval < min_val){
                            min_val = newval;
                            min_alg = alg;
                        }
                        uint8_t v = newval != val ? (newval - val > 0 ? 1: -1) : 0;
                        found[(queue + 1 + v)%3].push({state, alg, newval});
                        alg.pop_back();
                    }
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
        found[queue].pop();
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    return {{0,0},{0,0}};
}
void Astar::AstarSE(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, std::vector<Entry<KeyEdges, uint8_t>>& HE, std::vector<std::array<uint8_t, 2>>& alg){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::vector<std::array<uint8_t, 2>> alg; 
        uint8_t value;
    };
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    alg = {};
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t val = 0;
    uint8_t newval = 0;
    uint8_t min_val = 255;
    std::vector<std::array<uint8_t, 2>> min_alg = {};
    std::array<std::vector<node>, 3> found;
    found[0].push_back({state, alg, getHeuValSE(HC, HSF, HE, sf)});
    std::unordered_set<Puppet::StateFast> checked;
    checked.reserve(1000000);
    checked.insert(sf);
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(min_alg.size() + min_val)<< " alg: " << am.translateRightie(min_alg) << " val: " << int(min_val)
            <<" size of checked: "<< checked.size()<< std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        state = found[queue].back().state;
        alg = found[queue].back().alg;
        val = found[queue].back().value;
        found[queue].pop_back();
        if(!alg.empty()) last_face = alg.back()[0];
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    auto i = checked.insert(sf);
                    if(i.second){  //i.second is false if sf was already in the set
                        alg.push_back({face, uint8_t(t % 3)});
                        newval = getHeuValSE(HC, HSF, HE, sf);
                        if(newval == 0){
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "solution found. time: " << duration.count() <<" size of checked: "<< checked.size()<< std::endl;
                            std::cout << am.translateRightie(am.reverse(am.translateYX2(am.reverse(alg))))<< " " << int(alg.size()) << std::endl;
                            std::cout << "swapping..." << std::endl;
                            std::array<std::vector<node>, 3>().swap(found);
                            std::cout << "vektor swapped" << std::endl;
                            return;
                        }
                        if(newval - val > 1 ||val - newval > 1){
                            std::cout << int(val) << " " << int(newval) << std::endl;
                            std::cout << am.translateRightie(found[queue].back().alg) << " " << am.translateRightie(alg) << std::endl;

                        }
                        if(newval < min_val){
                            min_val = newval;
                            min_alg = alg;
                        }
                        //uint8_t v = newval != val ? (newval - val > 0 ? 1: -1) : 0;
                        found[(queue + 1 + newval - val)%3].push_back({state, alg, newval});
                        alg.pop_back();
                    }
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    std::cout << "vector empty and could not solve" << std::endl;
    return;
}

std::vector<std::array<uint8_t, 2>> Astar::AstarS(const Puppet::State& start, std::vector<Entry<KeyCorners, uint8_t>>& HC, std::vector<Entry<KeySF, uint8_t>>& HSF, 
    std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::array<uint8_t, 40> alg;
        uint8_t len;
        uint8_t value;
    };
    node cur;
    node next;
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t newval = 0;
    uint8_t min_val = 255;
    uint8_t min_len = 255;
    //int counter = 0;
    std::array<std::vector<node>, 3> found;
    found[0].push_back({state, {}, 0, getHeuValS(HC, HSF, HSE, sf, max)});
    //std::unordered_set<Puppet::StateFast> checked;
    //checked.reserve(20000000);
    //checked.insert(sf);
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(min_len + min_val)<< " val: " << int(min_val)
            <</*" size of checked: "<< checked.size()<<*/ std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        cur = found[queue].back();
        state = cur.state;
        found[queue].pop_back();
        if(cur.len > 0) last_face = cur.alg[cur.len-1]%3;
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    //auto i = checked.insert(sf);
                    //if(i.second){  //i.second is false if sf was already in the set
                        newval = getHeuValS(HC, HSF, HSE, sf, max);
                        if(newval == 0){
                            now = std::chrono::high_resolution_clock::now();
                            duration = now - start_time;
                            std::cout << "solution found. time: " << duration.count() <</*" size of checked: "<< checked.size()<<*/ std::endl;
                            std::vector<std::array<uint8_t, 2>> alg;
                            for(uint8_t i = 0; i < cur.len; i++){
                                alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                            }
                            alg.push_back({face, uint8_t(t % 3)});
                            //std::cout << am.translateRightie(am.reverse(am.translateYX2(am.reverse(alg))))<< " " << int(alg.size()) << std::endl;
                            //std::cout << "hits in checked: " << counter << std::endl;
                            std::cout << "found[0]: " << found[0].size() << std::endl;
                            std::cout << "found[1]: " << found[1].size() << std::endl;
                            std::cout << "found[2]: " << found[2].size() << std::endl;
                            return alg;
                        }
                        if(newval < min_val){
                            min_val = newval;
                            min_len = cur.len + 1;
                        } 
                        next.state = state;
                        next.alg = cur.alg;
                        next.alg[cur.len] = uint8_t(face + (t % 3) * 3);          
                        next.len = cur.len + 1;
                        next.value = newval;
                        found[(queue + 1 + newval - cur.value)%3].push_back(next);
                    //}else{counter++;}
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    std::cout << "vector empty and could not solve" << std::endl;
    return {{255,255}};
}

uint8_t Astar::getSuperValS(std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
    const std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max, const Puppet::StateFast& sf){
    uint8_t cfVal = HCF[MC[{sf.shape, sf.corners}]*F_SIZE + MF[sf.flips]];
    uint8_t seVal = getHeuVal<Entry<KeySE, uint8_t>>(HSE, sf);
    if(seVal == 255){
        seVal = max + 1;
    }
    if(cfVal == 255){
        std::cout << "not found in cf" << std::endl;
    }
    return cfVal > seVal ? cfVal : seVal;
}
    
std::vector<std::array<uint8_t, 2>> Astar::superstar(const Puppet::State& start, 
        std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
        std::vector<Entry<KeySE, uint8_t>>& HSE, const uint8_t max){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::array<uint8_t, 40> alg;
        uint8_t len;
        uint8_t value;
    };
    node cur;
    node next;
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t newval = 0;
    uint8_t min_val = 255;
    uint8_t min_len = 255;
    std::array<std::vector<node>, 3> found;
    found[0].push_back({state, {}, 0, getSuperValS(HCF, MC, MF, HSE, max, sf)});
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(min_len + min_val)<< " val: " << int(min_val) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        cur = found[queue].back();
        state = cur.state;
        found[queue].pop_back();
        if(cur.len > 0) last_face = cur.alg[cur.len-1]%3;
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    newval = getSuperValS(HCF, MC, MF, HSE, max, sf);
                    if(newval == 0){
                        now = std::chrono::high_resolution_clock::now();
                        duration = now - start_time;
                        std::cout << "solution found. time: " << duration.count() << std::endl;
                        std::vector<std::array<uint8_t, 2>> alg;
                        for(uint8_t i = 0; i < cur.len; i++){
                            alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                        }
                        alg.push_back({face, uint8_t(t % 3)});
                        std::cout << "found[0]: " << found[0].size() << std::endl;
                        std::cout << "found[1]: " << found[1].size() << std::endl;
                        std::cout << "found[2]: " << found[2].size() << std::endl;
                        return alg;
                    }
                    if(newval < min_val){
                        min_val = newval;
                        min_len = cur.len + 1;
                    } 
                    next.state = state;
                    next.alg = cur.alg;
                    next.alg[cur.len] = uint8_t(face + (t % 3) * 3);          
                    next.len = cur.len + 1;
                    next.value = newval;
                    found[(queue + 1 + newval - cur.value)%3].push_back(next);
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    std::cout << "vector empty and could not solve" << std::endl;
    return {{255,255}};    
}

uint8_t Astar::getUltraValS(std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
            std::vector<uint8_t>& HSE, std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME, const Puppet::StateFast& sf){
    uint8_t cfVal = HCF[MC[{sf.shape, sf.corners}]*F_SIZE + MF[sf.flips]];
    uint8_t seVal = HSE[static_cast<size_t>(MS[sf.shape])*E_SIZE + ME[sf.edges]];
    if(seVal == 255){
        std::cout << "no entry here" << std::endl;
    }
    return cfVal > seVal ? cfVal : seVal;
}

std::vector<std::array<uint8_t, 2>> Astar::ultrastar(const Puppet::State& start, 
        std::vector<uint8_t>& HCF, std::unordered_map<KeyCorners, uint32_t>& MC, std::unordered_map<Puppet::FlipsFast, uint32_t>& MF, 
        std::vector<uint8_t>& HSE, std::unordered_map<Puppet::ShapeFast, uint32_t>& MS, std::unordered_map<Puppet::Edges, uint32_t>& ME){
    auto start_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration{0.0};
    auto now = start_time;
    struct node{
        Puppet::State state;
        std::array<uint8_t, 50> alg;
        uint8_t len;
        uint8_t value;
    };
    node cur;
    node next;
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t newval = 0;
    uint8_t min_val = 255;
    uint8_t min_len = 255;
    std::array<std::vector<node>, 3> found;
    found[0].push_back({state, {}, 0, getUltraValS(HCF, MC, MF, HSE, MS, ME, sf)});
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            std::cout << "switched to next queue. best case: " <<  int(min_len + min_val)<< " val: " << int(min_val) << std::endl;
            now = std::chrono::high_resolution_clock::now();
            duration = now - start_time;
            std::cout << "time: " << duration.count() <<  std::endl;
            continue;
        }
        cur = found[queue].back();
        state = cur.state;
        found[queue].pop_back();
        if(cur.len > 0) last_face = cur.alg[cur.len-1]%3;
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    newval = getUltraValS(HCF, MC, MF, HSE, MS, ME, sf);
                    if(newval == 0){
                        now = std::chrono::high_resolution_clock::now();
                        duration = now - start_time;
                        std::cout << "solution found. time: " << duration.count() << std::endl;
                        std::vector<std::array<uint8_t, 2>> alg;
                        for(uint8_t i = 0; i < cur.len; i++){
                            alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                        }
                        alg.push_back({face, uint8_t(t % 3)});
                        std::cout << "found[0]: " << found[0].size() << std::endl;
                        std::cout << "found[1]: " << found[1].size() << std::endl;
                        std::cout << "found[2]: " << found[2].size() << std::endl;
                        return alg;
                    }
                    if(newval < min_val){
                        min_val = newval;
                        min_len = cur.len + 1;
                    } 
                    next.state = state;
                    next.alg = cur.alg;
                    next.alg[cur.len] = uint8_t(face + (t % 3) * 3);          
                    next.len = cur.len + 1;
                    next.value = newval; 
                    if(cur.value > newval + 1 || cur.value < newval - 1){
                        std::cout << int(newval) << " oje " << int(cur.value) << std::endl;
                        std::vector<std::array<uint8_t, 2>> alg;
                        for(uint8_t i = 0; i < cur.len; i++){
                            alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                        }
                        alg.push_back({face, uint8_t(t % 3)});
                        std::cout << "alg: " << am.translateRightie(alg) << std::endl;
                        Puppet::StateFast sf_old = puppet.getFastState(cur.state);
                        std::cout << size_t(size_t(MS[sf.shape]*E_SIZE) + ME[sf.edges]) << " " << size_t(size_t(MS[sf_old.shape]*E_SIZE) + ME[sf_old.edges]) << std::endl;
                        newval = cur.value+1; 
                    }                  
                    found[(queue + 1 + newval - cur.value)%3].push_back(next);
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    now = std::chrono::high_resolution_clock::now();
    duration = now - start_time;
    std::cout << "time: " << duration.count() <<  std::endl;
    std::cout << "vector empty and could not solve" << std::endl;
    return {{255,255}};    
}

std::vector<std::array<uint8_t, 2>> Astar::Astar_RSS(std::unordered_map<KeyCorners, uint32_t>& MC,
            std::unordered_map<Puppet::FlipsFast, uint32_t>& MF,
            std::unordered_map<Puppet::ShapeFast, uint32_t>& MS,
            std::unordered_map<Puppet::Edges, uint32_t>& ME,
            //std::vector<Entry<KeyCorners, uint32_t>>& HC,
            std::vector<Entry<Puppet::FlipsFast, uint32_t>>& HF,
            //std::vector<Entry<Puppet::ShapeFast, uint32_t>>& HS,
            std::vector<Entry<Puppet::Edges, uint32_t>>& HE,
            std::vector<Entry<KeyCorners, Puppet::Shape>>& H_shape,
            std::vector<uint8_t>& HCF,
            std::vector<uint8_t>& HSE){
    Puppet::State start;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pickSC(0, H_shape.size()-1);
    int index = pickSC(gen);
    start.shape = H_shape[index].value;
    start.corners = H_shape[index].key.corners;
    //puppet.solvedState(start);
    std::uniform_int_distribution<> pickF(0, HF.size()-1);
    start.flips = puppet.getFlipsFromFast(HF[pickF(gen)].key);
    //while(puppet.countEdgesSolved(start.edges) != 5){
        std::uniform_int_distribution<> pickE(0, HE.size()-1);
        start.edges = HE[pickE(gen)].key;
        puppet.solveParity(start.corners, start.edges);
    //}
    struct node{
        Puppet::State state;
        std::array<uint8_t, 50> alg;
        uint8_t len;
        uint8_t value;
    };
    node cur;
    node next;
    Puppet::State state = start;
    Puppet::StateFast sf = puppet.getFastState(start);
    int8_t last_face = -1;
    uint8_t queue = 0;
    uint8_t newval = 0;
    std::array<std::vector<node>, 3> found;
    found[0].push_back({state, {}, 0, getUltraValS(HCF, MC, MF, HSE, MS, ME, sf)});
    while(!found[0].empty()||!found[1].empty()||!found[2].empty()){
        if(found[queue].empty()){
            queue = (queue + 1) % 3; 
            continue;
        }
        cur = found[queue].back();
        state = cur.state;
        found[queue].pop_back();
        if(cur.len > 0) last_face = cur.alg[cur.len-1]%3;
        for(uint8_t face = 0; face < 3; ++face){
            if(face == last_face){continue;}
            bool HT_possible = false;
            for(uint8_t t = 2; t < 5; t++){
                if((HT_possible || t != 4) && puppet.doTurnState(state, face, t % 3)){
                    sf = puppet.getFastState(state);
                    newval = getUltraValS(HCF, MC, MF, HSE, MS, ME, sf);
                    if(newval == 0){
                        std::vector<std::array<uint8_t, 2>> alg;
                        for(uint8_t i = 0; i < cur.len; i++){
                            alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                        }
                        alg.push_back({face, uint8_t(t % 3)});
                        return alg;
                    }
                    next.state = state;
                    next.alg = cur.alg;
                    next.alg[cur.len] = uint8_t(face + (t % 3) * 3);          
                    next.len = cur.len + 1;
                    next.value = newval;    
                    if(cur.value > newval + 1 || cur.value < newval - 1){
                        std::cout << int(newval) << " oje " << int(cur.value) << std::endl;
                        std::vector<std::array<uint8_t, 2>> alg;
                        for(uint8_t i = 0; i < cur.len; i++){
                            alg.push_back({uint8_t(cur.alg[i]%3), uint8_t((cur.alg[i] - cur.alg[i]%3)/3)});
                        }
                        alg.push_back({face, uint8_t(t % 3)});
                        std::cout << "alg: " << am.translateRightie(alg) << std::endl;
                        Puppet::StateFast sf_old = puppet.getFastState(cur.state);
                        std::cout << size_t(size_t(MS[sf.shape]*E_SIZE) + ME[sf.edges]) << " " << size_t(size_t(MS[sf_old.shape]*E_SIZE) + ME[sf_old.edges]) << std::endl;
                        std::cout << int(HSE[size_t(size_t(MS[sf.shape]*E_SIZE) + ME[sf.edges])]) << " " << int(HSE[size_t(size_t(MS[sf_old.shape]*E_SIZE) + ME[sf_old.edges])]) << std::endl;
                        newval = cur.value+1; 
                    }                   
                    found[(queue + 1 + newval - cur.value)%3].push_back(next);
                    puppet.doTurnState(state, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    return {{255,255}}; 
}