#pragma once

#include "ofMain.h"
#include "lineScape.h"
#include "ofxFFTBase.h"
#include "ofxMidi.h"


#include "../ivi_FFT.h"
#include "../ivi_MIDI.h"


class ivi_Rapport : public ofBaseApp, public ofxMidiListener {
    
    ofEasyCam myCamera;
    
    int scapeLines = 10;
    float downFact = 0.0;
    int distance = 60000;
    float bendFact = 0;
    float fftAmt = 1.0;
    float noiseAmt = 0.0;
    
    float camDist = 0;
    float camOff = 0;
    float camPitch = 0;
    
    int distance_T = 60000;
    float bendFact_T = 0;
    float fftAmt_T = 1.0;
    float noiseAmt_T = 0.0;
    
    float camDist_T = 0;
    float camOff_T = 0;
    float camPitch_T = 0;
    
    float midiEase = 0.005;
    
    lineScape testScape;
    
public:
    void setup();
    void update(ivi_FFT* FFTInstance);
    
    void draw();
    ofFbo* drawFBO(bool calc);
    
    void newMidiMessage(ofxMidiMessage& eventArgs);

private:
    float getBandWidth();
    int freqToIndex(int freq);
    
    vector<float> getLogAverages(vector<float> samples);
    
    ofFbo output;
    
};
