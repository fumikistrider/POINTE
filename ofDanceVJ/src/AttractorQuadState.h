#pragma once

#include "ofxState.h"
#include "SharedData.h"

#include "ofxGui.h"
#include "Flock2d.h"
#include "Flock3d.h"
#include "ofxGrabCam.h"
#include "ofxOsc.h"
#include "ofxAssimpModelLoader.h"

class AttractorQuadState : public itg::ofxState<SharedData> {

public:
	~AttractorQuadState();
	void setup();
	void update();
	void draw();
	void stateExit();

	void keyPressed(int key);

	string getName();

	Flock3d flock;
	ofxPanel gui;
	ofxToggle boid_vector;
	ofxFloatSlider align;
	ofxFloatSlider cohesion;
	ofxFloatSlider separate;
	ofxFloatSlider dist_align;
	ofxFloatSlider dist_cohesion;
	ofxFloatSlider dist_separate;
	ofxFloatSlider max_force;
	ofxFloatSlider max_speed;
	ofxFloatSlider max_turn;
	ofxFloatSlider attr_force;
	ofxFloatSlider attr_dist;
	ofxFloatSlider attr_x;
	ofxFloatSlider attr_y;
	ofxFloatSlider attr_z;
	ofxFloatSlider camera_x;
	ofxFloatSlider camera_y;
	ofxFloatSlider camera_z;
	ofxFloatSlider camera_lookat_x;
	ofxFloatSlider camera_lookat_y;
	ofxFloatSlider camera_lookat_z;
	ofxIntSlider   space_size;
	ofxColorSlider bg_color_start;
	ofxColorSlider bg_color_end;


	ofLight light;
	ofLight lightShoes;

	ofxGrabCam camera;
	ofMatrix4x4 savedPose;
	ofFbo fbo;
	ofxOscReceiver osc;
	ofxOscSender   oscSender;

	ofxAssimpModelLoader orpheLeft;
	ofxAssimpModelLoader orpheRight;
	ofQuaternion quatLeft;
	ofQuaternion quatRight;

};
