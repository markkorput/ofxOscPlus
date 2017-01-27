#pragma once

// ofxOsc
#include "ofxOscReceiver.h"

namespace ofxOscPlus{
    class Receiver : public ofxOscReceiver {
    
    public: // methods
        
        bool updateParameter(ofParameterGroup & group, const ofxOscMessage& msg);
        ofAbstractParameter* paramForMessage(ofParameterGroup & group, const ofxOscMessage& message);

    };
}
