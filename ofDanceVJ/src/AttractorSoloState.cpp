#include "AttractorSoloState.h"

//--------------------------------------------------------------
string AttractorSoloState::getName() {
	return "AttractorSolo";
}


//--------------------------------------------------------------
void AttractorSoloState::setup() {
	ofEnableAlphaBlending();

	gui.setup();

	osc.setup(12345);
	oscSender.setup("127.0.0.1", 12346);

	gui.add(boid_vector.setup("boid_vector", false));
	gui.add(align.setup("align", 65, 0, 100));
	gui.add(cohesion.setup("cohesion", 20, 0, 100));
	gui.add(separate.setup("separate", 28, 0, 100));
	gui.add(dist_align.setup("dist_align", 65, 0, 100));
	gui.add(dist_cohesion.setup("dist_cohesion", 47, 0, 100));
	gui.add(dist_separate.setup("dist_separate", 63, 0, 100));
	gui.add(max_force.setup("max_force", 18, 0, 50));
	gui.add(max_speed.setup("max_speed", 2.5, 0, 10));
	gui.add(max_turn.setup("max_turn", 75, 0, 100));
	gui.add(attr_force.setup("attr_force", 0, -1, 1));
	gui.add(attr_dist.setup("attr_dist", 750, 0, 1000));
	gui.add(camera_x.setup("camera_x", 0, -100, 100));
	gui.add(camera_y.setup("camera_y", 0, -100, 100));
	gui.add(camera_z.setup("camera_z", 0, -100, 100));
	gui.add(camera_lookat_x.setup("camera_lookat_x", 0, -150, 150));
	gui.add(camera_lookat_y.setup("camera_lookat_y", 0, -150, 150));
	gui.add(camera_lookat_z.setup("camera_lookat_z", 0, -150, 150));
	gui.add(attr_x.setup("attr_x", 0, -100, 100));
	gui.add(attr_y.setup("attr_y", 0, -100, 100));
	gui.add(attr_z.setup("attr_z", 0, -100, 100));
	gui.add(space_size.setup("space_size", 100, 100, 250));
	gui.add(bg_color_start.setup("bg_color_start", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.add(bg_color_end.setup("bg_color_end", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
	gui.loadFromFile("gui.xml");

	camera.setPosition(camera_x, camera_y, camera_z);
	camera.lookAt(ofVec3f(camera_lookat_x, camera_lookat_y, camera_lookat_z));

	light.enable();
	light.setPointLight();
	//light.lookAt(ofVec3f(0, 0, 0));
	light.setPosition(0, 500, 0);
	light.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	light.setDiffuseColor(ofFloatColor(0.2, 0.2, 1.0));
	light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));

	lightShoes.setSpotlight();
	lightShoes.setPosition(0, 0, 1000);
	lightShoes.setAmbientColor(ofFloatColor(0.9, 0.3, 0.3, 1.0));
	lightShoes.setDiffuseColor(ofFloatColor(1.0, 0.4, 0.4));
	lightShoes.setSpecularColor(ofFloatColor(0.3, 0.1, 0.1));

	flock.setup(500, 0, 0, 0, 100);
	flock.setBounds(-space_size, -space_size, -space_size, space_size, space_size, space_size);
	flock.setBoundmode(0);

	// make attrPts
	flock.addAttractionPoint(attr_x, attr_y, attr_z, attr_dist, attr_force);

	ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofBackground(0);
	ofSetColor(255, 100);

	orpheLeft.loadModel("Orphe3D_L.3ds", false);
	orpheRight.loadModel("Orphe3D_R.3ds", false);

	fbo.allocate(ofGetWidth(), ofGetHeight());

}


//--------------------------------------------------------------
void AttractorSoloState::stateExit() {

	gui.saveToFile("gui.xml");

}
//--------------------------------------------------------------
AttractorSoloState::~AttractorSoloState() {

	gui.saveToFile("gui.xml");

}


//--------------------------------------------------------------
void AttractorSoloState::update() {

	// check for waiting messages
	while (osc.hasWaitingMessages()) {
		// get the next message
		ofxOscMessage m;
		osc.getNextMessage(m);

		// check for Orphe Hub
		if (m.getAddress() == "/RIGHT/sensorValues" || m.getAddress() == "/LEFT/sensorValues") {
			attr_x = m.getArgAsFloat(3) * -100;
			attr_y = m.getArgAsFloat(2) * -100;
			attr_z = m.getArgAsFloat(1) * -100;

			// Shoes quat
			quatLeft.w() = m.getArgAsFloat(0);
			quatLeft.x() = m.getArgAsFloat(2);
			quatLeft.y() = m.getArgAsFloat(3);
			quatLeft.z() = m.getArgAsFloat(1);

			if (m.getArgAsFloat(9) < 0) {
				max_speed = 0.1;
				attr_force = 1.0;
			}

			float accel = abs(m.getArgAsFloat(7)) + abs(m.getArgAsFloat(8));
			if (accel > 1.0) {
				attr_dist = attr_dist + accel;
			}
		}
		else if (m.getAddress() == "/RIGHT/gesture" || m.getAddress() == "/LEFT/gesture") {

			string type = m.getArgAsString(0);
			string dir = m.getArgAsString(1);

			if (type == "STEP" && dir == "TOE") {
				attr_force = attr_force + m.getArgAsFloat(2);
				if (attr_force > 1.0) attr_force = 1.0;
			}
		}
		// check for Duration
		else if (m.getAddress() == "/camera_x") {
			camera.setPosition(m.getArgAsFloat(0),
				camera.getPosition().y,
				camera.getPosition().z);
		}
		else if (m.getAddress() == "/camera_y") {
			camera.setPosition(camera.getPosition().x,
				m.getArgAsFloat(0),
				camera.getPosition().z);
		}
		else if (m.getAddress() == "/camera_z") {
			camera.setPosition(camera.getPosition().x,
				camera.getPosition().y,
				m.getArgAsFloat(0));
		}

	}

	flock.setBounds(-space_size, -space_size, -space_size, space_size, space_size, space_size);
	flock.setAlign(align);
	flock.setCohesion(cohesion);
	flock.setSeparate(separate);
	flock.setDistAlign(dist_align);
	flock.setDistCohesion(dist_cohesion);
	flock.setDistSeparation(dist_separate);
	flock.setMaxForce(max_force);
	flock.setMaxSpeed(max_speed);
	flock.setMaxTurn(max_turn);
	for (int i = 0; i < flock.attractionPoints.size(); i++) {
		AttractionPoint3d * ap = flock.attractionPoints[i];
		ap->force = attr_force;
		ap->sensorDist = attr_dist;
		ap->x = attr_x;
		ap->y = attr_y;
		ap->z = attr_z;
	}
	camera_x = camera.getPosition().x;
	camera_y = camera.getPosition().y;
	camera_z = camera.getPosition().z;
	camera_lookat_x = camera.getLookAtDir().x;
	camera_lookat_y = camera.getLookAtDir().y;
	camera_lookat_z = camera.getLookAtDir().z;

	camera.lookAt(ofVec3f(0, 0, 0));
	//camera.lookAt(ofVec3f(attr_x, attr_y, attr_z));

	flock.update();

	fbo.begin();
	{
		ofClear(0,0);
		ofBackgroundGradient(bg_color_start,bg_color_end, OF_GRADIENT_LINEAR);
		ofEnableDepthTest();
		camera.begin();

		ofSetColor(255, 64);
		int rep = space_size / 10;
		ofDrawGrid(10.0f, rep, true);

		for (int i = 0; i < flock.attractionPoints.size(); i++) {
			AttractionPoint3d * ap = flock.attractionPoints[i];
			ofPushMatrix();
			ofTranslate(ap->x, ap->y, ap->z);

			// Shoes
			ofScale(0.05, 0.05, 0.05);
			ofVec3f qaxis;
			float qangle;
			quatLeft.getRotate(qangle, qaxis);
			ofRotate(180);
			ofRotate(qangle, qaxis.x, qaxis.y, qaxis.z);
			//ofRotateZ(180);
			lightShoes.lookAt(ofVec3f(0, 0, 0));
			lightShoes.setPosition(camera.getPosition().x * 100, camera.getPosition().y * 100, camera.getPosition().z * 100);
			lightShoes.enable();
			ofSetColor(255, 255, 255, 255);
			orpheLeft.drawFaces();
			lightShoes.disable();
			ofPopMatrix();

			// Attraction Area
			ofPushMatrix();
			ofTranslate(ap->x, ap->y, ap->z);
			ofSetColor(200, 0, 0, 200);
			ofSphere(ap->sensorDist / 10);
			ofPopMatrix();

		}
		for (int i = 0; i < flock.size(); i++) {
			Boid3d * b = flock.get(i);
			ofPushMatrix();
			ofTranslate(b->x, b->y, b->z);
			if (flock.attractionPoints[0]->sensorDist / 3 > ofDist(b->x, b->y, b->z, flock.attractionPoints[0]->x, flock.attractionPoints[0]->y, flock.attractionPoints[0]->z)) {
				ofSetColor(255, 0, 0);
			}
			else {
				ofSetColor(255);
			}
			ofSphere(2);
			ofPopMatrix();
			if (boid_vector) {
				float lm = 10.f;
				ofLine(b->x, b->y, b->z, b->x + b->vx*lm, b->y + b->vy*lm, b->z + b->vz*lm);
			}
		}


		camera.end();
		ofDisableDepthTest();

		ofSetColor(255);
	}
	fbo.end();

	// ’l‚ð–ß‚·
	if (attr_force > -0.7) {
		attr_force = attr_force - 0.01;
	}
	if (attr_dist > 80) {
		attr_dist = attr_dist - 1;
	}
	if (max_speed < 1.0) {
		max_speed = max_speed + 0.02;
	}
	else if (max_speed < 7.5) {
		max_speed = max_speed + 0.2;
	}
}


//--------------------------------------------------------------
void AttractorSoloState::draw() {

	ofSetColor(255,255,255,255);
	fbo.draw(0, 0);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	gui.draw();

}


//--------------------------------------------------------------
void AttractorSoloState::keyPressed(int key) {

	if (key == ' ') {
		ofxOscMessage m;
		m.setAddress("/camera_x");
		m.addFloatArg(camera_x);
		oscSender.sendMessage(m);

		m.setAddress("/camera_y");
		m.addFloatArg(camera_y);
		oscSender.sendMessage(m);

		m.setAddress("/camera_z");
		m.addFloatArg(camera_z);
		oscSender.sendMessage(m);

	}

}

