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
	bool walkForward(double* velocity);
	bool walkBackward(double* velocity);
	bool strafeLeft(double* velocity);
	bool strafeRight(double* velocity);
	void jump();

	void applyGravity();

private:
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