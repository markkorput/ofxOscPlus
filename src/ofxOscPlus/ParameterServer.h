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
        void setup(ofParameterGroup & paramGroup, int port, int limit=10);
        void update();
        
        int getClientCount(){ return senders.size(); }

    private: // methods
        
        void registerCallbacks(bool _register=true);
        ofAbstractParameter* paramForMessage(const ofxOscMessage& message);
        shared_ptr<Sender> getSender(string host, int port);

    private: // callback methods
        
        void onParameterChanged( ofAbstractParameter & parameter );

    private: // attributes
        
        int nPort, nLimit;
        Receiver receiver;
        vector<shared_ptr<Sender>> senders;
        shared_ptr<Sender> updatingSender;
        ofParameterGroup *parameterGroup;
        bool bUpdating;
    };
}
