#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Video Synth");
	ofSetWindowShape(1280, 720);
	ofSetFrameRate(60);
	ofBackground(ofColor::white);

	ofColor customColor(10, 100, 5, 0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	stripePattern();

	ofPopMatrix();
}

void ofApp::stripePattern()
{
	ofSetColor(ofColor::black);
	ofSetLineWidth(3.0);

	ofNoFill();

	for (int i = -50; i < 50; i++) {
		ofPushMatrix();

		//Rotate and Translate the coordinates system.
		ofTranslate(i * 20, 0);
		ofRotate(i * 7);
		ofSetColor(2 * i, 1 * i, 3 * i, 255);
		//ofLine(0, -100, 0, 100);
		//Scale coordinate system 6 times
		ofScale(6, 6);
		ofTriangle(0, 0, -50, 100, 50, 100);
		ofPopMatrix();
	}
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
