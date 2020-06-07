#include "ofApp.h"

bool showGui;

//--------------------------------------------------------------
void ofApp::setup(){
	//Settings.
	ofSetWindowTitle("Video Synth");
	ofSetWindowShape(1280, 720);
	ofSetFrameRate(60);
	ofBackground(ofColor::white);

	//gui title=Parameters and store in a file wit name settings.xml.
	gui.setup("Parameters", "settings.xml");
	// (starting value, min value, max value).
	gui.add(countX.setup("countX", 50, 0, 200));
	gui.add(stepX.setup("stepX", 20, 0, 200));
	gui.add(twistX.setup("twistX", 5, -45, 45));

	//Matrix Pattern GUI.
	gui.add(countY.setup("countY", 0, 0, 50));
	gui.add(stepY.setup("stepY", 20, 0, 200));
	gui.add(twistY.setup("twistY", 0, -30, 30));
	gui.add(pinchY.setup("pinchY", 0, 0, 1));


	//Global group settings.
	globalGroup.setup("Global");
	globalGroup.add(Scale.setup("Scale", 1, 0.0, 1));
	globalGroup.add(Rotate.setup("Rotate", 0, -180, 180));
	globalGroup.add(Background.setup("Background", 255, 0, 255));

	//Add the group to the gui panel.
	gui.add(&globalGroup);

	//Primitive group setup.
	primGroup.setup("Primitive");
	primGroup.add(shiftY.setup("shiftY", 0.0, -1000.0, 1000.0));
	primGroup.add(rotate.setup("rotate", 0.0, -180.0, 180.0));

	primGroup.add(size.setup("size", ofVec2f(6, 6), ofVec2f(0, 0), ofVec2f(20, 20)));
	primGroup.add(color.setup("color", ofColor::black, ofColor(0, 0, 0, 0), ofColor::white));
	primGroup.add(filled.setup("filled", false));
	primGroup.add(type.setup("type", false));

	gui.add(&primGroup);

	showGui = true;
	//Load last GUI State.
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	//Takes the color from the slider.
	ofBackground(Background);

	//push coordinate system on a stack
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	matrixPattern();
	//return from the stack
	ofPopMatrix();

	if (showGui) gui.draw();
}

void ofApp::stripePattern()
{
	ofSetColor(color);
	ofSetLineWidth(1.0);

	if (filled) ofFill(); else ofNoFill();

	for (int i = -countX; i <= countX; i++) {
		ofPushMatrix();

		//Rotate and Translate the coordinates system.
		ofTranslate(i * stepX, 0);
		ofRotate(i * twistX);


		//Use pow to give more precsision in the small values of Scale.
		float Scl = pow(Scale, 4.0f);
		ofScale(Scl, Scl);
		ofRotate(Rotate);

		//Primitive Controls.
		ofScale(size->x, size->y);
		ofRotate(rotate);
		ofTranslate(0, shiftY);

		if (type) ofRect(-50, -50, 100, 100);
		else ofTriangle(0, 0, -50, 100, 50, 100);

		ofPopMatrix();
	}
}

void ofApp::matrixPattern() {
	for (int y = -countY; y <= countY; y++) {
		ofPushMatrix();
		//--------------
		if (countY > 0) {
			float scl = ofMap(y, -countY, countY, 1 - pinchY, 1);
			ofScale(scl, scl);
		}
		ofTranslate(0, y * stepY);
		ofRotate(y * twistY);
		stripePattern();
		//--------------
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'z') showGui = !showGui;
	if (key == OF_KEY_RETURN) ofSaveScreen("screenshot" + ofToString(ofRandom(0,1000),0) + ".png");

	if (key == 's') {
		ofFileDialogResult res;
		res = ofSystemSaveDialog("preset.xml", "Saving Preset");
		if (res.bSuccess) gui.saveToFile(res.filePath);
	}

	if (key ==  'l') {
		ofFileDialogResult res;
		res = ofSystemLoadDialog("Loading Preset");
		if (res.bSuccess) gui.loadFromFile(res.filePath);
	}

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

void ofApp::exit() {
	gui.saveToFile("settings.xml");
};

