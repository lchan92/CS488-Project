#include "primitive.hpp"
#include "AppWindow.hpp"
#include <math.h>
#include <iostream>


Primitive::~Primitive()
{
}

void Primitive::setBoundaries(QMatrix4x4 transformMatrix) {}

bool Primitive::faceIntersectsBox(QVector4D p1, QVector4D p2) {
	return false;
}



Block::~Block() {}

void Block::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_cube(transformMatrix);
}

void Block::setBoundaries(QMatrix4x4 transformMatrix) {
	mVertex1 = QVector4D(0, 0, 0, 1);
	mVertex2 = QVector4D(1, 1, 1, 1);

	mVertex1 = transformMatrix * mVertex1;
	mVertex2 = transformMatrix * mVertex2;
}

bool Block::faceIntersectsBox(QVector4D p1, QVector4D p2) {
	// std::cout << "mVertex1: " << mVertex1.x() << "," << mVertex1.y() << "," << mVertex1.z() << std::endl;
	// std::cout << "mVertex2: " << mVertex2.x() << "," << mVertex2.y() << "," << mVertex2.z() << std::endl;


	QVector4D nearBottomLeft = mVertex1;
	QVector4D nearBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex1.z(), 1);
	QVector4D nearTopRight = QVector4D(mVertex2.x(), mVertex2.y(), mVertex1.z(), 1);
	QVector4D nearTopLeft = QVector4D(mVertex1.x(), mVertex2.y(), mVertex1.z(), 1);

	QVector4D farBottomLeft = QVector4D(mVertex1.x(), mVertex1.y(), mVertex2.z(), 1);
	QVector4D farBottomRight = QVector4D(mVertex2.x(), mVertex1.y(), mVertex2.z(), 1);
	QVector4D farTopRight = mVertex2;
	QVector4D farTopLeft = QVector4D(mVertex1.x(), mVertex2.y(), mVertex2.z(), 1);

	// bottom, front, left, right, back, top
	if (intersectsFace(p1, p2, nearBottomLeft, farBottomRight) ||
		intersectsFace(p1, p2, nearBottomLeft, nearTopRight) ||
		intersectsFace(p1, p2, farBottomLeft, nearTopLeft) ||
		intersectsFace(p1, p2, farBottomRight, nearTopRight) ||
		intersectsFace(p1, p2, farBottomLeft, farTopRight) ||
		intersectsFace(p1, p2, nearTopLeft, farTopRight)) {
		return true;
	}

	return false;
}

bool Block::intersectsFace(QVector4D a1, QVector4D a2, QVector4D b1, QVector4D b2) {
	//see: http://gamemath.com/2011/09/detecting-whether-two-boxes-overlap/
	// std::cout << "starting test" << std::endl;
	// std::cout << "a1: " << a1.x() << "," << a1.y() << "," << a1.z() << std::endl;
	// std::cout << "a2: " << a2.x() << "," << a2.y() << "," << a2.z() << std::endl;
	// std::cout << "b1: " << b1.x() << "," << b1.y() << "," << b1.z() << std::endl;
	// std::cout << "b2: " << b2.x() << "," << b2.y() << "," << b2.z() << std::endl;


	if (a2.x() < b1.x()) return false; // too far to left
	// std::cout << "passed left test" << std::endl;
	if (a1.x() > b2.x()) return false; // too far to right
	// std::cout << "passed right test" << std::endl;

	if (a2.y() < b1.y()) return false; // too far down
	// std::cout << "passed down test" << std::endl;
	if (a1.y() > b2.y()) return false; // too far up
	// std::cout << "passed up test" << std::endl;

	if (a1.z() < b1.z()) return false; // too far back
	// std::cout << "passed back test" << std::endl;
	if (a2.z() > b2.z()) return false; // too far forward
	// std::cout << "passed forward test" << std::endl;

	return true;
}





Sphere::~Sphere() {}

void Sphere::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_sphere(transformMatrix);
}

void Sphere::setBoundaries(QMatrix4x4 transformMatrix) {
}

bool Sphere::faceIntersectsBox(QVector4D p1, QVector4D p2) {
	return false;
}


