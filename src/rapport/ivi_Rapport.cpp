#include "ivi_Rapport.h"
#include <math.h>

//--------------------------------------------------------------
void ivi_Rapport::setup(){

    ofEnableAntiAliasing();
    ofEnableSmoothing();
    
    scapeLines = 20;
    testScape = lineScape(scapeLines,distance);
    
    myCamera.setupPerspective(true,80,100,500000, ofVec2f(0.0f, 0.0f));
    //myCamera.setDistance(20000);
    
    output.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);
}

//--------------------------------------------------------------
void ivi_Rapport::update(ivi_FFT* FFTInstance){
    
    // UPDATE MIDI DATA
    
    distance += (int) (distance_T-distance)*midiEase;
    bendFact += (bendFact_T-bendFact)*midiEase*20;
    fftAmt += (fftAmt_T-fftAmt)*midiEase*50;
    noiseAmt += (noiseAmt_T-noiseAmt)*midiEase*10;
    
    camDist += (camDist_T-camDist)*midiEase;
    camOff += (camOff_T-camOff)*midiEase;
    camPitch += (camPitch_T-camPitch)*midiEase;
    
    testScape.updateBend(bendFact);
    testScape.updateMixer(fftAmt,noiseAmt);
    testScape.updateMaxZ(distance);
    

    myCamera.setDistance(testScape.maximumZ*0.5+2000);
        
    testScape.setFFTData(FFTInstance->getLogAverages());
    testScape.update();

}

//--------------------------------------------------------------
void ivi_Rapport::draw(){
    ofBackground(0);
    
    myCamera.begin();
    
    ofTranslate(myCamera.getPosition());
    ofRotateX(camPitch*180);
    ofTranslate(-myCamera.getPosition());
    
    
    ofTranslate(-ofGetWidth()*25,1000+ofGetHeight()/2.0,-(scapeLines*testScape.getSpacing())+2000);
    
    ofTranslate(0,-camOff-camPitch*50000,camDist*3);
    
    
    testScape.draw();
    
    myCamera.end();
        
}

ofFbo* ivi_Rapport::drawFBO(bool calc) {
    output.begin();
    ofClear(0,255);
    if(calc) draw();
    output.end();
    
    return &output;
}


//--------------------------------------------------------------
void ivi_Rapport::newMidiMessage(ofxMidiMessage& msg) {
    // make a copy of the latest message
    if(msg.status == MIDI_NOTE_ON) {
    
        int height = ofMap(msg.pitch,21,108,0.0f,1.0f)*87.0f;
        
    } else if(msg.status == MIDI_CONTROL_CHANGE) {
        if(msg.control == IVI_MIDI_RAP_NUMLINES) {
            scapeLines = (int) ofMap(msg.value,0,127,2,40);
            
            //cout << scapeLines << endl;
            testScape = lineScape(scapeLines,distance);
            testScape.updateBend(bendFact);
            testScape.updateAll();
            testScape.downresAll(downFact);
            //testScape.updateLineCount((int) ofMap(msg.value,0,127,1,70));
        } else if(msg.control == IVI_MIDI_RAP_DOWNRES) {
            downFact = ofMap(127
                             -msg.value,0,127,0,1);
            testScape.downresAll(downFact);
        } else if(msg.control == IVI_MIDI_RAP_DISTANCE) {
            distance_T = ofMap(max(msg.value,1),0,127,0,400000,true);
        } else if(msg.control == IVI_MIDI_RAP_BEND) {
            bendFact_T = ofMap(msg.value,0,127,-1,1,true);
        } else if(msg.control == IVI_MIDI_RAP_FFTAMT) {
            fftAmt_T = ofMap(msg.value,0,127,0,1,true);
        } else if(msg.control == IVI_MIDI_RAP_NOISEAMT) {
            noiseAmt_T = ofMap(msg.value,0,127,0,1,true);
        } else if(msg.control == IVI_MIDI_RAP_CAMDIST) {
            camDist_T = ofMap(msg.value,0,127,-10000,10000,true);
        } else if(msg.control == IVI_MIDI_RAP_CAMOFFSET) {
            camOff_T = ofMap(msg.value,0,127,-5000,5000,true);
        } else if(msg.control == IVI_MIDI_RAP_CAMPITCH) {
            camPitch_T = ofMap(msg.value,0,127,-0.5,0.5,true);
        }
    }
}
