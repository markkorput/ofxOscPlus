//--------------------------------------------------------------
// ofApp.h
//--------------------------------------------------------------

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOscPlus.h"
#include "client.h"

class ofApp : public ofBaseApp{

public: // common OF methods

    void setup();
    void update();
    void draw();

    void keyPressed(int key);

private: // methods

    void setupServerGui();

private: // callbacks

    void onLayoutUpdate(ofxOscPlus::ParameterClient & client);
    void onAddParamBtn();
    void onAddClientBtn();

private: // attributes

    // params
    ofParameterGroup serverParams, subGroup, clientParams;
    ofParameter<float> sizeParam;
    ofParameter<bool> bParam;
    ofParameter<ofPoint> pointParam;
    ofParameter<ofColor> colorParam;
    ofParameter<string> strParam;

    vector<shared_ptr<ofAbstractParameter>> dynamicParams;

    // gui
    ofxPanel serverGui;

    // ofxOscPlus demo; server/clients
    ofxOscPlus::ParameterServer server;
    vector<shared_ptr<Client>> clients;
};

//--------------------------------------------------------------
// ofApp.cpp
//--------------------------------------------------------------

void ofApp::setup(){
    ofLogToFile("log.txt");
    ofSetWindowTitle("ofxOscPlus - example");
    ofSetWindowShape(600,400);
    ofSetWindowPosition(5,10);

    // setup server
    serverParams.setName("TestParams");
    serverParams.add(sizeParam.set("size", 5.0f));
    serverParams.add(strParam.set("name", "undefined"));
    subGroup.setName("SubParams");
    subGroup.add(bParam.set("yesno", false));
    subGroup.add(pointParam.set("pnt", ofPoint(0.0f)));
    subGroup.add(colorParam.set("clr", ofColor::green));

    serverParams.add(subGroup);

    server.setup(serverParams);
    setupServerGui();
}

//--------------------------------------------------------------
void ofApp::update(){
    server.update();

    for(auto client : clients){
        client->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    serverGui.draw();
    ofDrawBitmapStringHighlight("Press;\n1 - to add a param\n2 - to create a new client", 10, 20);
    server.drawDebug(10, 70);

    for(auto client : clients){
        client->draw();
    }
}

void ofApp::onAddParamBtn(){
    shared_ptr<ofParameter<float>> param = make_shared<ofParameter<float>>();
    serverParams.add(param->set("DynamicParamNo" + ofToString(dynamicParams.size()+1), ofRandom(10.0f)));
    dynamicParams.push_back(param);
    setupServerGui();
}

void ofApp::onAddClientBtn(){
    shared_ptr<Client> client = make_shared<Client>();
    client->setup(8085+clients.size(), ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    clients.push_back(client);
}

void ofApp::setupServerGui(){
    serverGui.setup(serverParams, "", 250.0f, 10.0f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1'){
        onAddParamBtn();
        return;
    }

    if(key == '2'){
        onAddClientBtn();
    }
}

//--------------------------------------------------------------
// main.cpp
//--------------------------------------------------------------

int main( ){
    ofSetupOpenGL(1024,768,OF_WINDOW);
    ofRunApp(new ofApp());
}
