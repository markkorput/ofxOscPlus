#include "ofxOscParameterSyncPlus.h"

ofxOscParameterSyncPlus::ofxOscParameterSyncPlus() {
	bUpdatingParameter = false;
}

ofxOscParameterSyncPlus::~ofxOscParameterSyncPlus(){
    registerCallbacks(false);
}


void ofxOscParameterSyncPlus::setup(ofParameterGroup & group, int localPort, const std::string& host, int remotePort){
    if(localPort == remotePort){
        ofLogWarning() << "ofxOscParameterSyncPlus::setup got same localPort as remotePort";
    }

	syncGroup = group;
    registerCallbacks();
	
	sender.setup(host, remotePort);
	receiver.setup(localPort);
}

void ofxOscParameterSyncPlus::update(){
	if(receiver.hasWaitingMessages()){
		bUpdatingParameter = true;
		receiver.getParameter(syncGroup);
		bUpdatingParameter = false;
	}
}

void ofxOscParameterSyncPlus::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(syncGroup.parameterChangedE(),this,&ofxOscParameterSyncPlus::onParameterChanged);
    } else {
        ofRemoveListener(syncGroup.parameterChangedE(),this,&ofxOscParameterSyncPlus::onParameterChanged);
    }
}

void ofxOscParameterSyncPlus::onParameterChanged( ofAbstractParameter & parameter ){
	if(bUpdatingParameter) return;
	sender.sendParameter(parameter);
}
