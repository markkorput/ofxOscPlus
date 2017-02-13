#pragma once

#include "ofParameterGroup.h"
#include "ofxJSON.h"

namespace ofxOscPlus {

    class Layout {
    public: // methods

        Layout();
        void setup(ofParameterGroup &rootGroup);


        bool toJson(Json::Value &json);
        string toJsonText();
        bool fromJson(const string& jsonText);
        bool fromJson(const Json::Value &json);

    private: // methods

        Json::Value& serialize(const ofParameterGroup &group, Json::Value &json);
        Json::Value& serialize(const ofParameter<float> param, Json::Value &json);
        Json::Value& serialize(const ofParameter<int> param, Json::Value &json);
        Json::Value& serialize(const ofParameter<bool> param, Json::Value &json);
        Json::Value& serialize(const ofParameter<ofColor> param, Json::Value &json);
        Json::Value& serialize(const ofParameter<ofPoint> param, Json::Value &json);
        bool deserialize(const Json::Value& json, ofParameterGroup &group);

    private: // attributes

        ofParameterGroup *rootGroup;
        vector<shared_ptr<ofAbstractParameter>> allocatedParams;
    };
}
