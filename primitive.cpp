#include "primitive.hpp"
#include "AppWindow.hpp"
#include <math.h>
#include <iostream>


Primitive::~Primitive()
{
	mTranslateX = 0;
	mTranslateY = 0;
}

void Primitive::setBoundaries(QMatrix4x4 transformMatrix) {}

bool Primitive::faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction) {
	return false;
}

bool Primitive::isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor) {
	return false;
}

double Primitive::EPSILON = 0.0001;



Block::Block(int type, double translateX, double translateY, double translateZ) {
	switch (type) {
		case 0: { //wood
			mReflectFactor = 0;
			mTransparency = 1;

			mTextureIDs.push_back(1);
			mTextureIDs.push_back(4);
			break;
		}
		case 1: { //metallic glassy
			mReflectFactor = 0.3;
			mTransparency = 0.3;


			mTextureIDs.push_back(2);
			mTextureIDs.push_back(4);
			break;
		}
		case 2: { //toy
			mReflectFactor = 0;
			mTransparency = 1;


			mTextureIDs.push_back(3);
			mTextureIDs.push_back(0);
		}
		default: {
			mReflectFactor = 0;
			mTransparency = 1;

			mTextureIDs.push_back(1);
			mTextureIDs.push_back(2);
			break;
		}
	}

	mTranslateX = translateX;
	mTranslateY = translateY;
	mTranslateZ = translateZ;
}

Block::~Block() {}

void Block::walk_gl(QMatrix4x4 transformMatrix) const {
	AppWindow::m_viewer->draw_cube(transformMatrix, mTextureIDs, mReflectFactor, mTransparency);
}

void Block::setBoundaries(QMatrix4x4 transformMatrix) {
	mVertex1 = QVector4D(0, 0, 1, 1);
	mVertex2 = QVector4D(1, 1, 0, 1);

	mVertex1 = transformMatrix * mVertex1;
	mVertex2 = transformMatrix * mVertex2;
}





bool Block::faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction) {
	switch (direction) {
		case 0:
			return intersectsFront(p1, p2, velocity, boxVelocity);
			break;
		case 1:
			return intersectsBack(p1, p2, velocity, boxVelocity);
			break;
		case 2:
			return intersectsRight(p1, p2, velocity, boxVelocity);
			break;
		case 3:
			return intersectsLeft(p1, p2, velocity, boxVelocity);
			break;
		case 4:
			return intersectsTop(p1, p2, velocity, boxVelocity);
			break;
		case 5:
			return intersectsBottom(p1, p2, velocity, boxVelocity);
			break;  
		default:
			break;
	}

	return false;
}

bool Block::isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor) {
	bool result = false;

	if (mReflectFactor == 0) {
		return result;
	}


	double tempDistance;

	// BACK
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		p1.z() <= mVertex2.z()) {
		*face = 0;

		tempDistance = mVertex2.z() - p1.z();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;
	
		result = true;
	}

	// BOTTOM
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z())) &&
		p2.y() <= mVertex1.y()) {
		*face = 1;

		tempDistance = mVertex1.y() - p2.y();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;

		result = true;
	}

	// LEFT
	if ((betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z())) &&
		p2.x() <= mVertex1.x()) {
		*face = 2;

		tempDistance = mVertex1.x() - p2.x();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;

		result = true;
	}

	// RIGHT
	if ((betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z())) &&
		p1.x() >= mVertex2.x()) {
		*face = 3;

		tempDistance = p1.x() - mVertex2.x();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;

		result = true;
	}

	// TOP
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z())) &&
		p1.y() >= mVertex2.y()) {
		*face = 4;
		
		tempDistance = p1.y() - mVertex2.y();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;

		result = true;
	}

	// FRONT
	if ((betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x())) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y())) &&
		p2.z() >= mVertex1.z()) {
		*face = 5;

		tempDistance = p2.z() - mVertex1.z();
		if (result)
			*distance = tempDistance < *distance? tempDistance : *distance;
		else
			*distance = tempDistance;

		result = true;
	}


	if (result)
		*reflectFactor = mReflectFactor;

	return result;
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



bool Block::aboveAndUnderBox(QVector4D p1, QVector4D p2) {
	if (p1.y() <= mVertex1.y() &&
		p2.y() >= mVertex2.y())
		return true;
	return false;
}

