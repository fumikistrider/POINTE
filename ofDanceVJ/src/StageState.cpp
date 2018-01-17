#include "StageState.h"

//--------------------------------------------------------------
string StageState::getName() {
	return "Stage";
}


//--------------------------------------------------------------
void StageState::setup() {
	ofEnableAlphaBlending();

	gui.setup();
	gui.add(toggleGrid.setup("toggleGrid", false));
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
	gui.loadFromFile("gui_stage.xml");

	gui_light.setup();
	gui_light.add(specular_light.setup("specular_light", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(diffuse_light.setup("diffuse_light", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(ambient_light.setup("ambient_light", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(specular_material.setup("specular_material", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(diffuse_material.setup("diffuse_material", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(ambient_material.setup("ambient_material", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(emmisive_material.setup("emmisive_material", ofFloatColor(0.5, 0.5, 0.5), ofFloatColor(0, 0, 0), ofFloatColor(1, 1, 1)));
	gui_light.add(shiness_material.setup("shiness", 0, -128, 128));
	gui_light.setPosition(ofGetWidth() - 200, 0);
	gui_light.loadFromFile("gui_stage_light.xml");

	camera.setPosition(camera_x, camera_y, camera_z);
	camera.lookAt(ofVec3f(camera_lookat_x, camera_lookat_y, camera_lookat_z));

	ofEnableSmoothing();
	light.enable();
	light.setPointLight();
	light.setPosition(0, 100, 100);
	light.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	light.setDiffuseColor(ofFloatColor(0.8, 0.8, 1.0));
	light.setSpecularColor(ofFloatColor(1.0, 1.0, 1.0));

	lightShoes.setSpotlight();
	lightShoes.setPosition(0, 0, 1000);
	lightShoes.setAmbientColor(ofFloatColor(0.9, 0.3, 0.3, 1.0));
	lightShoes.setDiffuseColor(ofFloatColor(1.0, 0.4, 0.4));
	lightShoes.setSpecularColor(ofFloatColor(0.3, 0.1, 0.1));

	flock.setup(1, 0, 0, 0, 100);
	flock.setBounds(-space_size, -space_size, -space_size, space_size, space_size, space_size);
	flock.setBoundmode(1);

	// make attrPts
	flock.addAttractionPoint(attr_x, attr_y, attr_z, attr_dist, attr_force);

	ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofBackground(0);
	ofSetColor(255, 100);

	orpheLeft.loadModel("Orphe3D_L.3ds", false);
	orpheRight.loadModel("Orphe3D_R.3ds", false);
	//stage.loadModel("Cyclorama.fbx");
	stage.loadModel("models/stage.dae");
	fbo.allocate(ofGetWidth(), ofGetHeight());

}


//--------------------------------------------------------------
void StageState::stateExit() {

	gui.saveToFile("gui_stage.xml");
	gui_light.saveToFile("gui_stage_light.xml");

}

StageState::~StageState() {

	gui.saveToFile("gui_stage.xml");
	gui_light.saveToFile("gui_stage_light.xml");

}

//--------------------------------------------------------------
void StageState::update() {

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
		ofClear(0);
		ofEnableDepthTest();
		ofEnableLighting();
		camera.begin();

		light.enable();
		light.setAmbientColor(ambient_light);
		light.setDiffuseColor(diffuse_light);
		light.setSpecularColor(specular_light);
		material_stage.setAmbientColor(ambient_material);
		material_stage.setDiffuseColor(diffuse_material);
		material_stage.setSpecularColor(specular_material);
		material_stage.setEmissiveColor(emmisive_material);
		material_stage.setShininess(shiness_material);

		ofPushMatrix();
		ofSetColor(255, 255, 255, 255);
		stage.setScale(1000, 1000, 1000);
		stage.setPosition(0, 0, 0);
		ofRotate(180);
		//material_stage.begin();
		//stage.enableMaterials();
		stage.drawFaces();
		//material_stage.end();
		ofPopMatrix();

		if (toggleGrid) {
			int rep = space_size / 10;
			ofDrawGrid(10.0f, rep, true);
		}

		for (int i = 0; i < flock.attractionPoints.size(); i++) {
			AttractionPoint3d * ap = flock.attractionPoints[i];
			ofPushMatrix();
			{
				//// Left Shoes
				ofTranslate(ap->x + 10, ap->y, ap->z);
				ofScale(0.05, 0.05, 0.05);
				ofVec3f qaxis;
				float qangle;
				quatLeft.getRotate(qangle, qaxis);
				ofRotate(180);
				ofRotate(qangle, qaxis.x, qaxis.y, qaxis.z);
				ofSetColor(255, 255, 255, 255);
				orpheLeft.drawFaces();
			}
			ofPopMatrix();

			ofPushMatrix();
			{
				//// Right Shoes
				ofTranslate(ap->x - 10, ap->y, ap->z);
				ofScale(0.05, 0.05, 0.05);
				ofVec3f qaxis;
				float qangle;
				quatRight.getRotate(qangle, qaxis);
				ofRotate(180);
				ofRotate(qangle, qaxis.x, qaxis.y, qaxis.z);
				ofSetColor(255, 255, 255, 255);
				orpheRight.drawFaces();
			}
			ofPopMatrix();

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
void StageState::draw() {

	ofSetColor(255);
	fbo.draw(0, 0);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	gui.draw();
	gui_light.draw();
}


//--------------------------------------------------------------
void StageState::keyPressed(int key) {


}

