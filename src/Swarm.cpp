#include "Swarm.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'

swarm::swarm(){
    light.setAmbientColor(ofColor(0, 0, 0));
    poscam[0]=0;
    poscam[0]=0;
    poscam[0]=0;
    mode=true;
}

void swarm::init(int nParticles, float positionDispersion, float velocityDispersion,string name){

    if (nParticles!=0)
    {param.setName(name);}

    // Check if we've already initialised
    if(particles.size() != 0){
        // clear out old data
        ofLogWarning("swarm") <<  "Swarm: Already initialised";

        particles.clear();
    }

    ofSeedRandom();
    //
    ofVec3f position, velocity;
    ofColor color;
    for(int i = 0; i < nParticles; i++){
        position.x = (ofRandom(2.0f) - 1.0f)  * positionDispersion;
        position.y = (ofRandom(2.0f) - 1.0f)  * positionDispersion;
        position.z = (ofRandom(2.0f) - 1.0f)  * positionDispersion;

        velocity.x = (ofRandom(0.5f) - 0.25f)  * velocityDispersion;
        velocity.y = (ofRandom(0.5f) - 0.25f)  * velocityDispersion;
        velocity.z = (ofRandom(0.5f) - 0.25f)  * velocityDispersion;

        color.r = ofRandom(255.0f);
        color.g = ofRandom(255.0f);
        color.b = 150.0f;
        color.a = 255.0f;

        particle newParticle;
        newParticle.position = position;
        newParticle.velocity = velocity;
        newParticle.color = color;
        stringstream ss;
        ss << i;
        string str = ss.str();


        newParticle.parameters.setName(str);
        ofVec3f posi;
        posi[0]=position.x;
        posi[1]= -1*position.z;
        posi[2]=position.y;
        //newParticle.parameters.add( newParticle.pos.set("Cartesian",posi));
        //param.add(newParticle.parameters);
        param.add(newParticle.pos.set("Cartesian"+str,posi));
        // add our new particle to the vector
        particles.push_back(newParticle);

        ofxAssimpModelLoader Model;
         Model.loadModel("penguin.3ds", 1);
         Model.setRotation(0,180,1,0,0);
        Model.setScale(0.001, 0.001,0.001);
        Model.setPosition(position.x,position.y,position.z);
        modeles.push_back(Model);
    }

}

void swarm::customDraw(){
    // We run the update ourselves manually. ofNode does
    //  not do this for us.

if(mode==false){

    update();

 }

    //--
    // Draw particles

    // We use the position of the first
    //  particle as the position of the
    //  light.
    ofPushStyle();
    light.enable();
    light.setPosition(poscam);



    for(unsigned int i = 0; i < particles.size(); i++){

        material.setDiffuseColor(particles[i].color);

        material.begin();
        //ofDrawSphere(particles[i].position, 0.5);
if(mode==true){
        ofParameter<ofVec3f> ps;
        ps=param.getVec3f(i);
        ofVec3f p;
        ofVec3f P;
        p=ps;
        P[0]=p[0];
        P[1]=p[2];
        P[2]=-p[1];

//        particles[i].position=p; // in case it changed in score and note on QT
//        particles[i].pos=p;
        modeles[i].setPosition(P[0],P[1],P[2]);
    particles[i].position=P;
        modeles[i].drawFaces();

}
else{

 modeles[i].setPosition(particles[i].position[0],particles[i].position[1],particles[i].position[2]);
modeles[i].drawFaces();}

    }
    material.end();
    light.disable();
    ofDisableLighting();

    //
    //--



    // Render light as white sphere

    ofPopStyle();
}

void swarm::update(){

    // Calculate time past per frame
    float dt = ofGetLastFrameTime();

    // Update positions, velocities
    for(unsigned int i = 0; i < particles.size(); i++){






        // -----------
        //
        //	MOTION MATHS
        //
        //		'Simple Harmonic Motion' + a little extra
        // ----
        //

        // [1] apply velocity to postion
        //  (i.e. integrate velocity)
        //
        //  v = dx / dt (*)
        //  x = x + dx [every frame]
        //
        // therefore
        //  x = x + v * dt (*)
        //

            // (velcotity is taken from previous frame)
            particles[i].position += particles[i].velocity * dt;

            ofVec3f posi;
            posi[0]=particles[i].position.x ;
            posi[1]=-particles[i].position.z ;
            posi[2]=particles[i].position.y;
            particles[i].pos=posi;
            modeles[i].setPosition(particles[i].position[0],particles[i].position[1],particles[i].position[2]);

            // [2] apply spring force to velocity
            //  (i.e. integrate acceleration)
            //
            //  a = -k * x (this is the shm restoring force, aka spring force)
            //  a = dv / dt
            //
            // therefore from (*)s above
            //  (v = v + dv)
            //
            //  v = v + (dt * a)
            //  v = v + (dt * -k * x)
            //
            particles[i].velocity += -SPRING_CONSTANT * particles[i].position * dt;


            // [3] to get a super simple kind of 'flocking' behaviour
            //  we add a second spring force to velocity relative
            //  to the position of the light
            // NOTICE: THIS ISN'T REAL FLOCKING!

            particles[i].velocity += -SPRING_CONSTANT * (particles[i].position - light.getPosition()) * dt;









            // [4] Force a maximum velocity
            if(particles[i].velocity.length() > MAX_VELOCITY){
                particles[i].velocity /= particles[i].velocity.length() * MAX_VELOCITY;
            }

        }




}


void swarm::updatepos(ofVec3f pos){
    poscam=pos;
    light.setPosition(poscam);
    ofSetColor(255, 255, 255);


}

void swarm::updatemode(bool m){
    mode=m;

}
