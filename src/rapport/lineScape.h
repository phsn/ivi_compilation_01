//
//  lineScape.h
//  linescape_01
//
//  Created by Philipp Sniechota on 20.09.15.
//
//

#ifndef __linescape_01__lineScape__
#define __linescape_01__lineScape__

#include <stdio.h>
#include "ofMain.h"
#include "line.h"


class lineScape {
    
    vector<line> lineList;
    vector<ofPoint> pointList;
    
    vector <float> myFFT;

public:


    int numPoints = 150;
    
    int numObj;
    float speed;
    float spacing;
    int maximumZ;
    float bendFact = 0.0;
    
    int skipLoops = 0;
    
    bool noDRAW = false;
    bool updateAllLines = false;
    
    lineScape();
    lineScape(int numLines, int maxZ);
    void update();
    void draw();
    
    void updateLineCount(int numLines);
    void updateAll();
    
    void updateMaxZ(int dist);
    void updateBend(float bFact);

    void downresAll(float dFact);
    
    void updateMixer(float fftStrength,float noiseStrength);

    float getSpacing();
    
    void setFFTData(vector<float> FFTData);
    
private:
    double CosineInterpolate (double y1,double y2,double mu);
};

#endif /* defined(__linescape_01__lineScape__) */
