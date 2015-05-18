#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofEnableDepthTest();
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    rot = 0;
    scaler = 300 / Geo::GeoUtils::EARTH_RADIUS_KM;
    
    string texture = "earth-ud";
    

    // syphon setup
    //outputServer.setName("Orbital Debris");
    
    
    // load & allocate texture map
    textureMapImage.loadImage(texture +"-texture.jpg");
    textureMap.allocate(8192, 4096, GL_RGB, false);
    textureMap.loadData(textureMapImage.getPixels(), 8192, 4096, GL_RGB);
    
    // load & allocate normal map
    normalMapImage.loadImage(texture +"-normal.png");
    normalMap.allocate(8192, 4096, GL_RGB, false);
    normalMap.loadData(normalMapImage.getPixels(), 8192, 4096, GL_RGB);
    
    // load & allocate specular map
    specularMapImage.loadImage(texture +"-specular.png");
    specularMap.allocate(8192, 4096, GL_RGB, false);
    specularMap.loadData(specularMapImage.getPixels(), 8192, 4096, GL_RGB);

    cloudsMapImage.loadImage("earth-clouds.png");
    cloudsMap.allocate(8192, 4096, GL_RGBA, false);
    cloudsMap.loadData(cloudsMapImage.getPixels(), 8192, 4096, GL_RGBA);
    
    // setup & link shader
    shader.load("normal.vert", "normal.frag");
    
    float earthRadius = Geo::GeoUtils::EARTH_RADIUS_KM;

    cloudsSphere.set(earthRadius*1.01, 32);
    earthSphere.set(earthRadius, 32);
    
    ofQuaternion quat;
    quat.makeRotate(180, 0, 1, 0);
    earthSphere.rotate(quat);
    cloudsSphere.rotate(quat);
    
//    debris = new objectSwarm("debris.txt", ofColor(205, 250, 250));
    
    loadDebrises();
    
    cam.lookAt(ofVec3f(0.0f, 0.0f, 0.0f));
    
    Poco::DateTime now;
    current = now;
    last = now;
    
    explosionTime.assign(2007, 1, 11);
    
    titleFont.loadFont("Brown-Regular.ttf", 60);
    dateFont.loadFont("inputMono.ttf", 18);
    

    setupLights();
    setupGui();
    setupAnimatables();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    Poco::DateTime now;
    // create a time span
    Poco::Timespan diff(((now - last).totalMilliseconds() * timeSpeed)*Poco::Timespan::MILLISECONDS);
    
    
    
    current += diff;
    
    last = now;
    
    if (rotateScene) {
        rot += rotationSpeed / 1000;
    }

    for (int i = 0; i < debrises.size(); i++) {
        debrises[i]->update(current);
    }
    
    if (autoPilot) {
        // update animatables
        float dt = 1.0f / ofGetFrameRate();
        titleColor.update(dt);
        zPosition.update(dt);
        yPosition.update(dt);
        cam.lookAt(ofVec3f(0.f, 0.f, 0.f));
        cam.setPosition(0, yPosition, zPosition);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
    
    glShadeModel(GL_SMOOTH);
    ofEnableLighting();
    
    cam.begin();
    
    ofPushMatrix();
    ofRotate(rot, 0, 1, 0);
    ofScale(scaler,scaler,scaler);
    
    
    shader.begin();
    shader.setUniformTexture("textureMap", textureMap, textureMap.getTextureData().textureID);
    shader.setUniformTexture("normalMap", normalMap, normalMap.getTextureData().textureID);
    shader.setUniformTexture("glossMap", specularMap, specularMap.getTextureData().textureID);
    earthSphere.draw();
    shader.end();

    cloudsMap.bind();
    cloudsSphere.draw();
    cloudsMap.unbind();
    ofDisableLighting();

    
    for (int i = 0; i < debrises.size(); i++) {
        debrises[i]->draw();
    }
    
//    ofPushMatrix();
//    ofRotate(90, 1, 0, 0);
//    ofPushStyle();
//    ofNoFill();
//    ofSetColor(200, 100, 250);
//    ofSetCircleResolution(100);
//    ofCircle(0, 0, 0, 42164);
//    ofPushMatrix();
//    ofTranslate(42164, 0, 0);
//    ofLine(0, 0, 0, 2000, 4000, 0);
//    ofLine(2000, 4000, 0, 4000, 4000, 0);
//    ofPushMatrix();
//    ofTranslate(4000, 4000, 0);
//    ofDrawBitmapString("Geostationary Orbit (42164km)", 0, 0);
//    ofPopMatrix();
//    ofPopMatrix();
//    ofPopStyle();
//    ofPopMatrix();
//
    
    ofPopMatrix();
    
    cam.end();
    glDisable(GL_DEPTH_TEST);
    
    //outputServer.publishScreen();
    
    drawGui();
    
}

