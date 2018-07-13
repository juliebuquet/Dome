#include "ofApp.h"
#include <X11/Xlib.h>
#include "ofxjoystick.h"
#include "SDL.h"
#include "ossia/include/ossia_export.h"


#undef main
//--------------------------------------------------------------
void ofApp::setup(){
    tex=true;
    mode = false;
    //------------------------------------------------
    joy_.setup(GLFW_JOYSTICK_1);
    ofSetVerticalSync(true);

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();
    // cam.init(ofGetWidth(),ofGetHeight());
    init = true;
    ofBackground(0,0,0);
    ofEnableDepthTest();
    ofEnableNormalizedTexCoords();
    ofEnableAntiAliasing();
    ofDisableArbTex();

    ofTrueTypeFont::setGlobalDpi(72);
    verdana.loadFont("verdana.ttf", 18, true, true, true);
    verdana.setLineHeight(18.0f);
    verdana.setLetterSpacing(1.037);

    // Create cubes and their materials for the floor
    Material* sand = new Material("sand.png");
    materials.push_back(sand);
    Material* stonebrick = new Material("stonebrick.png");
    materials.push_back(stonebrick);
    Material* mossy = new Material("stonebrick_mossy.png");
    materials.push_back(mossy);
    Cube* c;

    Cube* d;
    for (int i=0; i < 2; i++) {
        for (int j=-20; j < 20; j++) {
            for (int k=-20; k < 20; k++) {
                if (i==1){
                    c = new Cube(sand);
                }else{
                    c = new Cube(stonebrick);
                }
                c->move(j, -(i+1), k); //position of the cube
                cubes.push_back(c);
            }
        }
    }


    //-----------------------------------draw particles//-------------------------------------------------------------------------------------
    nodeSwarm.init(5, 50, 10,"SWARM1");

    //-------------------------------------------------------------------------------Parameter settings (sent to i-score)//---------------------------------------------------------------------

    parameters.setName("settings");
    ofVec3f O;
    O[0]=0;
    O[1]=0;
    O[2]=0;
    parameters.add(Origine.set("Origine",O));
    parameters.add(RotX.set("Pitch",0));
    parameters.add(RotY.set("Roll",0));
    parameters.add(RotZ.set("Yaw",0));
    parameters.add(frozen.set("Frozen",false));
    parameters.add(nodeSwarm.param);


    //*********************************************************************************//
    //   This is where we setup our OSCQuery Server:
    // NB this is the only change from of's gui/ParameterGroupExample
    server.setup(parameters, 1213, 4477, "Particles");
    // i.e. this will create an OSCquery server from 'parameters' ParameterGroup
    // scan all sub-ParameterGroups and children Parameters recursively
    // and expose the whole thing to ports 1233 for OSC and 4477 for Websocket
    // All network communication (sending, receving, listening, queries and replies)
    // will then be managed by the internal implementation of libossia
    //*********************************************************************************//
 //server.findNode("SWARM1/Cartesian0").setRepetition("Filetered");


}

void ofApp::vSyncChanged(bool & vSync){
    ofSetVerticalSync(vSync);
}


