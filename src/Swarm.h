#pragma once

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

#define SPRING_CONSTANT 0.1f
#define MAX_VELOCITY 30.0f

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'
class swarm : public ofNode {
    struct particle {
        ofVec3f position;
        ofVec3f velocity;
        ofColor color;

        ofParameterGroup parameters;
        ofParameter<ofVec3f> pos;
        ofParameter<ofVec3f> Origine;



    };

public:
    swarm();
    void init(int nParticles, float positionDispersion, float velocityDispersion, string name);
    void customDraw();
    void updatepos(ofVec3f pos);
    void updatemode(bool m);
    bool mode;
    ofLight light;
    ofMaterial material;
    ofParameterGroup param;
    ofVec3f poscam;


protected:
    // we call this update function ourselves
    //  at the beginning of customDraw
    void update();

    // objects
    vector<particle>particles;
    vector<ofxAssimpModelLoader> modeles;
};