bool Block::leftAndRightOfBox(QVector4D p1, QVector4D p2) {
	if (p1.x() <= mVertex1.x() &&
		p2.x() >= mVertex2.x())
		return true;
	return false;
}

bool Block::inFrontAndBehindBox(QVector4D p1, QVector4D p2) {
	if (p1.z() >= mVertex1.z() &&
		p2.z() <= mVertex2.z())
		return true;
	return false;
}




bool Block::intersectsFront(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	double z = fmin(p1.z(), p2.z());

	if (oldVelocityZ < 0 && boxVelocity.z() > 0 &&
		p1.z() > mVertex1.z()) { // loose check to make sure we're in front first
		velocity->setZ(mVertex1.z() - z + 0.05);
		oldVelocityZ = boxVelocity.z();
		return true;
	}

	if (z - mVertex1.z() > -EPSILON && 
		(z + velocity->z()) - (mVertex1.z() + boxVelocity.z()) < EPSILON &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2))) {
		oldVelocityZ = boxVelocity.z();

		if (boxVelocity.z() > 0) {
			velocity->setZ(boxVelocity.z());
		} else if (boxVelocity.z() < 0) {
			if (velocity->z() == 0) {
				velocity->setZ(0);
			} else {
				double ratio = (mVertex1.z() + boxVelocity.z() - z)/velocity->z();
				velocity->setZ(velocity->z() * ratio);
			}
		} else {
			if (velocity->z() == 0) {
				velocity->setZ(0);
			} else {
				double ratio = (mVertex1.z() - z)/velocity->z();
				if (ratio < 0.001) ratio = 0;

				velocity->setZ(velocity->z() * ratio);
			}
		}

		return true;
	}

	return false;
}

bool Block::intersectsBack(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	double z = fmax(p1.z(), p2.z());

	if (oldVelocityZ > 0 && boxVelocity.z() < 0 &&
		p2.z() < mVertex2.z()) { // loose check to make sure character is positioned somewhere that can collide with the back
		velocity->setZ(mVertex2.z() - z - 0.05);
		oldVelocityZ = boxVelocity.z();
		return true;
	}

	if (z - mVertex2.z() < EPSILON && 
		(z + velocity->z()) - (mVertex2.z() + boxVelocity.z()) > -EPSILON &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2))) {
		oldVelocityZ = boxVelocity.z();

		if (boxVelocity.z() > 0) {
			if (velocity->z() == 0) {
				velocity->setZ(0);
			}
			else {
				double ratio = (mVertex2.z() + boxVelocity.z() - z)/velocity->z();
				velocity->setZ(velocity->z() * ratio);
			}
		} else if (boxVelocity.z() < 0) {
			velocity->setZ(boxVelocity.z());
		} else {
			if (velocity->z() == 0) {
				velocity->setZ(0);
			} else {
				double ratio = (mVertex2.z() - z)/velocity->z();
				if (ratio < 0.001) ratio = 0;

				velocity->setZ(velocity->z() * ratio);
			}
		}

		return true;
	}

	return false;
}

bool Block::intersectsRight(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	double x = fmin(p1.x(), p2.x());

	if (oldVelocityX < 0 && boxVelocity.x() > 0 &&
		p2.x() > mVertex2.x()) { // loose check to make sure we're on the right
		velocity->setX(mVertex2.x() - x + 0.05);
		oldVelocityX = boxVelocity.x();
		return true;
	}

	if (x - mVertex2.x() > -EPSILON && 
		(x + velocity->x()) - (mVertex2.x() + boxVelocity.x()) < EPSILON &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2))) {
		oldVelocityX = boxVelocity.x();

		if (boxVelocity.x() > 0) {
			velocity->setX(boxVelocity.x());
		} else if (boxVelocity.x() < 0) {
			if (velocity->x() == 0) {
				velocity->setX(0);
			} else {
				double ratio = (mVertex2.x() + boxVelocity.x() - x)/velocity->x();
				velocity->setX(velocity->x() * ratio);
			}
		} else {
			if (velocity->x() == 0) {
				velocity->setX(0);
			} else {
				double ratio = (mVertex2.x() - x)/velocity->x();
				if (ratio < 0.001) ratio = 0;

				velocity->setX(velocity->x() * ratio);
			}
		}

		return true;
	}

	return false;
}

