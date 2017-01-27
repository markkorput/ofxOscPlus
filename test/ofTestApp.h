#pragma once

#include "ofMain.h"
#include "ofxUnitTests.h"

class ofTestApp : public ofxUnitTestsApp{
public:

    void run();

private: // methods

    void testLayout();
    void testLayoutSerialize();
    void testLayoutDeserialize();

};
