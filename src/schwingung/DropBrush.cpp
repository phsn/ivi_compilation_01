
#include "DropBrush.h"

DropBrush::DropBrush(){
    birthTime = ofGetElapsedTimef();
    
    this->setup();
}

DropBrush::DropBrush(int histSteps, int sFrames, float mSize){
    birthTime = ofGetElapsedTimef();
    deathTime = 0;
    historySteps = histSteps;
    subFrames = sFrames;
    maxSize = mSize;
    lifeDur = 0;
    
    this->setup();
}

DropBrush::DropBrush(int histSteps, int sFrames, float mSize, float duration){
    birthTime = ofGetElapsedTimef();
    deathTime = 0;
    historySteps = histSteps;
    subFrames = sFrames;
    maxSize = mSize;
    lifeDur = duration;
    
    this->setup();
}

DropBrush::~DropBrush(){
    history.clear();
    ready = false;
    vector<ofVec3f>().swap(history);
    brushMesh.clear();
}

//--------------------------------------------------------------
void DropBrush::setup(){
    
    dropPath.setup(300, 1000, 300);
    
    brushMesh.setMode(OF_PRIMITIVE_POINTS);
    
    for (int i=0; i < historySteps; i++) {
        history.push_back(ofVec3f(0,0,0));
    }
    
}

//--------------------------------------------------------------
void DropBrush::startPos(ofVec3f startP){
    history.clear();
    
    for (int i=0; i < historySteps; i++) {
        history.push_back(startP);
    }
    
    position = startP;
    
    ready = true;
    
}

//--------------------------------------------------------------
void DropBrush::update(ofVec3f newPos){
    position = newPos;
    if(deathTime > 0 && ofGetElapsedTimef()-deathTime >= fadeTime) {
        beKilled = true;
    }
}

//--------------------------------------------------------------
void DropBrush::draw(){
    if(ready) {
    int vertexCount = 0;
    brushMesh.clear();
    glPointSize(maxSize);
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (int i=historySteps-1; i >= 0; i--) {
        if (i == 0 && history.size() > 0) {
            history[i] = position;
        } else {
            float prgrs, bprgrs,curSize;
            for (int j=0; j < subFrames; j++) {
                prgrs = j/float(subFrames);
                bprgrs = 1-ofMap(i-prgrs, 0, historySteps-1, 0, 1);
                
                /////
                // DEATH PROGRESS, schwanz kürzer werden.
                //
                
                curColor = ofColor(startColor.r * bprgrs + endColor.r * (1-bprgrs),
                                   startColor.g * bprgrs + endColor.g * (1-bprgrs),
                                   startColor.b * bprgrs + endColor.b * (1-bprgrs));
                if(history.size() > 0) {
                    if(deathTime > 0) {
                        float dPrgs = (ofGetElapsedTimef()-deathTime)/fadeTime;
                        brushMesh.addVertex(ofVec3f(
                                            ofMap(prgrs,0,1,history[i].x,history[i-1].x),
                                            ofMap(prgrs,0,1,history[i].y,history[i-1].y),
                                            ofMap(prgrs,0,1,history[i].z,history[i-1].z)));
                    } else {
                        brushMesh.addVertex(ofVec3f(
                                                    ofMap(prgrs,0,1,history[i].x,history[i-1].x),
                                                    ofMap(prgrs,0,1,history[i].y,history[i-1].y),
                                                    ofMap(prgrs,0,1,history[i].z,history[i-1].z)));
                    }
                }
                brushMesh.addColor(curColor);
                float curSize = maxSize;
                //if(deathTime > 0) curSize *= (1-(ofGetElapsedTimef()-deathTime)/fadeTime);
                if(deathTime > 0) curSize *= 1-easeOut_Quart(ofGetElapsedTimef()-deathTime+
                                                              (1-easeOut_Quart(ofGetElapsedTimef()-deathTime,0,1,fadeTime))*(1-bprgrs),
                                                              0,1,fadeTime+(1-easeOut_Quart(ofGetElapsedTimef()-deathTime,0,1,fadeTime)));
                curSize = (curSize > 0) ? curSize : 0;
                //curSize = floor(curSize*10)/10.0f;
                //cout << curSize << endl;
                brushMesh.addNormal(ofVec3f(bprgrs,curSize,(1-easeOut_Quart(ofGetElapsedTimef()-deathTime,0,1,fadeTime))*(1-bprgrs)));
                vertexCount++;
            }
            if(history.size() > 0) {
                history[i] = history[i-1];
            }
        }
    }
    
    brushMesh.draw();
    }
    
}

//--------------------------------------------------------------
void DropBrush::kill(){
    if(ofGetElapsedTimef()-birthTime > minTime) {
        deathTime = ofGetElapsedTimef();
    } else {
        deathTime = ofGetElapsedTimef() + minTime - (ofGetElapsedTimef()-birthTime);
    }
}

//--------------------------------------------------------------
void DropBrush::setOffset(float offset){
    privateOffset = offset;
}

//--------------------------------------------------------------
void DropBrush::setReverse(bool n_reverse){
    dropPath.reverse = n_reverse;
}

//--------------------------------------------------------------
float DropBrush::getOffset(){
    return privateOffset;
}


//--------------------------------------------------------------
float DropBrush::easeOut_Quart(float t,float b,float c,float d){
    return c * sin(t/d * (PI/2)) + b;
}