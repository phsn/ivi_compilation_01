#include "ivi_Symmetrie.h"

//--------------------------------------------------------------
void ivi_Symmetrie::setup(ivi_OSC & oscThread){
    
    parentOSCThread = &oscThread;
    
    baseFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    kalaidoFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    
    squareSize = 2*ofGetHeight()/sqrt(3);
    
    kalaidoEnabled = true;
    
    kalaido.load( "shader/sys_symmetrie/mykaleido" );
        
    gui.setup("ivi_symmetrie_01", "ivi_symmetrie_01", ofGetWidth()/2-100,ofGetHeight()-150);
    gui.add(numRepetitions.set("Repetitions", 10, 1, 100));
    gui.add(accumulation.set("Accumulation", 70.0, 0.0, 100.0));
    gui.add(fisheye_distortion.set("Distortion", 0.6, -1.0, 1.0));
    gui.add(fisheye_abberation.set("Chromatic Abberation", 70.0, 0.0, 200.0));
    
    centerObj.setup(ofVec3f(0,0,0), ofVec3f(0,0,0), ofVec3f(1,1,1));
    
    inputPos = ofVec2f(0,0);
    autoPos_T = ofVec2f(0,0);

}

//--------------------------------------------------------------
void ivi_Symmetrie::update(){
    // check for waiting messages
    /*
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        handleOSC(m);
    }*/
    
    if(autoPos) {
        inputPos += (autoPos_T-inputPos)*0.1;
    }
}

//--------------------------------------------------------------
void ivi_Symmetrie::draw(){
    
    ofSetBackgroundAuto(true);

    kalaidoFBO.begin();
    drawKalaido();
    kalaidoFBO.end();
    
    kalaidoFBO.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
    gui.draw();
}

ofFbo* ivi_Symmetrie::drawFBO(bool calc) {
    //ofSetBackgroundAuto(true);
    
    kalaidoFBO.begin();
    ofClear(0,255);
    if(calc) drawKalaido();
    kalaidoFBO.end();
    
    return &kalaidoFBO;
}


//--------------------------------------------------------------
void ivi_Symmetrie::drawKalaido(){
    baseFBO.begin();
    drawBase();
    baseFBO.end();
    ofSetColor( 255 );
    
    if ( kalaidoEnabled ) {
        kalaido.begin();
        kalaido.setUniform1i( "numRepetitions", numRepetitions);
        kalaido.setUniform2f( "screenSize", ofGetWidth(), ofGetHeight());
    }
    
    baseFBO.draw( 0, 0, ofGetWidth(), ofGetHeight() );
    
    if ( kalaidoEnabled ) {
        kalaido.end();
    }

}

//--------------------------------------------------------------
void ivi_Symmetrie::drawBase(){
    ofSetColor(255,255,255,(1.0f-accumulation/100.0f)*255.0f);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    //ofSetColor(255,255,255,120);
    //ofTriangle(ofGetWidth()/2, 0, ofGetWidth()/2+squareSize/2, ofGetHeight(),ofGetWidth()/2-squareSize/2, ofGetHeight());
    
    centerColor = ofColor(0);
    
    ofSetColor(centerColor);
    
    //centerObj.update(ofVec3f(ofGetMouseX(), ofGetMouseY()-50, 0), ofVec3f(0,0,180*ofGetMouseX()/float(ofGetWidth())), ofVec3f(1,4,1));
    centerObj.update(ofVec3f(inputPos.x, inputPos.y-50, 0), ofVec3f(0,0,180*inputPos.x/float(ofGetWidth())), ofVec3f(1,4,1));
    
    centerObj.draw();
    /*
    ofTranslate(ofGetWidth()*0.6,0);
    ofRotateZ(90*ofGetMouseX()/float(ofGetWidth()));
    ofTranslate(-ofGetWidth()*0.6,0);
    ofSetColor(0, 0, 0);
    //ofEllipse(ofGetMouseX(),ofGetMouseY(),200,200);
    ofRect(0,ofGetMouseY()-50,ofGetWidth(),100);*/
}

//--------------------------------------------------------------
void ivi_Symmetrie::onTickRhythm(ofVec2f & tObj){
    if(autoPos) {
        autoPos_T.x = ofGetWidth()*0.5+ofxGaussian()*ofGetWidth()*0.2;
        autoPos_T.y = ofGetHeight()*0.5+ofxGaussian()*ofGetHeight()*0.2;
        
        parentOSCThread->sendFloat("/_2d_touch/x", autoPos_T.x/(float) ofGetWidth());
        parentOSCThread->sendFloat("/_2d_touch/y", autoPos_T.y/(float) ofGetHeight());
    }
}

//--------------------------------------------------------------
void ivi_Symmetrie::handleOSC(ofxOscMessage & m) {
    if(!autoPos) {
        if(m.getAddress() == "/_2d_touch/x") {
            inputPos.x = m.getArgAsFloat(0)*ofGetWidth();
        } else if(m.getAddress() == "/_2d_touch/y") {
            inputPos.y = (1-m.getArgAsFloat(0))*ofGetHeight();
        }
    }
    if(m.getAddress() == "/_2d_auto/x") {
        autoPos = (m.getArgAsFloat(0) > 0) ? true : false;
        //cout << "autoPos:\t" << autoPos << endl;
        //cout << "autoPos_T:\t" << autoPos_T << endl;
    }
}



//--------------------------------------------------------------
void ivi_Symmetrie::keyPressed(int key){
    
    if (key == 'k' ) {
        kalaidoEnabled = !kalaidoEnabled;
    } else if(key == 'b') {
        centerObj.blur = !centerObj.blur;
    }

}
