
#include "DropEmitter.h"

DropEmitter::DropEmitter(){}
DropEmitter::~DropEmitter(){}


//--------------------------------------------------------------
void DropEmitter::setup(int bigRadius, int smallRadius, int innrRadius){
  
    brushShader.load("brushShader");

    
}

//--------------------------------------------------------------
void DropEmitter::update(){
    
    for(int i=0; i<brushes.size(); i++) {
        brushes[i].update(brushes[i].dropPath.getPos(brushes[i].speed*ofGetElapsedTimef()+brushes[i].getOffset(),0));
    }

    ofRemove(brushes, killBrush);
    
    //cout << "Brushes: " << brushes.size() << endl;
}

//--------------------------------------------------------------
void DropEmitter::draw(){
    brushShader.begin();
    for(int i=0; i<brushes.size(); i++) {
        brushes[i].draw();
    }
    brushShader.end();
    
}

//--------------------------------------------------------------
void DropEmitter::emit(int numDrops){
    
    int startSize = brushes.size();
    float offset = ofRandom(-100, 100);
    bool rev = ofRandom(1.0) > 0.5;
    
    for(int i=0; i<numDrops; i++) {
        float noise=ofNoise(i*100.0);
        brushes.push_back(DropBrush(25, 16, 10));
        brushes[startSize+i].tone = curTone;
        brushes[startSize+i].speed = speed;
        //brushes[startSize+i].endColor = toneToColor(curTone);
        brushes[startSize+i].setReverse(curTone%2 == 0);
        brushes[startSize+i].dropPath.setTone(curTone);
        brushes[startSize+i].setOffset(offset+i/float(numDrops));
        brushes[startSize+i].startPos(brushes[startSize+i].dropPath.getPos(brushes[startSize+i].speed*ofGetElapsedTimef()+offset+i/float(numDrops),0));
    }
    
}


//--------------------------------------------------------------
void DropEmitter::delete_tone(int dTone) {
    for(int i=0; i < brushes.size(); i++) {
        if(brushes[i].tone == dTone) {
            brushes[i].kill();
        }
    }
}

//--------------------------------------------------------------
ofColor DropEmitter::toneToColor(int tone) {
    int rVals[] = {255,255,255,51,195,142,127,144,187,183,169,171,255};
    int gVals[] = {14,127,255,204,241,202,139,0,117,70,103,6,14};
    int bVals[] = {0,1,0,50,255,255,253,255,252,138,124,53,0};
    
    //blend = (prgs*12)%1.0;
    
    int octPart = (tone-3)%12;
    int range[] = {floor(octPart),ceil(octPart)};
    int t_r = rVals[octPart];
    int t_g = gVals[octPart];
    int t_b = bVals[octPart];
    
    return ofColor(t_r,t_g,t_b);

}

//--------------------------------------------------------------
bool DropEmitter::killBrush(DropBrush &brsh) {
    return brsh.beKilled;
}
