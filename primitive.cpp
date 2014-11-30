#include "primitive.hpp"
#include "AppWindow.hpp"
#include <math.h>
#include <iostream>


Primitive::~Primitive()
{
}

void Primitive::setBoundaries(QMatrix4x4 transformMatrix) {}

bool Primitive::faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction) {
	return false;
}

bool Primitive::isOverBox(QVector4D p1, QVector4D p2, double* height, float* reflectFactor) {
	return false;
}



Block::Block(int type) {
	switch (type) {
		case 0: { //wood
			mReflectFactor = 0;

			mTextureIDs.push_back(1);
			mTextureIDs.push_back(3);
			break;
		}
		case 1: { //glass
			mReflectFactor = 0.3;

			mTextureIDs.push_back(2);
			mTextureIDs.push_back(3);
			break;
		}
		default: {
			mReflectFactor = 0;

			mTextureIDs.push_back(1);
			mTextureIDs.push_back(2);
			break;
		}
	}
}

Block::~Block() {}

void Block::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_cube(transformMatrix, mTextureIDs, mReflectFactor);
}

void Block::setBoundaries(QMatrix4x4 transformMatrix) {
	mVertex1 = QVector4D(0, 0, 1, 1);
	mVertex2 = QVector4D(1, 1, 0, 1);

	mVertex1 = transformMatrix * mVertex1;
	mVertex2 = transformMatrix * mVertex2;
}





bool Block::faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction) {
	switch (direction) {
		case 0: {
			return intersectsFront(p1, p2, velocity);
			break;
		}
		case 1:
			return intersectsBack(p1, p2, velocity);
			break;
		case 2:
			return intersectsRight(p1, p2, velocity);
			break;
		case 3:
			return intersectsLeft(p1, p2, velocity);
			break;
		case 4:
			return intersectsTop(p1, p2, velocity);
			break;
		case 5:
			return intersectsBottom(p1, p2, velocity);
			break;  
		default:
			break;
	}

	return false;
}

bool Block::isOverBox(QVector4D p1, QVector4D p2, double* height, float* reflectFactor) {
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z())) &&
		p1.y() >= mVertex2.y()) {
		*height = p1.y() - mVertex2.y();
		*reflectFactor = mReflectFactor;
		return true;
	}

	return false;
}




bool Block::betweenLeftRight(double x, double left, double right) {
	if (x <= left) return false;
	if (x >= right) return false;
	return true;
}

bool Block::betweenTopBottom(double y, double top, double bottom) {
	if (y >= top) return false;
	if (y <= bottom) return false;
	return true;
}

bool Block::betweenFrontBack(double z, double front, double back) {
	if (z >= front) return false;
	if (z <= back) return false;
	return true;
}

bool Block::intersectsFront(QVector4D p1, QVector4D p2, double* velocity) {
	double z = fmin(p1.z(), p2.z());

	if (z >= mVertex1.z() && (z + (*velocity)) <= mVertex1.z() &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()))) {
		double ratio = (mVertex1.z() - z)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}

bool Block::intersectsBack(QVector4D p1, QVector4D p2, double* velocity) {
	double z = fmax(p1.z(), p2.z());

	if (z <= mVertex2.z() && (z + (*velocity)) >= mVertex2.z() &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()))) {
		double ratio = (mVertex2.z() - z)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}

bool Block::intersectsRight(QVector4D p1, QVector4D p2, double* velocity) {
	double x = fmin(p1.x(), p2.x());

	if (x >= mVertex2.x() && (x + (*velocity)) <= mVertex2.x() &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()))) {
		double ratio = (mVertex2.x() - x)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}

bool Block::intersectsLeft(QVector4D p1, QVector4D p2, double* velocity) {
	double x = fmax(p1.x(), p2.x());

	if (x <= mVertex1.x() && (x + (*velocity)) >= mVertex1.x() &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()))) {
		double ratio = (mVertex1.x() - x)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}

bool Block::intersectsTop(QVector4D p1, QVector4D p2, double* velocity) {
	// see for inspiration on using velocity:
	// http://techny.tumblr.com/post/42125198333/3d-collision-detection-and-resolution-using-sweeping
	double y = fmin(p1.y(), p2.y());

	if (y >= mVertex2.y() && (y + (*velocity)) <= mVertex2.y() &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()))) {
		// collision detected
		double ratio = (mVertex2.y() - y)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}

bool Block::intersectsBottom(QVector4D p1, QVector4D p2, double* velocity) {
	double y = fmax(p1.y(), p2.y());

	if (y <= mVertex1.y() && (y + (*velocity)) >= mVertex1.y() &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()))) {
		double ratio = (mVertex1.y() - y)/(*velocity);
		*velocity = *velocity * ratio;
		return true;
	}

	return false;
}









Sphere::~Sphere() {}

void Sphere::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_sphere(transformMatrix);
}

void Sphere::setBoundaries(QMatrix4x4 transformMatrix) {
}

bool Sphere::faceIntersectsBox(QVector4D p1, QVector4D p2, double* velocity, int direction) {
	return false;
}

bool Sphere::isOverBox(QVector4D p1, QVector4D p2, double* height, float* reflectFactor) {
	return false;
}
