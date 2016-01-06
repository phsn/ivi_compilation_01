#pragma once

#include "ofMain.h"
#include "DropBrush.h"
#include "Spiropath.h"

class DropEmitter {
    
public:
    DropEmitter();
    ~DropEmitter();
    
    void setup(int bigRadius, int smallRadius, int innrRadius);
    void update();
    void draw();
    
    static bool killBrush(DropBrush &brsh);

    void emit(int numDrops);
    void kill();
    void delete_tone(int dTone);
    
    int curTone;
    
    int numDrops = 1;
    float speed = 0.6;

private:
    
    Spiropath Path;
    vector<DropBrush> brushes;
    
    ofShader brushShader;
    ofColor toneToColor(int tone);
        
};