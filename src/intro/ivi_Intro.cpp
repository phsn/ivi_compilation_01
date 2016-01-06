#include "ivi_Intro.h"

//--------------------------------------------------------------
void ivi_Intro::setup(){
    
    introPlayer.loadMovie("movies/ivi_animation.mov");
    
    smoothAverages.assign(20,0);
    
    mainFBO.allocate( ofGetWidth()*2, ofGetHeight()*2, GL_RGB );
    output.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );

}

//--------------------------------------------------------------
void ivi_Intro::update(ivi_FFT* FFTInstance){
    
    introPlayer.update();
    
    if(introPlayer.getCurrentFrame() == introPlayer.getTotalNumFrames()-1
       && !introOut) {
        introPlayer.setPaused(true);
    }
    
    if(introPlayer.getPosition() == 0.0 && introOut) {
        introPlayer.setLoopState(OF_LOOP_NORMAL);
        introPlayer.setPaused(true);
        introOut = false;
    }
    
    vector<float> logSamples = FFTInstance->getLogAverages();
    logSamples.pop_back();
    logSamples.pop_back();
    
    for(int i=0; i < smoothAverages.size(); i++) {
        smoothAverages[i] += (logSamples[i]-smoothAverages[i])*0.4;
    }
    
}

//--------------------------------------------------------------
void ivi_Intro::draw(){
    
    ofBackground(0);
    
    float scale = 0.45;
    introPlayer.draw(ofGetWidth()/2-101*scale,ofGetHeight()/2-86*scale,202*scale,173*scale);
    
    float innerRadius = 180;
    float outerRadius = 70*fade;
    
    float width = 15;
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ofRotate(90.0f/((float)smoothAverages.size())+ofGetElapsedTimef()*3);
    
    ofFill();
    ofSetColor(255);
    
    for(int i=0; i < smoothAverages.size(); i++) {
        /*
        ofSetColor(255,0,0);
        ofRect(-width/2,innerRadius,width,outerRadius);
        */
        ofSetColor(255);
        ofRect(-width/2,innerRadius,width,smoothAverages[i]*outerRadius);
        ofRotate(180.0f/((float)smoothAverages.size()));
    }
    for(int i=smoothAverages.size()-1; i >= 0; i--) {
    /*
         ofSetColor(255,0,0);
         ofRect(-width/2,innerRadius,width,outerRadius);
        */
        ofSetColor(255);
        ofRect(-width/2,innerRadius,width,smoothAverages[i]*outerRadius);
        ofRotate(180.0f/((float)smoothAverages.size()));
    }
    
}

//--------------------------------------------------------------
ofFbo* ivi_Intro::drawFBO(bool calc){
    
    if(calc) {
    
    mainFBO.begin();
    ofScale(2.0,2.0);
    ofClear(0, 255);
    draw();
    mainFBO.end();
    
    output.begin();
    ofClear(0, 255);
    mainFBO.draw(0,0,ofGetWidth(),ofGetHeight());
    output.end();
        
    }
    
    return &output;
}

//--------------------------------------------------------------
void ivi_Intro::logoIn() {
    introPlayer.play();
}

//--------------------------------------------------------------
void ivi_Intro::logoOut() {
    introPlayer.setPosition(1.0f);
    introPlayer.setLoopState(OF_LOOP_PALINDROME);
    introPlayer.setPaused(false);
    introOut = true;

}