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
            signup(msg.getArgAsString(0), msg.getArgAsInt(1));
            continue;
        }

        if(msg.getAddress() == "/ofxOscPlus/signoff" && msg.getNumArgs() == 2){
            signoff(msg.getArgAsString(0), msg.getArgAsInt(1));
            continue;
        }

        if(msg.getAddress() == "/ofxOscPlus/layout" && msg.getNumArgs() == 2){
            sendLayout(msg.getArgAsString(0), msg.getArgAsInt(1));
            continue;
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

shared_ptr<Sender> ParameterServer::getSender(const string &host, int port){
    for(auto sender : senders){
        if(sender->getHost() == host && sender->getPort() == port)
            return sender;
    }
    
    return nullptr;
}

void ParameterServer::signup(const string &host, int port){
    // create sender instance
    shared_ptr<Sender> sender = make_shared<Sender>();
    // configure sender instance to send to client who's host/port data we just received
    sender->setup(host, port);
    // stores in our senders vector
    senders.push_back(sender);
}

bool ParameterServer::signoff(const string &host, int port){
    // create sender instance
    shared_ptr<Sender> sender = getSender(host, port);
    
    if(sender == nullptr)
        return false;
    
    for(auto it=senders.begin(); it!=senders.end(); it++){
        if(it->get() == sender.get()){
            senders.erase(it);
            ofLog() << "ofxOscPlus client signed off (host: " << sender->getHost() << ", port: " << sender->getPort() << ")";
            return true;
        }
    }

    ofLogWarning() << "Could not find client to signoff (host: " << sender->getHost() << ", port: " << sender->getPort() << ")";
    return false;
}

void ParameterServer::sendLayout(const string &host, int port){
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
    sender.setup(host, port);
    sender.sendMessage(responseMsg, false);
}

void ParameterServer::onParameterChanged( ofAbstractParameter & parameter ){
    for(auto sender : senders){
        // don't send to this sender if we just received this param update from this sender
        if(updatingSender == nullptr || sender.get() != updatingSender.get())
            sender->sendParameter(parameter);
    }
}

