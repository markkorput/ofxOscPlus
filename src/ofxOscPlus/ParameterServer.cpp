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

//        ofLog() << "server got message: " << msg.getAddress();
//        for(int i=0; i<msg.getNumArgs(); i++){
//            ofLog() << "with: " << msg.getArgAsString(i);
//        }

        if(msg.getAddress() == "/ofxOscPlus/signup"){
            if(msg.getNumArgs() == 2)
                signup(msg.getArgAsString(0), msg.getArgAsInt(1));
            else if(msg.getNumArgs() == 1)
                signup(msg.getRemoteIp(), msg.getArgAsInt(0));
            else
                ofLogWarning() << "Expected 1 or 2 arguments with signup OSC request";

            continue;
        }

        if(msg.getAddress() == "/ofxOscPlus/signoff"){
            if(msg.getNumArgs() == 2)
                signoff(msg.getArgAsString(0), msg.getArgAsInt(1));
            else if(msg.getNumArgs() == 1)
                signoff(msg.getRemoteIp(), msg.getArgAsInt(0));
            else
                ofLogWarning() << "Expected 1 or 2 arguments with signoff OSC request";

            continue;
        }

        if(msg.getAddress() == "/ofxOscPlus/layout"){
            if(msg.getNumArgs() == 2)
                sendLayout(msg.getArgAsString(0), msg.getArgAsInt(1));
            else if(msg.getNumArgs() == 1)
                sendLayout(msg.getRemoteIp(), msg.getArgAsInt(0));
            else
                ofLogWarning() << "Expected 1 or 2 arguments with layout OSC request";

            continue;
        }

        updatingSender = getSender(msg.getRemoteIp(), msg.getRemotePort());
        receiver.updateParameter(*parameterGroup, msg);
        updatingSender = nullptr;
    }
}

void ParameterServer::drawDebug(int x, int y){
    string info;
    info = "ofxOscPlus::ParameterServer\n---------------------------";
    info += "\n\nNumber of clients connected: " + ofToString(getClientCount());
    for(auto sender : senders){
        info += "\nClient: " + sender->getHost() + ":" + ofToString(sender->getPort());
    }
    ofDrawBitmapStringHighlight(info, x, y);
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
    shared_ptr<Sender> sender = getSender(host, port);
    
    if(sender != nullptr){
        ofLogWarning() << "Got signup with existing host/port, signing off existing client";
        signoff(host, port);
    }

    // create sender instance
    sender = make_shared<Sender>();
    // configure sender instance to send to client who's host/port data we just received
    sender->setup(host, port);
    // stores in our senders vector
    senders.push_back(sender);
}

bool ParameterServer::signoff(const string &host, int port){
    // create sender instance
    shared_ptr<Sender> sender = getSender(host, port);

    if(sender != nullptr){
        signoff(sender);
        return;
    }

    ofLogWarning() << "Could not find sender instance to signoff (host: " << host << ", port: " << port << ")";
    return false;
}

bool ParameterServer::signoff(shared_ptr<Sender> sender){
    for(auto it=senders.begin(); it!=senders.end(); it++){
        if(it->get() == sender.get()){
            senders.erase(it);
            ofLog() << "ofxOscPlus client signed off (host: " << sender->getHost() << ", port: " << sender->getPort() << ")";
            return true;
        }
    }

    ofLogWarning() << "Could not find sender instance to sign off (host: " << sender->getHost() << ", port: " << sender->getPort() << ")";
    return false;
}

void ParameterServer::sendLayout(const string &host, int port){
    auto sender = getSender(host, port);

    if(sender == nullptr){
        ofLogWarning() << "Couldn't find sender to send layout to";
        return;
    }

    // create layout payload in json-format
    Layout layout;
    layout.setup(*parameterGroup);
    string layoutJsonText = layout.toJsonText();

    // create osc message
    ofxOscMessage responseMsg;
    responseMsg.setAddress("/ofxOscPlus/layout");
    responseMsg.addStringArg(layoutJsonText);
    // ofLog() << layoutJsonText;

    // send message
    sender->sendMessage(responseMsg, false);
}

void ParameterServer::onParameterChanged( ofAbstractParameter & parameter ){
    for(auto sender : senders){
        // don't send to this sender if we just received this param update from this sender
        if(updatingSender == nullptr || sender.get() != updatingSender.get())
            sender->sendParameter(parameter);
    }
}

