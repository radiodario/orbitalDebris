//
//  objectSwarm.h
//  orbitalDebris
//
//  Created by Dario Villanueva on 25/04/2015.
//
//
#pragma once


#include "ofMain.h"
#include "ofxSatellite.h"

using namespace ofx;

class objectSwarm {
    public:
        void update(Poco::DateTime time);
        void draw();

        // constructor
        objectSwarm(const std::string &filename, ofColor colour);
    
    private:
        std::vector<Satellite::Satellite> objects;
        ofMesh objectMesh;
        ofImage pTexture;
        ofColor colour;
};
