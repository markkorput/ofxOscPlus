#pragma once

// ofxOsc
#include "ofxOscSender.h"

namespace ofxOscPlus {
    class Sender : public ofxOscSender {
    
    public: // methods

        void setup( const std::string &hostname, int port );

    public: // getters/setters
        
        const string& getHost(){ return hostCache; }
        int getPort(){ return portCache; }

    private: // attributes
        
        string hostCache;
        int portCache;
    };
}
