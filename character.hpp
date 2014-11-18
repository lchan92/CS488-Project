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
	const int MAX_JUMPS = 2;

	int mJumpCount;

	QVector3D mStartPosition, mPosition;
};

#endif