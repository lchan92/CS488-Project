#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <QtOpenGL>
#include "scene.hpp"

class Character {
public:
	Character();
	virtual ~Character();

	void setRoot(SceneNode* root);
	void draw();
	QVector3D getPosition();

	// movement functions
	void rotateY(float amount);
	void walkForward();
	void walkBackward();
	void strafeLeft();
	void strafeRight();
	void jump();

	void applyGravity();

private:
	// constants
	int MAX_JUMPS;
	float GRAVITY;

	int mJumpCount;

	//PHYSICS
	float mVerticalVelocity;


	SceneNode* mRoot;
};

#endif