bool Block::intersectsLeft(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	double x = fmax(p1.x(), p2.x());

	if (oldVelocityX > 0 && boxVelocity.x() < 0 &&
		p1.x() < mVertex1.x()) { // make sure we're on the left of the object
		velocity->setX(mVertex1.x() - x - 0.05);
		oldVelocityX = boxVelocity.x();
		return true;
	}

	if (x - mVertex1.x() < EPSILON && 
		(x + velocity->x()) - (mVertex1.x() + boxVelocity.x()) > -EPSILON &&
		(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2))) {
		oldVelocityX = boxVelocity.x();

		if (boxVelocity.x() > 0) {
			if (velocity->x() == 0) {
				velocity->setX(0);
			} else {
				double ratio = (mVertex1.x() + boxVelocity.x() - x)/velocity->x();
				velocity->setX(velocity->x() * ratio);
			}
		} else if (boxVelocity.x() < 0) {
			velocity->setX(boxVelocity.x());
		} else {
			if (velocity->x() == 0) {
				velocity->setX(0);
			} else {
				double ratio = (mVertex1.x() - x)/velocity->x();
				if (ratio < 0.001) ratio = 0;

				velocity->setX(velocity->x() * ratio);
			}
		}

		return true;
	}

	return false;
}

bool Block::intersectsTop(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	// see for inspiration on using velocity:
	// http://techny.tumblr.com/post/42125198333/3d-collision-detection-and-resolution-using-sweeping
	double y = fmin(p1.y(), p2.y());

	// case where box changes direction and character goes inside
	if (oldVelocityY < 0 && boxVelocity.y() > 0)  {
		velocity->setY(mVertex2.y() - y + 0.03);
		oldVelocityY = boxVelocity.y();

		velocity->setX(boxVelocity.x());
		velocity->setZ(boxVelocity.z());
		return true;
	}

	if (y - mVertex2.y() > -EPSILON && 
		(y + velocity->y()) - (mVertex2.y() + boxVelocity.y()) < EPSILON &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2))) {
		// collision detected
		oldVelocityY = boxVelocity.y();

		if (boxVelocity.y() > 0) {
			velocity->setY(boxVelocity.y());
		} else if (boxVelocity.y() < 0) {
			if (velocity->y() == 0) {
				velocity->setY(0);
			} else {
				double ratio = (mVertex2.y() + boxVelocity.y() - y)/velocity->y();
				velocity->setY(velocity->y() * ratio);
			}
		} else {
			if (velocity->y() == 0) {
				velocity->setY(0);
			} else {
				double ratio = (mVertex2.y() - y)/velocity->y();
				if (ratio < 0.001) ratio = 0;

				velocity->setY(velocity->y() * ratio);
			}
		}

		velocity->setX(boxVelocity.x());
		velocity->setZ(boxVelocity.z());
		return true;
	}

	oldVelocityY = 0;
	return false;
}

bool Block::intersectsBottom(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	double y = fmax(p1.y(), p2.y());

	if (oldVelocityY > 0 && boxVelocity.y() < 0) {
		velocity->setY(mVertex1.y() - y - 0.05);
		oldVelocityY = boxVelocity.y();
		return true;
	}

	if (y - mVertex1.y() < EPSILON && 
		(y + velocity->y()) - (mVertex1.y() + boxVelocity.y()) > -EPSILON &&
		(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) &&
		(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2))) {
		oldVelocityY = boxVelocity.y();

		if (boxVelocity.y() > 0) {
			if (velocity->y() == 0) {
				velocity->setY(0);
			} else {
				double ratio = (mVertex1.y() + boxVelocity.y() - y)/velocity->y();
				velocity->setY(velocity->y() * ratio);
			}
		} else if (boxVelocity.y() < 0) {
			velocity->setY(boxVelocity.y());
		} else {
			if (velocity->y() == 0) {
				velocity->setY(0);
			} else {
				double ratio = (mVertex1.y() - y)/velocity->y();
				if (ratio < 0.001) ratio = 0;

				velocity->setY(velocity->y() * ratio);
			}
		}

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

bool Sphere::faceIntersectsBox(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity, int direction) {
	return false;
}

bool Sphere::isOverBox(QVector4D p1, QVector4D p2, int* face, double* distance, float* reflectFactor) {
	return false;
}
