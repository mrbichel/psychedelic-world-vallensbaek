#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxBiquadFilter.h"
#include "ofxAssimpModelLoader.h"
#include "ofxVoro.h"
#include "ofxSyphon.h"
#include "ofxDOF.h"


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
    ofPlanePrimitive landscape;
    ofCylinderPrimitive cyl;
    
    int w;
    int h;
    
    ofxSyphonServer syphonOut;
    ofFbo outFbo;
    
    ofLight light;
    
    ofLight spotLight;
    ofLight directionalLight;
    
    ofMaterial mat;
    
    ofxPanel panel;
    ofParameterGroup params;
    
    ofxKinect kinect;
    
    ofxCvColorImage colorImg;
    
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    bool bThreshWithOpenCV;
    bool bDrawPointCloud;
    
    ofParameter<int> nearThreshold;
    ofParameter<int> farThreshold;
    
    ofParameter<int> minBlobSize;
    
    ofParameter<int> maxBlobSize;
    
    ofParameter<float> focalRange;
    ofParameter<float> focalDistance;
    ofParameter<float> focalBlur;
    
    ofVec3f viewerOffset; // kinect pos - needs to be rotated into projection matrix
    
    ofPlanePrimitive wall;
    
    // wall model
    // voronoi break through - world opens
    
    // depth of field effect
    
    // colors ?
    // has blob
    ofParameter<ofVec3f> camRefPos;
    ofParameter<float> destroy;
    ofxBiquadFilter3f viewerOffsetFiltered;
    
    ofParameter<ofVec3f> lightPos;
    
    
    ofxAssimpModelLoader modelLoader;
    
    vector<ofPoint> cellCentroids;
    vector<float>   cellRadius;
    vector<ofVboMesh>  cellMeshes;
    vector<ofVboMesh>  cellMeshWireframes;
    ofMesh m;
    
    
    vector<ofVboMesh>  triangles;
    
    ofxDOF dof;
    
    // state
    
    bool hasBlob = false;
    unsigned int long lastBlobArrivedTime;
    
    
    
    
};
