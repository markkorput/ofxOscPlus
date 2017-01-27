#include "ofMain.h"
#include "ofApp.h"

#ifdef DEBUG
    #include "ofTestApp.h"
    #include "ofAppNoWindow.h"
#endif

//========================================================================
int main( ){
#ifdef DEBUG
    if(std::getenv("TEST")){
        ofInit();
        auto window = make_shared<ofAppNoWindow>();
        auto app = make_shared<ofTestApp>();
        ofRunApp(window, app);
        return ofRunMainLoop();
    }
#endif

	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
