#pragma once

// ofxOsc
#include "ofxOscSender.h"
// ofxOscPlus
#include "ofxOscReceiverPlus.h"

class ofxOscParameterSyncPlus {
    
public: // methods

	ofxOscParameterSyncPlus();
	~ofxOscParameterSyncPlus();

    void setup(ofParameterGroup & group, int localPort, const std::string& remoteHost, int remotePort);
	void update();

private: // methods
    
    void registerCallbacks(bool _register=true);

private: // callbacks methods

	void onParameterChanged( ofAbstractParameter & parameter );

private: // attributes

	ofxOscSender sender;
	ofxOscReceiverPlus receiver;
	ofParameterGroup syncGroup;
	bool bUpdatingParameter;
};
