#ifndef TROPHY_HPP
#define TROPHY_HPP

#include "mesh.hpp"
#include "scene.hpp"
#include <QtOpenGL>

class Trophy {
public:
	Trophy();
	virtual ~Trophy();


	void setMapRoot(SceneNode* mapRoot);
	void bind();
	void draw();



	void setPosition();
	QVector4D getPosition();

	void updatePosition();
	void updateBoundingBox();

	bool isOverBox(int* face, double* height, float* reflectFactor);

	bool isInRange(QVector4D position);
	void descend();

	Mesh* mMesh;
private:
	QVector4D mVertex1, mVertex2;
	QVector4D mPosition;

	SceneNode* mMapRoot;
};

#endif