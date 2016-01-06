#include "ivi_Schwingung.h"

//--------------------------------------------------------------
void ivi_Schwingung::setup(){
    
    testBrush = DropBrush();
    
    // Mit dieser Formel kann ich meine Brennweite der Kamera oben bequem
    // in Millimetern angeben (Quelle: http://kmp.bdimitrov.de/technology/fov.html)
    
    float focalLength = 12;
    
    float factor = 43.266615300557;
    float fov = -1*(2*atan(factor/(2*focalLength))*180/PI);
    
    myCam = ofEasyCam();
    myCam.setupPerspective(true,fov,100,50000, ofVec2f(0.0f, 0.0f));
    myCam.setTarget(ofVec3f(0,0,0));
    myCam.setPosition(ofVec3f(0,0,0));
    myCam.setDistance(1000);
    
    myEmitter.setup(300, 1000, 300);
    
    baseFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    repeatFBO.allocate( ofGetWidth(), ofGetHeight(), GL_RGB );
    
    repeatShader.load("repeatShader");
    fisheye.load("fisheye");

}

//--------------------------------------------------------------
void ivi_Schwingung::update(){
    
    testBrush.update(ofVec3f(ofGetMouseX(),ofGetMouseY(),0));
 
    myEmitter.update();
    
    rShd_Rotation += (rShd_RotationInput-rShd_Rotation)*rShd_Ease;
    rShd_Scale += (rShd_ScaleInput-rShd_Scale)*rShd_Ease;
    
}

//--------------------------------------------------------------
void ivi_Schwingung::draw(){
    
    repeatFBO.begin();
    drawRepeat();
    repeatFBO.end();
    
    if(shaderEnabled) {
        fisheye.begin();
        fisheye.setUniform2f( "iResolution", ofGetWidth(), ofGetHeight()*1.77);
        fisheye.setUniform2f( "iMouse", ofGetMouseX(), ofGetMouseY());
        fisheye.setUniform1f( "chromAbb", ofGetMouseY());
    }
    
    repeatFBO.draw(0,0);
    
    if(shaderEnabled) {
        fisheye.end();
    }
    
    //drawMidiUI();
}

//--------------------------------------------------------------
ofFbo* ivi_Schwingung::drawFBO(bool calc){
    
    if(calc) {
    
    repeatFBO.begin();
    drawRepeat();
    repeatFBO.end();
    
    }
    
    return &repeatFBO;
}


//--------------------------------------------------------------
void ivi_Schwingung::drawRepeat(){
    
    float speed = 0.4;
    
    baseFBO.begin();
    
    ofPushMatrix();
    
    ofBackground(0);
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
    ofScale(0.2,0.2,0.2);
    myEmitter.draw();
    
    ofPopMatrix();
    
    baseFBO.end();
    
    if(shaderEnabled) {
        repeatShader.begin();
        repeatShader.setUniform1i( "copies", rShd_Copies);
        repeatShader.setUniform1f( "scale", rShd_Scale);
        repeatShader.setUniform1f( "rotation", rShd_Rotation);
        repeatShader.setUniform2f( "screenSize", ofGetWidth(), ofGetHeight());
    }
    
    baseFBO.draw(0,0);
    
    if(shaderEnabled) {
        repeatShader.end();
    }
}


//--------------------------------------------------------------
void ivi_Schwingung::keyPressed(int key){
    if(key == 's') {
        shaderEnabled = !shaderEnabled;
    }
}

//--------------------------------------------------------------
void ivi_Schwingung::newMidiMessage(ofxMidiMessage& msg) {
    // make a copy of the latest message
    if(msg.status == MIDI_NOTE_ON) {

        int height = ofMap(msg.pitch,21,108,0.0f,1.0f)*87.0f;

        if(msg.velocity > 0) {
            
            myEmitter.curTone = height;
            myEmitter.emit(myEmitter.numDrops);
            if(!ofContains(midiNotes,height)) {
                midiNotes.push_back(height);
            }
       
        } else {
            //cout << "OFF " << msg.pitch << endl;
            if(midiNotes.size() > 0) {
                int idx = ofFind(midiNotes,height);
                midiNotes[idx] = midiNotes.back();
                midiNotes.pop_back();
                myEmitter.delete_tone(height);
            }
        }
        /*for (auto c : midiNotes)
            cout << c << ' ';        //cout << height << endl;
        cout << endl;*/
    } else if(msg.status == MIDI_CONTROL_CHANGE) {
        if(msg.control == IVI_MIDI_SHW_SPEED) {
            myEmitter.speed = ofMap(msg.value,0,127,0.0,1.0);
        } else if(msg.control == IVI_MIDI_SHW_NUMDROPS) {
            myEmitter.numDrops = (int) ofMap(msg.value,0,127,1,5);
        } else if(msg.control == IVI_MIDI_SHW_RSHD_COPIES) {
            rShd_Copies = (int) ofMap(msg.value,0,127,1,20);
        } else if(msg.control == IVI_MIDI_SHW_RSHD_ROTATION) {
            rShd_RotationInput = ofMap(msg.value,0,127,-90,90);
        } else if(msg.control == IVI_MIDI_SHW_RSHD_SCALE) {
            rShd_ScaleInput =  ofMap(msg.value,0,127,0.0,2.0);
        }
    }
}