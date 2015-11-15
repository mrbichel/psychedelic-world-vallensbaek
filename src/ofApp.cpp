#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofEnableAntiAliasing();
    
    w = 1920;
    h = 1080*2;
    
    outFbo.allocate(w, h);
    
    syphonOut.setName("wall");
    
    params.setName("ss");
    
    ofEnableAlphaBlending();
    
    params.add( lightPos.set("light pos", ofVec3f(0,0,0),    ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)));
    params.add(camRefPos.set("cam ref pos", ofVec3f(0,0,0), ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)));
    
    params.add(nearThreshold.set("nearThreshold", 0, 0, 255));
    params.add( farThreshold.set("farThreshold", 0, 0, 255));
    
    params.add(   focalRange.set("focal range", 0, 0, 200));
    params.add(focalDistance.set("focal dist", 0, 0, 200));
    params.add(    focalBlur.set("focal blur", 0, 0, 200));
    
    //params.add(viewerOffset.set("viewerOffset", ofVec3f(0,0,0),ofVec3f(0,0,0), ofVec3f(kinect.width,kinect.height,1000)));
    
    params.add(destroy.set("destroy", 0, 0, 1));
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    light.setup();
    
    //light.setSpotlight();
    //light.setPosition(0, 0, -80);
    light.setAttenuation(0.1);
    
    //light.setSpotlightCutOff(<#float spotCutOff#>)
    light.setDiffuseColor( ofColor(240.f, 255.f, 240.f));
    
    // specular color, the highlight/shininess color //
    light.setSpecularColor( ofColor(255.f, 255.f, 0.f));
    light.setPointLight();
    light.enable();
    
    spotLight.setDiffuseColor(  ofColor(255.f, 230.f, 230.f));
    spotLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    // turn the light into spotLight, emit a cone of light //
    spotLight.setSpotlight();
    
    // size of the cone of emitted light, angle between light axis and side of cone //
    // angle range between 0 - 90 in degrees //
    spotLight.setSpotlightCutOff( 50 );
    
    // rate of falloff, illumitation decreases as the angle from the cone axis increases //
    // range 0 - 128, zero is even illumination, 128 is max falloff //
    spotLight.setSpotConcentration( 45 );
    
    spotLight.enable();
    
    
    // Directional Lights emit light based on their orientation, regardless of their position //
    directionalLight.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    directionalLight.setSpecularColor(ofColor(255.f, 255.f, 255.f));
    directionalLight.setDirectional();
    
    
    // set the direction of the light
    // set it pointing from left to right -> //
    directionalLight.setOrientation( ofVec3f(0, 0, 1) );
    directionalLight.enable();
    
    mat.setShininess( 220 );
    // the light highlight of the material //
    mat.setSpecularColor(ofColor(255, 255, 255, 255));
    mat.setAmbientColor( ofColor(255, 255, 255, 255));
    mat.setEmissiveColor(ofColor(255, 255, 255, 255));
    
    panel.setup(params);
    viewerOffsetFiltered.setFc(0.065);
    
    kinect.init();
    kinect.open();
    
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
    farThreshold = 70;
    
    panel.loadFromFile("settings.xml");
    
    cam.setupPerspective();
    cam.setPosition(ofVec3f(0,0,-cam.getImagePlaneDistance()));
    cam.lookAt(ofVec3f(0,0,1));
    
    
    camRefPos.set(cam.getPosition());
    
    cout<<cam.getImagePlaneDistance()<<endl;
    //cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    modelLoader.loadModel("Normalised.obj");
    //modelLoader.update();
    
    //modelLoader.setScaleNomalization(true);
    
    m = modelLoader.getMesh(0);
    
    //  Define a container
    //
    
    dof.setup(w, h);
    
    
    //  Add the cell seed to the container
    //
    
    float maxX, minX, maxY, minY, maxZ, minZ = NULL;
    
    for(int i = 0; i < m.getNumVertices();i++){
        
        ofVec3f p = m.getVertices()[i];
        
        maxX = max(maxX, p.x);
        minX = min(minX, p.x);
        
        maxY = max(maxY, p.y);
        minY = min(minY, p.y);
        
        maxZ = max(maxZ, p.z);
        minZ = min(minZ, p.z);
    }
    
    
    /*cout<<minX<<endl;
     cout<<maxX<<endl;
     cout<<minY<<endl;
     cout<<maxY<<endl;
     cout<<minZ<<endl;
     cout<<maxZ<<endl;*/
    
    voro::container con(-100,100,
                        -100,100,
                        -120,-50,
                        1,1,1,
                        true,true,true,
                        8);
    
    for(int i = 0; i < m.getUniqueFaces().size();i++){
        
        ofMeshFace f;
        ofVboMesh vboF;
        
        f = m.getUniqueFaces()[i];
        
        vboF.addVertex(f.getVertex(0));
        vboF.addVertex(f.getVertex(1));
        vboF.addVertex(f.getVertex(2));
        
        triangles.push_back(vboF);
        
    }
    
    
    
    
    for(int i = 0; i < m.getNumVertices();i+=3){
        
        
        //ofPoint newCell = ofPoint(ofRandom(-w*1,w*0.5),
        //                          ofRandom(-h*0.5,h*0.5),
        //                          ofRandom(-w*0.1,w*0.1));
        
        
        //ofPoint newCell = ofPoint(m.getVertices()[i]);
        
        
        
        //ofPoint newCell = ofPoint(ofRandom(-100,100),
        //                         ofRandom(-100,100),
        //                       ofRandom(-100,-50));
        
        
        //newCell.x = m.getVertices()[i].x;
        //newCell.y = m.getVertices()[i].y;
        //newCell.z = m.getVertices()[i].z;
        
        //if(insideContainer(con, newCell)) cout<<i<<":   "<<newCell.z<<endl;
        
        //addCellSeed(con, newCell, i, true);
    }
    
    //addCellsSeeds(con, m.getVertices(), true);
    
    
    /*cellMeshes.clear();
     cellCentroids.clear();
     cellMeshWireframes.clear();
     cellRadius.clear();*/
    
    cellMeshes = getCellsFromContainer(con,0.0);
    //cellMeshWireframes = getCellsFromContainer(con,0.0,true);
    cellRadius = getCellsRadius(con);
    cellCentroids = getCellsCentroids(con);
    
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
        
        
        if(contourFinder.nBlobs > 0) {
            if(!hasBlob) {
                lastBlobArrivedTime = ofGetElapsedTimeMillis();
                hasBlob = true;
            }
        } else {
            if(hasBlob) {
                // TODO: wait a few frames
                hasBlob = false;
            }
        }
        
        
        if(hasBlob) {
            if(destroy < 1)
                destroy += 0.01;
        } else {
            if(destroy > 0)
                destroy *= 0.98;
        }
        
        // choose largest blob - stick to it
        for(int i=0; i<contourFinder.nBlobs; i++) {
            if(i == 0) {
                viewerOffset.x = contourFinder.blobs[i].centroid.x * 10;
                viewerOffset.z = contourFinder.blobs[i].centroid.y;
                //viewerOffset.y = kinect.getDistanceAt(viewerOffset);
                // some get distance at
            }
        }
    }
    
    viewerOffsetFiltered.update(viewerOffset);
    
    cam.setPosition(camRefPos.get() - viewerOffsetFiltered.value());
    //cam.lookAt(ofVec3f(0,0,-1000));
    cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    light.setPosition(lightPos.get());
    
    dof.setFocalRange(focalRange.get());
    dof.setFocalDistance(focalDistance.get());
    dof.setBlurAmount(focalBlur.get());
    
}



