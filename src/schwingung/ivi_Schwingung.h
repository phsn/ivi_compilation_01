#pragma once

#include "ofMain.h"
#include "DropEmitter.h"
#include "DropBrush.h"
#include "ofxMidi.h"
#include "Spiropath.h"

#include "../ivi_MIDI.h"

class ivi_Schwingung : public ofBaseApp {
    
public:
    void setup();
    void update();
    
    void draw();
    ofFbo* drawFBO(bool calc);
    
    void drawRepeat();

    void keyPressed(int key);
    
    ofFbo baseFBO;
    ofFbo repeatFBO;

    ofShader repeatShader;
    ofShader fisheye;
    
    int rShd_Copies = 20;
    float rShd_Rotation = 60;
    float rShd_RotationInput = 60;
    float rShd_Scale = 0.1;
    float rShd_ScaleInput = 0.1;
    float rShd_Ease = 0.01;

    bool shaderEnabled = true;;
    
    DropEmitter myEmitter;
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    
    vector<int> midiNotes;
    
    ofEasyCam myCam;
    DropBrush testBrush;

};
