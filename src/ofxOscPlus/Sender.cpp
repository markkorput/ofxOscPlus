#include "Sender.h"

using namespace ofxOscPlus;

void Sender::setup( const std::string &hostname, int port ){
    ofxOscSender::setup(hostname, port);
    hostCache = hostname;
    portCache = port;
}
