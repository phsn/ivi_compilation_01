#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

#include "../ivi_FFT.h"

class ivi_Intro : public ofBaseApp{

public:
		void setup();
		void update(ivi_FFT* FFTInstance);
        void draw();
        ofFbo* drawFBO(bool calc);
    
    bool introOut = false;
    
    float fade = 1.0;
    
    void logoIn();
    void logoOut();
    
    ofVideoPlayer introPlayer;
    
    vector<float> smoothAverages;
    
private:
    
    ofFbo mainFBO;
    ofFbo output;
    
    float getBandWidth();
    int freqToIndex(int freq);
    
    vector<float> getLogAverages(vector<float> samples);
    
};
