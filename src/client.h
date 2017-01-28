#pragma once

// OF
#include "ofParameterGroup.h"
// ofxGui
#include "ofxGui.h"
// ofxOscPlus
#include "ofxOscPlus/ParameterClient.h"

class Client {

    public: // methods

        Client();
        void setup(int port=8085, float x=10.0f, float y=10.0f);
        void update();
        void draw();

    public: // params
    
        ofParameterGroup params;

    private: // callback methods
    
    void onLayoutUpdate(ofxOscPlus::ParameterClient & client);

    private: // attributes
    
        ofxPanel gui;
        ofxOscPlus::ParameterClient client;
        float x, y;
};
