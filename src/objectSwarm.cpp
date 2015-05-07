//
//  objectSwarm.cpp
//  orbitalDebris
//
//  Created by Dario Villanueva on 25/04/2015.
//
//

#include "objectSwarm.h"

objectSwarm::objectSwarm(const std::string &filename, ofColor objectColour) {
    
    objects = Satellite::Utils::loadTLEFromFile(filename);
 
    objectMesh.setMode(OF_PRIMITIVE_POINTS);
    
    // initialise some points
    for (int i = 0; i < objects.size(); i++) {
        objectMesh.addVertex(ofPoint(0, 0, 0));
    }
    
    std::cout << "loaded " << objects.size() << " points" << std::endl;
    
    colour = objectColour;
    
    ofDisableArbTex();
    pTexture.loadImage("dot2.png");
    
    
}

void objectSwarm::update(Poco::DateTime moment) {
    
    std::vector<Satellite::Satellite>::const_iterator iter = objects.begin();
    
    Geo::ElevatedCoordinate pos;
    
    int i = 0;
    
    while (iter != objects.end()) {
        try {
            pos = Satellite::Utils::toElevatedCoordinate((*iter).find(moment).ToGeodetic());
            
            ofQuaternion latRot;
            ofQuaternion longRot;
            
            latRot.makeRotate(pos.getLatitude(), 1, 0, 0);
            longRot.makeRotate(pos.getLongitude(), 0, 1, 0);
            
            ofVec3f center = ofVec3f(0, 0, pos.getElevation() / 1000 + Geo::GeoUtils::EARTH_RADIUS_KM);
            
            ofVec3f worldPoint = latRot * longRot * center;
            
            objectMesh.setVertex(i, worldPoint);
            
        }
        catch (SatelliteException e) {
            // inform of error
        }
        catch (DecayedException e) {
            // inform of error
        }
        ++iter;
        ++i;
    }
    
    
}

void objectSwarm::draw() {
//    ofEnablePointSprites();
    glPointSize(3.);
//    pTexture.getTextureReference().bind();
    ofPushStyle();
    ofSetColor(colour);
    objectMesh.draw();
    ofPopStyle();
//    ofDisablePointSprites();
}