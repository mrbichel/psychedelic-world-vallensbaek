#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxBiquadFilter.h"
#include "ofxAssimpModelLoader.h"
#include "ofxSyphon.h"
#include "ofxCv.h"

#define STRINGIFY(A) #A
/*
 
 todo
 
 better dof
 - animate Y based on dist to kinect
 - calibrate filter
 - dof pulses
 - continues movement 
 - reaction to multiple persons 
 - color fade 
 - noise flitchy destruction pules 
 - mix with random fbo content maybe 
 - pulses comes from sudden movement on
 - return to center before closing
 
 */

/*
enum State
{
    Idle,
    Active,
};*/

class triangle {
    
public:
    triangle() {
        
    }
    
    ofVboMesh mesh;
    ofVec3f centroid;
    float rand;
    
    //ofQuaternion rot;
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawFrame();
    
    ofCamera cam;
    ofCamera testModelCam;
    ofPlanePrimitive landscape;
    ofCylinderPrimitive cyl;
    
    int w;
    int h;
    
    ofxSyphonServer syphonOut;
    ofFbo outFbo;
    ofFbo dofFbo;
    
    ofLight light;
    
    ofShader DOFShader;
    
    ofLight spotLight;
    ofLight directionalLight;
    
    ofMaterial mat;
    
    ofxPanel panel;
    ofParameterGroup params;
    
    ofxKinect kinect;
    
    ofImage depthImage; // grayscale depth image
    ofImage grayThreshNear; // the near thresholded image
    ofImage grayThreshFar; // the far thresholded image
    ofImage diff;
    ofImage bgImg;
    
    ofxCv::ContourFinder contourFinder;
    
    ofParameter<float> blur;
    ofParameter<bool> setBg;
    ofParameter<float> minAreaRadius;
    ofParameter<float> contourThreshold;
    
    ofParameter<float> minBlobSize;
    
    ofParameter<float> maxBlobSize;
    
    ofParameter<float> aperture;
    ofParameter<float> focus;
    ofParameter<float> maxBlur;
    
    ofVec3f viewerOffset; // kinect pos - needs to be rotated into projection matrix
    
    ofPlanePrimitive wall;
    
    // wall model
    // voronoi break through - world opens
    
    // depth of field effect
    
    // colors ?
    // has blob
    ofParameter<ofVec3f> camRefPos;
    ofParameter<float> destroy;
    ofxBiquadFilter1f destroyFiltered;
    ofxBiquadFilter3f viewerOffsetFiltered;
    
    ofParameter<ofVec3f> lightPos;
    
    
    ofxAssimpModelLoader modelLoader;
    
    ofMesh m;
    
    vector<ofVboMesh>  triangles;
        
    // state
    
    bool hasBlob = false;
    unsigned int long lastBlobArrivedTime;
    
    
    void texturedQuad(float x, float y, float width, float height, float s = 1.0, float t = 1.0);
    
    
};
