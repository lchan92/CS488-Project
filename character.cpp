#include "character.hpp"
#include <iostream>
#include "Viewer.hpp"
#include "AppWindow.hpp"



Character::Character() {
	MAX_JUMPS = 2000;
	GRAVITY = -0.03f;
	DEATH_HEIGHT = -50;

	mJumpCount = 0;
	mAlive = true;

	mVerticalVelocity = 0.0f;
	mOnSurface = false;
}

Character::~Character() {}


void Character::setMapRoot(SceneNode* mapRoot) {
	mMapRoot = mapRoot;
}


void Character::bind() {
    mMesh = new Mesh();
    mMesh->load("obj/coyote/Coyotito.obj", "textures/coyote.jpg");
	
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

QVector4D Character::getVertex1() {
	return mVertex1;
}

QVector4D Character::getVertex2() {
	return mVertex2;
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

bool Character::isOverBox(int* face, double* height, float* reflectFactor) {
	return mMapRoot->isOverBox(mVertex1, mVertex2, face, height, reflectFactor);
}




void Character::rotateY(float amount) {
	updatePosition();
}

void Character::walkForward(QVector3D* velocity) {
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 0)) {
	}

	mMesh->translate(*velocity);
	updatePosition();
}

void Character::walkBackward(QVector3D* velocity) {
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 1)) {
	}

	mMesh->translate(*velocity);
	updatePosition();
}

void Character::strafeLeft(QVector3D* velocity) {
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 2)) {
	}

	mMesh->translate(*velocity);
	updatePosition();
}

void Character::strafeRight(QVector3D* velocity) {
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 3)) {
	}

	mMesh->translate(*velocity);
	updatePosition();
}

void Character::jump() {
	if (mJumpCount < MAX_JUMPS) {
		if (mJumpCount == 0)
			mVerticalVelocity = 0.8f;
		else
			mVerticalVelocity = 0.5f;

		mJumpCount++;
		mOnSurface = false;

		Viewer::mSounds->playJump();
	}
}

bool Character::applyGravity(QVector3D* velocity) {
	mVerticalVelocity += GRAVITY;
	
	if (mVerticalVelocity < -3.0f) {
		mVerticalVelocity = -3.0f;
	}

	velocity->setY(mVerticalVelocity);

	int direction;
	if (mOnSurface) {
		direction = 4;
	} else {
		direction = mVerticalVelocity < 0? 4: 5;
	}

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, direction))	{
		// reset position of character to not get stuck in physics when landing
		mVerticalVelocity = velocity->y();

		mMesh->translate(*velocity);
		updatePosition();

		// reset jump count since we're on a surface
		if (direction == 4) {
			mJumpCount = 0;
			mOnSurface = true;
		}
		
		if (mVerticalVelocity < 0) {
			mVerticalVelocity = 0;
		}

	} else {
		mOnSurface = false;
		mMesh->translate(*velocity);
		updatePosition();
	}

	return mOnSurface;
}

void Character::checkFrontCollisions(QVector3D* velocity) {
	// COLLIDE FRONT FACES WITH CHARACTER
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 0)) {
		mMesh->translate(*velocity);
		updatePosition();
	}
}

void Character::checkBackCollisions(QVector3D* velocity) {
	// COLLIDE BACK FACES WITH CHARACTER
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 1)) {
		mMesh->translate(*velocity);
		updatePosition();
	}
}

void Character::checkLeftCollisions(QVector3D* velocity) {
	// COLLIDE BACK FACES WITH CHARACTER
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 2)) {
		mMesh->translate(*velocity);
		updatePosition();
	}
}

void Character::checkRightCollisions(QVector3D* velocity) {
	// COLLIDE BACK FACES WITH CHARACTER
	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 3)) {
		mMesh->translate(*velocity);
		updatePosition();
	}
}
