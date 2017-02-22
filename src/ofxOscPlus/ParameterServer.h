#pragma once

// OF
#include "ofParameterGroup.h"
// ofxOscPlus
#include "Receiver.h"
#include "Sender.h"

namespace ofxOscPlus {
    
    class ParameterServer {

    public: // methods

        ParameterServer();
        void setup(ofParameterGroup & paramGroup, int port=8081, int limit=10);
        void update();
        void drawDebug(int x=0, int y=0);
        
        int getClientCount(){ return senders.size(); }

    private: // methods
        
        void registerCallbacks(bool _register=true);
        ofAbstractParameter* paramForMessage(const ofxOscMessage& message);

        shared_ptr<Sender> getSender(const string &host, int port);
        void signup(const string &host, int port);
        bool signoff(const string &host, int port);
        bool signoff(shared_ptr<Sender> sender);
        void sendLayout(const string &host, int port);

    private: // callback methods
        
        void onParameterChanged( ofAbstractParameter & parameter );

    private: // attributes
        
        int nPort, nLimit;
        Receiver receiver;
        vector<shared_ptr<Sender>> senders;
        shared_ptr<Sender> updatingSender;
        ofParameterGroup *parameterGroup;
    };
}
