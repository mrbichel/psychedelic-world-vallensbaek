#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"

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
    
    ofParameter<ofVec3f>  viewerPos;
    
    ofCamera cam;
    ofPlanePrimitive landscape;
		
    ofCylinderPrimitive cyl;
    
    int w;
    int h;
    
    ofLight light;
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
    
    ofParameter<ofVec3f> viewerOffset;
    
    
    ofPlanePrimitive wall;
    
    // wall model
    // voronoi break through - world opens
    
    // depth of field effect
    
    // colors ?
    
    
    // has blob
    
    
    
    
    
};
