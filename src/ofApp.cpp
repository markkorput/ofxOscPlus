#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // setup server
    serverParams.setName("TestParams");
    serverParams.add(sizeParam.set("size", 5.0f));
    server.setup(serverParams);
    serverGui.setup(serverParams);
    
    // setup client
    clientParams.setName("ClientParams");
    client.setup(clientParams);
    onLayoutUpdate(client);

    ofAddListener(client.layoutUpdateEvent, this, &ofApp::onLayoutUpdate);
}

//--------------------------------------------------------------
void ofApp::update(){
    server.update();
    client.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    serverGui.draw();
    clientGui.draw();
}


void ofApp::onAddParamBtn(){
    shared_ptr<ofParameter<float>> param = make_shared<ofParameter<float>>();
    serverParams.add(param->set("Dynamic Param #" + ofToString(dynamicParams.size()+1), ofRandom(10.0f)));
    dynamicParams.push_back(param);
    serverGui.setup(serverParams);
}

void ofApp::onSyncBtn(){
    client.requestLayout();
}

//--------------------------------------------------------------
void ofApp::onLayoutUpdate(ofxOscPlus::ParameterClient & client){
    clientGui.setup(clientParams, "", 400);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == '1'){
        onAddParamBtn();
        return;
    }
    
    if(key == '2'){
        onSyncBtn();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
