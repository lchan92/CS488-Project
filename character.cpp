#include "character.hpp"
#include <iostream>



Character::Character() {
	MAX_JUMPS = 2;
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
	mVertex1 = mRoot->m_trans * QVector4D(-1, -1, 1, 1);
	mVertex2 = mRoot->m_trans * QVector4D(1, 1, -1, 1);

}



void Character::draw() {
	mRoot->walk_gl();
}



void Character::updatePosition() {
	mPosition = mRoot->m_trans * QVector4D(0,0,0,1);
	// std::cout << "mPosition: " << mPosition.x() << "," << mPosition.y() << "," << mPosition.z() << std::endl;
	updateBoundingBox();
}

QVector3D Character::getPosition() {
	return mPosition.toVector3D();
}



void Character::rotateY(float amount) {
	mRoot->rotate('y', amount);
	updatePosition();
}

bool Character::walkForward() {
	double diff;

	QVector4D farBottomLeft = QVector4D(mVertex1.x(), mVertex1.y(), mVertex2.z(), 1);
	QVector4D farTopRight = mVertex2;

	if (!mMapRoot->faceIntersectsBox(farBottomLeft, farTopRight, &diff, 0)) {
		mRoot->translate(QVector3D(0,0,-1));
		updatePosition();

		return true;
	}

	return false;
}

bool Character::walkBackward() {
	double diff;

	QVector4D nearBottomLeft = mVertex1;
	QVector4D nearTopRight = QVector4D(mVertex2.x(), mVertex2.y(), mVertex1.z(), 1);

	if (!mMapRoot->faceIntersectsBox(nearBottomLeft, nearTopRight, &diff, 1)) {
		mRoot->translate(QVector3D(0,0,1));
		updatePosition();

		return true;
	}

	return false;
}

bool Character::strafeLeft() {
	double diff;

	QVector4D nearBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex1.z(), 1);
	QVector4D farTopRight = mVertex2;

	if (!mMapRoot->faceIntersectsBox(nearBottomRight, farTopRight, &diff, 2)) {
		mRoot->translate(QVector3D(-1,0,0));
		updatePosition();

		return true;
	}

	return false;
}

bool Character::strafeRight() {
	double diff;

	QVector4D nearBottomLeft = mVertex1;
	QVector4D farTopLeft = QVector4D(mVertex1.x(), mVertex2.y(), mVertex2.z(), 1);

	if (!mMapRoot->faceIntersectsBox(nearBottomLeft, farTopLeft, &diff, 3)) {
		mRoot->translate(QVector3D(1,0,0));
		updatePosition();

		return true;
	}
	return false;
}

void Character::jump() {
	if (mJumpCount < MAX_JUMPS) {
		mVerticalVelocity = 1.0f;
		mJumpCount++;
	}
}

void Character::applyGravity() {
	double diff;

	QVector4D nearBottomLeft = mVertex1;
	QVector4D farBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex2.z(), 1);

	// bottom, top, front, back, left, right
	if (mMapRoot->faceIntersectsBox(nearBottomLeft, farBottomRight, &diff, 4)) {
		// reset position of character to not get stuck in physics when landing
		if (diff > -2) {
		mRoot->translate(QVector3D(0, -diff, 0));
		updatePosition();

		}

		// reset jump count since we're on a surface
		mJumpCount = 0;

		if (mVerticalVelocity < 0) {
			mVerticalVelocity = 0;
		}
	} else {
		mVerticalVelocity += GRAVITY;
		
		if (mVerticalVelocity < -3.0f) {
			mVerticalVelocity = -3.0f;
		}
	}

	mRoot->translate(QVector3D(0, mVerticalVelocity, 0));
	updatePosition();
}


	// QVector4D nearBottomLeft = mVertex1;
	// QVector4D nearBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex1.z(), 1);
	// QVector4D nearTopRight = QVector4D(mVertex2.x(), mVertex2.y(), mVertex1.z(), 1);
	// QVector4D nearTopLeft = QVector4D(mVertex1.x(), mVertex2.y(), mVertex1.z(), 1);

	// QVector4D farBottomLeft = QVector4D(mVertex1.x(), mVertex1.y(), mVertex2.z(), 1);
	// QVector4D farBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex2.z(), 1);
	// QVector4D farTopRight = mVertex2;
	// QVector4D farTopLeft = QVector4D(mVertex1.x(), mVertex2.y(), mVertex2.z(), 1);
