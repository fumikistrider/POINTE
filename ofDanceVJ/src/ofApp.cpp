#include "ofApp.h"
#include "RedState.h"
#include "GreenState.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// setup shared data
	stateMachine.getSharedData().counter = 0;
	stateMachine.getSharedData().lastUpdate = ofGetElapsedTimeMillis();

	// initialise state machine
	stateMachine.addState<RedState>();
	stateMachine.addState<GreenState>();
	stateMachine.changeState("green");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0, 0, 0);
	ofDrawBitmapString("Mouse click changes state", 20, 20);
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
