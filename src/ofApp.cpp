#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    w = ofGetWidth();
    h = ofGetHeight();
    
    landscape.set(10*w,3*h, 4, 4);
    landscape.rotate(90, 1, 0, 0);
    landscape.setPosition(0, 1*h, 0);
    //landscape.setHeight(0.2);
    
    params.setName("ss");
    
    params.add(viewerPos.set("pos", ofVec3f(0,0,0), ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)));
    
 params.add(nearThreshold.set("nearThreshold", 0, 0, 255));
 params.add(farThreshold.set("farThreshold", 0, 0, 255));
 params.add(viewerOffset.set("viewerOffset", ofVec3f(0,0,0),ofVec3f(0,0,0), ofVec3f(kinect.width,kinect.height,1000)));
    
    
    light.setPointLight();
    light.setup();
    
    
    panel.setup(params);
    
    kinect.init();
    
    kinect.open();
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
    farThreshold = 70;
    
    
    cam.setPosition(0,0,-2000);
    cam.setupPerspective();
    
    viewerPos.set(cam.getPosition());
    
    cam.lookAt(ofVec3f(0,0,0));
    
    
    
    panel.loadFromFile("settings.xml");

    
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    kinect.update();

    if(kinect.isFrameNew()) {
        
        // find blob
        grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        
        grayImage.flagImageChanged();
        
        contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
        
        // choose largest blob - stick to it
        for(int i=0; i<contourFinder.nBlobs; i++) {
            
            if(i == 0) {
                
                // add filtering
                viewerOffset = contourFinder.blobs[i].centroid;
                
                // some get distance at
            }
        }
    }
    
    cam.setPosition(viewerPos.get() - viewerOffset.get());
    
    //
    cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    //cam.setupPerspective();
}



void ofApp::drawFrame() {
    // bottom
    ofDrawBox(-0.5*w, -0.5*h, 0,
              
              1*w, 10, 10);
    
    //
    ofDrawBox(1*w, 0, 0,
              10, 1*h, 10);
    
    //top
    ofDrawBox(0.5*w, 0.5*h, 0,
              1*w, 10, 10);
    
    //
    ofDrawBox(-1*w, 0, 0,
              10, 1*h, 10);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSeedRandom(1);
    
    ofEnableLighting();

    ofEnableDepthTest();
    
    light.enable();
    ofBackground(0, 0, 0);
    cam.begin();
    
    //ofTranslate(w/2, h/2);
    ofScale(0.5, 0.5);
    
    ofSetColor(100,100,100);

    //landscape.draw();
    
    ofSetColor(255,255,255);
    //ofDrawPlane(0,0, 2, 2);
    //ofDrawSphere(0, 0, 0.02*w);
    
    /*ofDrawSphere(-1*w, -1*h, 0.02*w);
    ofDrawSphere(1*w, 1*h, 0.02*w);
    
    ofDrawSphere(-1*w, 1*h, 0.02*w);
    ofDrawSphere(1*w, -1*h, 0.02*w);
    */
    
    ofPushMatrix();
    
    
    
    
    for(int i=0; i<100; i++) {
        ofPushMatrix();
        
        ofTranslate(0, 0, i*40);
        //ofScale(ofRandom(0.8, 6), ofRandom(1, 4), 1);
        drawFrame();
        ofPopMatrix();
        
    }
    
    for(int i=0; i<2000; i++) {
        
        ofPushMatrix();
        
        ofColor col;
        col.set(255,255,255);
        //col.setBrightness(220);
        //col.setSaturation(255);
        //col.setHueAngle(sin((i/200)+ofGetElapsedTimef()/10)*360);
        
        //col.a = 160;
        
        ofSetColor(col);
        
        ofTranslate(0,0,i*1);
        
        ofRotateX(i*-2 + ofGetElapsedTimef());
        ofRotateY(i*-2 + ofGetElapsedTimef());
        ofRotateZ(i*-0.5 + ofGetElapsedTimef());
        
        //ofDrawBox(i*4, 0, 0, 0.009*w, 1.9*h, 0.009*w);
        ofLine(0
               , 0, 0, 0.5*h);
        //ofDrawCylinder(i*10, 0, 0.0009*w, 1.9*h);
        
        ofPopMatrix();
    }
    
    
    light.draw();
    
    ofPopMatrix();
    
    cam.end();
    
    
    //cam.draw();
    
    light.disable();
    
    ofDisableLighting();
    
    ofDisableDepthTest();
    
    ofSetColor(255, 255, 255);
    
    
    
    kinect.drawDepth(10, 10, 40, 30);
    kinect.draw(42, 10, 40, 30);
    
    grayImage.draw(10, 32, 40, 30);
    contourFinder.draw(10, 32, 40, 30);

    
    panel.draw();
    
    
}

void ofApp::exit() {
    panel.saveToFile("settings.xml");
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
