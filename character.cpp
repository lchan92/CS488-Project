#include "character.hpp"
#include <iostream>



Character::Character() {
	MAX_JUMPS = 2000;
	GRAVITY = -0.02f;

	mJumpCount = 0;

	mVerticalVelocity = 0.0f;
}

Character::~Character() {}



void Character::setRoot(SceneNode* root) {
	mRoot = root;
	updatePosition();
}

void Character::setMapRoot(SceneNode* mapRoot) {
	mMapRoot = mapRoot;
}



void Character::updateBoundingBox() {
	// NEED TO CHANGE THIS LATER ON

	// get all the vertices of the box that encloses the character to determine axis aligned bounding box
	QVector4D vertices[8];
	vertices[0] = mRoot->m_trans * QVector4D(-1, -1, 1, 1);
	vertices[1] = mRoot->m_trans * QVector4D(1, -1, 1, 1);
	vertices[2] = mRoot->m_trans * QVector4D(1, 1, 1, 1);
	vertices[3] = mRoot->m_trans * QVector4D(-1, 1, 1, 1);
	vertices[4] = mRoot->m_trans * QVector4D(-1, -1, -1, 1);
	vertices[5] = mRoot->m_trans * QVector4D(1, -1, -1, 1);
	vertices[6] = mRoot->m_trans * QVector4D(1, 1, -1, 1);
	vertices[7] = mRoot->m_trans * QVector4D(-1, 1, -1, 1);

	double minX = vertices[0].x();
	double maxX = minX;
	double minY = vertices[0].y();
	double maxY = minY;
	double minZ = vertices[0].z();
	double maxZ = minZ;

	for (int i = 1; i < 8; i++) {
		if (vertices[i].x() < minX) 
			minX = vertices[i].x();
		if (vertices[i].x() > maxX)
			maxX = vertices[i].x();

		if (vertices[i].y() < minY)
			minY = vertices[i].y();
		if (vertices[i].y() > maxY)
			maxY = vertices[i].y();
	
		if (vertices[i].z() < minZ)
			minZ = vertices[i].z();
		if (vertices[i].z() > maxZ)
			maxZ = vertices[i].z();
	}

	mVertex1 = QVector4D(minX, minY, maxZ, 1);
	mVertex2 = QVector4D(maxX, maxY, minZ, 1);

	// mVertex1 = mRoot->m_trans * QVector4D(-1, -1, 1, 1);
	// mVertex2 = mRoot->m_trans * QVector4D(1, 1, -1, 1);
}



void Character::draw() {
	mRoot->walk_gl();
}



void Character::updatePosition() {
	mPosition = mRoot->m_trans * QVector4D(0,0,0,1);
	updateBoundingBox();
}

QVector3D Character::getPosition() {
	return mPosition.toVector3D();
}



void Character::rotateY(float amount) {
	mRoot->rotate('y', amount);
	updatePosition();
}

void Character::walkForward(double* velocity) {
	*velocity = -0.5;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 0)) {
	}

	mRoot->translate(QVector3D(0,0,*velocity));
	updatePosition();
}

void Character::walkBackward(double* velocity) {
	*velocity = 0.5;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 1)) {
	}

	mRoot->translate(QVector3D(0,0,*velocity));
	updatePosition();
}

void Character::strafeLeft(double* velocity) {
	*velocity = -0.2;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 2)) {
	}

	mRoot->translate(QVector3D(*velocity,0,0));
	updatePosition();
}

void Character::strafeRight(double* velocity) {
	*velocity = 0.2;

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, 3)) {
	}

	mRoot->translate(QVector3D(*velocity,0,0));
	updatePosition();
}

void Character::jump() {
	if (mJumpCount < MAX_JUMPS) {
		if (mJumpCount == 0)
			mVerticalVelocity = 0.8f;
		else
			mVerticalVelocity = 0.5f;

		mJumpCount++;
	}
}

void Character::applyGravity(double* velocity) {
	mVerticalVelocity += GRAVITY;
	
	if (mVerticalVelocity < -3.0f) {
		mVerticalVelocity = -3.0f;
	}

	*velocity = mVerticalVelocity;;

	int direction = (mVerticalVelocity < 0)? 4: 5; //intersect with top if falling, bottom if rising

	if (mMapRoot->faceIntersectsBox(mVertex1, mVertex2, velocity, direction))	{
		// reset position of character to not get stuck in physics when landing
		mVerticalVelocity = *velocity;
		mRoot->translate(QVector3D(0,*velocity,0));
		updatePosition();

		// reset jump count since we're on a surface
		if (direction == 4) mJumpCount = 0;

		if (mVerticalVelocity < 0) {
			mVerticalVelocity = 0;
		}
	} else {
		mRoot->translate(QVector3D(0,*velocity,0));
		updatePosition();
	}
}
