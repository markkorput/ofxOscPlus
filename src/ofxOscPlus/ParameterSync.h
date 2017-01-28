#pragma once

// ofxOsc
#include "ofxOscSender.h"
// ofxOscPlus
#include "Receiver.h"

namespace ofxOscPlus {
    
    class ParameterSync {
        
    public: // methods

        ParameterSync();
        ~ParameterSync();

        void setup(ofParameterGroup & group, int localPort, const std::string& remoteHost, int remotePort);
        void update();

    private: // methods
        
        void registerCallbacks(bool _register=true);

    private: // callbacks methods

        void onParameterChanged( ofAbstractParameter & parameter );

    private: // attributes

        ofxOscSender sender;
        Receiver receiver;
        ofParameterGroup syncGroup;
        bool bUpdatingParameter;
    };

}
