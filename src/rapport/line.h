//
//  line.h
//  linescape_01
//
//  Created by Philipp Sniechota on 20.09.15.
//
//

#ifndef __linescape_01__line__
#define __linescape_01__line__

#include <stdio.h>
#include "ofMain.h"


class line {
    
    ofMesh lineMesh;
    vector<ofPoint> initPos;
    vector<ofPoint> drawPos;
    int segCount;
    
    ofPoint translation;
    float opac;
    
    float bendFact;
    float downresFact;
    
    double waveVal;
    vector<ofPoint> waveV;
    float waveAmp;
    float waveFreq;
    float waveSpeed;
    float waveOffset;
    
    double noiseVal;
    vector<ofPoint> noiseV;
    
    vector<ofPoint> pointOffsets;
    
    float zLimit;
    
    vector<float> myFFT;
    
public:
    line();
    line(int segments);
    void update();
    void draw();
    
    
    void setPositions(vector<ofPoint> points);
    void setPointOffsets(vector<ofPoint> offsets);
    void setPointOffset(int idx, ofVec3f offset);
    void easePointOffset(int idx, ofVec3f offset, float easeAmount);
    vector<ofPoint> getPointOffsets();
    ofVec3f getPointNormal(int idx);


    void setTranslation(ofPoint newPosition);
    void bend(float bendFactor);
    void downres(float downresFactor);
    void move(ofPoint movingVector, float limitZ);
    void setWave(float amp, float freq, float speed);
    void setWave(float amp, float freq, float speed, float offset);
    void setOpacity(float opacity);

    vector<ofPoint> getInitPos();
    vector<ofPoint> getDrawPos();
    int getSegmentCount();
    ofPoint getTranslation();
    float getProgress();
    
    float fftAmt = 1.0;
    float noiseAmt = 0.0;
    
    int length;
    
};


#endif /* defined(__linescape_01__line__) */
