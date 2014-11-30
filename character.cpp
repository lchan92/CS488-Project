#include "character.hpp"
#include <iostream>
#include "Viewer.hpp"
#include "AppWindow.hpp"



Character::Character() {
	MAX_JUMPS = 2000;
	GRAVITY = -0.02f;
	DEATH_HEIGHT = -50;

	mJumpCount = 0;
	mAlive = true;

	mVerticalVelocity = 0.0f;
}

Character::~Character() {}


void Character::setMapRoot(SceneNode* mapRoot) {
	mMapRoot = mapRoot;
}


void Character::bind() {
    mMesh = new Mesh();
    mMesh->load("obj/coyote/Coyotito.obj");
	
	mMesh->bind();

	updatePosition();
}





void Character::draw() {
	AppWindow::m_viewer->draw_mesh(mMesh);
}







QVector3D Character::getPosition() {
	return mPosition.toVector3D();
}

QVector4D Character::getInitPosition() {
	return mMesh->mInitPosition;
}

QVector3D Character::getCameraLookAtPosition() {
	if (mAlive) {
		return getPosition();
	} else {
		QVector3D pos = QVector3D(mPosition.x(), DEATH_HEIGHT, mPosition.z());
		return pos;
	}
}






void Character::updatePosition() {
	mPosition = mMesh->getTransform() * mMesh->mInitPosition;
	updateBoundingBox();

	if (mPosition.y() <= DEATH_HEIGHT) {
		mAlive = false;
	}
}

void Character::updateBoundingBox() {
	mVertex1 = mMesh->getTransform() * mMesh->mVertex1;
	mVertex2 = mMesh->getTransform() * mMesh->mVertex2;
}



bool Character::isAlive() {
	return mAlive;
}

bool Character::isOverBox(double* height) {
	return mMapRoot->isOverBox(mVertex1, mVertex2, height);
}




void Character::rotateY(float amount) {
	updatePosition();
}

void Character::walkForward(double* velocity) {
	*velocity = -0.5;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 0)) {
	}

	mMesh->translate(QVector3D(0,0,*velocity));
	updatePosition();
}

void Character::walkBackward(double* velocity) {
	*velocity = 0.5;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 1)) {
	}

	mMesh->translate(QVector3D(0,0,*velocity));
	updatePosition();
}

void Character::strafeLeft(double* velocity) {
	*velocity = -0.2;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 2)) {
	}

	mMesh->translate(QVector3D(*velocity,0,0));
	updatePosition();
}

void Character::strafeRight(double* velocity) {
	*velocity = 0.2;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 3)) {
	}

	mMesh->translate(QVector3D(*velocity,0,0));
	updatePosition();
}

void Character::jump() {
	if (mJumpCount < MAX_JUMPS) {
		if (mJumpCount == 0)
			mVerticalVelocity = 0.8f;
		else
			mVerticalVelocity = 0.5f;

		mJumpCount++;

		Viewer::mSounds->playJump();
	}
}

bool Character::applyGravity(double* velocity) {
	bool onSurface = false;

	mVerticalVelocity += GRAVITY;
	
	if (mVerticalVelocity < -3.0f) {
		mVerticalVelocity = -3.0f;
	}

	*velocity = mVerticalVelocity;;

	int direction = (mVerticalVelocity < 0)? 4: 5; //intersect with top if falling, bottom if rising

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, direction))	{
		// reset position of character to not get stuck in physics when landing
		mVerticalVelocity = *velocity;
		mMesh->translate(QVector3D(0,*velocity,0));
		updatePosition();

		// reset jump count since we're on a surface
		if (direction == 4) {
			mJumpCount = 0;
			onSurface = true;
		}
		
		if (mVerticalVelocity < 0) {
			mVerticalVelocity = 0;
		}
	} else {
		mMesh->translate(QVector3D(0,*velocity,0));
		updatePosition();
	}

	return onSurface;
}
