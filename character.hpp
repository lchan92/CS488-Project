#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <QtOpenGL>
#include "scene.hpp"
#include "mesh.hpp"

class Character {
public:
	Character();
	virtual ~Character();

	void setMapRoot(SceneNode* mapRoot);
	void bind();

	void draw();

	QVector3D getPosition();
	QVector4D getInitPosition();
	QVector3D getCameraLookAtPosition();
	bool isOverBox(double* height);

	void updatePosition();
	void updateBoundingBox();

	bool isAlive();


	// movement functions
	void rotateY(float amount);
	void walkForward(double*  velocity);
	void walkBackward(double*  velocity);
	void strafeLeft(double*  velocity);
	void strafeRight(double*  velocity);
	void jump();

	bool applyGravity(double*  velocity);

private:
	// constants
	int MAX_JUMPS;
	float GRAVITY;
	float DEATH_HEIGHT;

	int mJumpCount;
	bool mAlive;

	//PHYSICS
	float mVerticalVelocity;

	QVector4D mPosition;
	//BOUNDING BOX
	QVector4D mVertex1, mVertex2;

	SceneNode *mMapRoot;
	Mesh* mMesh;
};

#endif