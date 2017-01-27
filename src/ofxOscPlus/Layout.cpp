#include "Layout.h"

using namespace ofxOscPlus;


Layout::Layout() : rootGroup(NULL){
    
}

void Layout::setup(ofParameterGroup &rootGroup){
    this->rootGroup = &rootGroup;
}

bool Layout::toJson(Json::Value &json){
    if(!rootGroup){
        return false;
    }

    Json::Value sub;
    json[rootGroup->getName()] = serialize(*rootGroup, sub);
    return true;
}

Json::Value& Layout::serialize(const ofParameterGroup &group, Json::Value &json){
    for(int i=0; i<(int)group.size(); i++){
        string type = group.getType(i);
        string name = group.getName(i);
        Json::Value subJson;

        if(type == typeid(ofParameterGroup).name()){
            serialize(group.getGroup(i), subJson);
        }

        if(type == typeid(ofParameter<int>).name()){
            serialize(group.getInt(i), subJson);
        }

        if(type == typeid(ofParameter<float>).name()){
            serialize(group.getFloat(i), subJson);
        }

        if(type == typeid(ofParameter<bool>).name()){
            serialize(group.getBool(i), subJson);
        }

        if(type == typeid(ofParameter<ofColor>).name()){
            serialize(group.getColor(i), subJson);
        }

        if(type == typeid(ofParameter<ofPoint>).name()){
            serialize(group.getPoint(i), subJson);
        }

        json[name] = subJson;
    }
    
    return json;
}

Json::Value& Layout::serialize(const ofParameter<float> param, Json::Value &json){
    json["type"] = "float";
    json["value"] = param.get();
    return json;
}

Json::Value& Layout::serialize(const ofParameter<int> param, Json::Value &json){
    json["type"] = "int";
    json["value"] = param.get();
    return json;
}

Json::Value& Layout::serialize(const ofParameter<bool> param, Json::Value &json){
    json["type"] = "bool";
    json["value"] = param.get();
    return json;
}

Json::Value& Layout::serialize(const ofParameter<ofColor> param, Json::Value &json){
    json["type"] = "color";
    json["value"] = ofToString(param.get());
    return json;
}

Json::Value& Layout::serialize(const ofParameter<ofPoint> param, Json::Value &json){
    json["type"] = "point";
    json["value"] = ofToString(param.get());
    return json;
}


bool Layout::fromJson(const string &jsonText){
    ofxJSONElement json;
    
    if(!json.parse(jsonText)){
        ofLogWarning() << "Couldn't parse json: " << jsonText;
        return false;
    }
    
    return fromJson(json);
}

bool Layout::fromJson(const Json::Value &json){
    if(json.isArray()){
        ofLogWarning() << "didn't expect an array";
        return false;
    }
    
    ofParameterGroup* currentGroup = rootGroup;

    for(auto attr : json.getMemberNames()){
        if(json[attr]["type"].type() != Json::nullValue)
            continue;
        
        currentGroup->setName(attr);
        deserialize(json[attr], *currentGroup);
    }
    
    return true;
}

bool Layout::deserialize(const Json::Value& json, ofParameterGroup &group){
    for(auto attr : json.getMemberNames()){
        
        // no type means it's another sub group
        if(json[attr]["type"].type() == Json::nullValue){
            shared_ptr<ofParameterGroup> newGroup = make_shared<ofParameterGroup>();
            newGroup->setName(attr);
            deserialize(json[attr], *newGroup.get());
            
            group.add(*newGroup);
            allocatedParams.push_back(newGroup);
            continue;
        }
        
        if(json[attr]["type"].asString() == "int"){
            shared_ptr<ofParameter<int>> param = make_shared<ofParameter<int>>();
            group.add(param->set(attr, json[attr]["value"].asInt()));
            allocatedParams.push_back(param);
            continue;
        }
        
        if(json[attr]["type"].asString() == "bool"){
            shared_ptr<ofParameter<bool>> param = make_shared<ofParameter<bool>>();
            group.add(param->set(attr, json[attr]["value"].asBool()));
            allocatedParams.push_back(param);
            continue;
        }
        
        if(json[attr]["type"].asString() == "float"){
            shared_ptr<ofParameter<float>> param = make_shared<ofParameter<float>>();
            group.add(param->set(attr, json[attr]["value"].asFloat()));
            allocatedParams.push_back(param);
            continue;
        }

        if(json[attr]["type"].asString() == "color"){
            shared_ptr<ofParameter<ofColor>> param = make_shared<ofParameter<ofColor>>();
            vector<string> numbers = ofSplitString(json[attr]["value"].asString(), ", ");
            if(numbers.size() >= 4){
                group.add(param->set(attr, ofColor(ofToInt(numbers[0]),
                                                   ofToInt(numbers[1]),
                                                   ofToInt(numbers[2]),
                                                   ofToInt(numbers[3]))));
                allocatedParams.push_back(param);
            }
            continue;
        }
        
        if(json[attr]["type"].asString() == "point"){
            shared_ptr<ofParameter<ofPoint>> param = make_shared<ofParameter<ofPoint>>();
            vector<string> numbers = ofSplitString(json[attr]["value"].asString(), ", ");
            if(numbers.size() >= 3){
                group.add(param->set(attr, ofPoint(ofToFloat(numbers[0]),
                                                   ofToFloat(numbers[1]),
                                                   ofToFloat(numbers[2]))));
                allocatedParams.push_back(param);
                continue;
            }
        }
    }
    
    return true;
}
