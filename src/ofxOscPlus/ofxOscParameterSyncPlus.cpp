#include "ofxOscParameterSyncPlus.h"

//ofxOscParameterSyncPlus::ofxOscParameterSyncPlus() {
//	updatingParameter = false;
//}
//
//ofxOscParameterSyncPlus::~ofxOscParameterSyncPlus(){
//	ofRemoveListener(syncGroup.parameterChangedE(),this,&ofxOscParameterSyncPlus::parameterChanged);
//}
//
//
//void ofxOscParameterSyncPlus::setup(ofParameterGroup & group, int localPort, const std::string& host, int remotePort){
//	syncGroup = group;
//	ofAddListener(syncGroup.parameterChangedE(),this,&ofxOscParameterSyncPlus::parameterChanged);
//	sender.setup(host,remotePort);
//	receiver.setup(localPort);
//}
//
//void ofxOscParameterSyncPlus::update(){
//	if(receiver.hasWaitingMessages()){
//		updatingParameter = true;
//		receiver.getParameter(syncGroup);
//		updatingParameter = false;
//	}
//}
//
//void ofxOscParameterSyncPlus::parameterChanged( ofAbstractParameter & parameter ){
//	if(updatingParameter) return;
//	sender.sendParameter(parameter);
//}
