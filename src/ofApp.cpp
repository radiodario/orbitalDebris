#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    ofEnableDepthTest();
//    ofEnableAlphaBlending();
//    
//
    
    ofSetFrameRate(60);
    
    
    rot = 0;
    
    scaler = 300 / Geo::GeoUtils::EARTH_RADIUS_KM;
    
    colorMap.loadImage("color_map_1024.jpg");
    
    earthSphere.set(Geo::GeoUtils::EARTH_RADIUS_KM, 10);
    ofQuaternion quat;
    quat.makeRotate(180, 0, 1, 0);
    earthSphere.rotate(quat);
//    earthSphere.mapTexCoords(0,
//                             colorMap.getTextureReference().getTextureData().tex_u,
//                             colorMap.getTextureReference().getTextureData().tex_t,
//                             0);
    
    myLocation = Geo::ElevatedCoordinate(51.507406923983446,
                                         -0.12773752212524414,
                                         0.05);
    
    ofHttpResponse response = ofLoadURL("http://www.celestrak.com/NORAD/elements/1999-025.txt");
    
    if (200 == response.status)
    {
        satellites = Satellite::Utils::loadTLEFromBuffer(response.data);
    }
    else
    {
        ofLogError("ofApp::setup()") << "Unable to load : " << response.error;
    }
    
//    debrisMesh.setMode(OF_PRIMITIVE_POINTS);
    debrisMesh.setMode(OF_PRIMITIVE_POINTS);
    
    for (int i = 0; i < satellites.size(); i++) {
        debrisMesh.addVertex(ofPoint(0, 0, 0));
    }
    
    cam.setPosition(0, 0, 0);
    
}

//--------------------------------------------------------------
void ofApp::update(){

    std::vector<Satellite::Satellite>::const_iterator iter = satellites.begin();
    Poco::DateTime now;
    
    Geo::ElevatedCoordinate pos;
    
    int i = 0;
    
    while (iter != satellites.end())
    {
        try {
            
            pos = Satellite::Utils::toElevatedCoordinate((*iter).find(now).ToGeodetic());
        
            
            ofQuaternion latRot;
            ofQuaternion longRot;
            
            latRot.makeRotate(pos.getLatitude(), 1, 0, 0);
            longRot.makeRotate(pos.getLongitude(), 0, 1, 0);
            
            //our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
            ofVec3f center = ofVec3f(0,0, pos.getElevation() / 1000 + Geo::GeoUtils::EARTH_RADIUS_KM);
            //multiplying a quat with another quat combines their rotations into one quat
            //multiplying a quat to a vector applies the quat's rotation to that vector
            //so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
            ofVec3f worldPoint = latRot * longRot * center;
            
            debrisMesh.setVertex(i, worldPoint);
            
            //set the bitmap text mode billboard so the points show up correctly in 3d
            //            std::stringstream ss;
            //
            //            ss << iter->Name() << std::endl;
            //            ss << " Latitude (deg): " << pos.getLatitude() << std::endl;
            //            ss << "Longitude (deg): " << pos.getLongitude() << std::endl;
            //            ss << " Elevation (km): " << pos.getElevation() / 1000 << std::endl;
            
            //ofDrawBitmapString(ss.str(), worldPoint);
        }
        catch (SatelliteException e) {
            
        }
        ++iter;
        ++i;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    cam.begin();
    
    ofPushMatrix();
//    rot += 0.01;
    ofRotate(rot, 0, 1, 0);
    ofScale(scaler, scaler, scaler);
    
    ofSetColor(255);
    colorMap.bind();
    earthSphere.drawWireframe();
    colorMap.unbind();
    
    debrisMesh.draw();
    
    
    
    
    
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
