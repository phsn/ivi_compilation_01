#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    t_ch01 = 1.0;
    t_ch02,t_ch03,t_ch04 = 0.0;
    
    
    //  MIDI SETUP  ///////////////////////

    // midiIn.listPorts();
    midiIn.openPort(0);
    midiIn.addListener(this);
    
    //  FFT  //////////////////////////////
    
    // FFT.listDevices();
    FFT.setup(5);
    
    //  SYSTEME  //////////////////////////
    
    sys_Intro.setup();
    sys_Schwingung.setup();
    sys_Rapport.setup();
    
    mixerFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    mixerShader.load( "shader/mixerShader" );
    
    iviOutput.setName("iviOutput");

}

//--------------------------------------------------------------
void ofApp::update(){
    
    std::stringstream strm;
    strm << "ivi Server - fps: " << ofGetFrameRate();
    ofSetWindowTitle(strm.str());
    
    FFT.update();
    
    if(t_ch01 > 0) sys_Intro.update(&FFT);
    if(t_ch02 > 0) sys_Schwingung.update();
    if(t_ch03 > 0) sys_Rapport.update(&FFT);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofFbo* sys_IntroFBO = sys_Intro.drawFBO((t_ch01 > 0));
    ofFbo* sys_SchwingungFBO = sys_Schwingung.drawFBO((t_ch02 > 0));
    ofFbo* sys_RapportFBO = sys_Rapport.drawFBO((t_ch03 > 0));
    
    mixerShader.begin();
    mixerShader.setUniform1f("t_input01", t_ch01);
    mixerShader.setUniform1f("t_input02", t_ch02);
    mixerShader.setUniform1f("t_input03", t_ch03);
    mixerShader.setUniformTexture("input01", sys_IntroFBO->getTextureReference(),1);
    mixerShader.setUniformTexture("input02", sys_SchwingungFBO->getTextureReference(),2);
    mixerShader.setUniformTexture("input03", sys_RapportFBO->getTextureReference(),3);

    mixerFBO.draw(0,0);
    
    mixerShader.end();
    
    iviOutput.publishScreen();
    
    drawMidiUI();
   
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='p') {
        sys_Intro.logoIn();
    } else if(key=='c') {
        sys_Intro.logoOut();
    }
}


//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    midiMessage = msg;
    sys_Schwingung.newMidiMessage(msg);
    sys_Rapport.newMidiMessage(msg);

    if(msg.status == MIDI_CONTROL_CHANGE) {
        if(msg.control == IVI_MIDI_MIX_CH01) {
            //t_ch01 = ofMap(msg.value,0,127,0.0,1.0);
            sys_Intro.fade = ofMap(msg.value,0,127,0.0,1.0);
        } else if(msg.control == IVI_MIDI_MIX_CH02) {
            t_ch02 = ofMap(msg.value,0,127,0.0,1.0);
        } else if(msg.control == IVI_MIDI_MIX_CH03) {
            t_ch03 = ofMap(msg.value,0,127,0.0,1.0);
        } else if(msg.control == IVI_MIDI_MIX_CH04) {
            t_ch04 = ofMap(msg.value,0,127,0.0,1.0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawMidiUI(){
    // draw the last recieved message contents to the screen
    text << "Received: " << ofxMidiMessage::getStatusString(midiMessage.status);
    ofDrawBitmapString(text.str(), 20, 20);
    text.str(""); // clear
    
    text << "channel: " << midiMessage.channel;
    ofDrawBitmapString(text.str(), 20, 34);
    text.str(""); // clear
    
    text << "pitch: " << midiMessage.pitch;
    ofDrawBitmapString(text.str(), 20, 48);
    text.str(""); // clear
    ofRect(20, 58, ofMap(midiMessage.pitch, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "velocity: " << midiMessage.velocity;
    ofDrawBitmapString(text.str(), 20, 96);
    text.str(""); // clear
    ofRect(20, 105, ofMap(midiMessage.velocity, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "control: " << midiMessage.control;
    ofDrawBitmapString(text.str(), 20, 144);
    text.str(""); // clear
    ofRect(20, 154, ofMap(midiMessage.control, 0, 127, 0, ofGetWidth()-40), 20);
    
    text << "value: " << midiMessage.value;
    ofDrawBitmapString(text.str(), 20, 192);
    text.str(""); // clear
    
    if(midiMessage.status == MIDI_PITCH_BEND) {
        ofRect(20, 202, ofMap(midiMessage.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()-40), 20);
    }
    else {
        ofRect(20, 202, ofMap(midiMessage.value, 0, 127, 0, ofGetWidth()-40), 20);
    }
}