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
	mMesh->load("obj/chicken/hardcore_chicken.obj", "textures/chicken.jpg");

	mMesh->bind();
	setPosition();
}

void Trophy::draw() {
	AppWindow::m_viewer->draw_mesh(mMesh);
}




void Trophy::setPosition() {	
	mMesh->translate(QVector3D(25,65,-142));
	updatePosition();
}

QVector4D Trophy::getPosition() {
	return mPosition;
}

void Trophy::updatePosition() {
	mPosition = mMesh->mInitPosition;
	mPosition = mMesh->getTransform() * mMesh->getRotationTransform() * mPosition;
}

void Trophy::updateBoundingBox() {
	mVertex1 = mMesh->getTransform() * mMesh->mVertex1;
	mVertex2 = mMesh->getTransform() * mMesh->mVertex2;
}



bool Trophy::isOverBox(int* face, double* height, float* reflectFactor) {
	return mMapRoot->isOverBox(mVertex1, mVertex2, face, height, reflectFactor);
}

bool Trophy::isInRange(QVector4D position) {
	QVector4D distance = mPosition - position;

	if (distance.length() < 10) {
		return true;
	}

	return false;
}

void Trophy::descend() {
	if (mPosition.y() > 60) {
		mMesh->translate(QVector3D(0,-0.1,0));
		mMesh->rotate('y', 10);
		updatePosition();
	}
}