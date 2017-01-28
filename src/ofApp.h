#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscPlus/ParameterServer.h"
#include "ofxOscPlus/ParameterClient.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void setupServerGui();

    private: // callbacks
    
        void onLayoutUpdate(ofxOscPlus::ParameterClient & client);
        void onAddParamBtn();
        void onSyncBtn();

    private: // attributes
    
        ofParameterGroup serverParams, clientParams;
        ofParameter<float> sizeParam;
        ofxPanel serverGui, clientGui;
        vector<shared_ptr<ofAbstractParameter>> dynamicParams;
    
        ofxOscPlus::ParameterServer server;
        ofxOscPlus::ParameterClient client;
};
