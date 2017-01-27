#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxOscPlus/ParameterSync.h"

class Client {

    public: // methods

        Client();
        void setup(int in_port, string out_host, int out_port, float x=0.0f, float y=0.0f);
        void update();
        void draw();

    public: // params
    
        ofParameterGroup params;
        ofParameter<float> sizeParam;

    private: // attributes
    
        ofxPanel gui;
        ofxOscPlus::ParameterSync sync;
};
