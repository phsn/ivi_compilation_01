
#include "iviObj.h"

iviObj::iviObj(){}
iviObj::~iviObj(){}

void iviObj::setup(ofVec3f setPos, ofVec3f setRot, ofVec3f setScale){
    this->setPosition(setPos);
    this->setOrientation(setRot);
    this->setScale(setScale);
    birthState = this->getGlobalTransformMatrix();
    blur = true;
}

void iviObj::update(ofVec3f newPos, ofVec3f newRot, ofVec3f newScale){
    beforeState = prevState;
    prevState = this->getGlobalTransformMatrix();
    //beforeState = prevState;
    this->setPosition(newPos);
    this->setOrientation(newRot);
    this->setScale(newScale);
}

void iviObj::draw(){
    
    ofMatrix4x4 curMtx = this->getGlobalTransformMatrix();
    
    int numSamples = 24;
    float prgs;
    ofMatrix4x4 blendMtx;
    
    if(blur) {
    
    for(int i=numSamples; i >= 1 ; i--) {
        prgs = float(i)/float(numSamples-1);
        /*
        for(int n=0; n < 4; n++) {
            if(n < 3) {
                float curLen = ofVec3f(curMtx.getPtr()[n*4],curMtx.getPtr()[n*4+1],curMtx.getPtr()[n*4+2]).length();
                float prevLen = ofVec3f(beforeState.getPtr()[n*4],beforeState.getPtr()[n*4+1],beforeState.getPtr()[n*4+2]).length();
                float blendLen = (1-prgs)*curLen+prgs*prevLen;
                
                ofVec3f blendVec = ofVec3f( (1-prgs)*curMtx.getPtr()[n*4]   + prgs*beforeState.getPtr()[n*4],
                                            (1-prgs)*curMtx.getPtr()[n*4+1] + prgs*beforeState.getPtr()[n*4+1],
                                            (1-prgs)*curMtx.getPtr()[n*4+2] + prgs*beforeState.getPtr()[n*4+2]);
                
                blendVec.normalize();
                blendVec *= blendLen;
                blendMtx.getPtr()[n*4] = blendVec.x;
                blendMtx.getPtr()[n*4+1] = blendVec.y;
                blendMtx.getPtr()[n*4+2] = blendVec.z;
                
            } else {
            blendMtx.getPtr()[n*4] = (1-prgs)*curMtx.getPtr()[n*4]
                                  + prgs*beforeState.getPtr()[n*4];
            blendMtx.getPtr()[n*4+1] = (1-prgs)*curMtx.getPtr()[n*4+1]
                                    + prgs*beforeState.getPtr()[n*4+1];
            blendMtx.getPtr()[n*4+2] = (1-prgs)*curMtx.getPtr()[n*4+2]
                                    + prgs*beforeState.getPtr()[n*4+2];

            
            }
        }*/
        
        for(int n=0; n < 16; n++) {
            blendMtx.getPtr()[n] = (1-prgs)*curMtx.getPtr()[n] + prgs*beforeState.getPtr()[n];
        }
        
        ofPushMatrix();
        ofMultMatrix(blendMtx);
        ofSetColor(ofGetStyle().color,255*(1-prgs));
        ofRect(0,0,0,100,100);
        ofPopMatrix();
    }
        
    } else {
        ofPushMatrix();
        ofMultMatrix(curMtx);
        ofSetColor(ofGetStyle().color);
        ofRect(0,0,0,100,100);
        ofPopMatrix();

    }

}

