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
	// check if character can intersect with the front face + make sure we're at least in front of the other side of the box
	if (!(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) ||
		!(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) ||
		!(p2.z() > mVertex2.z()))
		return false;

	/* CASES: */	
	// character stationary
	if (velocity->z() == 0) {
		// 1: box stationary, character stationary
		if (boxVelocity.z() == 0) {
			double distance = p2.z() - mVertex1.z();

			if (distance > -EPSILON && distance < EPSILON)
				return true;
		}

		// 2: box moving at character, character stationary
		if (boxVelocity.z() > 0) {
			// if the box + it's next move will intersect the character, move the character
			double newBoxZ = mVertex1.z() + boxVelocity.z();

			if (newBoxZ - p2.z() > -EPSILON) {
				velocity->setZ(newBoxZ - p2.z());
				return true;
			}
		}

		return false;
	}
	// character towards front face 
	else if (velocity->z() < 0) {
		double newCharacterZ = p2.z() + velocity->z();

		// 3: box stationary
		if (boxVelocity.z() == 0) {
			// if character + velocity goes inside the box
			if (newCharacterZ - mVertex1.z() < EPSILON) {
				velocity->setZ(mVertex1.z() - p2.z());
				return true;
			}
		}

		double newBoxZ = mVertex1.z() + boxVelocity.z();

		// 4: box moving away or at character
		// if character + velocity goes inside of box + its velocity
		if (newCharacterZ - newBoxZ < EPSILON) {
			velocity->setZ(newBoxZ - p2.z());
			return true;
		}
	}

	return false;
}

bool Block::intersectsBack(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	if (!(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) ||
		!(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) ||
		!(p1.z() < mVertex1.z()))
		return false;

	// see intersects front for cases, similar logic
	if (velocity->z() == 0) {
		// box stationary
		if (boxVelocity.z() == 0) {
			double distance = mVertex2.z() - p1.z();

			if (distance > -EPSILON && distance < EPSILON)
				return true;
		}

		// box moving at character
		if (boxVelocity.z() < 0) {
			double newBoxZ = mVertex2.z() + boxVelocity.z();

			if (newBoxZ - p1.z() < EPSILON) {
				velocity->setZ(newBoxZ - p1.z());
				return true;
			}
		}
	} 
	// character moving towards face
	else if (velocity->z() > 0) {
		double newCharacterZ = p1.z() + velocity->z();

		// box stationary
		if (boxVelocity.z() == 0) {
			if (newCharacterZ - mVertex2.z() > -EPSILON) {
				velocity->setZ(mVertex2.z() - p1.z());
				return true;
			}
		}

		double newBoxZ = mVertex2.z() + boxVelocity.z();

		// box moving at or away from character
		if (newCharacterZ - newBoxZ > -EPSILON) {
			velocity->setZ(newBoxZ - p1.z());
			return true;
		}
	}

	return false;
}

bool Block::intersectsRight(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	if (!(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) ||
		!(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2)) ||
		!(p1.x() > mVertex1.x()))
		return false;

	// see intersects front case for logic
	if (velocity->x() == 0) {
		if (boxVelocity.x() == 0) {
			double distance = p1.x() - mVertex2.x();

			if (distance > -EPSILON && distance < EPSILON)
				return true;
		}

		if (boxVelocity.x() > 0) {
			double newBoxX = mVertex2.x() + boxVelocity.x();

			if (newBoxX - p1.x() > -EPSILON) {
				velocity->setX(newBoxX - p1.x());
				return true;
			}
		}
	} else if (velocity->x() < 0) {
		double newCharacterX = p1.x() + velocity->x();

		if (boxVelocity.x() == 0) {
			if (newCharacterX - mVertex2.x() < EPSILON) {
				velocity->setX(mVertex2.x() - p1.x());
				return true;
			}
		}

		double newBoxX = mVertex2.x() + boxVelocity.x();

		if (newCharacterX - newBoxX < EPSILON) {
			velocity->setX(newBoxX - p1.x());
			return true;
		}
	}

	return false;
}

bool Block::intersectsLeft(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	if (!(betweenTopBottom(p1.y(), mVertex2.y(), mVertex1.y()) ||
			betweenTopBottom(p2.y(), mVertex2.y(), mVertex1.y()) ||
			aboveAndUnderBox(p1, p2)) ||
		!(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2)) ||
		!(p2.x() < mVertex2.x()))
		return false;

	if (velocity->x() == 0) {
		if (boxVelocity.x() == 0) {
			double distance = mVertex1.x() - p2.x();

			if (distance > -EPSILON && distance < EPSILON)
				return true;
		}

		if (boxVelocity.x() < 0) {
			double newBoxX = mVertex1.x() + boxVelocity.x();

			if (newBoxX - p2.x() < EPSILON) {
				velocity->setX(newBoxX - p2.x());
				return true;
			}
		}
	} else if (velocity->x() > 0) {
		double newCharacterX = p2.x() + velocity->x();

		if (boxVelocity.x() == 0) {
			if (newCharacterX - mVertex1.x() > -EPSILON) {
				velocity->setX(mVertex1.x() - p2.x());
				return true;
			}
		}

		double newBoxX = mVertex1.x() + boxVelocity.x();

		if (newCharacterX - newBoxX > -EPSILON) {
			velocity->setX(newBoxX - p2.x());
			return true;
		}
	}

	return false;
}

