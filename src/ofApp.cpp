#include "ofApp.h"
#include <X11/Xlib.h>
#include "ofxjoystick.h"
#include "SDL.h"
#include "ossia/include/ossia_export.h"
#undef main
//--------------------------------------------------------------
void ofApp::setup(){

 tex=true;
    //------------------------------------------------
    joy_.setup(GLFW_JOYSTICK_1);
    ofSetVerticalSync(true);
    ofEnableDepthTest();
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
    Swarm2.setName("SWARM2");
    int nb=5; // choose teh number of domes
    for(int i=0;i<nb;i++){

        Dome newDome;
        ofVec3f B;
        B[0]=floor(ofRandom(-20,20));
        B[1]=0;
        B[2]=floor(ofRandom(-20,20));

        ofVec3f Base1;
        Base1[0]=B[0];
        Base1[1]= -1*B[2];
        Base1[2]=B[1];
        ofDrawSphere(B, 1);
        stringstream ss;
        ss << i;
        string str = ss.str();
        newDome.position = B;
        postest.push_back(B);
        newDome.para_dome.setName(str);
        Swarm2.add( newDome.pos.set("Cartesian_"+str,Base1));
        domes.push_back(newDome);

        Domes.push_back(B);

    }


    parameters.add(Swarm2);



    //*********************************************************************************//
    //   This is where we setup our OSCQuery Server:
    // NB this is the only change from of's gui/ParameterGroupExample
    server.setup(parameters, 1213, 4477, "ofxOscQueryDemo");
    // i.e. this will create an OSCquery server from 'parameters' ParameterGroup
    // scan all sub-ParameterGroups and children Parameters recursively
    // and expose the whole thing to ports 1233 for OSC and 4477 for Websocket
    // All network communication (sending, receving, listening, queries and replies)
    // will then be managed by the internal implementation of libossia
    //*********************************************************************************//



}

void ofApp::vSyncChanged(bool & vSync){
    ofSetVerticalSync(vSync);
}


//--------------------------------------------------------------
void ofApp::update(){

 // Constrain mouse cursor inside window



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

    //------------------------------------------------------------------------------------------------------------ Draw scene------------------------------------------------
    cam3.begin();

    drawAxes();
    ofScale(100,100,100); // keep i-score units  ( 1 = radius of a dome)

    for (unsigned int i=0; i<cubes.size(); i++) { //draw cube
        cubes[i]->updatetex(tex);
        cubes[i]->draw();
    }

    ofVec3f posi;// update position of the camera to send to i-score
    posi[0]=cam3.getPosition().x/100; // transformation to match with i-score axes
    posi[1]= -1*cam3.getPosition().z/100;
    posi[2]=cam3.getPosition().y/100;
    Origine.set("Origine",posi);
    RotX.set("Pitch",cam3.rotationX*(-1));
    RotY.set("Roll",cam3.rotationY*(-1));
    RotZ.set("Yaw",cam3.rotationZ*(-1));
    ofSetColor(255,255,255,75);
    //-------------------------------------------------------------------------------Draw Domes---------------------------------------------------------------------
    for(int i=0;i<Domes.size();i++){ // draw domes
ofVec3f posi;
posi[0]=0;
posi[1]=0;
posi[2]=0;
        stringstream ss;
        ss << i;
        string str = ss.str();

        ofParameter<ofVec3f> ps;
        ps=Swarm2.getVec3f(i);
        ofVec3f p;
        ofVec3f P;
        posi=ps;
        p=ps;

        if(p==postest[i]){// si on a la meme valeur qu'avant (frame d'avant) // normalement on envoie pas de signal à iscore
            cnt+=1;


        }
        else{
            cnt=0;
        }
        postest[i]=p;

        p=p+0.1;



        if(cnt<1){
        domes[i].pos=p; }

        P[0]=posi[0];
        P[1]=posi[2];
        P[2]=-posi[1];
     Domes[i]=P;

domes[i].pos=posi;


        ofDrawSphere(P, 1);

}

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

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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

}


