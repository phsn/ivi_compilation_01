#pragma once

#include "ofMain.h"

class iviObj : public ofNode {
    
public:
    iviObj();
    ~iviObj();
    
    void setup(ofVec3f setPos, ofVec3f setRot, ofVec3f setScale);
    void update(ofVec3f newPos, ofVec3f newRot, ofVec3f newScale);
    void draw();
    
    ofMatrix4x4 birthState;
    ofMatrix4x4 prevState;
    ofMatrix4x4 beforeState;
    
    bool blur;

};
