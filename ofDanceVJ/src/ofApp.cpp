#include "ofApp.h"
#include "RedState.h"
#include "GreenState.h"
#include "AttractorSoloState.h"
#include "AttractorQuadState.h"
#include "StageState.h"

//--------------------------------------------------------------
void ofApp::setup(){

	// setup shared data
	stateMachine.getSharedData().counter = 0;
	stateMachine.getSharedData().lastUpdate = ofGetElapsedTimeMillis();

	stateMachine.getSharedData().oscReceiver.setup(55555);
	stateMachine.getSharedData().oscSender.setup("127.0.0.1", 55550);

	stateMachine.getSharedData().setupMidi();

	// print input ports to console
	midiIn.listPorts(); 

	midiIn.openPort(0);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port

	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);

	// add ofApp as a listener
	midiIn.addListener(this);

	// print received messages to the console
	midiIn.setVerbose(true);

	// initialise state machine
	stateMachine.addState<RedState>();
	stateMachine.addState<GreenState>();
	stateMachine.addState<AttractorSoloState>();
	stateMachine.addState<AttractorQuadState>();
	stateMachine.addState<StageState>();
	stateMachine.changeState("AttractorQuad");
	//stateMachine.changeState("Stage");
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
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// make a copy of the latest message
	midiMessage = msg;
	stateMachine.getSharedData().midiMessage = msg;

	// debug output
	stringstream miditext;

	miditext << "[MIDI]" << midiMessage.status;
	miditext << " channel:" << midiMessage.channel;
	miditext << " control:" << midiMessage.control;
	miditext << " value:" << midiMessage.value;
	miditext << " pitch:" << midiMessage.pitch;
	miditext << " velocity:" << midiMessage.velocity;
	miditext << " Received:" << ofxMidiMessage::getStatusString(midiMessage.status);
	if (ofxMidiMessage::getStatusString(midiMessage.status) == "Sysex") {
		miditext << endl << " L (ex):" << midiMessage.toString();
	}

	cout << miditext << endl;


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
