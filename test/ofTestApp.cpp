#ifdef DEBUG

#include "ofTestApp.h"

#include "../src/ofxOscPlus/Layout.h"

void ofTestApp::run(){
    ofLogToFile("log-test.txt");
    testLayout();
}

void ofTestApp::testLayout(){
    testLayoutSerialize();
    testLayoutDeserialize();
    testDiscovery();
    testParameterServer();
}

void ofTestApp::testLayoutSerialize(){
    // create a params layout
    ofParameterGroup params;
    ofParameter<float> flParam;
    ofParameter<ofColor> clrParam;
    ofParameter<bool> bParam;
    ofParameter<ofPoint> pointParam;

    params.setName("testGroup");
    params.add(flParam.set("decimal", 1.0f));
    params.add(clrParam.set("color", ofColor::white));
    params.add(bParam.set("yezno", true));
    params.add(pointParam.set("punto", ofPoint(3.0f)));

    string expected;
    // serialize it
    ofxOscPlus::Layout layout;
    layout.setup(params);
    Json::Value json;
    layout.toJson(json);

    // ofLog() << json.toStyledString();
    test_eq(json["testGroup"]["decimal"]["type"].asString(), "float", "float type");
    test_eq(json["testGroup"]["decimal"]["value"].asString(), "1", "float value");
    test_eq(json["testGroup"]["color"]["type"].asString(), "color", "color type");
    test_eq(json["testGroup"]["color"]["value"].asString(), "255, 255, 255, 255", "color value");
    test_eq(json["testGroup"]["yezno"]["type"].asString(), "bool", "bool type");
    test_eq(json["testGroup"]["yezno"]["value"].asString(), "true", "bool value");
    test_eq(json["testGroup"]["punto"]["type"].asString(), "point", "bool type");
    test_eq(json["testGroup"]["punto"]["value"].asString(), "3, 3, 3", "bool value");

}

void ofTestApp::testLayoutDeserialize(){
    // create a params layout
    ofParameterGroup sourceParams, destParams;
    ofParameter<float> flParam;
    ofParameter<ofColor> clrParam;
    ofParameter<bool> bParam;
    ofParameter<ofPoint> pointParam;

    sourceParams.setName("testGroup");
    sourceParams.add(flParam.set("decimal", 1.0f));
    sourceParams.add(clrParam.set("color", ofColor::white));
    sourceParams.add(bParam.set("yezno", true));
    sourceParams.add(pointParam.set("punto", ofPoint(3.0f)));

    ofxOscPlus::Layout sourceLayout, destLayout;
    sourceLayout.setup(sourceParams);
    destLayout.setup(destParams);
    
    test_eq(destParams.getName(), "", "uninitialized group name should be empty");
    test_eq(destParams.size(), 0, "uninitialized group size should be zero");

    Json::Value json;
    sourceLayout.toJson(json);
    destLayout.fromJson(json);

    test_eq(destParams.getName(), "testGroup", "initialized group name");
    test_eq(destParams.size(), sourceParams.size(), "initialized group size");
    // test_eq(destParams.getType("decimal"), typeid(ofParameter<float>).name(), "initialized float param");
    test_eq(destParams.getFloat("decimal").get(), 1.0f, "initialized float param");
    // test_eq(destParams.getType("color"), typeid(ofParameter<ofColor>).name(), "initialized color param");
    test_eq(destParams.getColor("color").get(), ofColor::white, "initialized float param");
    // test_eq(destParams.getType("yezno"), typeid(ofParameter<bool>).name(), "initialized bool param");
    test_eq(destParams.getBool("yezno").get(), true, "initialized float param");
    test_eq(destParams.getPoint("punto").get(), ofPoint(3.0f), "initialized float param");
}

void ofTestApp::testDiscovery(){
    // TODO
}

void ofTestApp::testParameterServer(){
    
}

#endif // DEBUG
