#pragma once

// OF
#include "ofParameterGroup.h"
#include "ofEvent.h"
// ofxOscPlus
#include "Receiver.h"
#include "Sender.h"

namespace ofxOscPlus {
    
    class ParameterClient {
    
    public: // common methods
        
        ParameterClient();
        ~ParameterClient(){ destroy(); }
        void setup(ofParameterGroup & group, int port=8082, int limit=10, string serverHost="127.0.0.1", int serverPort=8081);
        void destroy();

        void update();
        void drawDebug(int x=0, int y=0);

    public: // operation methods

        void requestLayout();
        void disconnect();

    private: // methods
        
        void registerCallbacks(bool _register=true);
        void signup();
        void signoff();
        
    private: // callbacks
        
        void onParameterChanged( ofAbstractParameter & parameter );

    public: // events

        ofEvent<ParameterClient> layoutUpdateEvent;

    private: // attributes

        int nPort, nLimit;
        Receiver receiver;
        Sender sender;
        ofParameterGroup *parameterGroup;
        bool bUpdating;

    };
}
