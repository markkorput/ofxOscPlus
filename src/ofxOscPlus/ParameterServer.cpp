#include "ParameterServer.h"

using namespace ofxOscPlus;

ParameterServer::ParameterServer() : bUpdating(false){
    
}

void ParameterServer::setup(ofParameterGroup &paramGroup, int port, int limit){
    parameterGroup = &paramGroup;
    nPort = port;
    nLimit = limit;

    registerCallbacks();
    receiver.setup(nPort);
}


void ParameterServer::update(){
    bUpdating = true;

    ofxOscMessage msg;

    for(int i=0; i<nLimit; i++){
        if(!receiver.hasWaitingMessages())
            break;

        if(!receiver.getNextMessage(msg))
            break;

        if(receiver.updateParameter(*parameterGroup, msg))
            continue;

        if(msg.getAddress() == "/layout/please"){
            ofLog() << "TODO: send layout";
        }

        // TODO; Layout stuff?
    }
    
    bUpdating = false;

}

void ParameterServer::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(parameterGroup->parameterChangedE(), this, &ParameterServer::onParameterChanged);
    } else {
        ofRemoveListener(parameterGroup->parameterChangedE(), this, &ParameterServer::onParameterChanged);
    }
}

void ParameterServer::onParameterChanged( ofAbstractParameter & parameter ){
    if(bUpdating) return;
    // sender.sendParameter(parameter);
}

