#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ivi_OSC : public ofThread
{
public:
    
    ~ivi_OSC()
    {
        if (isThreadRunning()) stopThread();
    }
    
    void setup(std::string host,int port)
    {
        receiver.setup(port);
        sender.setup(host, port);
    }
    
    void start()
    {
        startThread();
    }
    
    void stop()
    {
        stopThread();
    }
    
    void threadedFunction()
    {
        while (isThreadRunning())
        {
            if (lock())
            {
                while (receiver.hasWaitingMessages())
                {
                    if (receiver.getNextMessage(&msg))
                    {
                        ofxOscMessage m = msg;
                        ofNotifyEvent(messageReceived, m, this);
                    }
                    
                }
            }
            sleep(10);
            unlock();
        }
    }
    
    void sendFloat(std::string adress, float value) {
        ofxOscMessage m;
        m.setAddress(adress);
        m.addFloatArg(value);
        sender.sendMessage(m);
    }
    
    void sendBools(std::string adress, vector<bool> values) {
        ofxOscMessage m;
        m.setAddress(adress);
        for(int i=0; i < values.size(); i++) {
            m.addFloatArg((values[i] == true) ? 1.0f : 0.0f);
        }
        sender.sendMessage(m);
    }
    
    ofEvent<ofxOscMessage> messageReceived;
    
private:
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    ofxOscMessage msg;
    
};