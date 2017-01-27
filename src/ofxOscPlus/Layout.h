#pragma once

#include "ofParameterGroup.h"

namespace ofxOscPlus {
    
    class Layout {
    public: // methods
        
        Layout();
        void setup(const ofParameterGroup &rootGroup);
        
        string serialize();
        bool deserialize(string layout);

    private: // attributes
        
        const ofParameterGroup *rootGroup;
    };
}
