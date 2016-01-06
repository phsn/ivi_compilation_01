#pragma once

#include "ofMain.h"
#include "Spiropath.h"


class DropBrush {
    
public:
    DropBrush();
    
    DropBrush(int histSteps, int sFrames, float mSize);
    DropBrush(int histSteps, int sFrames, float mSize, float duration);
    ~DropBrush();
    
    void setup();
    void update(ofVec3f newPos);
    void draw();
    void startPos(ofVec3f startP);

    void setReverse(bool n_reverse);
    void setOffset(float offset);
    float getOffset();
    
    void kill();
    
    float interpol(float progress, float startVal, float endVal);
    
    bool beKilled = false;
    
    int tone;
    Spiropath dropPath;
    float speed = 0.5;
    ofColor startColor = ofColor(255,255,255);
    ofColor endColor   = ofColor(100,30,90);
    
private:
    
    ofMesh brushMesh;
    
    bool ready = false;
    
    float birthTime;
    float deathTime;
    float lifeDur;
    
    float minTime = 0.1;
    float fadeTime = 0.5;
    
    int historySteps = 15;
    int subFrames = 8;
    
    float maxSize = 15.0;
    
    float privateOffset;
    
    ofVec3f position;
    vector<ofVec3f> history;
    
    ofColor curColor   = ofColor(0,0,0);
    
    float easeOut_Quart(float t,float b,float c,float d);
    
};
