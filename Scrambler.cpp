#include "Scrambler.hpp"

std::vector<std::array<uint8_t, 2>> Scrambler::RMS(uint8_t movecount){
    //Puppet puppet;
    Puppet::Shape shape;
    //AlgMani am;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::array<std::array<uint8_t, 2>, 9> turns = {{{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}}};
    std::vector<std::array<uint8_t, 2>> valid_turns;
    std::vector<std::vector<std::array<uint8_t, 2>>> impossible_turns;
    std::vector<std::array<uint8_t, 2>> scramble;
    bool impossible;
    uint8_t length;
    length = 0;
    puppet.cubeShape(shape);
    std::uniform_int_distribution<> pick(0, 8);
    std::array<uint8_t, 2> turn = turns[pick(gen)];
    uint8_t last_face = turn[0];
    puppet.doTurn(shape, turn[0], turn[1]);
    scramble.push_back(turn);
    impossible_turns.push_back({});
    while(scramble.size() < movecount){
        length = scramble.size() - 1;
        for (const auto& t : turns) {
            impossible = 0;
            for(const auto& i : impossible_turns.at(length)){
                if(t == i){
                    impossible = 1;
                    break;
                }
            }
            if (t[0] != last_face && !impossible) {
                valid_turns.push_back(t);
            }
        }
        if(valid_turns.size() == 0){
            turn = scramble.back();
            scramble.pop_back();
            puppet.doTurn(shape, turn[0],2 - turn[1]);
            last_face = scramble.back()[0];
            impossible_turns.at(length).clear();
            impossible_turns.at(length-1).push_back(turn);
            continue;   
        }
        std::uniform_int_distribution<> pick(0, valid_turns.size() - 1);
        turn = valid_turns[pick(gen)];
        if(!puppet.testTurn(shape, turn[0], turn[1])){
            impossible_turns.at(length).push_back(turn);
        }else if(turn[1] == 1 && !puppet.testTurn(shape, turn[0], 0) && !puppet.testTurn(shape, turn[0], 2)){
            impossible_turns.at(length).push_back(turn);
        }else{
            puppet.doTurn(shape, turn[0],turn[1]);
            last_face = turn[0];
            scramble.push_back(turn);
            impossible_turns.push_back({});
        }
        valid_turns.clear();
    }
    //std::cout << am.translateRightie(am.translateYX2(scramble)) << std::endl;
    return scramble;
}

std::vector<Puppet::ShapeBin> Scrambler::computeAllShapes(){
    Puppet::Shape shape;
    uint8_t last_turn = 0;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    Puppet::ShapeBin found_shape;
    std::vector<Puppet::ShapeBin> found_shapes;
    algs.push({{0,0}});
    algs.push({{0,1}});
    while(true){
        puppet.cubeShape(shape);
        std::vector<std::array<uint8_t, 2>> alg;
        if(!algs.empty()){
            alg = algs.front();
        }else{
            break;
        }
        algs.pop();
        for(auto turn : alg){
            puppet.doTurn(shape, turn[0], turn[1]);
        }
        last_turn = alg.back()[0];
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
                    }
                    alg.pop_back();
                    puppet.doTurn(shape, face, 2 - (t % 3));
                    HT_possible = true;
                }
            }
        }
    }
    return found_shapes;
}

auto Scrambler::findShapeFromShape(const Puppet::Shape& start, const Puppet::ShapeBin& end){
    Puppet::Shape shape;
    int8_t last_turn = -1;
    std::queue<std::vector<std::array<uint8_t, 2>>> algs;
    std::vector<std::array<uint8_t, 2>> alg;
    Puppet::ShapeBin found_shape = puppet.getBinShape(start);
    std::vector<Puppet::ShapeBin> found_shapes = {puppet.getBinShape(start)};
    if(std::memcmp(end.data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
        return std::vector<std::array<uint8_t, 2>> {{}};
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
            std::cout << "error in findShapeFromShape" << std::endl;
            return std::vector<std::array<uint8_t, 2>> {};
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
                        if(std::memcmp(end.data, found_shape.data, sizeof(Puppet::ShapeBin::data)) == 0){
                            return alg;
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

std::vector<std::array<uint8_t, 2>> Scrambler::RandomShape(uint8_t movecount, std::vector<Puppet::ShapeBin> allShapes){
    auto scramble = RMS(movecount);
    Puppet::Shape shape;
    puppet.cubeShape(shape);
    puppet.doAlgExpectPossible(shape, scramble);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> pick(0, allShapes.size()-1);
    Puppet::ShapeBin randShape;
    std::memcpy(randShape.data, allShapes[pick(gen)].data, sizeof(Puppet::ShapeBin::data));
    auto alg = findShapeFromShape(shape, randShape);
    again:
    if(alg.empty()){
        return scramble;
    }
    if(scramble.empty()){
        return alg;
    }
    if(scramble.back()[0] == alg[0][0]){
        int8_t dir = (2+alg[0][1] + scramble.back()[1])%4-1;
        if(dir == -1){
            alg = am.reverse(alg);
            alg.pop_back();
            alg = am.reverse(alg);
            scramble.pop_back();
            goto again;
        }
        alg = am.reverse(alg);
        scramble.pop_back();
        scramble.push_back({alg.back()[0], (uint8_t)dir});
        alg.pop_back();
        alg = am.reverse(alg);
    }
    scramble.insert(scramble.end(),alg.begin(),alg.end());
    return scramble;
}