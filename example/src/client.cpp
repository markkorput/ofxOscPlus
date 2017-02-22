#include "client.h"

Client::Client(){
    params.setName("ParamClient");
}

void Client::setup(int port, float x, float y){
    this->x = x;
    this->y = y;

    gui.setup(params, "", x, y);
    client.setup(params, port);

    ofAddListener(client.layoutUpdateEvent, this, &Client::onLayoutUpdate);
    closeBtn.addListener(this, &Client::onCloseBtn);
}

void Client::update(){
    client.update();
}

void Client::draw(){
    gui.draw();
}

void Client::onLayoutUpdate(ofxOscPlus::ParameterClient & client){
    gui.setup(params, "", x, y);
    gui.add(closeBtn.setup("close client"));
}

void Client::onCloseBtn(){
    client.disconnect();
}
