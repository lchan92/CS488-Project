#include "primitive.hpp"
#include "AppWindow.hpp"
#include <math.h>
#include <iostream>


Primitive::~Primitive()
{
}

void Primitive::setBoundaries(QMatrix4x4 transformMatrix) {}

bool Primitive::faceIntersectsBox(QVector4D p1, QVector4D p2, double* diff, int direction) {
	return false;
}



Block::~Block() {}

void Block::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_cube(transformMatrix);
}

void Block::setBoundaries(QMatrix4x4 transformMatrix) {
	mVertex1 = QVector4D(0, 0, 1, 1);
	mVertex2 = QVector4D(1, 1, 0, 1);

	mVertex1 = transformMatrix * mVertex1;
	mVertex2 = transformMatrix * mVertex2;
}

bool Block::faceIntersectsBox(QVector4D p1, QVector4D p2, double* diff, int direction) {
	// determines the coordinate of the face that it should directly be colliding with
	switch(direction) {
		case 0: { //collide with front face
			*diff = p2.z() - mVertex1.z();
			break;
		}
		case 1: { //collide with back face
			*diff = p1.z() - mVertex2.z();
			break;
		}
		case 2: { //colide with right face
			*diff = p1.x() - mVertex2.x();
			break;
		}
		case 3: { //collide with left face
			*diff = p1.x() - mVertex1.x();
			break;
		}
		case 4: { //collide with top/bottom
			*diff = p1.y() - mVertex2.y();
			break;
		}
		default: {
			*diff = 0;
			break;
		}
	}
//http://techny.tumblr.com/post/42125198333/3d-collision-detection-and-resolution-using-sweeping
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()))) {
		return true;
	}

	return false;
}

bool Block::betweenLeftRight(double x, double left, double right) {
	// std::cout << "test left right" << std::endl;
	if (x <= left) return false;
	// std::cout << "pass left" << std::sendl;
	if (x >= right) return false;
	// std::cout << "pass right" << std::endl;

	return true;
}

bool Block::betweenTopBottom(double y, double top, double bottom) {
	// std::cout << "test top botstom - y: " << y << "  top:" << top << "  bottom: " << bottom << std::endl;
	if (y >= top) return false;
	// std::cout << "pass top" << std::endl;
	if (y <= bottom) return false;
	// std::cout << "pass bottom" << std::endl;

	return true;
}

bool Block::betweenFrontBack(double z, double front, double back) {
	// std::cout << "test front back - z: " << z << "  front: " << front << "  back: " << back << std::endl;
	if (z >= front) return false;
	// std::cout << "pass front" << std::endl;
	if (z <= back) return false;
	// std::cout << "pass back" << std::endl;

	return true;
}




Sphere::~Sphere() {}

void Sphere::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_sphere(transformMatrix);
}

void Sphere::setBoundaries(QMatrix4x4 transformMatrix) {
}

bool Sphere::faceIntersectsBox(QVector4D p1, QVector4D p2, double* diff, int direction) {
	return false;
}


