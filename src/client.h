#pragma once

#include "ofMain.h"
#include "ofxGui.h"

#include "ofxOscParameterSyncPlus.h"

class Client {

    public: // methods

        Client();
        void setup(int in_port, string out_host, int out_port);
        void update();
        void draw();

    public: // params
    
        ofParameterGroup params;
        ofParameter<float> sizeParam;

    private: // attributes
    
        ofxPanel gui;
        ofxOscParameterSyncPlus sync;

};
