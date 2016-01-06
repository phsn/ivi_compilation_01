#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"

#include "ivi_FFT.h"
#include "ivi_MIDI.h"
#include "ivi_BEAT.h"
#include "ivi_OSC.h"

// SYSTEME
#include "intro/ivi_Intro.h"
#include "schwingung/ivi_Schwingung.h"
#include "rapport/ivi_Rapport.h"
#include "symmetrie/ivi_Symmetrie.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    //  SYSTEME UND FBOS  /////////////////
    
    ofFbo       mixerFBO;
    ofShader    mixerShader;
    float       t_ch01,t_ch02,t_ch03,t_ch04;
    
    ivi_Intro       sys_Intro;
    ivi_Schwingung  sys_Schwingung;
    ivi_Rapport     sys_Rapport;
    ivi_Symmetrie   sys_Symmetrie;

    //  MIDI EVENT HANDLING ///////////////
    
    ofxMidiIn       midiIn;
    void            newMidiMessage(ofxMidiMessage& eventArgs);
    
    //  MIDI UI  //////////////////////////
    
    ofxMidiMessage  midiMessage;
    stringstream    text;
    void            drawMidiUI();

    //  FFT  //////////////////////////////
    
    ivi_FFT FFT;
    
    //  SYPHON  ///////////////////////////
    
    ofxSyphonServer iviOutput;
    
    //  MULTI-THREADS  ////////////////////
    
    ivi_BEAT    beatThread;
    
    void        onTickRhythm(ofVec2f & tObj);
    void        onTick16(ofVec2f & tObj);
    void        onTick8(ofVec2f & tObj);
    void        onTick(ofVec2f & tObj);
    void        onBar(ofVec2f & bObj);
    void        onBPMChange(ofVec2f & tObj);
    void        onRhythmChange(ofVec2f & tObj);
    
    ivi_OSC     oscThread;
    
    void        handleOSC(ofxOscMessage & m);
    
    ///////////////////////////////////////

};