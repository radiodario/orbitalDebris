#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    
    
    rot = 0;
    
    scaler = 300 / Geo::GeoUtils::EARTH_RADIUS_KM;
    
    colorMap.loadImage("earth-texture.jpg");
    
    earthSphere.set(Geo::GeoUtils::EARTH_RADIUS_KM, 10);
    ofQuaternion quat;
    quat.makeRotate(180, 0, 1, 0);
    earthSphere.rotate(quat);
    earthSphere.mapTexCoords(0,
                             colorMap.getTextureReference().getTextureData().tex_u,
                             colorMap.getTextureReference().getTextureData().tex_t,
                             0);
    
    debris = new objectSwarm("debris.txt", ofColor(255, 255, 255, 1));
    
    
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
    
    cam.begin();
    
    ofPushMatrix();
    
    ofSetColor(255);
    colorMap.bind();
    earthSphere.draw();
    colorMap.unbind();
    
    debris->draw();
    
    ofPopMatrix();
    
    cam.end();
    
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
