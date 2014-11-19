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
}

void Character::draw() {
	mRoot->walk_gl();
}

QVector3D Character::getPosition() {
	QVector4D pos = mRoot->m_trans * QVector4D(0,0,0,1);

	return pos.toVector3D();
}



void Character::rotateY(float amount) {
	mRoot->rotate('y', amount);
}

void Character::walkForward() {
	mRoot->translate(QVector3D(0,0,-1));
}

void Character::walkBackward() {
	mRoot->translate(QVector3D(0,0,1));
}

void Character::strafeLeft() {
	mRoot->translate(QVector3D(-1,0,0));
}

void Character::strafeRight() {
	mRoot->translate(QVector3D(1,0,0));
}

void Character::jump() {
	if (mJumpCount < MAX_JUMPS) {
		mVerticalVelocity = 1.5f;
		mJumpCount++;
	}
}

void Character::applyGravity() {
	mVerticalVelocity += GRAVITY;
	mRoot->translate(QVector3D(0, mVerticalVelocity, 0));
}