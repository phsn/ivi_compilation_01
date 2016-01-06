//
//  lineScape.cpp
//  linescape_01
//
//  Created by Philipp Sniechota on 20.09.15.
//
//

#include "lineScape.h"

lineScape::lineScape(){}

lineScape::lineScape(int numLines, int maxZ){
    
    speed = 50;
    maximumZ = maxZ;
    spacing = maxZ/numLines;
    numObj = numLines;
    
    
    line tempLine;
    for(int i=0; i < numObj; i++) {
        tempLine = line(numPoints);
        //tempLine.setPositions(pointList);
        tempLine.setTranslation(ofPoint(0,0,i*spacing));
        lineList.push_back(tempLine);
    }
    
}

void lineScape::update() {
    
    if(!noDRAW) {
    
    int segCount = lineList[0].getSegmentCount();
    for(int j=0; j < segCount; j++) {
        float fIdx = ofMap(j,0,segCount/2,0,myFFT.size());
        if(j >= floor((segCount-1)/2.0f)) {
            // RECHTE SEITE
            fIdx = fIdx-myFFT.size();
        } else if(j == floor((segCount-1)/2.0f)-1) {
            fIdx = 1;
        }else {
            // LINKE SEITE
            fIdx = myFFT.size() - 1 - fIdx;
            //if(fIdx < 1) fIdx = 0;
        }
        int idx = floor(fIdx);
        double prgs = CosineInterpolate(myFFT[idx],myFFT[idx+1],fIdx-idx);
        lineList[lineList.size()-1].easePointOffset(j, ofVec3f(0,prgs*10000,0)*lineList[lineList.size()-1].getPointNormal(j), 0.1);
    }
    
    lineList[lineList.size()-1].bend(bendFact);
    
        if(!updateAllLines) {
            if(skipLoops == 0) {
                for(int i=0; i < lineList.size()-1; i++) {
                    lineList[i].setPointOffsets(lineList[i+1].getPointOffsets());
                    lineList[i].setPositions(lineList[i+1].getInitPos());
                }
            }
        } else {
            for(int i=0; i < lineList.size()-1; i++) {
                lineList[i].setPointOffsets(lineList[lineList.size()-1].getPointOffsets());
                lineList[i].setPositions(lineList[lineList.size()-1].getInitPos());
            }
            updateAllLines = false;
        }
        
    skipLoops = (skipLoops+1)%2;
    
    for(int i=0; i < numObj; i++) {
        lineList[i].move(ofPoint(0,0,speed),numObj*spacing);
        //lineList[i].bend(ofGetMouseX()/float(ofGetWidth()));
        //lineList[i].bend(1.005);
        //lineList[i].downres(ofGetMouseX()/float(ofGetWidth()));
        
        //lineList[i].setWave(700,0.3,1,lineList[i].getProgress()*1.5);
        
        lineList[i].update();
    }
        
    }
}


void lineScape::draw() {
    
    ofPushMatrix();
    ofTranslate(0,bendFact*lineList[0].length/TWO_PI,maximumZ/2);
    if(!noDRAW) {
        //speed = ofMap(ofGetMouseX(),0,ofGetWidth(),0,200);
        speed = 0;
        //cout << speed << endl;
        for(int i=0; i < numObj; i++) {
            lineList[i].draw();
        }
    }
    ofPopMatrix();
}

float lineScape::getSpacing() {
    return spacing;
}

void lineScape::setFFTData(vector<float> FFTData) {
    myFFT.clear();
    for(int i=0; i < FFTData.size(); i++) {
        myFFT.push_back(FFTData[i]);
    }
    myFFT.erase( myFFT.begin() );
}

void lineScape::updateLineCount(int numLines) {
    noDRAW = true;
    spacing = maximumZ/numLines;
    numObj = numLines;
    lineList.clear();
    
    line tempLine;
    for(int i=0; i < numObj; i++) {
        tempLine = line(numPoints);
        //tempLine.setPositions(pointList);
        tempLine.setTranslation(ofPoint(0,0,i*spacing));
        lineList.push_back(tempLine);
    }
    while(lineList.size() != numLines) {}
    noDRAW = false;
}

void lineScape::updateAll() {
    updateAllLines = true;
}


void lineScape::updateMaxZ(int dist) {
    maximumZ = dist;
    spacing = maximumZ/numObj;
    for(int i=0; i < numObj; i++) {
        lineList[i].setTranslation(ofPoint(0,0,i*spacing));
    }
}

void lineScape::updateBend(float bFact) {
    bendFact = bFact;
}

void lineScape::updateMixer(float fftStrength,float noiseStrength) {
    for(int i=0; i < numObj; i++) {
        lineList[i].fftAmt = fftStrength;
        lineList[i].noiseAmt = noiseStrength;
    }
}


void lineScape::downresAll(float dFact) {
    for(int i=0; i < numObj; i++) {
        lineList[i].downres(dFact);
    }
}

double lineScape::CosineInterpolate (
                         double y1,double y2,
                         double mu)
{
    double mu2;
    
    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
}
