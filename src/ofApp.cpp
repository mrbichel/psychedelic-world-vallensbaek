#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofDisableArbTex();
    
    string fragShaderSrc = STRINGIFY(
                                     uniform sampler2D tColor;
                                     uniform sampler2D tDepth;
                                     
                                     uniform float maxBlur;  // max blur amount
                                     uniform float aperture; // aperture - bigger values for shallower depth of field
                                     
                                     uniform float focus;
                                     uniform float aspect;
                                     
                                     void main() {
                                         vec2 vUv = gl_TexCoord[0].st;
                                         
                                         vec2 aspectcorrect = vec2( 1.0, aspect );
                                         
                                         vec4 depth1 = texture2D( tDepth, vUv );
                                         
                                         float factor = depth1.x - focus;
                                         
                                         vec2 dofblur = vec2 ( clamp( factor * aperture, -maxBlur, maxBlur ) );
                                         
                                         vec2 dofblur9 = dofblur * 0.9;
                                         vec2 dofblur7 = dofblur * 0.7;
                                         vec2 dofblur4 = dofblur * 0.4;
                                         
                                         vec4 col = vec4( 0.0 );
                                         
                                         col += texture2D( tColor, vUv.xy );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.15,  0.37 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.37,  0.15 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.40,  0.0  ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.37, -0.15 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.15,  0.37 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.37,  0.15 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.15, -0.37 ) * aspectcorrect ) * dofblur );
                                         
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.15,  0.37 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.37,  0.15 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.15,  0.37 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.37,  0.15 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.37, -0.15 ) * aspectcorrect ) * dofblur9 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.15, -0.37 ) * aspectcorrect ) * dofblur9 );
                                         
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.40,  0.0  ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur7 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur7 );
                                         
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29,  0.29 ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.4,   0.0  ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,  -0.4  ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29,  0.29 ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.4,   0.0  ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2( -0.29, -0.29 ) * aspectcorrect ) * dofblur4 );
                                         col += texture2D( tColor, vUv.xy + ( vec2(  0.0,   0.4  ) * aspectcorrect ) * dofblur4 );
                                         
                                         gl_FragColor = col / 41.0;
                                         gl_FragColor.a = 1.0;
                                     }
                                     );
    
    DOFShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
    DOFShader.linkProgram();

    
    // enable depth->video image calibration
    kinect.setRegistration(false);
    kinect.setDepthClipping(1000, 8000);
    kinect.enableDepthNearValueWhite(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();		// opens first available kinect
    
    // print the intrinsic IR sensor values
    if(kinect.isConnected()) {
        ofLogNotice() << "sensor-emitter dist: "    << kinect.getSensorEmitterDistance()    << "cm";
        ofLogNotice() << "sensor-camera dist:  "    << kinect.getSensorCameraDistance()     << "cm";
        ofLogNotice() << "zero plane pixel size: "  << kinect.getZeroPlanePixelSize()       << "mm";
        ofLogNotice() << "zero plane dist: "        << kinect.getZeroPlaneDistance()        << "mm";
    }
    
    
    ofEnableAntiAliasing();
    
    w = 1920;
    h = 1080*2;
    
    
    ofFbo::Settings fboSettings;
    fboSettings.width = w;
    fboSettings.height = h;
    fboSettings.numSamples = 8;
    
    outFbo.allocate(fboSettings);
    
    fboSettings.useDepth = true;
    fboSettings.depthStencilAsTexture = true;
    fboSettings.numSamples = 0;
    
    dofFbo.allocate(fboSettings);
    
    syphonOut.setName("wall");
    
    params.setName("ss");
    
    ofEnableAlphaBlending();
    
    params.add( lightPos.set("light pos", ofVec3f(0,0,0),    ofVec3f(-2000,-2000,-2000), ofVec3f(2000,2000,2000)));
    params.add(camRefPos.set("cam ref pos", ofVec3f(0,0,0), ofVec3f(-6000,-6000,-6000), ofVec3f(6000,6000,6000)));
    
    params.add(minAreaRadius.set("minAreaRadius", 0, 0, kinect.width/2));
    
    params.add( contourThreshold.set("threshold contour", 0, 0, 255));
    
     params.add( blur.set("blur", 10, 0, 255));
    params.add( setBg.set("setBg", false));
    
    params.add(   aperture.set("aperture", 0.5, 0, 1));
    params.add(focus.set("focus", 0.98, 0.95, 1));
    params.add(    maxBlur.set("max blur", 0.5, 0, 1));
    
    //params.add(viewerOffset.set("viewerOffset", ofVec3f(0,0,0),ofVec3f(0,0,0), ofVec3f(kinect.width,kinect.height,1000)));
    
    params.add(destroy.set("destroy", 0, 0, 1));
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    light.setup();
    
    //light.setSpotlight();
    //light.setPosition(0, 0, -80);
    //light.setAttenuation(0.1);
    
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
    
    mat.setShininess( 0.9f );
    // the light highlight of the material //
    //mat.setSpecularColor(ofColor(255, 255, 255, 255));
    //mat.setAmbientColor( ofColor(255, 255, 255, 255));
    mat.setEmissiveColor(ofColor(255, 255, 255, 255));
    
    panel.setup(params);
    viewerOffsetFiltered.setFc(0.005);
    viewerOffsetFiltered.clear(ofVec3f(0,0,0));
    destroyFiltered.setFc(0.005);
    destroyFiltered.clear(0);
    
    //colorImg.allocate(kinect.width, kinect.height);
    //grayImage.allocate(kinect.width, kinect.height);
    //grayThreshNear.allocate(kinect.width, kinect.height);
    //grayThreshFar.allocate(kinect.width, kinect.height);
    
    depthImage.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    ofxCv::imitate(diff, depthImage);
    ofxCv::imitate(bgImg, depthImage);
    
    bgImg.load("bgImg.png");
    
    panel.loadFromFile("settings.xml");
    
    //testModelCam.setupPerspective();
    //testModelCam.setPosition(ofVec3f(0,0,-cam.getImagePlaneDistance()));
    //testModelCam.lookAt(ofVec3f(0,0,1));
    
    cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    //modelLoader.get
    //testModelCam.se
    
    cam.setupPerspective();
    cam.setPosition(ofVec3f(0,0,-cam.getImagePlaneDistance()));
    cam.lookAt(ofVec3f(0,0,1));
    
    //camRefPos.set(cam.getPosition());
    
    //cout<<cam.getImagePlaneDistance()<<endl;
    //cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    modelLoader.loadModel("Normalised.obj");
    modelLoader.update();
    
    //modelLoader.setScaleNomalization(true);
    
    m = modelLoader.getMesh(0);
    
    //  Define a container
    //
    
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
    
    for(int i = 0; i < m.getUniqueFaces().size();i++){
        
        ofMeshFace f;
        ofVboMesh vboF;
        
        f = m.getUniqueFaces()[i];
        
        vboF.addVertex(f.getVertex(0));
        vboF.addVertex(f.getVertex(1));
        vboF.addVertex(f.getVertex(2));
        
        vboF.addColor(ofFloatColor(i*1.0/m.getUniqueFaces().size()*1.0));
        
        triangles.push_back(vboF);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    focus.set(ofMap(ofNoise(ofGetElapsedTimef()/10.0), 0, 1, 0.95, 1));
    
    kinect.update();
    if(kinect.isFrameNew()) {
        
        // find blob
        
        depthImage.setFromPixels(kinect.getDepthPixels());
        
        if(setBg.get()) {
            ofxCv::copy(depthImage, bgImg);
            bgImg.update();
            bgImg.save("bgImg.png");
            setBg.set(false);
        }
        
        ofxCv::absdiff(depthImage, bgImg, diff);
        ofxCv::blur(diff, blur.get());
        diff.update();
        
        contourFinder.setSortBySize(true);
        contourFinder.setMinAreaRadius(minAreaRadius.get());
        
        contourFinder.setThreshold(contourThreshold.get());
        contourFinder.findContours(diff);
        
        if(contourFinder.size() > 0) {
            if(!hasBlob) {
                lastBlobArrivedTime = ofGetElapsedTimeMillis();
                hasBlob = true;
            }
        } else {
            if(hasBlob) {
                hasBlob = false;
            }
        }
        
        if(hasBlob && ofGetElapsedTimeMillis() - lastBlobArrivedTime > 2000) {
            if(destroy < 1)
                destroy += 0.01;
        } else {
            if(destroy > 0)
                destroy *= 0.98;
        }
        
        destroy = ofClamp(destroy, 0, 1);
        
        // choose largest blob - stick to it
        for(int i=0; i<contourFinder.size(); i++) {
            if(i == 0) {
                if(destroyFiltered.value() > 0.5) {
                    
                    viewerOffset.x = ofMap(contourFinder.getCentroid(i).x, 0, kinect.width, -w/2, w/2);
                    viewerOffset.z = ofMap(contourFinder.getCentroid(i).y, 0, kinect.height, 0, 8<00);
                    
                    viewerOffset.y = ofMap(kinect.getDistanceAt(ofxCv::toOf(contourFinder.getCentroid(i))),
                                           4000, 6000, h/5, 0, true);
                    
                    //cout<<kinect.getDistanceAt(ofxCv::toOf(contourFinder.getCentroid(i)))<<endl;
                    // some get distance at
                }
            }
        }
    }
    
    viewerOffsetFiltered.update(viewerOffset);
    destroyFiltered.update(destroy);
    
    if(!hasBlob || destroyFiltered.value() < 0.5) {
        viewerOffset *= ofVec3f(0.9,0.9,0.9);
    }
    
    cam.setPosition(camRefPos.get() - viewerOffsetFiltered.value());
    //cam.lookAt(ofVec3f(0,0,-1000));
    cam.setupOffAxisViewPortal(ofVec3f(-1*w,-1*h,0), ofVec3f(-1*w,1*h,0), ofVec3f(1*w,1*h,0));
    
    light.setPosition(lightPos.get() + ofVec3f(ofNoise(ofGetElapsedTimef()/8.0)*w,ofNoise(ofGetElapsedTimef()/8.9)*h,0));
    
    
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
    
    
    /*outFbo.begin();
    ofBackground(60, 60, 60);
    testModelCam.begin();
    
    ofSetColor(255,255,255);
    
    ofPushMatrix();
    //ofScale(-1,-1,-1);
    ofTranslate(0,200, 0);
    //ofScale(2, 2);
    //m.drawWireframe();
    modelLoader.drawWireframe();
    ofPopMatrix();
    
    testModelCam.end();
    outFbo.end();*/
    
    
    ofPushMatrix();
    ofScale(1,-1,1);
    
    ofPopMatrix();
    
    dofFbo.begin();
    
    ofBackground(0);
    ofSetColor(255);
    ofFill();

    cam.begin();
    
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
                
                mat.begin();
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
                mat.end();
            } ofPopMatrix();
        } ofPopMatrix();
    
        
        ofPushMatrix(); {
        //ofPushStyle(); {
            ofTranslate(0,-1*h, 0);
            ofScale(16, 16);
            
            for (int i = 0; i < triangles.size(); i++){
                ofPushMatrix();
        
                ofTranslate(0,400.0*powf(destroyFiltered.value(),2), ofNoise(i*10.0)*i*destroyFiltered.value()*400.0*destroy.get());
        
                ofTranslate(triangles[i].getCentroid());
                ofRotateX(ofNoise(i*10.0)*i*destroy.get()*100.0*destroy.get());
                ofTranslate(-triangles[i].getCentroid());
        
                //
                mat.begin();
                triangles[i].draw();
                mat.end();
                
                ofPopMatrix();
            }
        }
        
    } ofPopMatrix();
    
    cam.end();
    
    dofFbo.end();
    
    outFbo.begin();
    ofBackground(0);
    ofSetColor(255);
    ofFill();
    
    DOFShader.begin();
    DOFShader.setUniformTexture("tColor", dofFbo.getTexture(), 0);
    DOFShader.setUniformTexture("tDepth", dofFbo.getDepthTexture(), 1);
    DOFShader.setUniform1f("aperture", aperture.get());
    DOFShader.setUniform1f("focus", focus.get());
    DOFShader.setUniform1f("maxBlur", maxBlur.get());
    DOFShader.setUniform1f("aspect", w / h);
    
    texturedQuad(0, 0, w, h);
    DOFShader.end();
    
    //dofFbo.draw(0, 0);
    
    outFbo.end();
    
    ofDisableLighting();
    ofDisableDepthTest();
    ofDisableAlphaBlending();
    ofSetColor(255, 255, 255);
    
    
    ofPushMatrix();
    ofScale(0.25,0.25);
    
    depthImage.draw(10, 10);
    kinect.draw(depthImage.getWidth() + 10, 10);
    
    bgImg.draw(diff.getWidth() + 10, depthImage.getHeight() + 10);
    
    ofPushMatrix();
    ofTranslate(10, depthImage.getHeight() + 10);
    diff.draw(0, 0);
    contourFinder.draw();
    ofPopMatrix();
    
    outFbo.draw(depthImage.getWidth() * 2 + 40, depthImage.getHeight()*2 + 40,outFbo.getWidth(), outFbo.getHeight());
    
    ofPopMatrix();
    
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

void ofApp::texturedQuad(float x, float y, float width, float height, float s, float t)
{
    // TODO: change to triangle fan/strip
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(x, y, 0);
    
    glTexCoord2f(s, 0);
    glVertex3f(x + width, y, 0);
    
    glTexCoord2f(s, t);
    glVertex3f(x + width, y + height, 0);
    
    glTexCoord2f(0, t);
    glVertex3f(x, y + height, 0);
    glEnd();
}
