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
	bool walkForward();
	bool walkBackward();
	bool strafeLeft();
	bool strafeRight();
	void jump();

	void applyGravity();

private:
	bool detectCollisions();

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