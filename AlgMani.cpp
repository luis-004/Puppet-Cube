#include "AlgMani.hpp"
AlgMani::AlgMani(){}
std::string AlgMani::translateRightie(const std::vector<std::array<uint8_t, 2>> alg){
    std::string translatedAlg = "";
    for(auto turn : alg){
        switch(turn[0]){
            case 0:
                translatedAlg += 'R';
                break;
            case 1:
                translatedAlg += 'U';
                break;
            case 2:
                translatedAlg += 'F';
                break;
        };
        switch(turn[1]){
            case 1:
                translatedAlg += '2';
                break;
            case 2:
                translatedAlg += '\'';
                break;
        };
    }
    return translatedAlg;
} 

std::string AlgMani::translateLeftie(const std::vector<std::array<uint8_t, 2>> alg){
    std::string translatedAlg = "";
    for(auto turn : alg){
        switch(turn[0]){
            case 0:
                translatedAlg += 'L';
                break;
            case 1:
                translatedAlg += 'U';
                break;
            case 2:
                translatedAlg += 'F';
                break;
        };
        switch(turn[1]){
            case 1:
                translatedAlg += '2';
                break;
            case 0:
                translatedAlg += '\'';
                break;
        };
    }
    return translatedAlg;
}

std::vector<std::array<uint8_t, 2>> AlgMani::translateBack(const std::string alg){
    std::vector<std::array<uint8_t, 2>> translatedAlg;
    for(auto turn : alg){
        switch(turn){
            case 'R':
                translatedAlg.push_back({0,0});
                break;
            case 'U':
                translatedAlg.push_back({1,0});
                break;
            case 'F':
                translatedAlg.push_back({2,0});
                break;
            case '\'':
                translatedAlg.back()[1] = 2;
                break;
            case '2':
                translatedAlg.back()[1] = 1;
                break;
        };
    }
    return translatedAlg;
}

std::vector<std::array<uint8_t, 2>>  AlgMani::translateYX2(std::vector<std::array<uint8_t, 2>> alg){
    uint8_t pos = 7;
    std::array<uint8_t, 3> translateFace = {2,1,0};
    std::vector<std::array<uint8_t, 2>> translatedAlg;
    for(auto turn : alg){
        uint8_t newFace = translateFace[turn[0]];
        translatedAlg.push_back({newFace,turn[1]});
        std::array<uint8_t, 4> movingCorners;
        if (turn[0] == 0) {
            movingCorners = {7, 3, 1, 5};
        } else if (turn[0] == 1) {
            movingCorners = {7, 6, 2, 3};
        } else {
            movingCorners = {7, 5, 4, 6};
        }
        auto corner =  std::find(movingCorners.begin(), movingCorners.end(), pos);
        if(movingCorners.end() != corner){
            size_t index = std::distance(movingCorners.begin(), corner);
            pos = movingCorners[(index + 1 + turn[1])%4];
            if(turn[1]!=1){
                switch(turn[0]){
                    case 0:
                        translateFace = {translateFace[0],translateFace[2],translateFace[1]};
                        break;
                    case 1:
                        translateFace = {translateFace[2],translateFace[1],translateFace[0]};
                        break;
                    case 2:
                        translateFace = {translateFace[1],translateFace[0],translateFace[2]};
                        break;
                }
            }
        }
    }
    return translatedAlg;
}
std::vector<std::array<uint8_t, 2>>  AlgMani::reverse(std::vector<std::array<uint8_t, 2>> alg){
    std::vector<std::array<uint8_t, 2>> reversedAlg;
    for(int8_t i=alg.size()-1; i >= 0; i--){
        uint8_t newFace = alg[i][0];
        uint8_t newDir = std::abs(alg[i][1]-2);
        reversedAlg.push_back({newFace,newDir});
    }
    return reversedAlg;
}
std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> AlgMani::optimizeRUF(std::vector<std::array<uint8_t, 2>> alg, std::array<uint8_t, 6> turns){
    std::array<uint8_t, 3> swaps = {0,1,2};
    uint8_t max = 0;
    uint8_t max_index = 0;
    uint8_t min = turns[0];
    uint8_t min_index = 0;
    for(uint8_t i = 0; i < 3; ++i){
        if(turns[i]>max){
            max = turns[i];
            max_index = i;
        }else if(turns[i]<min){
            min = turns[i];
            min_index = i;
        }
    }
    if(max_index != min_index){
        swaps[0] = max_index;
        swaps[1] = 3 - max_index - min_index;
        swaps[2] = min_index;
    }
    uint8_t stay = 0;
    for(uint8_t i = 0; i<3; ++i){
        if(swaps[i] == i){
            stay += (i + 1);
        }
    }
    if(stay == 6 ){
        return std::make_pair(alg, turns);
    }else if(stay == 0){
        return std::make_pair(twist(alg, swaps[0] % 2),
        std::array<uint8_t, 6>{turns[swaps[0]],turns[swaps[1]],turns[swaps[2]],turns[swaps[0]+3],turns[swaps[1]+3],turns[swaps[2]+3]});
    }else{
        return std::make_pair(swap(alg, stay - 1),
        std::array<uint8_t, 6>{turns[swaps[0]],turns[swaps[1]],turns[swaps[2]],turns[swaps[0]+3],turns[swaps[1]+3],turns[swaps[2]+3]});
    }   
}

std::pair<std::vector<std::array<uint8_t, 2>>, std::array<uint8_t, 6>> AlgMani::optimizeRF(std::vector<std::array<uint8_t, 2>> alg, std::array<uint8_t, 6> turns){
    if(turns[0]<turns[2] || (turns[0]==turns[2] && (turns[3]<turns[5]))){
        return std::make_pair(swap(alg, 1),
        std::array<uint8_t, 6>{turns[2],turns[1],turns[0],turns[5],turns[4],turns[3]});
    }else{
        return std::make_pair(alg,turns);
    }   
}

std::array<uint8_t, 6> AlgMani::getRUF(std::vector<std::array<uint8_t, 2>> alg){
    std::array<uint8_t, 6> turns = {0,0,0,0,0,0};
    for(auto turn : alg){
        turns[turn[0]]++;
        turns[turn[0]+3]++;
        if(turn[1] == 1){
            turns[turn[0]+3]++;
        }
    }
    return turns;
}
std::vector<std::array<uint8_t, 2>>  AlgMani::swap(std::vector<std::array<uint8_t, 2>> alg, uint8_t stay){
    std::vector<std::array<uint8_t, 2>> swap_alg;
    uint8_t new_dir;
    uint8_t new_face;
    for(auto turn : alg){
        new_dir = 2-turn[1];
        if(turn[0] == stay){
           swap_alg.push_back({turn[0], new_dir});
        }else{
            new_face = 3 - stay - turn[0]; 
            swap_alg.push_back({new_face, new_dir});
        }
    }
    return swap_alg;
}
std::vector<std::array<uint8_t, 2>>  AlgMani::twist(std::vector<std::array<uint8_t, 2>> alg, bool dir){
    std::vector<std::array<uint8_t, 2>> twist_alg;
    uint8_t new_face;
    for(auto turn : alg){
        new_face = (turn[0] + 1 + dir) % 3;
        twist_alg.push_back({new_face, turn[1]});
    }
    return twist_alg;
}
