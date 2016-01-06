//
//  line.cpp
//  linescape_01
//
//  Created by Philipp Sniechota on 20.09.15.
//
//

#include "line.h"

line::line(){}

line::line(int segments){
    segCount = segments;
    waveAmp, waveFreq, waveSpeed, waveOffset = 0;
    
    length = ofGetWidth()*50;
    
    translation = ofPoint(0,0,0);
    
    //lineMesh.setMode(OF_PRIMITIVE_LINES);
    lineMesh.setMode(OF_PRIMITIVE_POINTS);
    
    
    for(int i=0; i < segCount; i++) {
        initPos.push_back(ofPoint(i*(length/float(segCount)),0,0));
        
        drawPos.push_back(ofPoint(0,0,0));
        waveV.push_back(ofPoint(0,0,0));
        noiseV.push_back(ofPoint(0,0,0));
        pointOffsets.push_back(ofPoint(0,0,0));
        
        lineMesh.addVertex(ofPoint(0,0,0));
        //if(i < (segCount-1)) {
            lineMesh.addIndex(i);
            //lineMesh.addIndex(i+1);
        //ß}
    }
    
    this->bend(ofGetMouseX()/float(ofGetWidth()));
    this->update();
}

//--------------------------------------------------------------
void line::update(){
    float time = ofGetElapsedTimef() + waveOffset;
    for (int i=0; i < segCount; i++) {
        
        ofPoint normal;
        ofPoint AB;
        
        // NORMAL VECTOR CALCULATION
        if (i < segCount-1) {
            AB = initPos[i+1]-initPos[i];
            AB.normalize();
            normal = ofPoint(AB.y,-AB.x,AB.z);
        }
        
        waveVal = waveAmp*sin((i*waveFreq)+time*M_PI*2*waveSpeed);
        waveV[i] = waveVal*normal;
        
        noiseVal = 1000*2*(ofNoise( i/float(segCount)*8.0,
                                   translation.z/zLimit*2-ofGetElapsedTimef()/2.0,
                                   ofGetElapsedTimef()/5.0)-0.5);

        noiseV[i] = noiseVal*normal*noiseAmt;
        
        drawPos[i] = initPos[i] + pointOffsets[i]*fftAmt + waveV[i] + noiseV[i] + translation;
        
        /*
         for i in xrange(len(initObjects)):
         val = 10*math.sin(-time+freq*i/len(initObjects)*2*math.pi)
         if i < len(initObjects)-1:
         pA = initObjects[i].GetRelPos()
         pB = initObjects[i+1].GetRelPos()
         AB = pB-pA
         AB.Normalize()
         vec = c4d.Vector(AB.x,-AB.z,AB.y)
         #vec = c4d.Vector(0,1,0)
         offset = vec*val;
         objects[i].SetRelPos(initObjects[i].GetRelPos()+offset)
         */
    }
}

//--------------------------------------------------------------
void line::bend(float bendFactor){
    bendFact = bendFactor*2*M_PI;
    
    int cIndex = segCount/2;
    ofPoint center = ofPoint((cIndex-1)*length/float(segCount),0,0);
    initPos[cIndex] = ofPoint(center.x,center.y,center.z);
    
    float x,y,z;
    
    ofPoint pos = ofPoint(center.x,center.y,center.z);
    
    //cout << "start part 1" << endl;
    for (int i=0; i < cIndex; i++) {
        x = -(length/float(segCount))*cos(2*M_PI-i*bendFact/float(segCount)) + pos.x;
        y =  (length/float(segCount))*sin(2*M_PI-i*bendFact/float(segCount)) + pos.y;
        z = 0 + pos.z;
        initPos[cIndex-i-1] = ofPoint(x,y,z);
        pos = ofPoint(x,y,z);
        //cout << "Index No. " << cIndex-i-1 << " with Position " << pos << endl;
    }
    //cout << "end part 1" << endl;
    
    pos = ofPoint(center.x,center.y,center.z);
    
    //cout << "start part 2" << endl;
    for (int i=0; i < cIndex-1; i++) {
        x = (length/float(segCount))*cos((i+1)*bendFact/float(segCount)) + pos.x;
        y = -(length/float(segCount))*sin((i+1)*bendFact/float(segCount)) + pos.y;
        z = 0 + pos.z;
        initPos[cIndex+i+1] = ofPoint(x,y,z);
        pos = ofPoint(x,y,z);
        //cout << "Index No. " << cIndex+i+1 << " with Position " << pos << endl;
    }
    //cout << "end part 2" << endl;
}

