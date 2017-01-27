#include "Layout.h"

using namespace ofxOscPlus;


Layout::Layout(){
    
}

void Layout::setup(const ofParameterGroup &rootGroup){
    this->rootGroup = &rootGroup;
}

string Layout::serialize(){
    return "";
}

bool Layout::deserialize(string layout){
    return true;
}
