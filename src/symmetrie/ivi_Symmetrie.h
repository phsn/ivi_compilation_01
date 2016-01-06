#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxGaussian.h"

#include "iviObj.h"
#include "../ivi_BEAT.h"
#include "../ivi_OSC.h"

class ivi_Symmetrie : public ofBaseApp{

	public:
		void setup(ivi_OSC & oscThread);
		void update();
    
		void draw();
        ofFbo* drawFBO(bool calc);
    
        void drawKalaido();
        void drawBase();
    
        void onTickRhythm(ofVec2f & tObj);
    
        void handleOSC(ofxOscMessage & m);
        void keyPressed(int key);
    
    ivi_OSC* parentOSCThread;
    
    ofxPanel gui;
    ofParameter<int> numRepetitions;
    ofParameter<float> accumulation;
    ofParameter<float> fisheye_distortion;
    ofParameter<float> fisheye_abberation;

    ofVec2f inputPos;
    ofVec2f autoPos_T;
    bool autoPos = true;
    
    iviObj centerObj;
    ofColor centerColor;
    
    ofFbo baseFBO;
    ofFbo kalaidoFBO;
    
    bool kalaidoEnabled;
    
    ofShader kalaido;
    int squareSize;
		
};