//--------------------------------------------------------------
void ofApp::update(){
    //  // frameNum is a readonly parameter so this will fail to compile
    //  // unless we are inside the CirclesRenderer class
    //  // renderer.frameNum = 5;
    //    // Constrain mouse cursor inside window



    if(init){
        moveMouse(ofGetWidth()/2, ofGetHeight()/2);
        init = false;
    }
    else{
        int x = ofGetMouseX();
        int y = ofGetMouseY();
        if(x >= ofGetWindowWidth()-1)
            moveMouse(1,y);
        if(x <= 0)
            moveMouse(ofGetWindowWidth()-2,y);
        if(y <= 0)
            moveMouse(x,ofGetHeight()-2);
        if(y >= ofGetHeight()-1)
            moveMouse(x,1);
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    //------------------------------JOY CONTROL-------------------------------------
    //button control (up down)
    for (int i = 0; i < joy_.getButtonNum(); i++) {
        if (joy_.isPressed(i)) {

            cam3.updateState(i);
        }
        if (joy_.isPushing(i)) {

            cam3.updateState(i);
        }
        if (joy_.isRelease(i)) {

            cam3.updateState(-1);
        }
    }



    //Axes control

    ///////////LEFT (direction)
    int ax_x= joy_.getAxis(1);
    if(ax_x>0){

        cam3.updateAxis_x(1);

    }
    else if (ax_x<0){

        cam3.updateAxis_x(-1);

    }
    else{cam3.updateAxis_x(0);}


    int ax_y= joy_.getAxis(0);
    if(ax_y>0){

        cam3.updateAxis_y(1);

    }
    else if (ax_y<0){

        cam3.updateAxis_y(-1);

    }
    else{cam3.updateAxis_y(0);

    }

    ///////////RIGHT   (Rotation)
    int ax_X= joy_.getAxis(3);
    if(ax_X>0){

        cam3.updateAxis_X(1);

    }
    else if (ax_X<0){

        cam3.updateAxis_X(-1);

    }
    else{cam3.updateAxis_X(0);}


    int ax_Y= joy_.getAxis(4);
    if(ax_Y>0){

        cam3.updateAxis_Y(1);

    }
    else if (ax_Y<0){

        cam3.updateAxis_Y(-1);

    }
    else{cam3.updateAxis_Y(0);

    }

    //------------------------------------------------------------------------------------------------------------
    cam3.begin();

    drawAxes();
    ofScale(100,100,100);

    for (unsigned int i=0; i<cubes.size(); i++) {
        cubes[i]->updatetex(tex);
        cubes[i]->draw();
    }

    mode1=parameters.getBool("Frozen");

    mode=mode1;
    nodeSwarm.updatemode(mode);
    nodeSwarm.draw();

    ofVec3f posi;
    posi[0]=cam3.getPosition().x/100;
    posi[1]= -1*cam3.getPosition().z/100;
    posi[2]=cam3.getPosition().y/100;
    Origine.set("Origine",posi);
    RotX.set("Pitch",cam3.rotationX*(-1));
    RotY.set("Roll",cam3.rotationY*(-1));
    RotZ.set("Yaw",cam3.rotationZ*(-1));


}
//----------------------------------------------------------------
void ofApp::drawAxes()
{
    ofPushMatrix();
    ofSetLineWidth(2);
    ofSetColor(255, 0, 0);
    ofLine(0, 0, 0, 20, 0, 0);
    verdana.drawStringAsShapes("x", 25, 0);
    ofSetColor(0, 255, 0);
    ofLine(0, 0, 0, 0, 20, 0);
    verdana.drawStringAsShapes("y", 5, 25);
    ofSetColor(0, 0, 255);
    ofLine(0, 0, 0, 0, 0, 20);
    ofTranslate(0,0,22);
    verdana.drawStringAsShapes("z", -5, -5);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::moveMouse(int x, int y)
{
#if defined(__APPLE__)
    CGWarpMouseCursorPosition(CGPointMake(ofGetWindowPositionX()+x,ofGetWindowPositionY()+y));
#elif defined(_WIN32)
    SetCursorPos(x,y); // not tested
#else // xlib
    Display *display = XOpenDisplay(0);
    Window window;


    int state;
    XGetInputFocus(display,&window,&state);
    //XWarpPointer(display, None, window, 0, 0, 0, 0, x, y);
    XCloseDisplay(display);
#endif
    //cam.mousePressed(x, y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key=='b'){ // allows you to activate/desactivate the physical model that makes the particles move
        if (mode==true){
            mode=false;

        }
        else{
            mode=true;
        }

        frozen.set("Frozen",mode); // send the state to i-score

    }

    if(key=='n'){
        if (tex==true){
            tex=false;



        }
        else{
            tex=true;
        }


    }


}
//------------------------------------------------------------------
void ofApp::exit()
{
    for (unsigned int i=0; i<cubes.size(); i++) {
        delete cubes[i];
    }
    for (unsigned int i=0; i<materials.size(); i++) {
        delete materials[i];
    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){




}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //cam.init(ofGetWidth(),ofGetHeight());
}