//--------------------------------------------------------------
void line::downres(float downresFactor){
    downresFact = downresFactor;
}

//--------------------------------------------------------------
void line::draw(){
    int index;
    for (int i=0; i < segCount; i++) {
        float alpha = translation.z/(float)zLimit*255.0;
        // alpha *= 1.0-0.5*(1.0+cos(M_2_PI*10*i/float(segCount)));
        
        ofSetLineWidth(3);
        ofSetColor(ofColor(255,255,255,alpha));
        
        int step = int(ofMap(downresFact,0,1,1,29,true));
        //step = (step < 1) ? 1 : step;
        //step = (step > 20) ? 20 : step;
        
        if(downresFact > 0) {
            if(i%step == 1) {
                index = i;
            }
        } else {
            index = i;
        }
        lineMesh.setVertex(i, drawPos[index]);
        
    }
    glPointSize(2);
    lineMesh.draw();
    
}

void line::move(ofPoint movingVector, float limitZ) {
    translation  = translation + movingVector;
    translation.z = fmod(translation.z,limitZ);
    zLimit = limitZ;
}

float line::getProgress() {
    return translation.z/zLimit;
}

//
//--------------------------------------------------------------
//   GETTERS AND SETTERS
//--------------------------------------------------------------
//

void line::setPositions(vector<ofPoint> points) {
    if(points.size() == segCount) {
        initPos.clear();
        for(int i=0; i < segCount; i++) {
            initPos.push_back(points[i]);
        }
    } else {
        cout << "Error in line.cpp:" << endl
             << "Can't update positions - point count differs" << endl;
    }
}

void line::setPointOffsets(vector<ofPoint> offsets) {
    if(offsets.size() == segCount) {
        pointOffsets.clear();
        for(int i=0; i < segCount; i++) {
            pointOffsets.push_back(offsets[i]);
        }
    } else {
        cout << "Error in line.cpp:" << endl
        << "Can't update offsets - point count differs" << endl;
    }
}

void line::setPointOffset(int idx, ofVec3f offset) {
    if(idx >= 0 && idx < pointOffsets.size()) {
        pointOffsets[idx] = offset;
    } else {
        cout << "Error in line.cpp:" << endl
        << "Can't update Single Point - index out of range" << endl;

    }
}

void line::easePointOffset(int idx, ofVec3f offset, float easeAmount) {
    if(idx >= 0 && idx < pointOffsets.size()) {
        pointOffsets[idx] += (offset-pointOffsets[idx])*easeAmount;
    } else {
        cout << "Error in line.cpp:" << endl
        << "Can't ease Single Point - index out of range" << endl;
        
    }
}

vector<ofPoint> line::getPointOffsets() {
    return pointOffsets;
}

ofVec3f line::getPointNormal(int idx) {
    
    ofPoint normal;
    ofPoint AB;
    
    // NORMAL VECTOR CALCULATION
    if (idx < segCount-1) {
        AB = initPos[idx+1]-initPos[idx];
        AB.normalize();
        normal = ofPoint(AB.y,-AB.x,AB.z);
    }
    
    return normal;
}


void line::setTranslation(ofPoint newPosition) {
    translation = newPosition;
}

void line::setWave(float amp, float freq, float speed) {
    this->setWave(amp, freq, speed, 0);
}

void line::setWave(float amp, float freq, float speed, float offset) {
    waveAmp = amp, waveFreq = freq;
    waveSpeed = speed, waveOffset = offset;
}

vector<ofPoint> line::getInitPos() {
    return initPos;
}

vector<ofPoint> line::getDrawPos() {
    return drawPos;
}

ofPoint line::getTranslation() {
    return translation;
}

//--------------------------------------------------------------
int line::getSegmentCount(){
    return segCount;
}