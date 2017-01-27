#pragma once

#include "ofMain.h"
// ofxOscPlus
#include "Receiver.h"

namespace ofxOscPlus {
    
    class ParameterServer {
        
    public: // methods

        ParameterServer();
        void setup(ofParameterGroup & paramGroup, int port, int limit=10);
        void update();

    private: // methods
        
        void registerCallbacks(bool _register=true);
        ofAbstractParameter* paramForMessage(const ofxOscMessage& message);

    private: // callback methods
        
        void onParameterChanged( ofAbstractParameter & parameter );

    private: // attributes
        
        int nPort, nLimit;
        Receiver receiver;
        ofParameterGroup *parameterGroup;
        bool bUpdating;
    };
}
