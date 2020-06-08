#include "ofApp.h"

bool showGui;

//--------------------------------------------------------------
void ofApp::setup(){
	//Settings.
	ofSetWindowTitle("Video Synth");
	ofSetWindowShape(1280, 720);
	ofSetFrameRate(60);
	ofBackground(ofColor::white);

	//Load image.
	ofLoadImage(image, "eJ40ZCR.jpg");

	//Load Video.
	video.loadMovie("flowing.mp4");
	video.play();


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

	//Mixer Group 
	mixerGroup.setup("Mixer");
	mixerGroup.setHeaderBackgroundColor(ofColor::darkRed);
	mixerGroup.setBorderColor(ofColor::darkRed);

	mixerGroup.add(imageAlpha.setup("image", 100, 0, 255));
	mixerGroup.add(videoAlpha.setup("video", 200, 0, 255));
	mixerGroup.add(cameraAlpha.setup("camera", 100, 0, 255));

	//Shader Setup
	shader.load( "kaleido" );
	mixerGroup.add(kenabled.setup("kenabled", true));
	mixerGroup.add(ksectors.setup("ksectors", 10, 1, 100));
	mixerGroup.add(kangle.setup("kangle", 0, -180, 180));
	mixerGroup.add(kx.setup("kx", 0.5, 0, 1));
	mixerGroup.add(ky.setup("ky", 0.5, 0, 1));

	//Mixing 2D/3D.
	mixerGroup.add(show2d.setup("show2d", 255, 0, 255));
	mixerGroup.add(show3d.setup("show3d", 255, 0, 255));
	fbo3d.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	//DEFORM 3D Sphere.
	mixerGroup.add(rad.setup("rad", 250, 0, 500));
	mixerGroup.add(deform.setup("deform", 0.3, 0, 1.5));
	mixerGroup.add(deformFreq.setup("deformFreq", 3, 0, 10));
	mixerGroup.add(extrude.setup("extrude", 1, 0, 1));

	gui.minimizeAll();
	gui.add(&mixerGroup);
	
	//3D GRAPHICS.
	draw3DGroup.setup("3dgraphics");
	draw3DGroup.add(showWireFrame.setup("showWire", false));
	gui.add(&draw3DGroup);

	sphere.set(250, 80);
	vertices0 = sphere.getMesh().getVertices();

	//allocate offscreen texture for texturing 3d sphere.
	fbo2.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	float w = fbo2.getWidth();
	float h = fbo2.getHeight();

	//rectangle of coordinates of all square area but flipped in y axis (u1,v1) = (0,h) (u2,v2) = (w,0)
	sphere.mapTexCoords(0, h, w, 0);
	//better orientation for the textured sphere (not related to the texturing process).
	sphere.rotate(180, 0, 1, 0);

	//Allocate buffer for kaleidscope shader.
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGB);

	//----------------------------------------------------------
	showGui = true;
	//Load last GUI State.
	gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
	video.update();
	if (camera.isInitialized()) camera.update();
	vector<glm::vec3> &vertices = sphere.getMesh().getVertices();

	for (int i = 0; i < vertices.size(); i++) {
		auto v = glm::normalize(vertices0[i]);

		//the idea here is make the deformation periodically.
		float sx = sin(v.x * deformFreq);
		float sy = sin(v.y * deformFreq);
		float sz = sin(v.z * deformFreq);

		// apply deformation to the vertex based in a deform strenght and previously deform shifts.
		v.x += sy * sz * deform;
		v.y += sx * sz * deform;
		v.z += sx * sy * deform;

		//
		v = v * float(rad);
		vertices[i] = v;
	}

	ofPixels pixels;
	fbo2.readToPixels(pixels);

	for (int i = 0; i < vertices.size(); i++) {
		ofVec2f t = sphere.getMesh().getTexCoords()[i];
		//clamp to the pixels array dimensions.
		t.x = ofClamp(t.x, 0, pixels.getWidth() - 1);
		t.y = ofClamp(t.y, 0, pixels.getHeight() - 1);
		float br = pixels.getColor(t.x, t.y).getBrightness();
		vertices[i] *= 1 + br / 255.0  * extrude;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	//Redirection Drawing to the virtual screen.
	fbo.begin();
	draw2d();
	fbo.end();

	ofSetColor(255);
	//shader will be executed for each pixel of the screen.
	fbo2.begin();
	if (kenabled) {
		shader.begin();
		shader.setUniform1i("ksectors", ksectors);
		shader.setUniform1f("kangleRad", ofDegToRad(kangle));
		shader.setUniform2f("kcenter", kx * ofGetWidth(), ky*ofGetHeight());
		shader.setUniform2f("screenCenter", 0.5 * ofGetWidth(),
			0.5 * ofGetHeight());
	}
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	if (kenabled) shader.end();
	fbo2.end();

	//----------------------------------
	//draw 3d objects.
	fbo3d.begin();
	ofBackground(0, 0);
	draw3d();
	fbo3d.end();

	ofBackground(0);
	ofSetColor(255, show2d);
	fbo2.draw(0,0);
	ofSetColor(255, show3d);
	fbo3d.draw(0,0);

	//draw gui in the real screen always.
	if (showGui) gui.draw();
}

void ofApp::draw2d() {
	//Takes the color from the slider.
	ofBackground(Background);

	//----------------------------
	//Enable additive blending.
	ofDisableSmoothing();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	//Draw Image.
	ofSetColor(255, imageAlpha);
	image.draw(0, 0, ofGetWidth(), ofGetHeight());

	//Draw Video.
	ofSetColor(255, videoAlpha);
	video.draw(0, 0, ofGetWidth(), ofGetHeight());

	//Draw caputered live camera.
	if (camera.isInitialized()) {
		ofSetColor(255, cameraAlpha);
		camera.draw(0, 0, ofGetWidth(), ofGetHeight());
	}
	//Enable alpha blending for the rest of the draw process.
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	//-----------------------------

	//push coordinate system on a stack
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	matrixPattern();
	//return from the stack
	ofPopMatrix();
}

void ofApp::draw3d() {

	//Get texture of fb2.
	fbo2.getTextureReference().bind();

	//enable single point light, material and z-buffer.
	light.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 600);
	light.enable();
	material.begin();
	ofEnableDepthTest();

	//orbit camera.
	/*
	float time = ofGetElapsedTimef();
	float longitude = 10 * time;
	float latitude = 10 * sin(time * 0.8);
	float radius = 600 + 50 * (sin(time * 0.4));
	cam.orbit(longitude, latitude, radius);
	*/

	cam.begin();
	ofSetColor(ofColor::white);
	showWireFrame? sphere.drawWireframe(): sphere.draw();
	cam.end();

	//desactivate texture.
	fbo2.getTextureReference().unbind();

	//disable lighting, material, and z-buffer.
	ofDisableDepthTest();
	material.end();
	light.disable();
	ofDisableLighting();
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

	if (key == 'c') {
		camera.setDeviceID(0);
		camera.setDesiredFrameRate(30);
		camera.initGrabber(1280, 720);
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