void ofApp::drawFrame() {
    
    // full
    //ofSetColor(0,0,255,255);
    //ofDrawBox(-1*w, -1*h, 0, 2*w, 2*h, 10);
    
    // top
    
    //ofDrawBox(<#float x#>, <#float y#>, <#float z#>, <#float width#>, <#float height#>, <#float depth#>)
    
    //
    //ofDrawBox(1*w, 0, 0,
    //          10, 1*h, 10);
    
    //top
    //ofDrawBox(0.5*w, 0.5*h, 0,
    //          1*w, 10, 10);
    
    //
    //ofDrawBox(-1*w, 0, 0,
    //          10, 1*h, 10);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    ofEnableLighting();
    ofBackground(0, 0, 0);
    
    dof.begin(); mat.begin(); cam.begin();
    ofPushMatrix(); {
        
        for(int i=0; i<36; i++) {
        
            int ysteps = 8;
            int xsteps = 8;
            int y=ysteps;
        
            ofPushMatrix();{
            
                ofRotateZ((i%18)*10);
                ofTranslate(-1*w,-1*h,100 + (i*40));
            
                ofFill();
                ofSetColor(255, 255, 255);
                ofSetPolyMode(OF_POLY_WINDING_ODD);
                ofBeginShape(); {
                
                    for(int x=1; x<=xsteps; x++) {
                        ofVertex(x * (w*2/ysteps), 0, x*2);
                        ofVertex(0, y * ((h*2)/xsteps), x*2);
                        ofVertex(0+10,  y * ((h*2)/xsteps)+10, x*2);
                        ofVertex(x * (w*2/ysteps)+10, 10, x*2);
                        y--;
                    }
                
                } ofEndShape(true);
            
            } ofPopMatrix();
        
        
            ofPushMatrix(); {
            
                ofRotateZ((i%36)*10 + 180);
                ofTranslate(-1*w,-1*h,100 + (i*40));
            
                ysteps = 4;
                xsteps = 4;
            
                ofBeginShape(); {
                
                    y=ysteps;
                    for(int x=1; x<=xsteps; x++) {
                    
                        ofVertex(x * (w*2/ysteps), 0, x*2);
                        ofVertex(0, y * ((h*2)/xsteps), x*2);
                    
                        ofVertex(0+10,  y * ((h*2)/xsteps)+10, x*2);
                        ofVertex(x * (w*2/ysteps)+10, 10, x*2);
                    
                        y--;
                    }
                
                } ofEndShape(true);
            } ofPopMatrix();
        } ofPopMatrix();
    
        ofPushMatrix(); {
        //ofPushStyle(); {
            ofTranslate(0,-1*h, 0);
            ofScale(16, 16);
    
            
            
            for (int i = 0; i < triangles.size(); i++){
                ofSetColor(255,255,255,255);
                ofPushMatrix();
        
                ofTranslate(0,400*powf(destroy.get(),2), ofNoise(i*10)*i*destroy.get()*400*destroy.get());
        
                ofTranslate(triangles[i].getCentroid());
                ofRotateX(ofNoise(i*10)*i*destroy.get()*100*destroy.get());
                ofTranslate(-triangles[i].getCentroid());
        
                //
                triangles[i].draw();
                ofSetColor(0,0,0,255);
                triangles[i].drawWireframe();
                ofPopMatrix();
            }
        }
        
    } ofPopMatrix();
    
    cam.end(); mat.end(); dof.end();
    
    outFbo.begin();
    ofBackground(0,0,0);
    ofSetColor(255);
    dof.getFbo().draw(0, 0);
    outFbo.end();
    
    
    ofDisableLighting();
    ofDisableDepthTest();
    ofDisableAlphaBlending();
    ofSetColor(255, 255, 255);
    
    kinect.drawDepth(10, 10, 200, 150);
    kinect.draw(220, 10, 200, 150);
    
    grayImage.draw(10, 170, 200, 150);
    contourFinder.draw(10, 170, 200, 150);
    
    outFbo.draw(220, 170,outFbo.getWidth()/4, outFbo.getHeight()/4);
    
    panel.draw();
    
    
    syphonOut.publishTexture(&outFbo.getTexture());
    
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
