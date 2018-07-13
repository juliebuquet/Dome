#pragma once

#include "ofMain.h"
#include "ofxOscQueryServer.h"
#include "Material.h"
#include "Cube.h"
#include "Swarm.h"
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
    bool mode;
     bool mode1;




    void moveMouse(int x, int y);
    void keyPressed  (int key);

    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);


    void vSyncChanged(bool & vSync);



    ofParameterGroup parameters;
    ofXml settings;
    bool tex;
    ofTrueTypeFont font;
    ofParameter<ofVec3f> Origine;
    ofParameter<bool> frozen;
    ofParameter<float> RotX;
    ofParameter<float> RotY;
    ofParameter<float> RotZ;
    ofxJoystick joy_;

    //***************************************************//
    //     This is where we setup our OSCQuery Server:   //
    ofxOscQueryServer server; //We will set it up in ofApp.cpp
    //***************************************************//

    ofTrueTypeFont verdana;


    ofxGameCamera cam3;



    std::vector<Cube*> cubes;
    std::vector<Material*> materials;

    swarm nodeSwarm;



    bool init;

};
