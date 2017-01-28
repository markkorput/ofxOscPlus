#include "ParameterClient.h"
#include "Layout.h"

using namespace ofxOscPlus;

ParameterClient::ParameterClient() : bUpdating(false){
}

void ParameterClient::setup(ofParameterGroup &paramGroup, int port, int limit, string serverHost, int serverPort){
    parameterGroup = &paramGroup;
    nPort = port;
    nLimit = limit;
    
    sender.setup(serverHost, serverPort);
    receiver.setup(nPort);

    registerCallbacks();
    signup();
    requestLayout();
}

void ParameterClient::update(){
    ofxOscMessage msg;
    
    for(int i=0; i<nLimit; i++){
        if(!receiver.hasWaitingMessages())
            break;

        if(!receiver.getNextMessage(msg))
            break;

        // ofLogVerbose() << "client got OSC message: " << msg.getAddress();
        if(msg.getAddress() == "/ofxOscPlus/layout"){
            if(msg.getNumArgs() != 1){
                ofLogWarning() << "expected 1 arg with layout message";
                continue;
            }

            Layout layout;
            layout.setup(*parameterGroup);
            layout.fromJson(msg.getArgAsString(0));
            ofNotifyEvent(layoutUpdateEvent, *this, this);
            continue;
        }

        bUpdating = true;
        receiver.updateParameter(*parameterGroup, msg);
        bUpdating = false;
    }
}

void ParameterClient::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(parameterGroup->parameterChangedE(), this, &ParameterClient::onParameterChanged);
    } else {
        ofRemoveListener(parameterGroup->parameterChangedE(), this, &ParameterClient::onParameterChanged);
    }
}

void ParameterClient::signup(){
    ofxOscMessage msg;
    msg.setAddress("/ofxOscPlus/signup");
    msg.addIntArg(nPort);
    sender.sendMessage(msg, false);
}

void ParameterClient::requestLayout(){
    ofxOscMessage msg;
    msg.setAddress("/ofxOscPlus/layout");
    msg.addIntArg(nPort);
    sender.sendMessage(msg, false);
}

void ParameterClient::onParameterChanged( ofAbstractParameter & parameter ){
    if(bUpdating) return;
    sender.sendParameter(parameter);
}

