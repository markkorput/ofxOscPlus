#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // setup server
    serverParams.setName("TestParams");
    serverParams.add(sizeParam.set("size", 5.0f));
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
    serverParams.add(param->set("Dynamic Param #" + ofToString(dynamicParams.size()+1), ofRandom(10.0f)));
    dynamicParams.push_back(param);
    setupServerGui();
}

void ofApp::onAddClientBtn(){
    int effectiveWidth = ofGetWidth() - 200;
    int newClientIndex = clients.size();
    int guiIndex = newClientIndex+1;
    int guiPos = (guiIndex+1) * 150.0f;
    float guiY = 10.0f + guiPos/effectiveWidth * 150.0f;
    float guiX = 200.0f + guiPos % effectiveWidth;

    shared_ptr<Client> client = make_shared<Client>();
    client->setup(8085+newClientIndex, guiX, guiY);
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
