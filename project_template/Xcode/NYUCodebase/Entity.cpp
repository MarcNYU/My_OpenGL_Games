//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Marcus Williams on 2/10/16.
//  Copyright © 2016 SoundVoid. All rights reserved.
//

#include "Entity.hpp"
class Entity {
public:
    void Draw();
    float x;
    float y;
    float rotation;
    int textureID;
    float width;
    float height;
    float speed;
    float direction_x;
    float direction_y;
};