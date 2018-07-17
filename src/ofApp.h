#pragma once


#include "ofxOscQueryServer.h"
#include "Material.h"
#include "Cube.h"
#include "ofxAssimpModelLoader.h"
#include "ofEasyCam.h"
#include "ofxgamecamera.h"
#include "ofxjoystick.h"
#include <vector>



class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void drawAxes();
    void exit();




    void moveMouse(int x, int y);
    void keyPressed  (int key);

    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);


    void vSyncChanged(bool & vSync);


    ofParameter<bool> vSync;
    bool tex;
    ofParameterGroup parameters;
    ofXml settings;
    ofTrueTypeFont font;
    ofParameter<ofVec3f> Origine;
    ofParameter<float> RotX;
    ofParameter<float> RotY;
    ofParameter<float> RotZ;
    ofParameterGroup Swarm2;
    struct Dome {
        ofVec3f position;

        ofParameter<ofVec3f> pos;
        ofParameterGroup para_dome;



    };
    ofxJoystick joy_;
vector<Dome> domes;





    //***************************************************//
    //     This is where we setup our OSCQuery Server:   //
    ofxOscQueryServer server; //We will set it up in ofApp.cpp
    //***************************************************//

    ofTrueTypeFont verdana;
    ofxGameCamera cam3;

    bool bShowHelp;

    std::vector<Cube*> cubes;
    std::vector<Material*> materials;


    ofxAssimpModelLoader Model;

    bool init;

    std::vector<ofVec3f> Domes;
    ofParameterGroup D;
    ofParameter<ofVec3f> _Domes;

    vector< ofVec3f> postest;
    int cnt=0;
};
