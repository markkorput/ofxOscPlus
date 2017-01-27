#include "ParameterSync.h"

using namespace ofxOscPlus;

ParameterSync::ParameterSync() {
	bUpdatingParameter = false;
}

ParameterSync::~ParameterSync(){
    registerCallbacks(false);
}

void ParameterSync::setup(ofParameterGroup & group, int localPort, const std::string& host, int remotePort){
    if(localPort == remotePort){
        ofLogWarning() << "ofxOscPlus::ParameterSync::setup got same localPort as remotePort";
    }

	syncGroup = group;
    registerCallbacks();
	
	sender.setup(host, remotePort);
	receiver.setup(localPort);
}

void ParameterSync::update(){
	if(receiver.hasWaitingMessages()){
		bUpdatingParameter = true;
		receiver.getParameter(syncGroup);
		bUpdatingParameter = false;
	}
}

void ParameterSync::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(syncGroup.parameterChangedE(),this,&ParameterSync::onParameterChanged);
    } else {
        ofRemoveListener(syncGroup.parameterChangedE(),this,&ParameterSync::onParameterChanged);
    }
}

void ParameterSync::onParameterChanged( ofAbstractParameter & parameter ){
	if(bUpdatingParameter) return;
	sender.sendParameter(parameter);
}
