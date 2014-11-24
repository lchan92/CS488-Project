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
	void setMapRoot(SceneNode* mapRoot);

	void draw();

	QVector3D getPosition();

	void updatePosition();
	void updateBoundingBox();

	// movement functions
	void rotateY(float amount);
	void walkForward(double*  velocity);
	void walkBackward(double*  velocity);
	void strafeLeft(double*  velocity);
	void strafeRight(double*  velocity);
	void jump();

	void applyGravity(double*  velocity);

private:
	void initSDL();

	// constants
	int MAX_JUMPS;
	float GRAVITY;

	int mJumpCount;

	//PHYSICS
	float mVerticalVelocity;

	QVector4D mPosition;
	//BOUNDING BOX
	QVector4D mVertex1, mVertex2;

	SceneNode *mRoot, *mMapRoot;
};

#endif