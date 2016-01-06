#pragma once

#include "ofMain.h"
#include "ofxFFTBase.h"

class ivi_FFT : public ofBaseApp {

public:
		void setup(int deviceID);
		void update();
        void exit();
    
    vector<float> getLogAverages();
    void drawSamples();
    
    float analyzeSampleRange(float minRange, float maxRange);
    void visualizeSampleRange(float minRange, float maxRange);
    
    void listDevices();
    
private:
    
    void audioIn(float * input, int bufferSize, int nChannels);
    
    ofSoundStream soundStream;
    vector<float> samplesChannelL;
    vector<float> samplesChannelR;
    
    ofxFFTBase fftChannelL;
    ofxFFTBase fftChannelR;
    
    float getBandWidth();
    int freqToIndex(int freq);

};
