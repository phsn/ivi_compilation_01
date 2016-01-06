#pragma once

#include "ofMain.h"

class Spiropath {
    
public:
    Spiropath();
    ~Spiropath();
    
    void setup(int bigRadius, int smallRadius, int innrRadius);
    void update(int bigRadius, int smallRadius, int innrRadius);
    
    void setTone(int newTone);
    
    ofVec3f getPos(float percentage, float offset);
    ofVec3f getVel(float percentage, float pBefore, float offset, float pOffset);
    
    bool reverse = false;
    
private:
    
    int bigCircle = 300;
    float smallCircle = 1000;
    float innerRadius = 300;
    float scale = 1.0;
    
    
    float ratio = 5;
};
