#pragma once

// OF
#include "ofParameterGroup.h"
// addons
#include "ofxGui.h"
#include "ofxOscPlus.h"

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
        void onCloseBtn();

    private: // attributes

        ofxPanel gui;
        ofxButton closeBtn;
        ofxOscPlus::ParameterClient client;
        float x, y;
};