bool Block::intersectsTop(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	if (!(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) ||
		!(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2)) ||
		!(p1.y() > mVertex1.y()))
		return false;



	if (velocity->y() == 0) {
		if (boxVelocity.y() == 0) {
			double distance = p1.y() - mVertex2.y();

			if (distance > -EPSILON && distance < EPSILON)
				return true;
		}

		if (boxVelocity.y() > 0) {
			double newBoxY = mVertex2.y() + boxVelocity.y();

			if (newBoxY - p1.y() > -EPSILON) {
				velocity->setY(newBoxY - p1.y());
				return true;
			}
		}
	} else if (velocity->y() < 0) {
		double newCharacterY = p1.y() + velocity->y();

		if (boxVelocity.y() == 0) {
			if (newCharacterY - mVertex2.y() < EPSILON) {
				velocity->setY(mVertex2.y() - p1.y());
				return true;
			}
		}

		double newBoxY = mVertex2.y() + boxVelocity.y();

		if (newCharacterY - newBoxY < EPSILON) {
			velocity->setY(newBoxY - p1.y());
			return true;
		}
	} else {
		if (boxVelocity.y() > 0) {
			double newCharacterY = p1.y() + velocity->y();
			double newBoxY = mVertex2.y() + boxVelocity.y();

			if (newCharacterY - newBoxY < EPSILON) {
				velocity->setY(newBoxY - p1.y());
				return true;				
			}
		}
	}

	return false;
}

bool Block::intersectsBottom(QVector4D p1, QVector4D p2, QVector3D* velocity, QVector3D boxVelocity) {
	if (!(betweenLeftRight(p1.x(), mVertex1.x(), mVertex2.x()) || 
			betweenLeftRight(p2.x(), mVertex1.x(), mVertex2.x()) ||
			leftAndRightOfBox(p1, p2)) ||
		!(betweenFrontBack(p1.z(), mVertex1.z(), mVertex2.z()) ||
			betweenFrontBack(p2.z(), mVertex1.z(), mVertex2.z()) ||
			inFrontAndBehindBox(p1, p2)) ||
		!(p2.y() < mVertex2.y()))
		return false;

	if (velocity->y() == 0) {
		if (boxVelocity.y() == 0) {
			double distance = mVertex1.y() - p2.y();

			if (distance > -EPSILON && distance < EPSILON) {
				return true;
			}
		}

		if (boxVelocity.y() < 0) {
			double newBoxY = mVertex1.y() + boxVelocity.y();

			if (newBoxY - p2.y() < EPSILON) {
				velocity->setY(newBoxY - p2.y());
				return true;
			}
		}
	} else if (velocity->y() > 0) {
		double newCharacterY = p2.y() + velocity->y();

		if (boxVelocity.y() == 0) {
			if (newCharacterY - mVertex1.y() > -EPSILON) {
				velocity->setY(mVertex1.y() - p2.y());
				return true;
			}
		}

		double newBoxY = mVertex1.y() + boxVelocity.y();

		if (newCharacterY - newBoxY > -EPSILON) {
			velocity->setY(newBoxY - p2.y());
			return true;
		}
	}



	// if (y - mVertex1.y() < EPSILON && 
	// 	(y + velocity->y()) - (mVertex1.y() + boxVelocity.y()) > -EPSILON &&
	// 	 &&
	// 	) {
	// 	oldVelocityY = boxVelocity.y();

	// 	if (boxVelocity.y() > 0) {
	// 		if (velocity->y() == 0) {
	// 			velocity->setY(0);
	// 		} else {
	// 			double ratio = (mVertex1.y() + boxVelocity.y() - y)/velocity->y();
	// 			velocity->setY(velocity->y() * ratio);
	// 		}
	// 	} else if (boxVelocity.y() < 0) {
	// 		velocity->setY(boxVelocity.y());
	// 	} else {
	// 		if (velocity->y() == 0) {
	// 			velocity->setY(0);
	// 		} else {
	// 			double ratio = (mVertex1.y() - y)/velocity->y();
	// 			if (ratio < 0.001) ratio = 0;

	// 			velocity->setY(velocity->y() * ratio);
	// 		}
	// 	}

	// 	return true;
	// }

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
