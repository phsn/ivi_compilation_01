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
    sys_Symmetrie.setup(oscThread);
    
    mixerFBO.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);
    mixerShader.load( "shader/mixerShader" );
    
    iviOutput.setName("iviOutput");
    
    // THREADS  ///////////////////////////
    
    ofAddListener(beatThread.tickEvent, this, &ofApp::onTick);
    ofAddListener(beatThread.tick8Event, this, &ofApp::onTick8);
    ofAddListener(beatThread.tick16Event, this, &ofApp::onTick16);
    ofAddListener(beatThread.tickRhythmEvent, this, &ofApp::onTickRhythm);
    ofAddListener(beatThread.barEvent, this, &ofApp::onBar);
    
    ofAddListener(beatThread.bpmChange, this, &ofApp::onBPMChange);
    ofAddListener(beatThread.rhythmMaskChange, this, &ofApp::onRhythmChange);
    
    beatThread.start();
    
    ofAddListener(oscThread.messageReceived, this, &ofApp::handleOSC);
    
    oscThread.setup(IVI_OSC_HOST,IVI_OSC_PORT);
    oscThread.start();

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
    if(t_ch04 > 0) sys_Symmetrie.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofFbo* sys_IntroFBO = sys_Intro.drawFBO((t_ch01 > 0));
    ofFbo* sys_SchwingungFBO = sys_Schwingung.drawFBO((t_ch02 > 0));
    ofFbo* sys_RapportFBO = sys_Rapport.drawFBO((t_ch03 > 0));
    ofFbo* sys_SymmetrieFBO = sys_Symmetrie.drawFBO((t_ch04 > 0));

    
    mixerShader.begin();
    mixerShader.setUniform1f("t_input01", t_ch01);
    mixerShader.setUniform1f("t_input02", t_ch02);
    mixerShader.setUniform1f("t_input03", t_ch03);
    mixerShader.setUniform1f("t_input04", t_ch04);
    mixerShader.setUniformTexture("input01", sys_IntroFBO->getTextureReference(),1);
    mixerShader.setUniformTexture("input02", sys_SchwingungFBO->getTextureReference(),2);
    mixerShader.setUniformTexture("input03", sys_RapportFBO->getTextureReference(),3);
    mixerShader.setUniformTexture("input04", sys_SymmetrieFBO->getTextureReference(),4);
    
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
void ofApp::onBar(ofVec2f & bObj){

}

//--------------------------------------------------------------
void ofApp::onTick(ofVec2f & tObj){
    oscThread.sendFloat("/BeatState/x", (tObj.y)/3.0f);
    //cout << "TICK " << tObj.y << endl;
}

//--------------------------------------------------------------
void ofApp::onTick8(ofVec2f & tObj){
    
}

//--------------------------------------------------------------
void ofApp::onTick16(ofVec2f & tObj){
    
}

//--------------------------------------------------------------
void ofApp::onTickRhythm(ofVec2f & tObj){
    sys_Symmetrie.onTickRhythm(tObj);
}

//--------------------------------------------------------------
void ofApp::onBPMChange(ofVec2f & tObj){
    oscThread.sendFloat("/BPM_Slider/x", tObj.x/300.0f);
}

//--------------------------------------------------------------
void ofApp::onRhythmChange(ofVec2f & tObj) {
    oscThread.sendBools("/RhythmMask_01/x", beatThread.getRhythmMask());
}

void ofApp::handleOSC(ofxOscMessage & m) {
    beatThread.handleOSC(m);
    sys_Symmetrie.handleOSC(m);
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