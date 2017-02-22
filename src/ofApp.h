#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscPlus/ParameterServer.h"
#include "client.h"

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
        void onAddClientBtn();

    private: // attributes
    
        ofParameterGroup serverParams, subGroup, clientParams;
        ofParameter<float> sizeParam;
        ofParameter<bool> bParam;
        ofParameter<ofPoint> pointParam;
        ofParameter<ofColor> colorParam;
        ofParameter<string> strParam;
        ofxPanel serverGui;
        vector<shared_ptr<ofAbstractParameter>> dynamicParams;
        ofxOscPlus::ParameterServer server;
        vector<shared_ptr<Client>> clients;
};
