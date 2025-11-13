#include "Puppet.hpp"
Puppet::Puppet(){}

void Puppet::cubeShape(Shape& shape) {
    for(uint8_t x = 0; x <5; ++x){
        for(uint8_t y = 0; y < 5; ++y){
            for(uint8_t z = 0; z < 5; ++z){
                shape.data[x][y][z] = 0;
                if(x>2&&y>0&&z>0){
                    shape.data[x][y][z] +=1;
                }
                if(y>2&&x>0&&z>0){
                    shape.data[x][y][z] +=2;
                }
                if(z>2&&x>0&&y>0){
                    shape.data[x][y][z] +=4;
                }
            }
        }
    }
}

Puppet::ShapeBin Puppet::getBinShape(const Shape& shape){
    ShapeBin shapebin;
    for(uint8_t x = 0; x <5; ++x){
        for(uint8_t y = 0; y < 5; ++y){
            for(uint8_t z = 0; z < 5; ++z){
                shapebin.data[x][y][z] = shape.data[x][y][z] > 0;
            }
        }
    }
    return shapebin;
}
void Puppet::rotateShapeBin(ShapeBin& shape, bool dir){
    uint8_t curShape[5][5][5];
    std::memcpy(curShape, shape.data, sizeof(shape.data));
    std::array<uint8_t,3> K;
    for(K[0] = 0; K[0] < 5; ++K[0]){
        for(K[1] = 0; K[1] < 5; ++K[1]){
            for(K[2] = 0; K[2] < 5; ++K[2]){
                shape.data[K[0]][K[1]][K[2]] = curShape[K[1+dir]][K[(2+dir)%3]][K[dir]];
            }
        }
    }
}
void Puppet::mirrorShapeBin(ShapeBin& shape, uint8_t stay){
    uint8_t curShape[5][5][5];
    std::memcpy(curShape, shape.data, sizeof(shape.data));
    std::array<uint8_t,3> K;
    for(K[0] = 0; K[0] < 5; ++K[0]){
        for(K[1] = 0; K[1] < 5; ++K[1]){
            for(K[2] = 0; K[2] < 5; ++K[2]){
                shape.data[K[0]][K[1]][K[2]] = curShape[K[(3-stay)%3]][K[2-stay]][K[stay == 2 ? 2 : 1-stay]];
            }
        }
    }
}
bool Puppet::doTurn(Shape& shape, uint8_t face, uint8_t dir){
    uint8_t curShape[5][5][5];
    std::memcpy(curShape, shape.data, sizeof(shape.data));
    std::array<uint8_t, 4> movingCorners;
    if (face == 0) {
        movingCorners = {7, 3, 1, 5};
    } else if (face == 1) {
        movingCorners = {7, 6, 2, 3};
    } else {
        movingCorners = {7, 5, 4, 6};
    }
    std::array<uint8_t,3> K;
    for(K[0] = 0; K[0] < 5; ++K[0]){
        for(K[1] = 0; K[1] < 5; ++K[1]){
            for(K[2] = 0; K[2] < 5; ++K[2]){
                if(((0 < K[0] && K[0] < 4) && (0 < K[1] && K[1] < 4) && (0 < K[2] && K[2] < 4)) || 
                    (K[0] < 2 && K[1] < 2 && K[2] < 2)) {
                    continue;
                }
                auto corner = std::find(movingCorners.begin(), movingCorners.end(), curShape[K[0]][K[1]][K[2]]);
                if(corner != movingCorners.end()){
                    size_t index = std::distance(movingCorners.begin(), corner);
                    std::array<uint8_t, 3> new_K;
                    switch(face){
                        case 0:
                            new_K = {
                                K[0],
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 1 : 2] - 4*(dir > 0))),
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 2 : 1] - 4*(dir < 2)))
                            };
                            break;
                        case 1:
                            new_K = {
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 0 : 2] - 4*(dir < 2))),
                                K[1],
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 2 : 0] - 4*(dir > 0)))
                            };
                            break;
                        case 2:
                            new_K = {
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 0 : 1] - 4*(dir > 0))),
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 1 : 0] - 4*(dir < 2))),
                                K[2]
                            };
                            break;  
                    };
                    if(curShape[new_K[0]][new_K[1]][new_K[2]]!=0 && movingCorners.end() == std::find(movingCorners.begin(), movingCorners.end(), curShape[new_K[0]][new_K[1]][new_K[2]])){
                        std::memcpy(shape.data, curShape, sizeof(curShape));
                        return false;
                    }
                    shape.data[new_K[0]][new_K[1]][new_K[2]] += movingCorners[(index + 1 + dir) % 4];
                    shape.data[K[0]][K[1]][K[2]] -= movingCorners[index];
                }
            }
        }
    }
    return true;
}

