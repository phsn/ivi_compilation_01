#include "ivi_FFT.h"

//--------------------------------------------------------------
void ivi_FFT::setup(int deviceID){
    
    soundStream.setDeviceID(deviceID);
    
    int channelsOut = 0;        // number of requested output channels (i.e. 2 for stereo).
    int channelsIn = 2;         // number of requested input channels.
    int sampleRate = 44100;     // requested sample rate (44100 is typical).
    int bufferSize = 1024;       // requested buffer size (256 is typical).
    int numOfBuffers = 4;       // number of buffers to queue, less buffers will be more responsive, but less stable.
    
    soundStream.setup(this, channelsOut, channelsIn, sampleRate, bufferSize, numOfBuffers);
    
    samplesChannelL.assign(bufferSize, 0.0);
    samplesChannelR.assign(bufferSize, 0.0);
    
    fftChannelL.setBufferSize(bufferSize);
    fftChannelR.setBufferSize(bufferSize);
    
}

//--------------------------------------------------------------
void ivi_FFT::listDevices(){
    soundStream.listDevices();
}

//--------------------------------------------------------------
void ivi_FFT::exit() {
    soundStream.close();
}

//--------------------------------------------------------------
void ivi_FFT::update(){
    fftChannelL.update();
    fftChannelR.update();
}

//--------------------------------------------------------------
void ivi_FFT::drawSamples() {
    
    vector<float> logSamples = getLogAverages();
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight() * 1.0);
    
    int sampleWidth = ofGetWidth() / logSamples.size();
    int sampleHeight = ofGetHeight()*0.7;
    int numOfSamples = logSamples.size();
    
    ofSetColor(255,255,255,255);
    
    for(int i=0; i<numOfSamples; i++) {
        //float sIndex = exp(log(numOfSamples)/numOfSamples*i);
        
        int x = ofMap(i, 0, numOfSamples-1, 0, ofGetWidth()-sampleWidth);
        int y = 0;
        int w = sampleWidth;
        int h = -logSamples[i] * sampleHeight;
        
        ofRect(x, y, w, h);
    }
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ivi_FFT::visualizeSampleRange(float minRange, float maxRange) {
    
    ofPushMatrix();
    ofTranslate(0, ofGetHeight());
    
    ofSetColor(0,0,255,20);
    ofRect(minRange*ofGetWidth(),0,ofGetWidth()*(maxRange-minRange), -ofGetHeight() / 2);
    
    ofPopMatrix();
}

float ivi_FFT::analyzeSampleRange(float minRange, float maxRange) {
    
    vector<float> samples = getLogAverages();
    
    int numOfSamples = samples.size();
    int minSample = floor(minRange*numOfSamples);
    int maxSample = floor(maxRange*numOfSamples);
    
    float volume = 0.0;
    for(int i=minSample; i < maxSample; i++) {
        float sIndex = exp(log(numOfSamples)/numOfSamples*i);
        volume += samples[floor(sIndex)];
    }
    volume /= (maxSample-minSample);
    return volume;
}

//--------------------------------------------------------------
void ivi_FFT::audioIn(float * input, int bufferSize, int nChannels) {
    
    for(int i = 0; i<bufferSize; i++) {
        samplesChannelL[i] = input[i * 2 + 0];
        samplesChannelR[i] = input[i * 2 + 1];
    }
    
    float * dataL = &samplesChannelL[0];
    float * dataR = &samplesChannelR[0];
    
    fftChannelL.audioIn(dataL);
    fftChannelR.audioIn(dataR);
}

float ivi_FFT::getBandWidth(){
    return (2.0f/(float)fftChannelL.getBufferSize()) * (soundStream.getSampleRate() / 2.0f);
}

int ivi_FFT::freqToIndex(int freq) {
    // special case: freq is lower than the bandwidth of spectrum[0]
    if ( freq < getBandWidth()/2 ) return 0;
    // special case: freq is within the bandwidth of spectrum[512]
    if ( freq > soundStream.getSampleRate()/2 - getBandWidth()/2 ) return 512;
    // all other cases
    float fraction = (float)freq/(float) soundStream.getSampleRate();
    int i = round(fftChannelL.getBufferSize() * fraction);
    return i;
}

vector<float> ivi_FFT::getLogAverages() {
    
    vector<float> samples = fftChannelL.getFftNormData();
    
    vector<float> averages;
    int sampleRate = soundStream.getSampleRate();
    
    for (int i = 1; i < 12; i++)
    {
        float avg = 0;
        int lowFreq;
        if ( i == 0 )
            lowFreq = 0;
        else
            lowFreq = (int)((sampleRate/2) / (float) pow(2.0f, 12 - i));
        int hiFreq = (int)((sampleRate/2) / (float) pow(2.0f, 11 - i));
        int lowBound = freqToIndex(lowFreq);
        int hiBound = freqToIndex(hiFreq);
        
        for (int j = lowBound; j <= hiBound-0.5*(hiBound-lowBound); j++)
        {
            avg += samples[j];
        }
        
        avg /= (hiBound - 0.5*(hiBound-lowBound) - lowBound +1);
        
        averages.push_back(avg);
        avg = 0;
        
        for (int j = lowBound+0.5*(hiBound-lowBound); j <= hiBound; j++)
        {
            avg += samples[j];
        }
        
        avg /= (hiBound - 0.5*(hiBound-lowBound) - lowBound +1);
        
        // line has been changed since discussion in the comments
        // avg /= (hiBound - lowBound);
        averages.push_back(avg);
    }
    
    /*
    cout << "AVERAGES: ";
    for(int i=0; i < averages.size(); i++) {
        cout << averages[i] << " ";
    }
    cout << endl;
     */
    
    return averages;
}