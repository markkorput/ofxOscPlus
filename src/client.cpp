#include "client.h"

Client::Client(){
    params.setName("ParamClient");
    params.add(sizeParam.set("size", 5.0f));
}

void Client::setup(int in_port, string out_host, int out_port){
    gui.setup(params);
    sync.setup(params, in_port, out_host, out_port);
}

void Client::update(){
    sync.update();
}

void Client::draw(){
    gui.draw();
}