//--------------------------------------------------------------
void ofApp::loadDebrises() {

    tleFiles.listDir("tle3");
    
    std::cout << "loading " << tleFiles.size() << " files" << endl;
    for (int i = 0; i < tleFiles.size(); i++) {
        string path = tleFiles.getPath(i);
        ofLogNotice(tleFiles.getPath(i));
//        debrises.push_back(new objectSwarm(path, ofColor(128 + 128/tleFiles.size() * i, 255/tleFiles.size() * i, 255 -(255/tleFiles.size() * i))));
        debrises.push_back(new objectSwarm(path, ofColor(255)));
    }
}

//--------------------------------------------------------------
void ofApp::setupAnimatables() {
    titleColor.setColor(ofColor(255, 0));
    titleColor.setDuration(100.0f);
    titleColor.setRepeatType(LOOP_BACK_AND_FORTH);
    titleColor.setCurve(VERY_LATE_EASE_IN_EASE_OUT);
    titleColor.animateTo(ofColor(255, 255));
    
    zPosition.setCurve(EASE_IN_EASE_OUT);
    zPosition.setDuration(75.0f);
    zPosition.setRepeatType(LOOP_BACK_AND_FORTH);
    zPosition.animateFromTo(4000, 250);
    
    yPosition.setDuration(180.f);
    yPosition.setRepeatType(LOOP_BACK_AND_FORTH);
    yPosition.setCurve(EASE_IN_EASE_OUT);
    yPosition.animateFromTo(-1000, 1000);
}

//--------------------------------------------------------------
void ofApp::drawGui() {
    ofPushStyle();
    ofSetColor(255);
    if (drawText) {
        ofPushStyle();
        titleColor.applyCurrentColor();
//        titleFont.drawStringCentered("ORBITAL DEBRIS", ofGetWidth()/2, ofGetHeight()/2);
        titleFont.drawStringCentered("ORBITAL DEBRIS", ofGetWidth()/2, 100);
        ofPopStyle();
    }
    
    if (drawDate) {
        std::stringstream timeCompressionStr;
        timeCompressionStr << "Time Compression: " << timeSpeed << "x";
        dateFont.drawString(timeCompressionStr.str(), 20, ofGetHeight() - 60);

        string formatedTime = Poco::DateTimeFormatter::format(current, Poco::DateTimeFormat::ASCTIME_FORMAT);
        dateFont.drawString(formatedTime, 20, ofGetHeight() - 30);
    }
    
    if (drawFps) {
        std::stringstream frameRate;
        frameRate << (int)ofGetFrameRate();
        dateFont.drawString(frameRate.str(), ofGetWidth() - 50, 30);
    }
    
    if (!bHide) {
        gui.draw();
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::setupLights() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    
    GLfloat lmKa[] = {.0, .0, .0, 1.0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
    
    float earthRadius = Geo::GeoUtils::EARTH_RADIUS_KM;
    
    GLfloat light_ambient[] =  { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] =  { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { -earthRadius*2, 0, earthRadius*2, 1.0 }; //light directional if w=0 without shader works
    //    GLfloat light_position[] = { 220.0, 10.0, 0.0, 1.0 };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
    
    GLfloat mat_ambient[] =  {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_diffuse[] =  {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat mat_specular[] = {.80f, 0.80f, 0.80f, 1.0f};
    glMaterialfv(GL_FRONT,GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS, 2.0);
}

//--------------------------------------------------------------
void ofApp::setupGui() {

    setToExplosionTime.addListener(this, &ofApp::setToExplosionTimePressed);
    setToCurrentTime.addListener(this, &ofApp::setToCurrentTimePressed);
    
    ofxGuiSetFont("guiFont.ttf", 10);
    bHide = true;
    gui.setup();
    gui.add(drawText.setup("Show Title", true));
    gui.add(drawDate.setup("Show Date", true));
    gui.add(drawFps.setup("Show Framerate", false));
    gui.add(rotateScene.setup("Rotate Scene", true));
    gui.add(rotationSpeed.setup("Rotation speed", 10, 0, 100));
    gui.add(timeSpeed.setup("Time Compression", 10, 1, 10000));
    gui.add(setToExplosionTime.setup("Set Fengyun 1C exp."));
    gui.add(setToCurrentTime.setup("Set current time"));
    gui.add(autoPilot.setup("Autopilot", true));
 
    debrisToggle.setup();
    
    
}


//--------------------------------------------------------------
void ofApp::setToExplosionTimePressed() {
    current = explosionTime;
}

//--------------------------------------------------------------
void ofApp::setToCurrentTimePressed() {
    Poco::DateTime now;
    current = now;
}



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if( key == 'h' ){
        bHide = !bHide;
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
