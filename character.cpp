#include "character.hpp"



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


void Character::walkForward() {

}

void Character::walkBackward() {

}

void Character::strafeLeft() {

}

void Character::strafeRight() {

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