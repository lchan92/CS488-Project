#include "character.hpp"



Character::Character() {
	MAX_JUMPS = 2;

	mJumpCount = 0;
}

Character::~Character() {}



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
		// conduct jump

		mJumpCount++;
	}
}