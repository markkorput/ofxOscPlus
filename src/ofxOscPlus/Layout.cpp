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
    json[0] = serialize(*rootGroup, sub);
    return true;
}

string Layout::toJsonText(){
    Json::Value val;

    if(toJson(val))
        return val.toStyledString();

    return "";
}

Json::Value& Layout::serialize(const ofParameterGroup &group, Json::Value &json){
    Json::Value childrenJson;
    
    for(int i=0; i<(int)group.size(); i++){
        string type = group.getType(i);
        string name = group.getName(i);
        Json::Value subJson;

        if(type == typeid(ofParameterGroup).name()){
            serialize(group.getGroup(i), subJson);
        }

        if(type == typeid(ofParameter<string>).name()){
            serialize(group.getString(i), subJson);
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

        subJson["name"] = name;
        
        childrenJson[i] = subJson;
    }

    json["name"] = group.getName();
    json["children"] = childrenJson;
    return json;
}

Json::Value& Layout::serialize(const ofParameter<string> param, Json::Value &json){
    json["type"] = "string";
    json["value"] = param.get();
    return json;
}

Json::Value& Layout::serialize(const ofParameter<float> param, Json::Value &json){
    json["type"] = "float";
    json["value"] = param.get();
    json["min"] = param.getMin();
    json["max"] = param.getMax();
    return json;
}

Json::Value& Layout::serialize(const ofParameter<int> param, Json::Value &json){
    json["type"] = "int";
    json["value"] = param.get();
    json["min"] = param.getMin();
    json["max"] = param.getMax();
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
    json["min"] = ofToString(param.getMin());
    json["max"] = ofToString(param.getMax());
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
    if(!json.isArray() || json.size() != 1){
        ofLogWarning() << "Expected an array with one item";
        return false;
    }

    // don't use rootGroup->clear because that also resets the change event
    // which might relied upon by event listeners
    for(int i=rootGroup->size()-1; i>=0; i--)
        rootGroup->remove(i);

    deserialize(json[0], *rootGroup);
    return true;
}

bool Layout::deserialize(const Json::Value& json, ofParameterGroup &group){
    group.setName(json["name"].asString());
    
    if(!json["children"].isArray())
        return true;

    for(int i=0; i<json["children"].size(); i++){
        auto child = json["children"][i];

        // no type means it's another sub group
        if(child["children"].isArray()){
            shared_ptr<ofParameterGroup> newGroup = make_shared<ofParameterGroup>();
            deserialize(child, *newGroup.get());
            group.add(*newGroup);
            allocatedParams.push_back(newGroup);
            continue;
        }

        if(child["type"].asString() == "string"){
            shared_ptr<ofParameter<string>> param = make_shared<ofParameter<string>>();
            group.add(param->set(child["name"].asString(), child["value"].asString()));
            allocatedParams.push_back(param);
            continue;
        }

        if(child["type"].asString() == "int"){
            shared_ptr<ofParameter<int>> param = make_shared<ofParameter<int>>();
            group.add(param->set(child["name"].asString(), child["value"].asInt()));
            allocatedParams.push_back(param);
            continue;
        }

        if(child["type"].asString() == "bool"){
            shared_ptr<ofParameter<bool>> param = make_shared<ofParameter<bool>>();
            group.add(param->set(child["name"].asString(), child["value"].asBool()));
            allocatedParams.push_back(param);
            continue;
        }

        if(child["type"].asString() == "float"){
            shared_ptr<ofParameter<float>> param = make_shared<ofParameter<float>>();
            group.add(param->set(child["name"].asString(), child["value"].asFloat()));
            allocatedParams.push_back(param);
            continue;
        }

        if(child["type"].asString() == "color"){
            shared_ptr<ofParameter<ofColor>> param = make_shared<ofParameter<ofColor>>();
            vector<string> numbers = ofSplitString(child["value"].asString(), ", ");
            if(numbers.size() >= 4){
                group.add(param->set(child["name"].asString(), ofColor(ofToInt(numbers[0]),
                                                   ofToInt(numbers[1]),
                                                   ofToInt(numbers[2]),
                                                   ofToInt(numbers[3]))));
                allocatedParams.push_back(param);
            }
            continue;
        }
    
        if(child["type"].asString() == "point"){
            shared_ptr<ofParameter<ofPoint>> param = make_shared<ofParameter<ofPoint>>();
            vector<string> numbers = ofSplitString(child["value"].asString(), ", ");
            if(numbers.size() >= 3){
                group.add(param->set(child["name"].asString(), ofPoint(ofToFloat(numbers[0]),
                                                   ofToFloat(numbers[1]),
                                                   ofToFloat(numbers[2]))));
                allocatedParams.push_back(param);
                continue;
            }
        }
    }
    
    return true;
}
