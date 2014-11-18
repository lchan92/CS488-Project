#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <QtOpenGL>

class Character {
public:
	Character();
	virtual ~Character();

	// movement functions
	void walkForward();
	void walkBackward();
	void strafeLeft();
	void strafeRight();
	void jump();

private:
	int MAX_JUMPS;

	int mJumpCount;

	QVector3D mStartPosition, mPosition;
};

#endif