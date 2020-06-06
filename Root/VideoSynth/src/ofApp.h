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
};
