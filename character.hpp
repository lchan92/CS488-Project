#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
#include <QtOpenGL>
#include "scene.hpp"
#include "mesh.hpp"
#include "trophy.hpp"

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
	QVector4D getVertex1();
	QVector4D getVertex2();


	void updatePosition();
	void updateBoundingBox();


	bool isAlive();
	bool isOverBox(int* face, double* height, float* reflectFactor);


	// movement functions
	void rotateY(float amount);
	void walkForward(QVector3D*  velocity);
	void walkBackward(QVector3D*  velocity);
	void strafeLeft(QVector3D*  velocity);
	void strafeRight(QVector3D*  velocity);
	void jump();

	bool applyGravity(QVector3D*  velocity);

	void checkFrontCollisions(QVector3D* velocity);
	void checkBackCollisions(QVector3D* velocity);
	void checkLeftCollisions(QVector3D* velocity);
	void checkRightCollisions(QVector3D* velocity);
private:
	// constants
	int MAX_JUMPS;
	float GRAVITY;
	float DEATH_HEIGHT;

	int mJumpCount;
	bool mAlive;

	//PHYSICS
	float mVerticalVelocity;
	bool mOnSurface;

	QVector4D mPosition;
	//BOUNDING BOX
	QVector4D mVertex1, mVertex2;

	SceneNode *mMapRoot;
	Mesh* mMesh;
};

#endif