#include "Receiver.h"

using namespace ofxOscPlus;

bool Receiver::updateParameter(ofParameterGroup & group, const ofxOscMessage& msg){
    ofAbstractParameter * p = paramForMessage(group, msg);

    if(!p)
        return false;

    string typ = p->type();

    if(typ==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
        p->cast<int>() = msg.getArgAsInt32(0);
        return true;
    }

    if(typ==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
        p->cast<float>() = msg.getArgAsFloat(0);
        return true;
    }

    if(typ==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
        p->cast<double>() = msg.getArgAsDouble(0);
        return true;
    }

    if(typ==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
                                                     || msg.getArgType(0)==OFXOSC_TYPE_FALSE
                                                     || msg.getArgType(0)==OFXOSC_TYPE_INT32
                                                     || msg.getArgType(0)==OFXOSC_TYPE_INT64
                                                     || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                     || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                     || msg.getArgType(0)==OFXOSC_TYPE_STRING
                                                     || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
        p->cast<bool>() = msg.getArgAsBool(0);
        return true;
    }

    if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
        p->fromString(msg.getArgAsString(0));
        return true;
    }

    ofLogWarning() << "unknown param type; " << typ;
    return false;
}

ofAbstractParameter* Receiver::paramForMessage(ofParameterGroup & group, const ofxOscMessage& message){
    std::vector<std::string> address_parts = ofSplitString(message.getAddress(),"/",true);
    
    if(address_parts[0] != group.getEscapedName())
        return NULL;

    
    ofAbstractParameter * p = &group;
    ofParameterGroup * pGroup;

    for(auto it=address_parts.begin()+1; it!=address_parts.end(); it++){
        if(p->type() != typeid(ofParameterGroup).name())
            return NULL;
        
        pGroup = (ofParameterGroup *)p;

        if(!pGroup->contains(*it))
            return NULL;

        p = &pGroup->get(*it);
    }

    return p;
}

//bool ofxOscReceiver::getParameter(ofAbstractParameter & parameter)
//{
//    ofxOscMessage msg;
//
//    while(messagesChannel.tryReceive(msg)){
//        
//        ofAbstractParameter * p = &parameter;
//        std::vector<std::string> address = ofSplitString(msg.getAddress(),"/",true);
//
//        for(unsigned int i=0;i<address.size();i++){
//            if(p) {
//                if(address[i]==p->getEscapedName()){
//                    if(p->type()==typeid(ofParameterGroup).name()){
//                        if(address.size()>=i+1){
//                            p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
//                        }
//                    }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
//                        p->cast<int>() = msg.getArgAsInt32(0);
//                    }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
//                        p->cast<float>() = msg.getArgAsFloat(0);
//                    }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
//                        p->cast<double>() = msg.getArgAsDouble(0);
//                    }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FALSE
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT32
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_INT64
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_STRING
//                                                                             || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
//                        p->cast<bool>() = msg.getArgAsBool(0);
//                    }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
//                        p->fromString(msg.getArgAsString(0));
//                    }
//                }
//            }
//        }
//    }
//    return true;
//}
