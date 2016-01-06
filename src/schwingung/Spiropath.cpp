
#include "Spiropath.h"

Spiropath::Spiropath(){}
Spiropath::~Spiropath(){}


//--------------------------------------------------------------
void Spiropath::setup(int bigRadius, int smallRadius, int innrRadius){
    bigCircle = bigRadius;
    smallCircle = smallRadius;
    innerRadius = innrRadius;
}

//--------------------------------------------------------------
void Spiropath::update(int bigRadius, int smallRadius, int innrRadius){
    bigCircle = bigRadius;
    smallCircle = smallRadius;
    innerRadius = innrRadius;
}

//--------------------------------------------------------------
void Spiropath::setTone(int newTone){
    innerRadius = ofMap(newTone, 0,87,50,500);
    ratio = ofMap(newTone, 0,87,1,10);
    //bigCircle = ofMap(newTone, 0,87,100,1500);
    
}

//--------------------------------------------------------------
ofVec3f Spiropath::getPos(float percentage, float offset){
    
    percentage = (reverse) ? -percentage: percentage;
    
    float bigMoveX = (bigCircle-smallCircle)*sin(percentage*TWO_PI+offset);
    float bigMoveY = (bigCircle-smallCircle)*cos(percentage*TWO_PI+offset);
    float bigMoveZ = (bigCircle-smallCircle)*sin(percentage*TWO_PI+offset);
    
    float smallMoveX = innerRadius*sin(percentage*TWO_PI*-(1.0f/ratio)*(bigCircle/(smallCircle+0.0001f))+offset);
    float smallMoveY = innerRadius*cos(percentage*TWO_PI*-(1.0f/ratio)*(bigCircle/(smallCircle+0.0001f))+offset);
    float smallMoveZ = innerRadius*cos(percentage*TWO_PI*-(1.0f/ratio)*(bigCircle/(smallCircle+0.0001f))+offset);
    
    float combinedX = scale*(bigMoveX + smallMoveX);
    float combinedY = scale*(bigMoveY + smallMoveY);
    float combinedZ = 0*scale*(bigMoveZ + smallMoveZ);
    
    return ofVec3f(combinedX,combinedY,combinedZ);

}

//--------------------------------------------------------------
ofVec3f Spiropath::getVel(float percentage, float pBefore, float offset, float pOffset){
    return this->getPos(percentage, offset)-this->getPos(pBefore, pOffset);
}