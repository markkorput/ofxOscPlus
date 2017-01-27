#include "ParameterServer.h"

#include "Layout.h"

using namespace ofxOscPlus;

ParameterServer::ParameterServer() : updatingSender(nullptr){
    
}

void ParameterServer::setup(ofParameterGroup &paramGroup, int port, int limit){
    parameterGroup = &paramGroup;
    nPort = port;
    nLimit = limit;

    registerCallbacks();
    receiver.setup(nPort);
}

void ParameterServer::update(){
    ofxOscMessage msg;

    for(int i=0; i<nLimit; i++){
        if(!receiver.hasWaitingMessages())
            break;

        if(!receiver.getNextMessage(msg))
            break;

        if(msg.getAddress() == "/ofxOscPlus/signup" && msg.getNumArgs() == 2){
            // create sender instance
            shared_ptr<Sender> sender = make_shared<Sender>();
            // configure sender instance to send to client who's host/port data we just received
            sender->setup(msg.getArgAsString(0), msg.getArgAsInt(1));
            // stores in our senders vector
            senders.push_back(sender);
            // TODO; Layout stuff?
            ofLog() << "ofxOscPlus client signed up. TODO: send layout to signed up client";
            continue;
        }

        if(msg.getAddress() == "/ofxOscPlus/signoff" && msg.getNumArgs() == 2){
            // create sender instance
            shared_ptr<Sender> sender = getSender(msg.getArgAsString(0), msg.getArgAsInt(1));

            if(sender == nullptr)
                continue;
            
            for(auto it=senders.begin(); it!=senders.end(); it++){
                if(it->get() == sender.get()){
                    senders.erase(it);
                    ofLog() << "ofxOscPlus client signed off (host: " << sender->getHost() << ", port: " << sender->getPort() << ")";
                    continue;
                }
            }
        }

        if(msg.getAddress() == "/ofxOscPlus/layout" && msg.getNumArgs() == 2){
            // create layout payload in json-format
            Layout layout;
            layout.setup(*parameterGroup);
            string layoutJsonText = layout.toJsonText();

            // create osc response message
            ofxOscMessage responseMsg;
            responseMsg.setAddress("/ofxOscPlus/layout");
            responseMsg.addStringArg(layoutJsonText);

            // send message
            Sender sender;
            sender.setup(msg.getArgAsString(0), msg.getArgAsInt(1));
            sender.sendMessage(responseMsg, false);
        }

        updatingSender = getSender(msg.getRemoteIp(), msg.getRemotePort());
        receiver.updateParameter(*parameterGroup, msg);
        updatingSender = nullptr;
    }
}

void ParameterServer::registerCallbacks(bool _register){
    if(_register){
        ofAddListener(parameterGroup->parameterChangedE(), this, &ParameterServer::onParameterChanged);
    } else {
        ofRemoveListener(parameterGroup->parameterChangedE(), this, &ParameterServer::onParameterChanged);
    }
}

void ParameterServer::onParameterChanged( ofAbstractParameter & parameter ){
    for(auto sender : senders){
        // don't send to this sender if we just received this param update from this sender
        if(updatingSender == nullptr || sender.get() != updatingSender.get())
            sender->sendParameter(parameter);
    }
}

shared_ptr<Sender> ParameterServer::getSender(string host, int port){
    for(auto sender : senders){
        if(sender->getHost() == host && sender->getPort() == port)
            return sender;
    }
    
    return nullptr;
}
