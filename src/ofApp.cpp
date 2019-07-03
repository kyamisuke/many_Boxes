#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    this->shader.load("shader.vert", "shader.frag");
    this->blur.load("shader/blur.vert", "shader/blur.frag");
    this->bloom.load("shader/bloom.vert", "shader/bloom.frag");
    
//    this->sound.load("music/music1.wav");
//    this->sound.setLoop(true);
//    this->sound.play();
//    this->nBandsToGet = 60;
//    this->fftSmoothed = new float[8192];
//    for (int i=0; i<8192; i++) this->fftSmoothed[i] = 0;
    
    this->fbo.allocate(ofGetWidth(), ofGetHeight());
    this->onePassFbo.allocate(ofGetWidth(), ofGetHeight());
    this->twoPassFbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSeedRandom(39);
    
//    ofSoundUpdate();
//    this->volume = ofSoundGetSpectrum(this->nBandsToGet);
//
//    for (int i=0; i<this->nBandsToGet; i++) {
//        this->fftSmoothed[i] *= 0.96f;
//        if (this->fftSmoothed[i] < this->volume[i]) this->fftSmoothed[i] = this->volume[i];
//    }
    
    this->fbo.begin();
    ofClear(0, 0, 0, 0);
    this->shader.begin();
    
    this->shader.setUniform1f("time", ofGetElapsedTimef());
    this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    this->shader.setUniform2f("mouse", mouseX, mouseY);
    this->shader.setUniform1f("volume", ofNoise(ofRandom(100), ofGetFrameNum()*0.02));
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    this->shader.end();
    this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // X方向のガウシアンブラー
    this->onePassFbo.begin();

    this->blur.begin();

    this->blur.setUniformTexture("tex", this->fbo.getTexture(), 0);
    this->blur.setUniform1i("horizontal", true);
    this->blur.setUniform1f("strength", this->strength);
    this->fbo.draw(0, 0);

    this->blur.end();

    this->onePassFbo.end();
    //
    // Y方向のガウシアンブラー
    this->twoPassFbo.begin();

    this->blur.begin();
    this->blur.setUniformTexture("tex", this->onePassFbo.getTexture(), 0);
    this->blur.setUniform1i("horizontal", false);
    this->blur.setUniform1f("strength", this->strength);

    this->onePassFbo.draw(0, 0);

    this->blur.end();

    this->twoPassFbo.end();

    // ガウシアンブラーと元の描画内容を加算合成
    this->bloom.begin();
    this->bloom.setUniformTexture("tex", this->fbo.getTexture(), 0);
    this->bloom.setUniformTexture("blur", this->twoPassFbo.getTexture(), 1);

    this->fbo.draw(0, 0);

    this->bloom.end();
    
//    this->fbo.draw(0, 0);
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
