#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		void stripePattern();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		//draw methods.
		void draw2d();	

		//GUI Settings
		ofxPanel gui;
		ofxIntSlider countX;
		ofxFloatSlider stepX;
		ofxFloatSlider twistX;
		//GUI Group
		ofxGuiGroup globalGroup;
		ofxFloatSlider Scale;
		ofxFloatSlider Rotate;
		ofxFloatSlider Background;
		//Primitive Group.
		ofxGuiGroup primGroup;
		ofxFloatSlider shiftY, rotate;
		ofxVec2Slider size;
		ofxColorSlider color;
		ofxToggle filled, type;

		//Matrix Pattern Generation Variables.
		ofxIntSlider countY;
		ofxFloatSlider stepY, twistY, pinchY;
		void matrixPattern();

		//Images.
		ofTexture image;

		//Video.
		ofVideoPlayer video;

		//Live Video Grabber.
		ofVideoGrabber camera;

		//Video Mixer Group.
		ofxGuiGroup mixerGroup;
		ofxFloatSlider imageAlpha, videoAlpha, cameraAlpha;

		//Offscreen buffers.
		ofFbo fbo;
		ofFbo fbo2;
		//Shader
		ofShader shader;
		ofxToggle kenabled;
		ofxIntSlider ksectors;
		ofxFloatSlider kangle, kx, ky;

		//3D Graphics.
		ofSpherePrimitive sphere;
		ofEasyCam cam;
		ofxToggle showWireFrame;
		ofxGuiGroup draw3DGroup;

		//Light and material;
		ofLight light;
		ofMaterial material;

		void draw3d();

		//Deforming sphere parameters.
		ofxFloatSlider rad, deform, deformFreq, extrude;

		//Parameters for mix 2d and 3d.
		ofxFloatSlider show2d, show3d;
		ofFbo fbo3d;

		vector<glm::vec3> vertices0;
};