bool Puppet::testTurn(Shape& shape, uint8_t face, uint8_t dir){
    if(dir == 1){
        if(!testTurn(shape, face, 0) && !testTurn(shape, face, 2)){
            return false;
        }
    }
    std::array<uint8_t, 4> movingCorners;
    if (face == 0) {
        movingCorners = {7, 3, 1, 5};
    } else if (face == 1) {
        movingCorners = {7, 6, 2, 3};
    } else {
        movingCorners = {7, 5, 4, 6};
    }
    std::array<uint8_t,3> K;
    for(K[0] = 0; K[0] < 5; ++K[0]){
        for(K[1] = 0; K[1] < 5; ++K[1]){
            for(K[2] = 0; K[2] < 5; ++K[2]){
                if(((0 < K[0] && K[0] < 4) && (0 < K[1] && K[1] < 4) && (0 < K[2] && K[2] < 4)) || 
                    (K[0] < 2 && K[1] < 2 && K[2] < 2)) {
                    continue;
                }
                auto corner = std::find(movingCorners.begin(), movingCorners.end(), shape.data[K[0]][K[1]][K[2]]);
                if(corner != movingCorners.end()){
                    size_t index = std::distance(movingCorners.begin(), corner);
                    std::array<uint8_t, 3> new_K;
                    switch(face){
                        case 0:
                            new_K = {
                                K[0],
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 1 : 2] - 4*(dir > 0))),
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 2 : 1] - 4*(dir < 2)))
                            };
                            break;
                        case 1:
                            new_K = {
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 0 : 2] - 4*(dir < 2))),
                                K[1],
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 2 : 0] - 4*(dir > 0)))
                            };
                            break;
                        case 2:
                            new_K = {
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 0 : 1] - 4*(dir > 0))),
                                static_cast<uint8_t>(std::abs(K[dir == 1 ? 1 : 0] - 4*(dir < 2))),
                                K[2]
                            };
                            break;  
                    };
                    if(shape.data[new_K[0]][new_K[1]][new_K[2]]!=0 && movingCorners.end() == std::find(movingCorners.begin(), movingCorners.end(), shape.data[new_K[0]][new_K[1]][new_K[2]])){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Puppet::doAlgExpectPossible(Shape& shape, std::vector<std::array<uint8_t, 2>> alg){
    for(auto turn : alg){
        doTurn(shape, turn[0], turn[1]);
    }
}
bool Puppet::testAlg(Shape& shape, std::vector<std::array<uint8_t, 2>> alg){
    for(auto turn : alg){
        if(!doTurn(shape, turn[0], turn[1])){
            return false;
        }
    }
    return true;
}

bool Puppet::testCubeShape(const Shape& shape){
    for(uint8_t x = 1; x <5; ++x){
        for(uint8_t y = 1; y < 5; ++y){
            for(uint8_t z = 1; z < 5; ++z){
                if((x==4||y==4||z==4) && shape.data[x][y][z] == 0){
                    return false;
                }
            }
        }
    }
    return true;
}

bool Puppet::testYShape(const Shape& shape){
    bool ret = 1;
    for(uint8_t x = 1; x <5; ++x){
        for(uint8_t y = 1; y < 5; ++y){
            for(uint8_t z = 1; z < 5; ++z){
                if(((x==4&&y==4)||(y==4&&z==4)||(x==4&&z==4)) && shape.data[x][y][z] == 0){
                    ret=0;
                }
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    for(uint8_t x = 1; x <5; ++x){
        for(uint8_t y = 1; y < 5; ++y){
            for(uint8_t z = 0; z < 4; ++z){
                if(((x==4&&y==4)||(x==4&&z==0)||(y==4&&z==0)) && shape.data[x][y][z] == 0){
                    ret=0;
                }
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    for(uint8_t x = 0; x <4; ++x){
        for(uint8_t y = 1; y < 5; ++y){
            for(uint8_t z = 1; z < 5; ++z){
                if(((x==0&&y==4)||(y==4&&z==4)||(x==0&&z==4)) && shape.data[x][y][z] == 0){
                    ret=0;
                }
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    for(uint8_t x = 1; x <5; ++x){
        for(uint8_t y = 0; y < 4; ++y){
            for(uint8_t z = 1; z < 5; ++z){
                if(((x==4&&y==0)||(y==0&&z==4)||(x==4&&z==4)) && shape.data[x][y][z] == 0){
                    ret=0;
                }
            }
        }
    }
    return ret;
}

bool Puppet::testVShape(const Shape& shape){
    bool ret = 1;
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 1; x <5; ++x){
        for(z = 1; z < 5; ++z){
            if((x==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    x = 4;
    for(y = 1; y < 5; ++y){
        for(z = 1; z < 5; ++z){
            if((y==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    z = 4;
    for(x = 1; x <5; ++x){
        for(y = 1; y < 5; ++y){
                if((y==4||x==4) && shape.data[x][y][z] == 0){
                    ret=0;
                }
        }
    }
    return ret;
}

bool Puppet::testU2VShape(const Shape& shape){
    bool ret = 1;
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 0; z < 4; ++z){
            if((x==0||z==0) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    x = 4;
    for(y = 0; y < 4; ++y){
        for(z = 0; z < 4; ++z){
            if((y==0||z==0) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    z = 4;
    for(x = 0; x < 4; ++x){
        for(y = 0; y < 4; ++y){
                if((y==0||x==0) && shape.data[x][y][z] == 0){
                    ret=0;
                }
        }
    }
    return ret;
}

bool Puppet::testBShape(const Shape& shape){
    bool ret = 1;
    uint8_t y;
    uint8_t x = 0;
    uint8_t z;
    for(y = 1; y < 5; ++y){
        for(z = 1; z < 5; ++z){
            if((y==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    y = 0;
    for(x = 1; x < 5; ++x){
        for(z = 1; z < 5; ++z){
            if((x==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    z = 0;
    for(y = 1; y < 5; ++y){
        for(x = 1; x < 5; ++x){
            if((y==4||x==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    return ret;
}

bool Puppet::testLShape(const Shape& shape){
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 1; z < 5; ++z){
            if((x>=2||z>=3) && shape.data[x][y][z] == 0){
                goto s2;
            }
            if((x<2&&z<3) && shape.data[x][y][z] != 0){
                goto s2;
            }
        }
    }
    return true;
    s2:
    for(z = 0; z < 4; ++z){
        for(x = 1; x < 5; ++x){
            if((x>=3||z>=2) && shape.data[x][y][z] == 0){
                goto s3;
            }
            if((z<2&&x<3) && shape.data[x][y][z] != 0){
                goto s3;
            }
        }
    }
    return true;
    s3:
    x = 4;
    for(y = 0; y < 4; ++y){
        for(z = 1; z < 5; ++z){
            if((y>=2||z>=3) && shape.data[x][y][z] == 0){
                goto s4;
            }
            if((y<2&&z<3) && shape.data[x][y][z] != 0){
                goto s4;
            }
        }
    }
    return true;
    s4:
    for(z = 0; z < 4; ++z){
        for(y = 1; y < 5; ++y){
            if((y>=3||z>=2) && shape.data[x][y][z] == 0){
                goto s5;
            }
            if((z<2&&y<3) && shape.data[x][y][z] != 0){
                goto s5;
            }
        }
    }
    return true;
    s5:
    z = 4;
    for(y = 0; y < 4; ++y){
        for(x = 1; x < 5; ++x){
            if((y>=2||x>=3) && shape.data[x][y][z] == 0){
                goto s6;
            }
            if((y<2&&x<3) && shape.data[x][y][z] != 0){
                goto s6;
            }
        }
    }
    return true;
    s6:
    for(x = 0; x < 4; ++x){
        for(y = 1; y < 5; ++y){
            if((y>=3||x>=2) && shape.data[x][y][z] == 0){
                goto end;
            }
            if((x<2&&y<3) && shape.data[x][y][z] != 0){
                goto end;
            }
        }
    }
    return true;
    end:
    return false;
}

bool Puppet::testU2LShape(const Shape& shape){
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 1; z < 5; ++z){
            if((x>=2&&z>=3) && shape.data[x][y][z] != 0){
                goto s2;
            }
            if((x<2||z<3) && shape.data[x][y][z] == 0){
                goto s2;
            }
        }
    }
    return true;
    s2:
    for(z = 0; z < 4; ++z){
        for(x = 1; x < 5; ++x){
            if((x>=3&&z>=2) && shape.data[x][y][z] != 0){
                goto s3;
            }
            if((z<2||x<3) && shape.data[x][y][z] == 0){
                goto s3;
            }
        }
    }
    return true;
    s3:
    x = 4;
    for(y = 0; y < 4; ++y){
        for(z = 1; z < 5; ++z){
            if((y>=2&&z>=3) && shape.data[x][y][z] != 0){
                goto s4;
            }
            if((y<2||z<3) && shape.data[x][y][z] == 0){
                goto s4;
            }
        }
    }
    return true;
    s4:
    for(z = 0; z < 4; ++z){
        for(y = 1; y < 5; ++y){
            if((y>=3&&z>=2) && shape.data[x][y][z] != 0){
                goto s5;
            }
            if((z<2||y<3) && shape.data[x][y][z] == 0){
                goto s5;
            }
        }
    }
    return true;
    s5:
    z = 4;
    for(y = 0; y < 4; ++y){
        for(x = 1; x < 5; ++x){
            if((y>=2&&x>=3) && shape.data[x][y][z] != 0){
                goto s6;
            }
            if((y<2||x<3) && shape.data[x][y][z] == 0){
                goto s6;
            }
        }
    }
    return true;
    s6:
    for(x = 0; x < 4; ++x){
        for(y = 1; y < 5; ++y){
            if((y>=3&&x>=2) && shape.data[x][y][z] != 0){
                goto end;
            }
            if((x<2||y<3) && shape.data[x][y][z] == 0){
                goto end;
            }
        }
    }
    return true;
    end:
    return false;
}

bool Puppet::testLBShape(const Shape& shape){
    uint8_t y = 0;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 1; z < 5; ++z){
            if((x>=2||z>=3) && shape.data[x][y][z] == 0){
                goto s2;
            }
            if((x<2&&z<3) && shape.data[x][y][z] != 0){
                goto s2;
            }
        }
    }
    return true;
    s2:
    for(z = 0; z < 4; ++z){
        for(x = 1; x < 5; ++x){
            if((x>=3||z>=2) && shape.data[x][y][z] == 0){
                goto s3;
            }
            if((z<2&&x<3) && shape.data[x][y][z] != 0){
                goto s3;
            }
        }
    }
    return true;
    s3:
    x = 0;
    for(y = 0; y < 4; ++y){
        for(z = 1; z < 5; ++z){
            if((y>=2||z>=3) && shape.data[x][y][z] == 0){
                goto s4;
            }
            if((y<2&&z<3) && shape.data[x][y][z] != 0){
                goto s4;
            }
        }
    }
    return true;
    s4:
    for(z = 0; z < 4; ++z){
        for(y = 1; y < 5; ++y){
            if((y>=3||z>=2) && shape.data[x][y][z] == 0){
                goto s5;
            }
            if((z<2&&y<3) && shape.data[x][y][z] != 0){
                goto s5;
            }
        }
    }
    return true;
    s5:
    z = 0;
    for(y = 0; y < 4; ++y){
        for(x = 1; x < 5; ++x){
            if((y>=2||x>=3) && shape.data[x][y][z] == 0){
                goto s6;
            }
            if((y<2&&x<3) && shape.data[x][y][z] != 0){
                goto s6;
            }
        }
    }
    return true;
    s6:
    for(x = 0; x < 4; ++x){
        for(y = 1; y < 5; ++y){
            if((y>=3||x>=2) && shape.data[x][y][z] == 0){
                goto end;
            }
            if((x<2&&y<3) && shape.data[x][y][z] != 0){
                goto end;
            }
        }
    }
    return true;
    end:
    return false;
}

int Puppet::getVShape(const ShapeBin& shape){
    bool ret = 1;
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 1; x <5; ++x){
        for(z = 1; z < 5; ++z){
            if((x==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret){return 0;}
    ret = 1;
    x = 4;
    for(y = 1; y < 5; ++y){
        for(z = 1; z < 5; ++z){
            if((y==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret){return 1;}
    ret = 1;
    z = 4;
    for(x = 1; x <5; ++x){
        for(y = 1; y < 5; ++y){
                if((y==4||x==4) && shape.data[x][y][z] == 0){
                    ret=0;
                }
        }
    }
    if(ret){return 2;}
    return -1;
}

int Puppet::getBShape(const ShapeBin& shape){
    bool ret = 1;
    uint8_t y;
    uint8_t x = 0;
    uint8_t z;
    for(y = 1; y < 5; ++y){
        for(z = 1; z < 5; ++z){
            if((y==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret){return 1;}
    ret = 1;
    y = 0;
    for(x = 1; x < 5; ++x){
        for(z = 1; z < 5; ++z){
            if((x==4||z==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret){return 0;}
    ret = 1;
    z = 0;
    for(y = 1; y < 5; ++y){
        for(x = 1; x < 5; ++x){
            if((y==4||x==4) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret){return 2;}
    return -1;
}

int Puppet::getU2VShape(const ShapeBin& shape){
    bool ret = 1;
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 0; z < 4; ++z){
            if((x==0||z==0) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return 0;}
    ret = 1;
    x = 4;
    for(y = 0; y < 4; ++y){
        for(z = 0; z < 4; ++z){
            if((y==0||z==0) && shape.data[x][y][z] == 0){
                ret=0;
            }
        }
    }
    if(ret == 1){return 1;}
    ret = 1;
    z = 4;
    for(x = 0; x < 4; ++x){
        for(y = 0; y < 4; ++y){
                if((y==0||x==0) && shape.data[x][y][z] == 0){
                    ret=0;
                }
        }
    }
    if(ret == 1){return 2;}
    return -1;
}

int Puppet::getU2LShape(const ShapeBin& shape){
    uint8_t y = 4;
    uint8_t x;
    uint8_t z;
    for(x = 0; x < 4; ++x){
        for(z = 1; z < 5; ++z){
            if((x>=2&&z>=3) && shape.data[x][y][z] != 0){
                goto s2;
            }
            if((x<2||z<3) && shape.data[x][y][z] == 0){
                goto s2;
            }
        }
    }
    return 0;
    s2:
    for(z = 0; z < 4; ++z){
        for(x = 1; x < 5; ++x){
            if((x>=3&&z>=2) && shape.data[x][y][z] != 0){
                goto s3;
            }
            if((z<2||x<3) && shape.data[x][y][z] == 0){
                goto s3;
            }
        }
    }
    return 0;
    s3:
    x = 4;
    for(y = 0; y < 4; ++y){
        for(z = 1; z < 5; ++z){
            if((y>=2&&z>=3) && shape.data[x][y][z] != 0){
                goto s4;
            }
            if((y<2||z<3) && shape.data[x][y][z] == 0){
                goto s4;
            }
        }
    }
    return 1;
    s4:
    for(z = 0; z < 4; ++z){
        for(y = 1; y < 5; ++y){
            if((y>=3&&z>=2) && shape.data[x][y][z] != 0){
                goto s5;
            }
            if((z<2||y<3) && shape.data[x][y][z] == 0){
                goto s5;
            }
        }
    }
    return 1;
    s5:
    z = 4;
    for(y = 0; y < 4; ++y){
        for(x = 1; x < 5; ++x){
            if((y>=2&&x>=3) && shape.data[x][y][z] != 0){
                goto s6;
            }
            if((y<2||x<3) && shape.data[x][y][z] == 0){
                goto s6;
            }
        }
    }
    return 2;
    s6:
    for(x = 0; x < 4; ++x){
        for(y = 1; y < 5; ++y){
            if((y>=3&&x>=2) && shape.data[x][y][z] != 0){
                goto end;
            }
            if((x<2||y<3) && shape.data[x][y][z] == 0){
                goto end;
            }
        }
    }
    return 2;
    end:
    return -1;
}

bool Puppet::testLayer(const Shape& shape){
    bool ret = 1;
    for(uint8_t y = 1; y < 5; ++y){
        for(uint8_t z = 1; z < 5; ++z){
            if(shape.data[4][y][z] == 0){
                ret = 0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    for(uint8_t x = 1; x < 5; ++x){
        for(uint8_t z = 1; z < 5; ++z){
            if(shape.data[x][4][z] == 0){
                ret = 0;
            }
        }
    }
    if(ret == 1){return true;}
    ret = 1;
    for(uint8_t x = 1; x < 5; ++x){
        for(uint8_t y = 1; y < 5; ++y){
            if(shape.data[x][y][4] == 0){
                ret = 0;
            }
        }
    }
    return ret;
}

bool Puppet::testSameShapeBin(ShapeBin& shape_1,ShapeBin& shape_2){
    std::vector<Puppet::ShapeBin> found_shapes;
    ShapeBin temp = shape_1;
    found_shapes.push_back(temp);
    rotateShapeBin(temp, 0);
    found_shapes.push_back(temp);
    rotateShapeBin(temp, 0);
    found_shapes.push_back(temp);
    mirrorShapeBin(temp, 0);
    found_shapes.push_back(temp);
    rotateShapeBin(temp, 0);
    found_shapes.push_back(temp);
    rotateShapeBin(temp, 0);
    found_shapes.push_back(temp);
    for(const auto& s : found_shapes){
        if(std::memcmp(s.data, shape_2.data, sizeof(Shape::data)) == 0){return true;}
    }
    return false;
}

void Puppet::solvedCorners(Corners& corners){
    for(uint8_t i = 0; i< 7; ++i){
        corners.data[i]=i;
    }
}
void Puppet::rotateCornersCS(Corners& corners, bool dir){
    std::array<uint8_t, 3> swaps = {corners.data[0],corners.data[1],corners.data[2]};
    uint8_t stay = 0;
    for(uint8_t i = 0; i<3; ++i){
        if(swaps[i] == i){
            stay += (i + 1);
        }
    }
    if(stay != 6 && stay != 0 ){
        corners.data[(stay + dir) % 3] =  (stay + dir) % 3;
        corners.data[(stay + dir +1 ) % 3] =  (stay + dir - 1) % 3;
        corners.data[(stay + dir - 1) % 3] =  (stay + dir + 1) % 3;
    }
    swaps = {corners.data[3],corners.data[4],corners.data[5]};
    stay = 0;
    for(uint8_t i = 0; i<3; ++i){
        if(swaps[i] == (i + 3)){
            stay += (i + 1);
        }
    }
    if(stay != 6 && stay != 0 ){
        corners.data[(stay + dir) % 3 + 3] =  (stay + dir) % 3 + 3;
        corners.data[(stay + dir +1 ) % 3 + 3] =  (stay + dir - 1) % 3 + 3;
        corners.data[(stay + dir - 1) % 3 + 3] =  (stay + dir + 1) % 3 + 3;
    }
}
void Puppet::mirrorCornersCS(Corners& corners){
    std::array<uint8_t, 3> swaps = {corners.data[0],corners.data[1],corners.data[2]};
    uint8_t stay = 0;
    for(uint8_t i = 0; i<3; ++i){
        if(swaps[i] == i){
            stay += (i + 1);
        }
    }
    if(stay == 0){
        corners.data[0] = 3 - corners.data[0];  
        corners.data[1] = 2 - corners.data[1];  
        corners.data[2] = 1 - corners.data[2];  
    }
    swaps = {corners.data[3],corners.data[4],corners.data[5]};
    stay = 0;
    for(uint8_t i = 0; i<3; ++i){
        if(swaps[i] == (i + 3)){
            stay += (i + 1);
        }
    }
    if(stay == 0){
        corners.data[3] = 9 - corners.data[3];  
        corners.data[4] = 8 - corners.data[4];  
        corners.data[5] = 7 - corners.data[5];  
    }
}
void Puppet::doTurnCorners(Corners& corners, uint8_t face, uint8_t dir){
    uint8_t buf[7];
    for(uint8_t i = 0; i< 7; ++i){
        buf[i] = corners.data[i];
    }
    std::array<uint8_t, 4> movingCorners;
    if (face == 0) {
        movingCorners = {6,5,0,3};
    } else if (face == 1) {
        movingCorners = {6,3,1,4};
    } else {
        movingCorners = {6,4,2,5};
    }
    for(uint8_t i = 0; i< 7; ++i){
        auto corner = std::find(movingCorners.begin(), movingCorners.end(), i);
        if(corner != movingCorners.end()){
            size_t index = std::distance(movingCorners.begin(), corner);
            corners.data[i]=buf[movingCorners[(index+1+dir)%4]];
        }
    }
}
bool Puppet::testCornersSolved(const Corners& corners){
    for(uint8_t i = 0; i< 7; ++i){
        if(corners.data[i]!=i){
            return false;
        };
    }
    return true;
}

void Puppet::solvedEdges(Edges& edges){
    for(uint8_t i = 0; i< 9; ++i){
        edges.data[i]=i;
    }
}

void Puppet::doTurnEdges(Edges& edges, uint8_t face, uint8_t dir){
    uint8_t buf[9];
    for(uint8_t i = 0; i< 9; ++i){
        buf[i] = edges.data[i];
    }
    std::array<uint8_t, 4> movingEdges;
    if (face == 0) {
        movingEdges = {0,2,8,3};
    } else if (face == 1) {
        movingEdges = {1,0,6,4};
    } else {
        movingEdges = {2,1,7,5};
    }
    for(uint8_t i = 0; i< 9; ++i){
        auto edge = std::find(movingEdges.begin(), movingEdges.end(), i);
        if(edge != movingEdges.end()){
            size_t index = std::distance(movingEdges.begin(), edge);
            edges.data[i]=buf[movingEdges[(index+1+dir)%4]];
        }
    }
}

uint8_t Puppet::countEdgesSolved(const Edges& edges){
    uint8_t count = 0;
    for(uint8_t i = 0; i< 9; ++i){
        if(i == edges.data[i]){
            count++;
        }
    }
    return count;
}

void Puppet::printEdges(const Edges& edges){
    for(uint8_t i = 0; i< 9; ++i){
        std::cout << int(edges.data[i]);
    }
    std::cout << std::endl;
}

void Puppet::solvedFlips(Flips& flips){
    for(uint8_t i = 0; i< 9; ++i){
        flips.data[i]=1;
    }
}
void Puppet::doTurnFlips(Flips& flips, uint8_t face, uint8_t dir){
    bool buf[9];
    for(uint8_t i = 0; i< 9; ++i){
        buf[i] = flips.data[i];
    }
    std::array<uint8_t, 4> movingEdges;
    if (face == 0) {
        movingEdges = {0,2,8,3};
    } else if (face == 1) {
        movingEdges = {1,0,6,4};
    } else {
        movingEdges = {2,1,7,5};
    }
    for(uint8_t i = 0; i< 9; ++i){
        auto edge = std::find(movingEdges.begin(), movingEdges.end(), i);
        if(edge != movingEdges.end()){
            size_t index = std::distance(movingEdges.begin(), edge);
            flips.data[i]=buf[movingEdges[(index+1+dir)%4]] ^ (face == 2 && dir != 1);
        }
    }
}
uint8_t Puppet::countFlips(const Flips& flips){
    uint8_t count = 0;
    for(uint8_t i = 0; i< 9; ++i){
        if(!flips.data[i]){
            count++;
        }
    }
    return count;
}

void Puppet::printFlips(const Flips& flips){
    for(uint8_t i = 0; i< 9; ++i){
        std::cout << int(flips.data[i]);
    }
    std::cout << std::endl;
}

std::vector<uint8_t> Puppet::getEdgeCycle(const Edges& edges){
    std::vector<uint8_t> cycledEdges;
    std::vector<uint8_t> cycledEdges2;
    for(uint8_t i = 0; i< 9; ++i){
        if(i != edges.data[i]){
            cycledEdges.push_back(i);
        }
    }
    if(cycledEdges.size() == 0){
        return {0,0,0};
    }else if(cycledEdges.size() == 2){
        return {2,2,2};
    }
    int8_t offset=cycledEdges.at(0)%3;
    uint8_t m;
    for(auto i : cycledEdges){
        m = i%3; 
        cycledEdges2.push_back(i-m+(m-offset+3)%3);
    }
    cycledEdges.clear();
    if(cycledEdges2.at(1) >cycledEdges2.at(2)){
        m = cycledEdges2.at(1);
        offset = cycledEdges2.at(2);
        cycledEdges2.pop_back();
        cycledEdges2.pop_back();
        cycledEdges2.push_back(offset);
        cycledEdges2.push_back(m);
    }
    if(cycledEdges2.at(0) == 0){
        if(cycledEdges2.at(1) == 1){
            switch (cycledEdges2.at(2)){
                case 7: return {0,1,3}; 
                case 6: return {0,1,4}; 
                case 8: return {0,1,5}; 
            }
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 2){
            switch (cycledEdges2.at(2)){
                case 7: return {0,1,5}; 
                case 6: return {0,1,3}; 
                case 8: return {0,1,4}; 
                case 3: return {0,1,4}; 
                case 4: return {0,1,5}; 
            }
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 3){
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 4){
            switch (cycledEdges2.at(2)){
                case 6: return {0,3,8}; 
            }
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 5){
            switch (cycledEdges2.at(2)){
                case 6: return {0,3,7}; 
                case 8: return {0,4,7}; 
            }
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 6){
            switch (cycledEdges2.at(2)){
                case 7: return {0,3,5}; 
                case 8: return {0,3,4}; 
            }
            return cycledEdges2;
        }else{
            return {0,4,5};
        }
    }else if(cycledEdges2.at(0) == 3){
        if(cycledEdges2.at(1) == 4){
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 5){
            switch (cycledEdges2.at(2)){
                case 6: return {3,4,7}; 
                case 7: return {3,4,8}; 
                case 8: return {3,4,6}; 
            }
            return cycledEdges2;
        }else if(cycledEdges2.at(1) == 6){
            switch (cycledEdges2.at(2)){
                case 7: return {3,4,7}; 
                case 8: return {3,4,6}; 
            }
            return cycledEdges2;
        }else{
            return {3,4,8};
        }
    }else{
        return {3,4,5};
    }
}

std::vector<uint8_t> Puppet::getFlips(const Flips& flips){
    std::vector<uint8_t> flippedEdges;
    for(uint8_t i = 0; i< 9; ++i){
        if(!flips.data[i]){
            flippedEdges.push_back(i);
        }
    }
    return flippedEdges;
}

std::vector<uint8_t> Puppet::getEdgesSolved(const Edges& edges){
    std::vector<uint8_t> edgesSolved;
    for(uint8_t i = 0; i< 9; ++i){
        if(edges.data[i] == i){
            edgesSolved.push_back(i);
        }
    }
    return edgesSolved;
}

void Puppet::printCycle(const std::vector<uint8_t> cycle){
    if(cycle.size() == 0){
        std::cout << "0 ";
    }else{
        for(auto i : cycle){
            std::cout << int(i);
        }
        std::cout << " ";
    }   
}
void Puppet::printFlippedEdges(const std::vector<uint8_t>flippedEdges){
    if(flippedEdges.size() == 0){
        std::cout << "0 ";
    }else{
        for(auto i : flippedEdges){
            std::cout << int(i);
        }
        std::cout << " ";
    }
}