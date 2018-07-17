/*
 *  ofxGameCamera.cpp
 *
 *  Created by James George on 1/19/11.
 *  Copyright 2011 FlightPhase. All rights reserved.
 *
 */

#include "ofxgamecamera.h"

static float ClampAngle (float angle, float min, float max)
{
    if (angle < -360.0f)
        angle += 360.f;
    if (angle > 360.0f)
        angle -= 360.0f;
    return ofClamp(angle, min, max);
}


ofxGameCamera::ofxGameCamera()
{
    sensitivityX = 0.15f;
    sensitivityY = 0.15f;

    minimumX = -360.0f;
    maximumX =  360.0f;

    minimumY = -60.0f;
    maximumY =  60.0f;

    rotationX = 0.0f;
    rotationY = 0.0f;
    rotationZ = 0.0f;

    speed = 10.0f;

    lastMouse = ofVec2f(0,0);

    usemouse = true;
    autosavePosition = false;
    useArrowKeys = true;

    cameraPositionFile = "_gameCameraPosition.xml";





    joy_button=-1;
    X=0;
    Y=0;
    x=0;
    y=0;
}

void ofxGameCamera::begin(ofRectangle rect)
{




    ofVec3f startPos = getPosition();
    ofVec2f startRot = ofVec2f(rotationX, rotationY);





    //forward
    if(usemouse){
        if(ofGetKeyPressed('w') || (useArrowKeys && ofGetKeyPressed(OF_KEY_UP))||x==-1 ){
            dolly(-speed);

        }

        if(ofGetKeyPressed('s') || (useArrowKeys && ofGetKeyPressed(OF_KEY_DOWN))||x==1  ){
            dolly(speed);
        }

        if(ofGetKeyPressed('a') || (useArrowKeys && ofGetKeyPressed(OF_KEY_LEFT)) ||y==-1 ){
            truck(-speed);
        }

        if(ofGetKeyPressed('d') || (useArrowKeys && ofGetKeyPressed(OF_KEY_RIGHT))  || y==1){
            truck(speed);
        }

        if(ofGetKeyPressed('c') || (useArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_DOWN))  || joy_button==5){
            boom(-speed);
        }

        if(ofGetKeyPressed('e') || (useArrowKeys && ofGetKeyPressed(OF_KEY_PAGE_UP))|| joy_button==4){
            boom(speed);
        }
    }

    ofVec2f mouse( ofGetMouseX(), ofGetMouseY() );

    if(usemouse && ofGetMousePressed(0)|| X!=0 || Y!=0|| joy_button==6|| joy_button==7|| joy_button==0|| joy_button==1|| joy_button==2|| joy_button==3){

        if(joy_button==0){
            Y=1;

        }
        if(joy_button==1){
            X=1;

        }
        if(joy_button==2){
            Y=-1;

        }
        if(joy_button==3){
            X=-1;

        }
        //rotationX += (mouse.x - lastMouse.x) * sensitivityX;
        rotationX += 10*X* sensitivityX;
        rotationX = ClampAngle(rotationX, minimumX, maximumX);

        //rotationY += (mouse.y - lastMouse.y) * sensitivityY;
        rotationY +=10* Y * sensitivityY;
        rotationY = ClampAngle(rotationY, minimumY, maximumY);


        if(joy_button==6){

            rotationZ +=10 * sensitivityY;
            rotationZ = ClampAngle(rotationZ, minimumY, maximumY);}
        else if(joy_button==7){
            rotationZ -=10 * sensitivityY;
            rotationZ = ClampAngle(rotationZ, minimumY, maximumY);}

        updateRotation();

    }

    lastMouse = mouse;

    if(autosavePosition && (startPos != getPosition() || startRot != ofVec2f(rotationX, rotationY))){
        saveCameraPosition();
    }

    ofCamera::begin(rect);
}



void ofxGameCamera::updateRotation()
{
    setOrientation(ofQuaternion()); //reset
    rotate(ofQuaternion(-rotationX, getYAxis()));
    rotate(ofQuaternion(-rotationY, getXAxis()));
    rotate(ofQuaternion(-rotationZ, getZAxis()));
}

void ofxGameCamera::saveCameraPosition()
{
    ofxXmlSettings savePosition;
    savePosition.addTag("camera");
    savePosition.pushTag("camera");

    savePosition.addTag("position");
    savePosition.pushTag("position");

    savePosition.addValue("X", getPosition().x);
    savePosition.addValue("Y", getPosition().y);
    savePosition.addValue("Z", getPosition().z);

    savePosition.popTag(); //pop position

    savePosition.addTag("rotation");
    savePosition.pushTag("rotation");

    savePosition.addValue("X", rotationX);
    savePosition.addValue("Y", rotationY);

    savePosition.popTag(); //pop rotation

    savePosition.popTag(); //camera;

    savePosition.saveFile(cameraPositionFile);
}

void ofxGameCamera::loadCameraPosition()
{
    ofxXmlSettings loadPosition;
    if(loadPosition.loadFile(cameraPositionFile)){

        loadPosition.pushTag("camera");
        loadPosition.pushTag("position");
        // tig: defaulting with floats so as to get floats back.
        setPosition(ofVec3f(loadPosition.getValue("X", 0.),
                            loadPosition.getValue("Y", 0.),
                            loadPosition.getValue("Z", 0.)));
        loadPosition.popTag();

        loadPosition.pushTag("rotation");
        rotationX = loadPosition.getValue("X", 0.);
        rotationY = loadPosition.getValue("Y", 0.);
        loadPosition.popTag();

        loadPosition.popTag(); //pop camera;

        updateRotation();
    }
    else{
        ofLog(OF_LOG_ERROR, "ofxGameCamera: couldn't load position!");
    }

}


void ofxGameCamera::updateAxis_x(int i )
{
    x=i;
}

void ofxGameCamera::updateAxis_y(int i )
{
    y=i;
}

void ofxGameCamera::updateAxis_X(int i )
{
    X=i;
}

void ofxGameCamera::updateAxis_Y(int i )
{
    Y=i;
}

void ofxGameCamera::updateState(int i )
{
    joy_button=i;
}
