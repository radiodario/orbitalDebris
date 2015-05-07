#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    
    
    rot = 0;
    
    scaler = 300 / Geo::GeoUtils::EARTH_RADIUS_KM;
    
    string texture = "earth-ud";
    
    // load & allocate texture map
    textureMapImage.loadImage(texture +"-texture.jpg");
    textureMap.allocate(2048, 1024, GL_RGB, false);
    textureMap.loadData(textureMapImage.getPixels(), 2048, 1024, GL_RGB);
    
    // load & allocate normal map
    normalMapImage.loadImage(texture +"-normal.jpg");
    normalMap.allocate(2048, 1024, GL_RGB, false);
    normalMap.loadData(normalMapImage.getPixels(), 2048, 1024, GL_RGB);
    
    // load & allocate specular map
    specularMapImage.loadImage(texture +"-specular.jpg");
    specularMap.allocate(2048, 1024, GL_RGB, false);
    specularMap.loadData(specularMapImage.getPixels(), 2048, 1024, GL_RGB);

    cloudsMapImage.loadImage("earth-clouds.png");
    cloudsMap.allocate(2048, 1024, GL_RGBA, false);
    cloudsMap.loadData(cloudsMapImage.getPixels(), 2048, 1024, GL_RGBA);
    
    // setup & link shader
    shader.load("normalMap.vert", "normalMap.frag");
    
    cloudsSphere.set(Geo::GeoUtils::EARTH_RADIUS_KM*1.02, 32);
    earthSphere.set(Geo::GeoUtils::EARTH_RADIUS_KM, 32);
    ofQuaternion quat;
    quat.makeRotate(180, 0, 1, 0);
    earthSphere.rotate(quat);
    cloudsSphere.rotate(quat);
    
    debris = new objectSwarm("debris.txt", ofColor(255, 255, 255));
    

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    GLfloat lmKa[] = {1.0, 1.0, 1.0, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { .80, .80, 1.0, 1.0 };
    GLfloat light_position[] = { -1000, -1000, 2000, 0.0 }; //light directional if w=0 without shader works
//    GLfloat light_position[] = { 220.0, 10.0, 0.0, 1.0 };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
    
    GLfloat mat_ambient[] = {1.0f, 1.0f, 0.7f, 1.0f};
    GLfloat mat_diffuse[] = {0.1f, 0.2f, 1.0f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS, 60.0);
    

    cam.setPosition(0, 0, 0);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    Poco::DateTime now;

    debris->update(now);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
//    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
    
    glShadeModel(GL_SMOOTH);
    ofEnableLighting();
    
    cam.begin();
    
    ofPushMatrix();
    ofScale(scaler,scaler,scaler);
    ofSetColor(255);
    
    shader.begin();
    shader.setUniformTexture("colorMap", textureMap, 0);
    shader.setUniformTexture("normalMap", normalMap, 1);
    shader.setUniformTexture("glossMap", specularMap, 2);
    earthSphere.draw();
    shader.end();

    
    cloudsMap.bind();
    cloudsSphere.draw();
    cloudsMap.unbind();

    
    debris->draw();
    
    ofPopMatrix();
    
    cam.end();
    ofDisableLighting();
    glDisable(GL_DEPTH_TEST);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
