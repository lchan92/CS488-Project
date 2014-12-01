#include "trophy.hpp"
#include "AppWindow.hpp"

Trophy::Trophy() {
}

Trophy::~Trophy() {}


void Trophy::setMapRoot(SceneNode* mapRoot) {
	mMapRoot = mapRoot;
}

void Trophy::bind() {
	mMesh = new Mesh();
	mMesh->load("obj/chicken/hardcore_chicken.obj");

	mMesh->bind();
	setPosition();
}

void Trophy::draw() {
	AppWindow::m_viewer->draw_mesh(mMesh);
}




void Trophy::setPosition() {	
	mMesh->translate(QVector3D(25,60,-142));
	mPosition = mMesh->mInitPosition;
	mPosition = mMesh->getTransform() * mMesh->getRotationTransform() * mPosition;

	// BOUNDING BOX
	mVertex1 = mMesh->getTransform() * mMesh->mVertex1;
	mVertex2 = mMesh->getTransform() * mMesh->mVertex2;
}

QVector4D Trophy::getPosition() {
	return mPosition;
}

bool Trophy::isInRange(QVector4D position) {
	QVector4D distance = mPosition - position;

	if (distance.length() < 10) {
		return true;
	}

	return false;
}

bool Trophy::isOverBox(int* face, double* height, float* reflectFactor) {
	return mMapRoot->isOverBox(mVertex1, mVertex2, face, height, reflectFactor);
}

