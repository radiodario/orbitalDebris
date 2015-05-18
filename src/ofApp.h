#pragma once


#include "ofMain.h"
#include "objectSwarm.h"
#include "ofxCenteredTrueTypeFont.h"
#include "ofxGui.h"
#include "ofxAnimatableOfColor.h"
#include "ofxAnimatableFloat.h"
//#include "ofxSyphon.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void drawGui();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void setToExplosionTimePressed();
        void setToCurrentTimePressed();
    
    private:
    
        void setupLights();
        void loadDebrises();
        void setupGui();
        void setupAnimatables();
    
        ofxCenteredTrueTypeFont titleFont;
        ofTrueTypeFont dateFont;
    
        ofSpherePrimitive earthSphere;
        ofSpherePrimitive cloudsSphere;
        
        ofImage textureMapImage;
        ofImage normalMapImage;
        ofImage specularMapImage;
        ofImage cloudsMapImage;
    
        ofTexture textureMap;
        ofTexture normalMap;
        ofTexture specularMap;
        ofTexture cloudsMap;
        
        ofShader shader;
        
        float scaler;
        float rot;
    
        bool bHide;
    
        //ofxSyphonServer outputServer;
    
        ofxAnimatableOfColor titleColor;
        ofxAnimatableFloat zPosition;
        ofxAnimatableFloat yPosition;
    
        ofEasyCam cam;
        
        objectSwarm* debris;
    
        ofDirectory tleFiles;
        std::vector<objectSwarm*> debrises;
    
        Poco::DateTime current;
        Poco::DateTime explosionTime;
        Poco::DateTime last;
    
        // gui stuff
        ofxPanel gui;
    
        ofxToggle drawText;
        ofxToggle drawDate;
        ofxToggle drawFps;
        ofxToggle rotateScene;
        ofxToggle autoPilot;
        ofxFloatSlider rotationSpeed;
        ofxFloatSlider timeSpeed;
    
        ofxPanel debrisToggle;
    
        ofxButton setToExplosionTime;
        ofxButton setToCurrentTime;

};
